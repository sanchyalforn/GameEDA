#include "Player.hh"


/**
 * Write the name of your player && save this file
 * with the same name && .cc extension.
 */
#define PLAYER_NAME Creeper


struct PLAYER_NAME : public Player {

    /**
    * Factory: returns a new instance of this class.
    * Do not modify this function.
    */
    static Player* factory () {
        return new PLAYER_NAME;
    }

    /*------------------
        DECLARACIONS
          GLOBALS
    ------------------*/

    static constexpr int I[8]  = {-1, -1,  0,  1, 1, 1, 0, 1 };
    static constexpr int J[8]  = { 0, -1, -1, -1, 0, 1, 1, 1 };
    static constexpr int HI[4] = { -1,  0, 1,  0};
    static constexpr int HJ[4] = {  0,  1, 0, -1};

// --------------------------------------------
    using VE    =   vector <int>;
    using VVE   =   vector <VE>;
    using VC    =   vector <char>;
    using VVC   =   vector <VC>;
    using QP    =   queue <Position>;
    using PP    =   pair <int,Position>;
    using VIQ   =   vector <pair <Position,QP>>;

// --------------------------------------------

    VE              v_soldier;
    VE              v_helicopter;
    vector <Post>   v_posts;
    VVC             G;
    VVE             visitats;
    VIQ             v_cues;

// --------------------------------------------

    const int N       = 2;
    const int S       = 0;
    const int E       = 1;
    const int W       = 3;
    const int INFINIT = 1e9;


    /*------------------
        R&&OM STUFF
    ------------------*/
/*

    template <class Position> struct less : binary_function <Position,Position,bool> {
        bool operator() (const Position& x, const Position& y) const {if (x.i == y.i) return x.j < y.j; else return (x.i < y.i)}
    };

    class cmp {
        bool operator() (pair<...> p1, pair<...> p2) {
            if (p1.first != p2.first) return p1.first<p2.first;
            return ..;//el que triis fer
        }
    };
*/
    int manhattan_distance(const Position &p1, const Position &p2) {
        return abs(p1.i - p2.i) + abs(p1.j - p2.j);
    }

    Position sum(const Position &p1, const Position &p2) {
        return Position(p1.i + p2.i, p1.j + p2.j);
    }

    Position which_post(int id) {
        Position aux    = Position(-1,-1);
        Position sold   = data(id).pos;

        for (int i = 0; i < MAX; i++) {
            for (int j = 0; j < MAX; j++) {

                // Get Position
                Position temp = Position(i, j);
                // Check if city (not my property)
                if ((post_owner(temp.i,temp.j) != -2) && (post_owner(temp.i,temp.j) != me())) {

                    // Distance
                    if (aux.i == -1 or (manhattan_distance(temp,sold) < manhattan_distance(sold,aux))) {
                        aux = temp;
                    }
                }
            }
        }
        return aux;
    }

    int weight(int ori, const Position &pos) {
        if (pos.i == -1)
            return ori == N ? 1 : 2;;

        if (pos.i ==  1)
            return ori == S ? 1 : 2;

        if (pos.j ==  1)
            return ori == E ? 1 : 2;

        if (pos.j == -1)
            return ori == W ? 1 : 2;

    }

    //is a post being the objective or some of our soldier?
    //function used for helicopters
    bool post_being_objective(const Position &pos) {
        for (int i = 0; i < (int)v_soldier.size(); ++i)
            if (v_cues[i].first.i == pos.i and v_cues[i].first.j == pos.j) return true;
        return false;
    }

    /*----------------
        SOLDIER
    ----------------*/

    //Comparator of different escenarios
    bool heal_stuff (const Position &ally, const Position &enemy) {
        int Enemy_HP = data(which_soldier(enemy.i,enemy.j)).life;
        int Ally_HP  = data(which_soldier(ally.i,ally.j)).life;

        //same land
        if (what(ally.i,ally.j) == what(enemy.i,enemy.j)) {
            return Ally_HP > Enemy_HP;
        }

        //GRASS vs FOREST
        if (what(ally.i,ally.j)  > what(enemy.i,enemy.j)) {
            return (Ally_HP >= Enemy_HP - 50);
        }
        //FOREST vs GRASS
            return (Ally_HP >= Enemy_HP +50);


        if (Ally_HP > Enemy_HP       && (what(ally.i, ally.j) == what(enemy.i,enemy.j) || what(ally.i,ally.j) < what(enemy.i,enemy.j)))
            return true;
        if (Ally_HP == Enemy_HP      && (what(ally.i,ally.j)  <  what(enemy.i,enemy.j)))
            return true;
        if (Ally_HP >= Enemy_HP - 50 && (what(ally.i,ally.j)  <  what(enemy.i,enemy.j)))
            return true;
        return false;
    }

    void BFS(const Position &i_pos,const Position &f_pos, QP &qp){
        visitats = VVE (MAX, VE (MAX, false));
        queue <pair <Position,queue<Position> > > q;
        q.push({i_pos,queue<Position>()});
        bool trobat = false;
        visitats[i_pos.i][i_pos.j] = true;

        while ( !q.empty() &&  !trobat ) {
            pair <Position, QP > p = q.front(); q.pop();

            for (int i = 0; i < 8 && !trobat; ++i) {
                QP route = p.second;
                Position seg = sum(p.first,Position(I[i],J[i]));
                if (not visitats[seg.i][seg.j]) {
                    if (pos_ok(seg) && what(seg.i,seg.j) != MOUNTAIN
                        && what(seg.i,seg.j) != WATER
                        && fire_time(seg.i,seg.j) <= (int)route.size()
                        && (which_soldier(seg.i,seg.j) == 0
                        || data(which_soldier(seg.i,seg.j)).player != me())) {
                            route.push(Position(seg.i,seg.j));
                            q.push({seg,route});
                            visitats[seg.i][seg.j] = true;
                    }

                    if (seg.i == f_pos.i  && seg.j == f_pos.j){
                        qp = route;
                        trobat = true;
                    }
                }
            }
        }
    }


    /*----------------
        HELICOPTER
    ----------------*/

    /*
    NAPALM AREA

    * * * * *
    * * * * *
    * * H * *
    * * * * *
    * * * * *

    */

    int new_orientation(const Position &pos) {
        if (pos.i ==  1) return S;
        if (pos.i == -1) return N;
        if (pos.j ==  1) return E;
        if (pos.j == -1) return W;
    }


/*

    //dijkstra weighting the direction where is the helicopter looking to
    void dijkstra_hel(const Position &inicial, const Position &s, const Position &final, vector <Position> &v, int id) {
        priority_queue< PP, vector< PP >, greater<PP> > Q;
        VVE costs (MAX,VE(MAX,INFINIT));
        costs[s.i][s.j] = 0;
        VVE visitats (MAX,VE(MAX, false));
        Q.push(PP(0,Position(s.i,s.j)));
        while (!Q.empty()) {
            Position q = Q.top().second; Q.pop();
            int ori = data(id).orientation;
            if (not visitats[q.i][q.j])
                visitats[q.i][q.j] = true;
            for (int i = 0; i < 4; ++i) {
                Position aux = sum(q,Position(HI[i],J[i]));
                if (pos_ok(aux)) {
                    int c = weight(ori, Position(I[i],J[i]));
                    if (costs[aux.i][aux.j] > costs[q.i][q.j] + c) {
                        costs[aux.i][aux.j] = costs[q.i][q.j] + c;
                        Q.push(PP(costs[aux.i][aux.j],aux));
                        v.push_back(aux);
                    }
                }
            }
        }
    }
*/

    /*int search_algorithm_helicopter (const Position &p, int id, queue <Position> &q) {
        visitats = VVE(MAX,VE(MAX,false));
        QP Q;
        bool found = 0;
        visitats[p.i][p.j] = true;
        Q.push(p);

        while (! Q.empty()) {
            Position q = Q.front(); Q.pop();
            int ori = data(id).orientation;
            for (int i = 0; i < 4; ++i) {
                Position p = sum(q,Position(HI[i],HJ[i]));
                if (pos_ok(p)) {
                    if (!visitats[p.i][p.j] && which_helicopter(p.i,p.j)!= -1 && which_helicopter(p.i,p.j) != me()) {
                        visitats[p.i][p.j] = 1;
                        Q.push(p);
                    }

                }
            }


            for (int i = 0; i < 4; ++i) {
                Position p = sum(q,Position(HI[i],HJ[i]));
                if (which_post(p) != 0 && which_post(p) != me()) {
                }
            }
        }
        return -1;
    }*/

    bool parachuter_QuestionMark(int x, int y) {

        if ((what(x,y) == WATER) or (what(x,y) == MOUNTAIN)) return false;
        if (fire_time(x,y) != 0) return false;
        if (which_soldier(x,y) != 0) return false;
        return true;
    }

    bool napalm_QuestionMark(Position pos) {
        int num_enemics = 0;
        int num_meus = 0;
        bool post = false;
        for (int i = 0; i < 5; ++i)
            for (int j = 0; j < 5; ++j) {
                Position act = {pos.i - 2 + i,pos.j - 2 + j};
                int data_soldier = which_soldier(act.i,act.j);
                int owner_post = post_owner(act.i,act.j);
                if (data_soldier <= -1) continue;
                if (owner_post != -1 && !post_being_objective(Position(i,j)))
                    post = true;
                (which_soldier(act.i,act.j) == me() ? ++num_enemics : ++num_meus);
            }
        return ((num_meus < 3 && num_enemics > num_meus)
        ||      (num_enemics  >  2*num_meus-1)
        ||      (num_meus     < 2 && post));
    }

    void play_helicopter(int id) {

        // If we can, we throw napalm.
        Position pos = data(id).pos;
        if ((data(id).napalm == 0) && (napalm_QuestionMark(pos))) {
            Position hel = which_post(id);
            command_helicopter(id, NAPALM);
            return;
        }
        //Make function for helicopter

        //command_helicopter(id, CONTROL);
    }



      /*----------------
            MAIN
      ----------------*/

    virtual void play () {
        v_posts = posts();
        v_soldier = soldiers(me());
        v_helicopter = helicopters(me());
        v_cues = VIQ((int)v_soldier.size());

        for (int i = 0; i < (int)v_soldier.size(); ++i) {
            Position pos_obj = which_post(v_soldier[i]);
            Position pos_act = data(v_soldier[i]).pos;
            v_cues[i].first = pos_obj;

            BFS(pos_act, pos_obj,v_cues[i].second);

            Position next = v_cues[i].second.front(); v_cues[i].second.pop();
            command_soldier(v_soldier[i],next.i,next.j);
        }

        for (int i = 0; i < (int)v_helicopter.size(); ++i){
            if (not data(v_helicopter[i]).parachuters.empty())
                throw_parachuter(v_helicopter[i]);
            play_helicopter(v_helicopter[i]);
        }
    }
};

constexpr int PLAYER_NAME::I[8];
constexpr int PLAYER_NAME::J[8];
constexpr int PLAYER_NAME::HI[4];
constexpr int PLAYER_NAME::HJ[4];
/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);

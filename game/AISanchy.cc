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

    static constexpr int I[8] = {-1, -1,  0, 1, 1, 1, 0, 1 };
    static constexpr int J[8] = { 0, -1, -1,-1, 0, 1, 1, 1 };

    using VE    =   vector <int>;
    using VVE   =   vector <VE>;
    using VC    =   vector <char>;
    using VVC   =   vector <VC>;
    using QP    =   queue <Position>;
    using PP    =   pair <int,Position>;
    using VIQ   =   vector <pair <int,QP>>;

    VE              v_soldier;
    VE              v_helicopter;
    vector <Post>   v_posts;
    VVC             G;
    VVE             visitats;
    VIQ             v_cues;

    /*------------------
        R&&OM STUFF
    ------------------*/

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

    /*----------------
        SOLDIER
    ----------------*/

    bool heal_stuff (const Position &ally, const Position &enemy) {
        int Enemy_HP = data(which_soldier(enemy.i,enemy.j)).life;
        int Ally_HP  = data(which_soldier(ally.i,ally.j)).life;

        //same land
        if (what(ally.i,ally.j) == what(enemy.i,enemy.j)) {
            return Ally_HP > Enemy_HP;
        }

        //GRASS vs FOREST
        if (what(ally.i,ally.j)  > what(enemy.i,enemy.j)) {
            
        }
        //FOREST vs GRASS
        if (what(ally.i,ally.j)  < what(enemy.i,enemy.j)) {

        }


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

        while (not q.empty() && not trobat) {
            pair <Position, QP > p = q.front(); q.pop();

            for (int i = 0; i < 8 && not trobat; ++i) {
                QP route = p.second;
                Position seg = sum(p.first,Position(I[i],J[i]));
                if (not visitats[seg.i][seg.j]) {
                    if (pos_ok(seg) && what(seg.i,seg.j) != MOUNTAIN
                        && what(seg.i,seg.j) != WATER
                        && fire_time(seg.i,seg.j) <= route.size()
                        && (which_soldier(seg.i,seg.j) == 0 || (data(which_soldier(seg.i,seg.j)).player != me() &&  heal_stuff(p.first,seg)))) {
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
                int data_post = post_owner(act.i,act.j);
                if (data_soldier == -1)
                    continue;
                (which_soldier(act.i,act.j) == me() ? ++num_enemics : ++num_meus);

            }
        return ((num_meus < 3 && num_enemics > num_meus)
        ||      (num_enemics > 2*num_meus-1)
        ||      (num_meus    < 2 && post));
    }

    void play_helicopter(int id) {

        // If we can, we throw napalm.
        Position pos = data(id).pos;
        if ((data(id).napalm == 0) && (napalm_QuestionMark(pos))) {
            command_helicopter(id, NAPALM);
            return;
        }
        int c = random(1, 20);
        command_helicopter(id, c == 1 ? COUNTER_CLOCKWISE : FORWARD2);
    }

    void throw_parachuter(int helicopter_id) {
        // We get the data of the helicopter...
        Data in = data(helicopter_id);
        for (int i = 0; i < 5; ++i)
            for(int j = 0; j < 5; ++j){
                if ((not in.parachuters.empty())
                && (parachuter_QuestionMark(i,j))
                && (pos_ok(i,j)))
                    command_parachuter(i,j);
            }
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
            v_cues[i].first = v_soldier[i];
            //cerr << "id: " << v_cues[i].first;
            //cerr << " pos act: " << pos_act.i << " " << pos_act.j;
            //cerr << " pos_obj: " << pos_obj.i << " " << pos_obj.j << endl;

            BFS(pos_act, pos_obj,v_cues[i].second);

            Position next = v_cues[i].second.front(); v_cues[i].second.pop();
            command_soldier(v_soldier[i],next.i,next.j);
        }
        /*for (int i = 0; i < (int)v_helicopter.size(); ++i){
            if (not data(v_helicopter[i]).parachuters.empty())
                throw_parachuter(v_helicopter[i]);
            play_helicopter(v_helicopter[i]);
            if (data[v_helicopter[i]])
        }*/
    }
};

constexpr int PLAYER_NAME::I[8];
constexpr int PLAYER_NAME::J[8];

/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);

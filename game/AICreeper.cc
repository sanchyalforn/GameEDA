#include "Player.hh"

/**
* Write the name of your player and save this file
* with the same name and .cc extension.
*/

#define PLAYER_NAME Cubito


// DISCLAIMER: The following Demo player is *not* meant to do anything
// sensible. It is provided just to illustrate how to use the API.
// Please use AINull.cc as a template for your player.


struct PLAYER_NAME : public Player {

    static Player* factory () {
        return new PLAYER_NAME;
    }

    /*------------------
        DECLARACIONS
          GLOBALS
    ------------------*/
    static constexpr int I[8]  = {-1, -1,  0,  1, 1, 1, 0, 1 };
    static constexpr int J[8]  = { 0, -1, -1, -1, 0, 1, 1, 1 };
    static constexpr int HI[4] = { 1,  0, -1,  0};
    static constexpr int HJ[4] = { 0,  1,  0, -1};

    // --------------------------------------------

    const int S       = 0; //DOWN
    const int E       = 1; //RIGHT
    const int N       = 2; //UP
    const int W       = 3; //LEFT

    const int INFINIT = 1e9;

    // --------------------------------------------

    using VE    =   vector <int>;
    using VVE   =   vector <VE>;
    using VC    =   vector <char>;
    using VVC   =   vector <VC>;
    using QP    =   queue <Position>;
    using PP    =   pair <int,Position>;
    using VIQ   =   vector <pair <Position,QP>>;

    // --------------------------------------------

    VE                  v_soldier;
    VE                  v_helicopter;
    vector <Post>       v_posts;
    VVE                 visitats;
    VIQ                 v_cues_s;
    vector<queue<PP>>   v_cues_h;
    //set <int>           s_def;
    //set <int>           s_attack;

    /*------------------
        RANDOM STUFF
    ------------------*/

    //MANHATTAN DISTANCE FOR EURISTIC
    int manhattan_distance(const Position &p1, const Position &p2) {
        return abs(p1.i - p2.i) + abs(p1.j - p2.j);
    }

    //SUMM OF 2 POSITIONS
    Position sum(const Position &p1, const Position &p2) {
        return Position(p1.i + p2.i, p1.j + p2.j);
    }

    //IT CHOOSES THE POS TO GO FIRST OF ALL, LOOKING FOR THE NEAREST POST
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
                    if (aux.i == -1 or (manhattan_distance(temp,sold) < manhattan_distance(sold,aux)))
                        aux = temp;
                }
            }
        }
        return aux;
    }

    Position which_enemy(int id) {
        Position aux    = Position(-1,-1);
        Position sold   = data(id).pos;

        for (int i = 0; i < MAX; i++) {
            for (int j = 0; j < MAX; j++) {

                // Check if city (not my property)
                int id_sold = which_soldier(i,j);
                if (id_sold != 0 && id_sold != me() && data(id).life > data(id_sold).life) {

                    // Distance
                    if (aux.i == -1 or (manhattan_distance(temp,sold) < manhattan_distance(sold,aux)))
                        aux = Position(i,j);
                }
            }
        }
        return aux;
    }

    //NON EFFICIENT BFS THAT RETURNS THE ROUTE FROM POS ACT TO POST OBJ
    void BFS_soldier (const Position &i_pos,const Position &f_pos, QP &qp, int id){
        visitats = VVE (MAX, VE (MAX, false));
        queue <pair <Position,queue<Position>>> q;
        q.push({i_pos,queue<Position>()});
        bool trobat = false;
        visitats[i_pos.i][i_pos.j] = true;

        while ( !q.empty() &&  !trobat ) {
            pair <Position, QP > p = q.front(); q.pop();

            for (int i = 0; i < 8 && !trobat; ++i) {
                QP route = p.second;
                Position seg = sum(p.first,Position(I[i],J[i]));
                    if (! visitats[seg.i][seg.j]) {
                        if (pos_ok(seg)
                        && (what(seg.i,seg.j)          != MOUNTAIN
                        &&  what(seg.i,seg.j)          != WATER
                        && which_soldier(seg.i,seg.j)  == 0
                        && fire_time(seg.i,seg.j)      <= (int)route.size()
                        && (which_soldier(seg.i,seg.j) == 0))) {
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

    //DIJKSTRA THAT RETURNS THE ROUTE FROM POS ACT TO POS OBJ
    void Dijkstra() {

    }

    //LOOKS IF  ITS A GOOD PLACE TO THROW NAPALM
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

    //LOOKS IF ITS A GOOD PLACE TO THROW A PARACHUTER
    bool parachuter_QuestionMark(const Position &p) {
        int x = p.i;
        int y = p.j;
        if ((what(x,y) == WATER) or (what(x,y) == MOUNTAIN)) return false;
        if (fire_time(x,y) != 0) return false;
        if (which_soldier(x,y) != 0) return false;
        return true;
    }

    //IS THIS POST BEING THE OBJECTIVE OF ONE OF MY SOLDIERS?
    bool post_being_objective(const Position &pos) {
        for (int i = 0; i < (int)v_soldier.size(); ++i)
            if (v_cues_s[i].first.i == pos.i and v_cues_s[i].first.j == pos.j) return true;
        return false;
    }


    /*----------------
        SOLDIER
    ----------------*/

    void play_soldier(int id,int i) {

        if (s == "attack") { //SOLDIERS GO FOR OTHER POSTS
            Position act = data(id).pos;
            Position obj = which_post(id);
            BFS_soldier(act,obj,v_cues_s[i].second,id);
            Position next = v_cues_s[i].second.front();
            command_soldier(id,next.i,next.j);
        }

        if (s == "defend") { //SOLDIERS DEFEND THE POST

        }


    }

    //IF A POST MARKED IS STILL PART OF MY PROPERTY
    bool still_being_mine(const Position &pos) {
        return post_owner(pos.i,pos.j) == me();
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

    void play_helicopter(int id) {

        // If we can, we throw napalm.
        if (data(id).napalm == 0) {
            command_helicopter(id, NAPALM);
            return;
        }

        if (! data(id).parachuters.empty())
            throw_parachuter(id);


        int c = random(1, 5);
        command_helicopter(id, c == 1 ? COUNTER_CLOCKWISE : FORWARD2);
    }

    void throw_parachuter(int helicopter_id) {

        Data in = data(helicopter_id);
        for (int i = 0; i < 5; ++i)
            for(int j = 0; j < 5; ++j){
                if ((! in.parachuters.empty())
                && (parachuter_QuestionMark(Position(i,j)))
                && (pos_ok(i,j)))
                    command_parachuter(i,j);
            }
    }

    /**
    * Play method, invoked once per each round.
    */
    virtual void play () {

        int player      = me();
        v_helicopter = helicopters(player); // helicopters of my player
        v_soldier    = soldiers(player);    // soldiers of my player
        v_posts      = posts();
        v_cues_s     = VIQ((int)v_soldier.size());
        //v_cues_h     = ;

        for (int i = 0; i < (int)v_soldier.size(); ++i) {
            play_soldier(v_soldier[i],i);

        }

        for (int i = 0; i < (int)v_helicopter.size(); ++i) {
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

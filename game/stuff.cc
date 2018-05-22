#include "Player.hh"

/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */

#define PLAYER_NAME Maincra


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


    /*------------------
        RANDOM STUFF
    ------------------*/

    int manhattan_distance(const Position &p1, const Position &p2) {
        return abs(p1.i - p2.i) + abs(p1.j - p2.j);
    }

    Position sum(const Position &p1, const Position &p2) {
        return Position(p1.i + p2.i, p1.j + p2.j);
    }
/*
    void matrix_Dijkstra () {
        G = VVE(MAX,VE(MAX,INFINIT));
        for (int i = 0; i < MAX; ++i) {
            for (int j = 0; j < MAX; ++j) {
                if (fire_time(i,j) != 0) continue;
                if (what(i,j) == GRASS) G[i][j] = 3;
                else if (what(i,j) == FOREST) G[i][j] = 1;
                if (which_soldier(i,j) != 0) ++s;
                if ((which_soldier(i,j) != 0) && (data(which_soldier(i,j)).player != me()) && ) G[i][j] = 2;
                if ((post_value(i,j) != -2) && (post_value(i,j) != me())) G[i][j] = 'P';
            }
        }
    }
*/

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
                    && can_I_QuestionMark(seg,id)
                    && fire_time(seg.i,seg.j) <= (int)route.size()
                    && (which_soldier(seg.i,seg.j) == 0)) {
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

    void Dijkstra (const Position &i_pos, const Position &f_pos, QP &qp, int id) {

    }
/*
    int what(const Position& pos) {
        return what(pos.i,pos.j);
    }
*/
    bool can_I_QuestionMark(const Position& pos, int id) {
        if (!pos_ok(pos)) return false;
        if (data(id).type == HELICOPTER)
            return (what(pos.i,pos.j) != MOUNTAIN && which_helicopter(pos.i,pos.j) == 0);

        else {
            return (what(pos.i,pos.j)          != MOUNTAIN
                 && what(pos.i,pos.j)          != WATER
                 && which_soldier(pos.i,pos.j) == 0
                 &&  fire_time(pos.i,pos.j)    == 0);
        }
    }

    int new_orientation(const Position&act, const Position&next) {

        Position x = pos_sub(act,next);
        if (HI[0] ==  x.i && HJ[0] == x.j) return 0; //DOWN
        if (HI[1] ==  x.i && HJ[1] == x.j) return 1; //RIGHT
        if (HI[2] ==  x.i && HJ[2] == x.j) return 2; //UP
                                           return 3; //LEFT
    }

    bool hel_can_move(const Position &a, const Position &b, int id) {
        Position x = pos_sub(a,b);
        if (HI[0] ==  x.i && HJ[0] == x.j) return can_go_down(a,id); //DOWN
        if (HI[1] ==  x.i && HJ[1] == x.j) return can_go_right(a,id); //RIGHT
        if (HI[2] ==  x.i && HJ[2] == x.j) return can_go_up(a,id); //UP
        return can_go_left(a,id); //LEFT
    }

    void BFS_helicopter (const Position &inici, const Position &meta, queue<PP>&qp, int id) {
        visitats = VVE(MAX,VE(MAX,false));
        queue <pair<Position,queue <PP>>> Q;
        queue <PP> aux; aux.push({data(id).orientation,inici});
        Q.push({inici,aux});
        bool trobat = false;
        visitats[inici.i][inici.j] = true;

        while (not Q.empty() && !trobat) {

            pair <Position,queue <PP>> p = Q.front(); Q.pop();
            for (int i = 0; i < 4 && !trobat; ++i) {
                queue <PP> route = p.second;   //queue to actual pos
                int ori = route.front().first; //actual orientation
                Position next = sum(p.first,Position(HI[i],HJ[i]));
                cerr << "pos.i: " << route.front().second.i << " pos.j: " << route.front().second.j << endl;
                cerr << "next.i: " << next.i << " next.j: " << next.j << endl;
                if (! visitats[next.i][next.j]) {
                    if (pos_ok(next)
                    && can_I_QuestionMark(next,id)
                    && hel_can_move(p.first,next, id)) {
                        int new_ori = new_orientation(route.front().second,next);
                        cerr << "new_ori: " << new_ori << endl;
                        if (ori != new_ori){
                            route.push({new_ori,route.front().second}); //Same pos but diff ori
                            route.push({new_ori,next});
                            cerr << "ori != new_ori: i: " << next.i << " j: " << next.j << endl;
                        }

                        else
                            route.push({ori,next});

                        Q.push({next,route});
                        visitats[next.i][next.j] = true;
                    }

                    if (next.i == meta.i && next.j == meta.j){
                        qp = route;
                        trobat = true;
                    }
                }
            }
        }
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
                    if (aux.i == -1 or (manhattan_distance(temp,sold) < manhattan_distance(sold,aux)))
                        aux = temp;
                }
            }
        }
        return aux;
    }

    bool parachuter_QuestionMark(const Position &p) {
        int x = p.i;
        int y = p.j;
        if ((what(x,y) == WATER) or (what(x,y) == MOUNTAIN)) return false;
        if (fire_time(x,y) != 0) return false;
        if (which_soldier(x,y) != 0) return false;
        return true;
    }

    bool napalm_QuestionMark(const Position &pos) {
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

    bool post_being_objective(const Position &pos) {
        for (int i = 0; i < (int)v_soldier.size(); ++i)
            if (v_cues_s[i].first.i == pos.i and v_cues_s[i].first.j == pos.j) return true;
        return false;
    }


    /*----------------
        SOLDIER
    ----------------*/

    void play_soldier(int id, QP &qp) {
        Position obj = which_post(id);
        Position act = data(id).pos;
        BFS_soldier(act,obj,qp,id);
    }

    void move_soldier(int id, int i) {
        Position move = v_cues_s[i].second.front(); v_cues_s[i].second.pop();
        command_soldier(id,move.i,move.j);
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

    bool can_go_up(const Position &pos, int id){
        for (int i = -4; i <= -2; ++i)
            for (int j = -4; j <= 4; ++j){
                int aux_i = pos.i + i;
                int aux_j = pos.i + j;
                Position aux = Position(aux_i,aux_j);
                if (!can_I_QuestionMark(aux,id)) return false;
            }
        return true;
    }

    bool can_go_left(const Position &pos, int id){
        for (int i = -5; i <= 5; ++i)
            for (int j = -3; j <= -2; ++j){
                int aux_i = pos.i + i;
                int aux_j = pos.i + j;
                Position aux = Position(aux_i,aux_j);
                if (!can_I_QuestionMark(aux,id)) return false;
            }
        return true;
    }

    bool can_go_right(const Position &pos, int id){
        for (int i = -5; i <= 5; ++i)
            for (int j = 2; j <= 3; ++j){
                int aux_i = pos.i + i;
                int aux_j = pos.i + j;
                Position aux = Position(aux_i,aux_j);
                if (!can_I_QuestionMark(aux,id)) return false;
            }
        return true;
    }

    bool can_go_down(const Position &pos, int id){
        for (int i = 2; i <= 4; ++i)
            for (int j = -4; j <= 4; ++j){
                int aux_i = pos.i + i;
                int aux_j = pos.i + j;
                Position aux = Position(aux_i,aux_j);
                if (!can_I_QuestionMark(aux,id)) return false;
            }
        return true;
    }


    void play_helicopter(int id, queue<PP> &qp) {

        if (data(id).napalm == 0 && napalm_QuestionMark(data(id).pos)) {
            command_helicopter(id, NAPALM);
            return;
        }

        if (! data(id).parachuters.empty() && parachuter_QuestionMark(data(id).pos))
            throw_parachuter(id);

        //Position obj = which_post(id);
        Position act = data(id).pos;
        int c = random(1, 5);
        int ori = data(id).orientation;
        Position next = sum(act,Position(I[ori],J[ori]));
        if (what(next.i,next.j) == MOUNTAIN or !hel_can_move(act,next,id)) c = COUNTER_CLOCKWISE;
        command_helicopter(id, c == 1 ? COUNTER_CLOCKWISE : FORWARD2);
        /*BFS_helicopter(act,obj,qp,id);
        cerr << "obj_i: "  << obj.i << " obj_j: " << obj.j << endl;
        cerr << "next_i: " << qp.front().second.i << " next_j: " <<  qp.front().second.j << endl;
        cerr << "orientation: " <<  data(id).orientation << "orien_next" << qp.front().first << endl;*/
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

    /*void move_helicopter (int id,int i) {
        int code;
        PP move = v_cues_h[i].front();
        int act_ori = data(id).orientation;

        if (act_ori == move.first) {
            v_cues_h[i].pop();
            if (v_cues_h[i].front().first == act_ori){
                code = FORWARD2;
                v_cues_h[i].pop();
            }
            else
                code = FORWARD1;
        }

        else {
            if (act_ori == N) {
                if (move.first == E) code = CLOCKWISE;
                else if (move.first == S) code =  random(1, 2) ? CLOCKWISE : COUNTER_CLOCKWISE;
                else code = COUNTER_CLOCKWISE;
            }
            else if (act_ori == E) {
                if (move.first == S) code = CLOCKWISE;
                else if (move.first == W) code =  random(1, 2) ? CLOCKWISE : COUNTER_CLOCKWISE;
                else code = COUNTER_CLOCKWISE;
            }
            else if (act_ori == S) {
                if (move.first == W) code = CLOCKWISE;
                else if (move.first == N) code =  random(1, 2) ? CLOCKWISE : COUNTER_CLOCKWISE;
                else code = COUNTER_CLOCKWISE;
            }
            else {
                if (move.first == N) code = CLOCKWISE;
                else if (move.first == E) code =  random(1, 2) ? CLOCKWISE : COUNTER_CLOCKWISE;
                else code = COUNTER_CLOCKWISE;
            }

        }
        command_helicopter(id, code);
    }*/

    /*----------------
          MAIN
    ----------------*/

    virtual void play () {
        v_soldier    = soldiers(me());
        v_helicopter = helicopters(me());
        v_posts      = posts();
        v_cues_s     = VIQ((int)v_soldier.size());
        v_cues_h     = vector<queue<PP>>((int)v_helicopter.size());

        for (int i = 0; i < (int)v_soldier.size(); ++i) {
            play_soldier(v_soldier[i], v_cues_s[i].second);
            move_soldier(v_soldier[i], i);

        }

        for (int i = 0; i < (int)v_helicopter.size();++i){
            play_helicopter(v_helicopter[i],v_cues_h[i]);
            //move_helicopter(v_helicopter[i], i);
        }
    /*    if (round() == 0) {
            Position act = data(v_helicopter[0]).pos;
            Position obj = which_post(v_helicopter[0]);
            queue <PP> a;
            BFS_helicopter(act,obj,a,v_helicopter[0]);
        }
        */
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
    using P     =   pair <int,int>;
    using VIQ   =   vector <pair <Position,QP>>;

    // --------------------------------------------

    VE                  v_soldier;
    VE                  v_helicopter;
    vector <Post>       v_posts;
    VIQ                 v_cues_s;
    vector<queue<PP>>   v_cues_h;
    VVE                 visitats;
    VVE                 dist;

    /*------------------
        RANDOM STUFF
    ------------------*/

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

    //MANHATTAN DISTANCE FOR EURISTIC
    int manhattan_distance(const Position &p1, const Position &p2) {
        return abs(p1.i - p2.i) + abs(p1.j - p2.j);
    }

    //SUMM OF 2 POSITIONS
    Position sum(const Position &p1, const Position &p2) {
        return Position(p1.i + p2.i, p1.j + p2.j);
    }

    //IT CHOOSES THE POS TO GO FIRST OF ALL, LOOKING FOR THE NEAREST POST
    Position which_pos(int id) {
        Position aux    = Position(-1,-1);
        Position sold   = data(id).pos;

        for (int i = 0; i < MAX; ++i)
            for (int j = 0; j < MAX; ++j) {

                // Get Position
                Position temp = Position(i, j);

                if (which_soldier(i,j) != 0 && data(which_soldier(i,j)).player != me()) {
                    if (aux.i == -1 || ((manhattan_distance(temp,temp) < manhattan_distance(sold,aux)) && data(id).life > data(which_soldier(i,j)).life))
                        aux = temp;
                }
                // Check if city (not my property)
                else if ((post_owner(temp.i,temp.j) != -2) && (post_owner(temp.i,temp.j) != me())) {

                    // Distance
                    if (aux.i == -1 or (manhattan_distance(temp,sold) < manhattan_distance(sold,aux)))
                        aux = temp;
                }
            }
        return aux;
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


        Position act = data(id).pos;
        Position obj = which_pos(id);
        v_cues_s[i].first = obj;
        BFS_soldier(act,obj,v_cues_s[i].second,id);
        Position next = v_cues_s[i].second.front(); v_cues_s[i].second.pop();
        command_soldier(id,next.i,next.j);

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

    //LOOKS IF ITS A GOOD PLACE TO THROW A PARACHUTER
    bool parachuter_QuestionMark(const Position &p) {
        int x = p.i;
        int y = p.j;
        if ((what(x,y) == WATER) or (what(x,y) == MOUNTAIN)) return false;
        if (fire_time(x,y) != 0) return false;
        if (which_soldier(x,y) != 0) return false;
        return true;
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
                (which_soldier(act.i,act.j) == me() ? ++num_meus : ++num_enemics);
            }
        return ((num_meus < 3 && num_enemics > num_meus)
        ||      (num_enemics  >  2*num_meus-1)
        ||      (num_meus     < 2 && post));
    }

    void throw_parachuter(int helicopter_id) {

        Data in = data(helicopter_id);
        for (int i = 0; i < 5; ++i)
            for(int j = 0; j < 5; ++j){
                if ((! in.parachuters.empty())
                && (pos_ok(i,j))
                && parachuter_QuestionMark(Position(i,j)))
                    command_parachuter(i,j);
            }
    }

    void play_helicopter(int id) {

        // If we can, we throw napalm.
        if (data(id).napalm == 0) {
            command_helicopter(id, NAPALM);
            return;
        }

        if (!data(id).parachuters.empty())
            throw_parachuter(id);

        int c = random(1, 20);
        command_helicopter(id, c == 1 ? COUNTER_CLOCKWISE : FORWARD2);
    }
    /*----------------
          MAIN
    ----------------*/

    virtual void play () {

        int player   = me();
        v_helicopter = helicopters(player); // helicopters of my player
        v_soldier    = soldiers(player);    // soldiers of my player
        v_posts      = posts();
        v_cues_s     = VIQ((int)v_soldier.size());

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

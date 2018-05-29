#include "Player.hh"
#include <stack>

/**
* Write the name of your player and save this file
* with the same name and .cc extension.
*/

#define PLAYER_NAME PDeGAT


// DISCLAIMER: The following Demo player is *not* meant to do anything
// sensible. It is provided just to illustrate how to use the API.
// Please use AINull.cc as a template for your player.


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

    static constexpr int I[8] =  { 1, 1, 0, -1, -1, -1,  0,  1 };
    static constexpr int J[8] =  { 0, 1, 1,  1,  0, -1, -1, -1 };
    static constexpr int HI[4] = { 1,  -1,  0,  0};
    static constexpr int HJ[4] = { 0,   0,  1, -1};

    // --------------------------------------------

    const int S       = 0; //DOWN
    const int E       = 1; //RIGHT
    const int N       = 2; //UP
    const int W       = 3; //LEFT

    const int radius  = 2;
    const int INFINIT = 1e9;

    // --------------------------------------------

    using VE    =   vector <int>;
    using VVE   =   vector <VE>;
    using QP    =   queue <Position>;
    using VP    =   vector <Position>;
    using VVP   =   vector <VP>;

    // --------------------------------------------

    VE                  v_soldier;
    VE                  v_helicopter;
    vector <Post>       v_posts;
    VVE                 visitats;
    stack<Position>     Sl;
    stack<Position>     H;

    /*------------------
        RANDOM STUFF
    ------------------*/

    int manhattan_distance(const Position &p1, const Position &p2) {
        return abs(p1.i - p2.i) + abs(p1.j - p2.j);
    }

    Position suma(const Position &a, const Position &b) {
        return Position(a.i+b.i,a.j+b.j);
    }

    //IT CHOOSES THE POS TO GO FIRST OF ALL, LOOKING FOR THE NEAREST POST
    Position which_post(const Position &sold) {
        Position aux = Position(-1,-1);

        for (int i = 0; i < MAX; ++i)
            for (int j = 0; j < MAX; ++j) {

                // Get Position
                Position temp = Position(i, j);

                // Check if city (not my property)
                if ((post_owner(temp.i,temp.j) != -2) && (post_owner(temp.i,temp.j) != me())) {

                    // Distance
                    if (aux.i == -1 || (manhattan_distance(temp,sold) < manhattan_distance(sold,aux)))
                        aux = temp;
                }
            }
        return aux;
    }

    /*----------------
        SOLDIER
    ----------------*/

    void BFS_S (const Position &act, const Position &obj) {
        VVP pare (MAX,VP(MAX,Position(-1,-1)));
        pare[act.i][act.j] = act;
        queue <Position> Q; Q.push(act);

        while (!Q.empty()) {
            Position p = Q.front(); Q.pop();
            for (int i = 0; i < 8; ++i) {
                Position next = suma(p,Position(I[i],J[i]));
                if (pos_ok(next)
                &&  (pare[next.i][next.j].i == -1 or pare[next.i][next.j].j == -1)
                &&  fire_time(next.i,next.j) == 0
                &&  what(next.i,next.j) != MOUNTAIN
                &&  what(next.i,next.j) != WATER
                &&  which_soldier(next.i,next.j) == 0) {
                    pare[next.i][next.j] = p;
                    //cerr << p.i << ' '<<p.j<< endl;
                    Q.push(next);
                    //cerr << next.i << ' ' << next.j << " - "<< endl;
                }

                if (next.i == obj.i && next.j == obj.j){
                    pare[next.i][next.j] = p;
                    Sl = stack <Position>();
                    //cerr << S.size() << endl;
                    //cerr << next.i << ' ' << next.j << endl;
                    //cerr << pare[next.i][next.j].i << ' ' << pare[next.i][next.j].j << endl;
                    while (! (next.i == act.i && next.j == act.j)) {
                        Sl.push(next);

                        next = pare[next.i][next.j];
                    }
                    return;
                }
            }
        }
    }

    bool enemy_near (const Position &pos) {
        int my_life = data(which_soldier(pos.i,pos.j)).life;
        for (int i = pos.i - radius; i < pos.i + radius;  ++i)
            for (int j = pos.j - radius; j < pos.j + radius; ++j) {
                if (pos_ok(i,j) && which_soldier(i,j) != 0 && data(which_soldier(i,j)).player != me())
                    if ( my_life >= data(which_soldier(i,j)).life)
                        return true;
            }
            return false;
    }

    Position which_enemy(const Position &pos) {
        Position aux = Position(-1,-1);
        for (int i = pos.i - radius; i < pos.i + radius;  ++i)
            for (int j = pos.j - radius; j < pos.j + radius; ++j) {
                if (pos_ok(i,j) && which_soldier(i,j) != 0 && data(which_soldier(i,j)).player != me()) {
                    if (aux.i == -1
                    || (manhattan_distance(pos,Position(i,j)) < manhattan_distance(pos,aux)
                &&  data(which_soldier(pos.i,pos.j)).life >= data(which_soldier(i,j)).life))
                        aux = Position(i,j);
                }
            }
            return aux;
    }

    void play_soldier(int id) {
        Position act = data(id).pos;
        Position obj = (enemy_near(act) ? which_enemy(act) : which_post(act));

        BFS_S(act,obj);
        //cout << "arribo aqui" << round() << endl;
        Position x = Sl.top();
        //cerr << x.i <<' ' << x.j << endl;
        (manhattan_distance(act,obj) == 1 ? command_soldier(id,obj.i,obj.j) : command_soldier(id,x.i,x.j));

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

    int a(const Position &a) {
        if (HI[0] ==  a.i && HJ[0] == a.j) return 0; //DOWN
        if (HI[1] ==  a.i && HJ[1] == a.j) return 1; //RIGHT
        if (HI[2] ==  a.i && HJ[2] == a.j) return 2; //UP
                                           return 3; //LEFT
    }

    void BFS_H (const Position &act, const Position &obj) {
        VVP pare (MAX,VP(MAX,Position(-1,-1)));
        pare[act.i][act.j] = act;
        queue <Position> Q; Q.push(act);
        int id = which_helicopter(act.i,act.j);

        while (!Q.empty()) {
            Position p = Q.front(); Q.pop();
            for (int i = 0; i < 4; ++i) {
                Position next = suma(p,Position(HI[i],HJ[i]));
                if (pos_ok(next)
                &&  (pare[next.i][next.j].i == -1 or pare[next.i][next.j].j == -1)
                &&  fire_time(next.i,next.j) == 0
                &&  what(next.i,next.j) != MOUNTAIN
                &&  can_move(p,next,id)) {
                    pare[next.i][next.j] = p;
                    Q.push(next);
                }
                cerr << "arribo" << endl;
                if (next.i == obj.i && next.j == obj.j){
                    pare[next.i][next.j] = p;

                    while (! (next.i == act.i && next.j == act.j)) {
                        cerr << "he entrat i size: " << H.size() << endl;
                        H.push(next);

                        next = pare[next.i][next.j];
                    }
                    return;
                }
            }
        }
    }

    void play_helicopter(int id) {

        // If we can, we throw napalm.
        if (data(id).napalm == 0) {
            command_helicopter(id, NAPALM);
            return;
        }

        if (! data(id).parachuters.empty())
            throw_parachuter(id);


        Position obj =  (round() > 15 ? Position(30,30) : which_post(data(id).pos));
        H = stack <Position>();
        cerr << "peta aqui?" << endl;
        BFS_H(data(id).pos,obj);
        cerr << "o aqui?" << endl;
        int ori = data(id).orientation;
        cerr << H.size() << endl;
        auto Hel = H.top(); H.pop();
        cerr << "i aqui?" << endl;
        Position vector_unitar = Position (Hel.i -data(id).pos.i,Hel.j - data(id).pos.j);
        int new_orientation = a(vector_unitar);
        int c;
        cerr << "position act: i: " << data(id).pos.i << " j: " << data(id).pos.j << endl;
        cerr << "Position obj: i: "<< obj.i << " j: " << obj.j <<  endl;
        cerr << "next step: i: " << Hel.i << " j: " << Hel.j << endl;
        cerr << "orientation: " << ori << " new_ori: " << new_orientation << endl;
        if (ori == new_orientation) c = FORWARD1;
        else {
            if (ori == N) {
                if (new_orientation == E) c = CLOCKWISE;
                else if (new_orientation == S) c =  random(1, 2) ? CLOCKWISE : COUNTER_CLOCKWISE;
                else c = COUNTER_CLOCKWISE;
            }

            else if (ori == E) {
                if (new_orientation == S) c = CLOCKWISE;
                else if (new_orientation == W) c =  random(1, 2) ? CLOCKWISE : COUNTER_CLOCKWISE;
                else c = COUNTER_CLOCKWISE;
            }

            else if (ori == S) {
                if (new_orientation == W) c = CLOCKWISE;
                else if (new_orientation == N) c =  random(1, 2) ? CLOCKWISE : COUNTER_CLOCKWISE;
                else c = COUNTER_CLOCKWISE;
            }

            else {
                if (new_orientation == N) c = CLOCKWISE;
                else if (new_orientation == E) c =  random(1, 2) ? CLOCKWISE : COUNTER_CLOCKWISE;
                else c = COUNTER_CLOCKWISE;
            }
        }
        //cerr << "surto amb c =" << c << endl;
        command_helicopter(id,c);

    }


        bool can_move(const Position &a, const Position &b, int id) {
            Position x = Position(b.i-a.i, b.j-a.j);
            if (HI[0] ==  x.i && HJ[0] == x.j) return can_go_down(a,id); //DOWN
            if (HI[1] ==  x.i && HJ[1] == x.j) return can_go_right(a,id); //RIGHT
            if (HI[2] ==  x.i && HJ[2] == x.j) return can_go_up(a,id); //UP
                                               return can_go_left(a,id); //LEFT
        }

        bool can_go_up(const Position &pos, int id){
            for (int i = -4; i <= -2; ++i)
                for (int j = -4; j <= 4; ++j){
                    int aux_i = pos.i + i;
                    int aux_j = pos.i + j;
                    Position aux = Position(aux_i,aux_j);
                    if (what(aux.i,aux.j) == MOUNTAIN || which_helicopter(aux.i,aux.j) != 0) return false;
                }
            return true;
        }

        bool can_go_left(const Position &pos, int id){
            for (int i = -5; i <= 5; ++i)
                for (int j = -3; j <= -2; ++j){
                    int aux_i = pos.i + i;
                    int aux_j = pos.i + j;
                    Position aux = Position(aux_i,aux_j);
                    if (what(aux.i,aux.j) == MOUNTAIN || which_helicopter(aux.i,aux.j) != 0) return false;
                }
            return true;
        }

        bool can_go_right(const Position &pos, int id){
            for (int i = -5; i <= 5; ++i)
                for (int j = 2; j <= 3; ++j){
                    int aux_i = pos.i + i;
                    int aux_j = pos.i + j;
                    Position aux = Position(aux_i,aux_j);
                    if (what(aux.i,aux.j) == MOUNTAIN || which_helicopter(aux.i,aux.j) != 0) return false;
                }
            return true;
        }

        bool can_go_down(const Position &pos, int id){
            for (int i = 2; i <= 4; ++i)
                for (int j = -4; j <= 4; ++j){
                    int aux_i = pos.i + i;
                    int aux_j = pos.i + j;
                    Position aux = Position(aux_i,aux_j);
                    if (what(aux.i,aux.j) == MOUNTAIN || which_helicopter(aux.i,aux.j) != 0) return false;
                }
            return true;
        }

    //LOOKS IF  ITS A GOOD PLACE TO THROW A PARACHUTER
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
                (data(which_soldier(act.i,act.j)).player == me() ? ++num_meus : ++num_enemics);
            }
        return ((num_meus < 3 && num_enemics > num_meus)
        ||      (num_enemics  >  2*num_meus-1)
        ||      (num_meus     < 2 && post));
    }

    void throw_parachuter(int helicopter_id) {
        // We get the data of the helicopter...
        Data in = data(helicopter_id);
        Position pos = in.pos;
        int np = 4;
        for (int i = pos.i-2; i < pos.i+2 && np > 0 ; ++i)
            for (int j = pos.j-2; j < pos.j +2 && np > 0; ++j) {
                if (!in.parachuters.empty() && parachuter_QuestionMark(Position(i,j))) {
                    command_parachuter(i, j);
                    --np;
                }
            }
    }


    /**
    * Play method, invoked once per each round.
    */
    virtual void play () {
        int player   = me();
        v_soldier    = soldiers(player);
        v_helicopter = helicopters(player);


        for (int i = 0; i < (int)v_soldier.size(); ++i)
            play_soldier(v_soldier[i]);
        for (int i = 0; i < (int)v_helicopter.size(); ++i)
            play_helicopter(v_helicopter[i]);


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

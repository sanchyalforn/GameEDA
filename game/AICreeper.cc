#include "Player.hh"
#include <stack>

/**
* Write the name of your player and save this file
* with the same name and .cc extension.
*/

#define PLAYER_NAME Maincra


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

    static constexpr int I[8]  = {-1, -1,  0,  1,  0,  1,  0, -1 };
    static constexpr int J[8]  = { 0,  1,  1,  1, -1, -1, -1, -1 };
    static constexpr int HI[4] = { 1,  0, -1,  0};
    static constexpr int HJ[4] = { 0,  1,  0, -1};

    // --------------------------------------------
/*
    const int S       = 0; //DOWN
    const int E       = 1; //RIGHT
    const int N       = 2; //UP
    const int W       = 3; //LEFT
*/
    const int radius  = 2;

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
    stack<Position>     S;
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

    int BFS(const Position &act, const Position &obj){

        queue <pair<Position,int>> Q;
    	Q.push({act,-1});
    	visitats = VVE(MAX,VE(MAX,false));
    	visitats[act.i][act.j] = true;
        int aux = 0;
        bool found = false;
    	while (!Q.empty() && !found) {
    		auto p = Q.front(); Q.pop();
            for (int i = 0; i < 8 && !found; ++i) {
                int dir = (p.second == -1 ? i : p.second);
    			Position next = suma(p.first,Position(I[i],J[i]));
                if (pos_ok(next)) {
                    if (!visitats[next.i][next.j]) {
                        if (what(next.i,next.j) != MOUNTAIN
                        &&  what(next.i,next.j) != WATER
                        &&  fire_time(next.i,next.j) == 0){
                            Q.push({next,dir});
                            visitats[next.i][next.j] = true;
                        }
                    }
                }
                if (next.i == obj.i && next.j == obj.j)
                    return p.second;
            }
    	}
        return -1;
    }

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
                Position next = suma(p.first,Position(I[i],J[i]));
                    if (! visitats[next.i][next.j]) {
                        if (pos_ok(next)
                        &&  what(next.i,next.j) != MOUNTAIN
                        &&  what(next.i,next.j) != WATER
                        &&  fire_time(next.i,next.j) == 0
                        &&  which_soldier(next.i,next.j) == 0) {
                        route.push(Position(next.i,next.j));
                        q.push({next,route});
                        visitats[next.i][next.j] = true;
                    }

                    if (next.i == f_pos.i  && next.j == f_pos.j){
                        qp = route;
                        trobat = true;
                    }
                }
            }
        }
    }

    void BFS_ (const Position &act, const Position &obj) {
        VVP pare (MAX,VP(MAX,Position(-1,-1)));
        pare[act.i][act.j] = act;
        queue <Position> Q;
        Q.push(act);

        while (!Q.empty()) {
            auto p = Q.front(); Q.pop();

            for (int i = 0; i < 8; ++i) {
                Position next = suma(p,Position(I[i],J[i]));
                if (pos_ok(next)
                &&  pare[next.i][next.j].i == -1
                &&  fire_time(next.i,next.j) == 0
                &&  what(next.i,next.j) != MOUNTAIN
                &&  what(next.i,next.j) != WATER
                &&  which_soldier(next.i,next.j) < 1) {
                    pare[next.i][next.j] = p;
                }
                cerr <<"arribo " << i << endl;
                if (next.i == obj.i && next.j == obj.j){
                    cerr <<"arribo " << i << endl;
                    S = stack <Position>();
                    while (next.i != act.i && next.j != act.j) {
                        S.push(next);
                        next = pare[next.i][next.j];
                        return;
                    }
                }
            }
        }
    }

    Position enemy_near (const Position &pos) {
        int my_life = data(which_soldier(pos.i,pos.j)).life;
        for (int i = pos.i - radius; i < pos.i + radius;  ++i)
            for (int j = pos.j - radius; j < pos.j + radius; ++j) {
                if (pos_ok(i,j) && which_soldier(i,j) != 0 && data(which_soldier(i,j)).player != me())
                    if ( my_life >= data(which_soldier(i,j)).life)
                        return Position(i,j);
            }
            return Position(-1,-1);
    }

    void play_soldier(int id) {
        Position act = data(id).pos;
        Position obj  = enemy_near(act);
        if (obj.i == -1) obj = which_post(act);
        cerr << "pos i: " << act.i << " act.j: " << act.j << endl;
        cerr << "obj i: " << obj.i << " obj.j: " << obj.j << endl;
        QP qp;
        BFS_soldier(act,obj,qp,id);
        Position next = qp.front();
        cerr << "next i: " << next.i << " next.j: " << next.j << endl;
        command_soldier(id,next.i,next.j);
        //BFS_(act,obj);
        //auto x = S.top();
        //command_soldier(id,x.i,x.j);
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

        // With probability 20% we turn counter clockwise,
        // otherwise we try to move forward two steps.
        int c = random(1, 5);
        command_helicopter(id, c == 1 ? COUNTER_CLOCKWISE : FORWARD2);
    }

    void throw_parachuter(int id) {
        Position x = data(id).pos;
        for (int i = -2; i < 2; ++i)
            for (int j = -2; j < 2; ++j) {
                int x_i = x.i + i;
                int x_j = x.j + j;
                if (pos_ok(x_i,x_j) && ! data(id).parachuters.empty() && parachuter_QuestionMark(Position(x_i,x_j)))
                    command_parachuter(x_i,x_j);
            }
    }

    /**
    * Play method, invoked once per each round.
    */
    virtual void play () {
        int player   = me();
        v_soldier    = soldiers(player);
        v_helicopter = helicopters(player);

        for (int i = 0; i < (int)v_helicopter.size(); ++i)
            play_helicopter(v_helicopter[i]);

        for (int i = 0; i < (int)v_soldier.size(); ++i)
            play_soldier(v_soldier[i]);

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

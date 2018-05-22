#include "Player.hh"

/**
* Write the name of your player and save this file
* with the same name and .cc extension.
*/

#define PLAYER_NAME Gelocatil


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
    using QP    =   queue <Position>;

    // --------------------------------------------

    VE                  v_soldier;
    VE                  v_helicopter;
    vector <Post>       v_posts;
    VVE                 visitats;

    /*------------------
        RANDOM STUFF
    ------------------*/

    bool parachuter_QuestionMark(const Position &p) {
        int x = p.i;
        int y = p.j;
        if ((what(x,y) == WATER) or (what(x,y) == MOUNTAIN)) return false;
        if (fire_time(x,y) != 0) return false;
        if (which_soldier(x,y) != 0) return false;
        return true;
    }

    /*----------------
        SOLDIER
    ----------------*/

    void play_soldier(int id) {

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

    void throw_parachuter(int helicopter_id) {
        Position x = data(helicopter_id).pos;
        for (int i = -2; i < 2; ++i)
            for (int j = -2; j < 2; ++j) {
                int x_i = x.i + i;
                int x_j = x.j + j;
                if (pos_ok(x_i,x_j) && ! data(id).parachuters.empty() && parachuter_QuestionMark(x_i,x_j))
                    command_parachuter(x_i,x_j);
            }
        int ii = in.pos.i + random(-2, 2);
        int jj = in.pos.j + random(-2, 2);
        if (ii >= 0 and ii < MAX and jj >= 0 and jj < MAX)
            command_parachuter(ii, jj);
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

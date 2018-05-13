#include "Player.hh"
#include <cmath>
#include <stack>



/**
* Write the name of your player and save this file
* with the same name and .cc extension.
*/
#define PLAYER_NAME Maincra


struct PLAYER_NAME : public Player {

/**
* Factory: returns a new instance of this class.
* Do not modify this function.
*/

    static Player* factory () {
        return new PLAYER_NAME;
    }

    using VE = vector <int>;
    using VVE = vector <VE>;
    using VP = vector <Post>;
    using S = stack<int>;
    const int my_id = me();
    using P = pair <int,int>;
    int num_soldiers = 20;
    int num_parachuters = 0;

    VE v_soldiers (20);
    VE v_helicopters (2);
    VE v_parachuters (0);
    VP post_no_capturats (16);
    VP v_posts (16);
    S s;

    bool fora_limits(P p) {
        return not (p.first >= 0 and p.first < MAX and p.second < MAX and p.second >= 0);
    }


    void Dijkstra(int x, int y) {
        if (fora_limits({x,y}) or fire_time(x,y) > 0 or what(i,j) == 4) return;

    }

    /**
    * Play method, invoked once per each round.
    */
    virtual void play () {
        vsoldiers = soldiers(my_id);
        vhelicopters = helicopters(my_id);
        vposts = posts();
        if (round() == 0)
            vposts = posts();
        else {

        }
    }

};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);

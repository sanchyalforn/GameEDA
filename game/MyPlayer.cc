#include "Player.hh"
#include <cmath>



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
    using MEE = map <int,int>;
    using MI = map<int,int>::iterator;
    const int my_id = me();
    using P = pair <int,int>;

    VE v_soldiers;
    VE vhelicopters;
    VE vparachuters;
    VE vposts;


    //Funcio per escollir on tirar el napalm;
    void Napalm() {}

    //Funcio per escollir on tirar els parachuters;
    void Parachuters() {}

    //Funcio per guiar els soldatss
    void soldiers() {}

    /*Funcio per saber si atacar o no a un soldat

    bool atack_or_not() {

    }*/

    /*Decidir si un soldat va a una pos o no
    */
    bool go (P p1, P p2) {
        if (fire_time(p2.first,p2.second) == 0) return true;
        return distance(p1,p2) >= fire_time(p.first,p.second);
    }

    int distance (P p1, P p2) {
        int sum1 = (p2.first - p1.first)*(p2.first - p1.first) ;
        int sum2 = (p2.second - p1.second)*(p2.second - p1.second);
        return sqrt(sum1+sum2);
    }

    /**
    * Play method, invoked once per each round.
    */
    virtual void play () {
        vsoldiers = soldiers(my_id);
        vhelicopters = helicopters(my_id);
        if ( round() == 0) {
            vposts = posts;
        }
        else {

        }
    }

};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);

#include "Player.hh"



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


    //Funcio per escollir on tirar el napalm;
    void Napalm() {}

    //Funcio per escollir on tirar els parachuters;
    void Parachuters() {}

    //Funcio per guiar els soldatss
    void soldiers() {}

    //Funcio per saber si atacar o no a un soldat

    bool atack_or_not() {

    }

    /**
    * Play method, invoked once per each round.
    */
    virtual void play () {

    }

};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);

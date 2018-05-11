#include "Player.hh"


/**
* Write the name of your player and save this file
* with the same name and .cc extension.
*/
#define PLAYER_NAME Null


struct PLAYER_NAME : public Player {

/**
* Factory: returns a new instance of this class.
* Do not modify this function.
*/


    static Player* factory () {
        return new PLAYER_NAME;
    }


    //Funcio per escollir on tirar el napalm;
    void Napalm() {}

    //Funcio per escollir on tirar els parachuters;
    void Parachuters() {}

    //Funcio per guiar els soldatss
    void soldiers() {}

    //Funcio per saber si atacar o no dins un radi d'alcance i sabent el tipus de terreny

    bool atack_or_not(int radius, <T> terreny) {}

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

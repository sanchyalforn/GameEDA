#include "Player.hh"
#include <utility>
#include <queue>
using namespace std;

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
    using P = pair <int,int>;


    //FOREST GRASS WATER MOUNTAIN

    /* NAPALM

    * * * * *
    * * * * *
    * * H * *
    * * * * *
    * * * * *

    */

    VVE visitats;
    VE v_soldiers;
    VE v_helicopters;
    vector <Post> v_posts;

    /*bool napalm_QuestionMark(P pos) {
        int num_enemics = 0;
        int num_meus = 0;
        bool yes_OMG = false;
        for (int i = 0; i < 5; ++i)
            for (int j = 0; j < 5; ++j) {
                if ([pos.x - 2 + i][pos.x - 2 + j])
                    ++ num_enemics;
                if ([pos.x - 2 + i][pos.x - 2 + j])
                    ++num_meus;

            }
        return (num_enemics >  or (num_meus < 3 and num_enemics > num_meus) or (num_enemics > 2*num_meus-1))
    }*/

    /**
    * Play method, invoked once per each round.
    */
    virtual void play () {

        if (status() > 0.95) return;
        if (round() == 0){
            v_posts = posts();
            int post_size = v_posts.size();
            for (int i = 0; i < post_size; ++i) {

            }
        }
    }

};


/**
* Do not modify the following line.
*/
RegisterPlayer(PLAYER_NAME);

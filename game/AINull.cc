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


    using VE    = vector <int>;
    using VVE   = vector <VE>;
    using QP    = queue <Position>;
    using VQ    = vector <QP>;



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
    VQ v_cues;
    int i = 0;

    //EL PUNT ESTA FORA LIMITS?

    bool fora_limits(int i, int j) {
        return (i < 0 or i >= MAX or j < 0 or j >= MAX;
    }

    //QUIN POST ANAR?

    Position which_post (int id) {
        int sold_i = data(id).pos.i;
        int sold_j = data(id).pos.j;
        int aux_i = -1;
        int aux_j = -1;

        for (int i = 0; i < MAX; ++i)
            for (int j = 0; j < MAX; ++j) {
                if (fora_limits(i,j))
                    break;
                int owner = post_owner(i,j);
                if ((owner != NO_POST) and (owner = NOONE || owner != me())) {
                    if (aux_i == -1 or abs(i - sold_i) + abs(j - sold_j) < (abs(sold_i - sold_i) + abs(sold_j - sold_j)) {
                        aux_i = sold_i;
                        aux_j = sold_j;
                    }
                }

            }

        return Position (opt_i, opt_j);
    }

    //WORTH TIRAR NAPALM?

    bool napalm_QuestionMark(Position pos) {
        int num_enemics = 0;
        int num_meus = 0;
        bool yes_OMG = false;
        for (int i = 0; i < 5; ++i)
            for (int j = 0; j < 5; ++j) {
                int data_soldier = which_soldier(pos.x - 2 + i,pos.x - 2 + j);
                int data_post = post_owner(pos.x - 2 + i,pos.x - 2 + j);
                if (data_soldier == -1)
                    break;
                if (data_soldier != 0) {
                    //fer búsqueda de si és meu o no
                }

            }
        return ((num_meus < 3 and num_enemics > num_meus) or (num_enemics > 2*num_meus-1))
    }

    //Search algorithm per trobar la ruta

    QP BFS(int ii, int ij, int oi, int oj) {
        QP qp;

        return qp;
    }

    //"MAIN"

    virtual void play () {
        //Inicialització vectors cada ronda
        v_soldiers = soldiers(me());
        v_helicopters = helicopters(me());
        int sold_size = int(v_soldiers.size())

        //primera ronda pillar tots els posts
        if (round() == 0){
            v_posts = posts();
            int post_size = v_posts.size();
        }

        //Vector cues per fer el search algorithm de cada soldat
        vcues = VQ(sold_size);
        for (int i = 0; i < sold_size; ++i) {
            Position pos_obj = which_post(id);
            Position pos_act = data(v_soldiers).pos;
            vcues[i] = BFS(pos_act.i, pos_act.j, pos_obj.i.pos_obj.j);

            //Moure el soldat a la posició que toca
            Position next = vcues[i].front();
            vcues[i].pop();
            command_soldier(v_soldiers[i],next.i, next.j);
        }

    }


};


/**
* Do not modify the following line.
*/
RegisterPlayer(PLAYER_NAME);

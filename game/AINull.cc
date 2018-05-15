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
    using VQ    = vector <pair <int,QP>>;


    VVE visitats;
    VE v_soldiers;
    VE v_helicopters;
    vector <Post> v_posts;
    VQ v_cues;


    //FOREST GRASS WATER MOUNTAIN


    //POST OWNERS
    static const int NO_POST = -2;
    static const int NOONE = -1;

    /*
    NAPALM AREA

    * * * * *
    * * * * *
    * * H * *
    * * * * *
    * * * * *

    */

    //EL PUNT ESTA FORA LIMITS?

    bool fora_limits(int i, int j) {
        return (i < 0 or i >= MAX or j < 0 or j >= MAX);
    }

    //DISTANCIA?

    int distance(const Position &in, const Position &f) {
        return abs(f.i - in.i) + abs(f.j - in.j);
    }

    //CAP A ON ANAR HELICOPTER

    Position where (Position &act) {
        Position aux (-1,-1);
        for (int i = 0; i < MAX; ++i)
            for (int j = 0; j < MAX; ++j) {
                if (fora_limits(i,j))
                    break;
                int owner = post_owner(i,j);
                if ((owner != NO_POST) and (owner == NOONE or owner != me())) {
                    if (aux.i == -1)
                        aux = Position(i,j);
                }
            }
        return aux;
    }

    //QUIN POST ANAR?

    Position which_post (int id) {
        Position sold = data(id).pos;
        int act_v;
        int last_v;
        Position aux (-1,-1);

        for (int i = 0; i < MAX; ++i)
            for (int j = 0; j < MAX; ++j) {
                if (fora_limits(i,j))
                    break;
                int owner = post_owner(i,j);
                if ((owner != NO_POST) and (owner == NOONE or owner != me())) {
                    if (aux.i == -1)
                        act_v = last_v = post_value(i,j);
                    if (distance(sold,Position(i,j)) < distance(aux,sold)) {
                        if (last_v == act_v or act_v > last_v)
                            aux = Position(i,j);
                        else
                            if (distance(sold,Position(i,j)) <= distance(aux,Position(i,j))*1.3)
                                aux = sold;
                    }
                }
            }
        return aux;
    }

    //WORTH TIRAR NAPALM?

    bool napalm_QuestionMark(Position pos) {
        int num_enemics = 0;
        int num_meus = 0;
        bool yes_OMG = false;
        for (int i = 0; i < 5; ++i)
            for (int j = 0; j < 5; ++j) {
                int data_soldier = which_soldier(pos.i - 2 + i,pos.i - 2 + j);
                int data_post = post_owner(pos.i - 2 + i,pos.i - 2 + j);
                if (data_soldier == -1)
                    break;
                if (data_soldier != 0) {
                    //fer búsqueda de si és meu o no
                }

            }
        return ((num_meus < 3 and num_enemics > num_meus) or (num_enemics > 2*num_meus-1));
    }

    //Detecta si hi ha un enemic a la posicio i j

    bool enemy_there_QuestionMark(Position a) {
        int id = which_soldier(a.i,a.j);
        if (id == 0) return false;
        return find(v_soldiers.begin(),v_soldiers.end(),id) == v_soldiers.end();
    }

    //Search algorithm per trobar la ruta

    void bfs(const Position &i_pos, Position &f_pos, QP &qp){
        visitats = (MAX, VE (MAX, false));
        QP q;
        bool trobat = 0;
        QP.push_back(i_pos);
        visitats[i_pos.i][i_pos.j] = true;
        while (not q.empty()) {
            Position p = q.front();
            q.pop();
            visitats[p.i][p.j] = true;
            if (p == fpos)
                break;
            //UP
            if (what(p.i-1,p.j) != -1 or what(p.i-1,p.j) != MOUNTAIN or fire_time(p.i-1,p.j) == 0)
                q.push(Position(p.i-1,p.j));
            //RIGHT_UP
            if (what(p.i-1,p.j+1) != -1 or what(p.i-1,p.j+1) != MOUNTAIN or fire_time(p.i-1,p.j+1) == 0)
                q.push(Position(p.i-1,p.j+1));
            //RIGHT
            if (what(p.i,p.j+1) != -1 or what(p.i,p.j+1) != MOUNTAIN or fire_time(p.i,p.j+1) == 0)
                q.push(Position(p.i,p.j+1));
            //DOWN_RIGHT
            if (what(p.i+1,p.j+1) != -1 or what(p.i+1,p.j+1) != MOUNTAIN or fire_time(p.i+1,p.j+1) == 0)
                q.push(Position(p.i+1,p.j+1));
            //DOWN
            if (what(p.i+1,p.j) != -1 or what(p.i+1,p.j) != MOUNTAIN or fire_time(p.i+1,p.j) == 0)
                q.push(Position(p.i+1,p.j));
            //DOWN_LEFT
            if (what(p.i+1,p.j-1) != -1 or what(p.i+1,p.j-1) != MOUNTAIN or fire_time(p.i+1,p.j-1) == 0)
                q.push(Position(p.i+1,p.j-1));
            //LEFT
            if (what(p.i,p.j-1) != -1 or what(p.i,p.j-1) != MOUNTAIN or fire_time(p.i,p.j-1) == 0)
                q.push(Position(p.i,p.j-1));
            //LEFT_UP
            if (what(p.i-1,p.j-1) != -1 or what(p.i-1,p.j-1) != MOUNTAIN or fire_time(p.i-1,p.j-1) == 0)
                q.push(Position(p.i-1,p.j-1));

        }
    }

    //"MAIN"

    virtual void play () {
        //Inicialització vectors cada ronda
        v_soldiers = soldiers(me());
        v_helicopters = helicopters(me());
        int sold_size = int(v_soldiers.size());
        visitats = VVE(MAX, VE(MAX,false));
        v_cues = VQ(sold_size);

        //primera ronda pillar tots els posts
        if (round() == 0){
            v_posts = posts();
            int post_size = v_posts.size();
        }

        //Vector cues per fer el search algorithm de cada soldat
        for (int i = 0; i < sold_size; ++i) {
            Position pos_obj = v_posts[i].pos;
            Position pos_act = data(v_soldiers[i]).pos;
            v_cues[i].first = v_soldiers[i];
            BFS(pos_act, pos_obj,v_cues[i]);

            //MOVE SOLDIER
            /*Position next = vcues[i].front();
            vcues[i].pop();
            command_soldier(v_soldiers[i],next.i, next.j);
            */
            //Calcular POS on anar de cada helicopter
            Position move_hel_1 = data(v_helicopters[0]).pos;
            int orient_1 = data(v_helicopters[0]).orientation;
            move_hel_1 = where(move_hel_1);
            Position move_hel_2 = data(v_helicopters[1]).pos;
            int orient_2 = data(v_helicopters[1]).orientation;
            move_hel_2 = where(move_hel_2);

            //MOVE HELICOPTER
            //code_command_helicopter(id,code);
        }
    }
};


/**
* Do not modify the following line.
*/
RegisterPlayer(PLAYER_NAME);

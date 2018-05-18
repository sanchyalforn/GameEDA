#include "Player.hh"
#include <utility>
#include <queue>
#include <cstdlib>
using namespace std;

/**
* Write the name of your player and save this file
* with the same name and .cc extension.
*/
#define PLAYER_NAME Maincra


struct PLAYER_NAME : public Player {


    static constexpr int I[8] = {-1, -1,  0, 1, 1, 1, 0, 1 };
    static constexpr int J[8] = { 0, -1, -1,-1, 0, 1, 1, 1 };
    //const int HI[4] = { 1, 0, -1, 0 };
    //const int HJ[4] = { 0, 1, 0, -1 };

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
    using VIQ    = vector <pair <int,QP>>;


    VVE visitats;
    VE v_soldiers;
    VE v_helicopters;
    vector <Post> v_posts;
    VIQ v_cues;


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


    //DISTANCIA?

    int distance(const Position &in, const Position &f) {
        return abs(f.i - in.i) + abs(f.j - in.j);
    }

    //CAP A ON ANAR HELICOPTER

    Position where (Position &act) {
        Position aux = v_posts[0].pos;
        for (int j = 0; j < (int)v_posts.size(); ++j) {
            Position act = v_posts[j].pos;
            if (post_owner(act.i,act.j) != me()) {
                if (distance(act,aux) > distance(act,act))
                    aux = act;
            }
        }
        return aux;
    }

    //QUIN POST ANAR?

    Position which_post (int id) {
        Position sold = data(id).pos;
        int act_v = v_posts[0].value;
        int last_v = act_v;
        Position aux = v_posts[0].pos;
        for (int i = 0; i < (int)v_posts.size(); ++i) {
            Position act = v_posts[i].pos;
            if (post_owner(act.i,act.j) != me() or post_owner(act.i,act.j) == -1)
                if ((distance(sold,act) < distance(sold,aux)))
                    aux = act;
        }
        if (not pos_ok(aux.i,aux.j))
            aux = v_posts[(rand()%(int)v_posts.size())+1].pos;
        return aux;
    }

    bool parachuter_QuestionMark(int x, int y) {

        if ((what(x,y) == WATER) or (what(x,y) == MOUNTAIN)) return false;
        if (fire_time(x,y) != 0) return false;
        if (which_soldier(x,y) != 0) return false;
        return true;
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
        if (id <= 0) return false;
        return find(v_soldiers.begin(),v_soldiers.end(),id) == v_soldiers.end();
    }

    Position sum(const Position &inicial, const Position &sumada) {
        return Position(inicial.i + sumada.i, inicial.j + sumada.j);
    }

    //Search algorithm per trobar la ruta

    void BFS(const Position &i_pos,const Position &f_pos, QP &qp){
        visitats = VVE (MAX, VE (MAX, false));
        queue <pair <Position,queue<Position> > > q;
        q.push({i_pos,queue<Position>()});
        bool trobat = false;
        visitats[i_pos.i][i_pos.j] = true;
        while (not q.empty() and not trobat) {
            pair <Position, QP > p = q.front(); q.pop();
            for (int i = 0; i < 8 and not trobat; ++i) {

                QP route = p.second;
                Position seg = sum(p.first,Position(I[i],J[i]));
                if (not visitats[seg.i][seg.j]) {
                    if (pos_ok(seg) and what(seg.i,seg.j) != MOUNTAIN and what(seg.i,seg.j) != WATER and fire_time(seg.i,seg.j) == 0) {
                        route.push(Position(seg.i,seg.j));
                        q.push({seg,route});
                        visitats[seg.i][seg.j] = true;
                    }
                    if (seg.i == f_pos.i  and seg.j == f_pos.j){
                        qp = route;
                        trobat = true;
                    }
                }
            }
        }
    }

    //"MAIN"

    virtual void play () {


        //primera ronda pillar tots els posts
            v_posts = posts();
            int post_size = v_posts.size();

        if (status(me()) > 0.35) return;

        //Inicialització vectors cada ronda
        v_soldiers = soldiers(me());
        v_helicopters = helicopters(me());
        int sold_size = int(v_soldiers.size());
        v_cues = VIQ(sold_size);

        //Vector cues per fer el search algorithm de cada soldat
        for (int i = 0; i < sold_size; ++i) {
            Position pos_obj = which_post(v_soldiers[i]);
            Position pos_act = data(v_soldiers[i]).pos;
            v_cues[i].first = v_soldiers[i];
            BFS(pos_act, pos_obj,v_cues[i].second);

            //MOVE SOLDIER
            Position next = v_cues[i].second.front();
            v_cues[i].second.pop();
            command_soldier(v_soldiers[i],next.i, next.j);

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

constexpr int PLAYER_NAME::I[8];
constexpr int PLAYER_NAME::J[8];

/**
* Do not modify the following line.
*/
RegisterPlayer(PLAYER_NAME);

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

    /*------------------
        DECLARACIONS
          GLOBALS
    ------------------*/

    static constexpr int I[8] = {-1, -1,  0, 1, 1, 1, 0, 1 };
    static constexpr int J[8] = { 0, -1, -1,-1, 0, 1, 1, 1 };

    using VE    =   vector <int>;
    using VVE   =   vector <VE>;
    using VC    =   vector <char>;
    using VVC   =   vector <VC>;
    using QP    =   queue <Position>;
    using PP    =   pair <int,Position>;
    using VIQ   =   vector <pair <int,QP>>;

    VE              v_soldier;
    VE              v_helicopter;
    vector <Post>   v_posts;
    VVC             G;
    VVE             visitats;
    VIQ             v_cues;

    /*------------------
        RANDOM STUFF
    ------------------*/
    /*
    void write_matrix() {
        for (int i = 0; i < MAX; ++i) {
            for (int j = 0; j < MAX; ++j) {
                cout << G[i][j];
            }
            cout << endl;
        }
        cout << endl;
        cout << endl;
    }

    int matrix_Dijkstra () {
        int s = 0;
        G = VVC(MAX,VC(MAX,'X'));
        for (int i = 0; i < MAX; ++i) {
            for (int j = 0; j < MAX; ++j) {
                if (fire_time(i,j) != 0) continue;
                if (what(i,j) == GRASS) G[i][j] = 'G';
                else if (what(i,j) == FOREST) G[i][j] = 'F';
                if (which_soldier(i,j) != 0) ++s;
                if ((which_soldier(i,j) != 0) and (data(which_soldier(i,j)).player != me())) G[i][j] = 'S';
                if ((post_value(i,j) != -2) and (post_value(i,j) != me())) G[i][j] = 'P';
            }
        }
        return s;
    }
    */

    int matrix_BFS () {
        int s = 0;
        G = VVC(MAX,VC(MAX,'X'));
        for (int i = 0; i < MAX; ++i) {
            for (int j = 0; j < MAX; ++j) {
                if (fire_time(i,j) != 0) continue;
                if ((what(i,j) == GRASS) or (what(i,j) == FOREST)) G[i][j] = '.';
                if ((post_value(i,j) != -2) and (post_value(i,j) != me())) G[i][j] = 't';
            }
        }
        return s;
    }

    int manhattan_distance(const Position &p1, const Position &p2) {
        return abs(p1.i - p2.i) + abs(p1.j - p2.j);
    }

    Position sum(const Position &p1, const Position &p2) {
        return Position(p1.i + p2.i, p1.j + p2.j);
    }

    /*----------------
        SOLDIER
    ----------------*/

    Position which_post(int id) {
        Position aux    = Position(-1,-1);
        Position sold   = data(id).pos;

        for (int i = 0; i < MAX; i++) {
            for (int j = 0; j < MAX; j++) {

                // Get Position
                Position temp = Position(i, j);

                // Check if city (not my property)
                if ((post_owner(temp.i,temp.j) != -1) and (post_owner(temp.i,temp.j) != me())) {

                    // Distance
                    if (aux.i == -1 or (manhattan_distance(temp,sold) < manhattan_distance(temp,aux))) {
                        aux = temp;
                    }
                }
            }
        }
        return aux;
    }

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

    QP BFS_X(int f, int c) {

        VVE dist(MAX,VE(MAX,-1));

        dist[f][c] = 0;
        QP Q;
        QP qp;
        Q.push(Position(f,c));
        int d = 0;
        while (not Q.empty()) {
            Position v = Q.front();
            Q.pop();
            int x = v.i;
            int y = v.j;

            if (G[x][y] == 't' and d < dist[x][y]){
                d = dist[x][y];
                qp = Q;
            }

            if (G[x][y] == '.' or G[x][y] == 't') {
                G[x][y] = 'X';
                for (int i = 0; i < 8; ++i) {
                    Position next = {x+I[i],y+J[i]};
                    if (pos_ok(next) and G[next.i][next.j] != 'X') {
                        dist[x][y] = d + 1;
                        Q.push(Position(x,y));
                    }
                }
            }
        }
        return qp;
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

    bool parachuter_QuestionMark(int x, int y) {

        if ((what(x,y) == WATER) or (what(x,y) == MOUNTAIN)) return false;
        if (fire_time(x,y) != 0) return false;
        if (which_soldier(x,y) != 0) return false;
        return true;
    }

    bool napalm_QuestionMark(Position pos) {
        int num_enemics = 0;
        int num_meus = 0;
        bool post = false;
        for (int i = 0; i < 5; ++i)
            for (int j = 0; j < 5; ++j) {
                Position act = {pos.i - 2 + i,pos.j - 2 + j};
                int data_soldier = which_soldier(act.i,act.j);
                int data_post = post_owner(act.i,act.j);
                if (data_soldier == -1)
                    continue;
                (which_soldier(act.i,act.j) == me() ? ++num_enemics:++num_meus);

            }
        return ((num_meus < 3 and num_enemics > num_meus) or (num_enemics > 2*num_meus-1) or (num_meus < 2 and post));
    }

    void play_helicopter(int id) {

        // If we can, we throw napalm.
        Position pos = data(id).pos;
        if ((data(id).napalm == 0) and (napalm_QuestionMark(pos))) {
            command_helicopter(id, NAPALM);
            return;
        }
        int c = random(1, 5);
        command_helicopter(id, c == 1 ? COUNTER_CLOCKWISE : FORWARD2);
    }

    void throw_parachuter(int helicopter_id) {
        // We get the data of the helicopter...
        Data in = data(helicopter_id);
        for (int i = 0; i < 5; ++i)
            for(int j = 0; j < 5; ++j)
                if ((not data(helicopter_id).parachuters.empty()) and (parachuter_QuestionMark(i,j)) and pos_ok(i,j))
                    command_parachuter(i,j);
    }

      /*----------------
            MAIN
      ----------------*/

    virtual void play () {
        v_posts = posts();
        v_soldier = soldiers(me());
        v_helicopter = helicopters(me());
        cues_soldier = vector <QP> ((int)v_soldier.size());

        for (int i = 0; i < (int)v_soldier.size(); ++i) {
            Position pos_obj = which_post(v_soldiers[i]);
            Position pos_act = data(v_soldiers[i]).pos;
            v_cues[i].first = v_soldiers[i];
            BFS(pos_act, pos_obj,v_cues[i].second);

            Position next = cues_soldier[i].front(); cues_soldier[i].pop();
            command_soldier(v_soldier[i],next.i,next.j);
        }
        /*for (int i = 0; i < (int)v_helicopter.size(); ++i){
            if (not data(v_helicopter[i]).parachuters.empty())
                throw_parachuter(v_helicopter[i]);
            play_helicopter(v_helicopter[i]);
        }*/
    }

};

constexpr int PLAYER_NAME::I[8];
constexpr int PLAYER_NAME::J[8];

/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);

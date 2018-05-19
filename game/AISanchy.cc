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

    using VE    = vector <int>;
    using VVE   = vector <VE>;
    using VC    = vector <char>;
    using VVC   = vector <VC>;
    using QP    = queue <Position>;
    using PP    = pair <int,Position>;

    VE v_soldier;
    VE v_helicopter;
    vector <QP> cues_soldier;
    VVC G;
    VVE visitats;

    /*------------------
        RANDOM STUFF
    ------------------*/

    int matrix_Dijkstra () {
        int s = 0;
        //G = VVC(MAX,VC(MAX,'X'));
        for (int i = 0; i < MAX; ++i) {
            for (int j = 0; j < MAX; ++j) {
                if (fire_time(i,j) != 0) continue;
                if (what(i,j) == GRASS) G[i][j] = 'G';
                else if (what(i,j) == FOREST) G[i][j] = 'F';
                if (which_soldier(i,j) != 0) ++s;
                if ((which_soldier(i,j) != 0) and (which_soldier(i,j) != me())) G[i][j] = 'S';
                if ((post_value(i,j) != -2) and (post_value(i,j) != me())) G[i][j] = 'P';
            }
        }
        return s;
    }

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
                if ((post_value(temp.i,temp.j) != -1) and (post_value(temp.i,temp.j) != me())) {

                    // Distance
                    if (aux.i == -1 or (manhattan_distance(temp,sold) < manhattan_distance(temp,aux))) {
                        aux = temp;
                    }
                }
            }
        }
        return aux;
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
            return qp;
        }
    }



    /*----------------
        HELICOPTER
    ----------------*/

    bool parachuter_QuestionMark(int x, int y) {

        if ((what(x,y) == WATER) or (what(x,y) == MOUNTAIN)) return false;
        if (fire_time(x,y) != 0) return false;
        if (which_soldier(x,y) != 0) return false;
        return true;
    }

    bool napalm_QuestionMark(Position pos) {
        int num_enemics = 0;
        int num_meus = 0;
        bool yes_OMG = false;
        for (int i = 0; i < 5; ++i)
            for (int j = 0; j < 5; ++j) {
                Position act = {pos.i - 2 + i,pos.j - 2 + j}
                int data_soldier = which_soldier(act.i,act.j);
                int data_post = post_owner(act.i,act.j);
                if (data_soldier == -1)
                    continue;
                (which_soldier(act.i,act.j) == me() ? ++num_enemics:++num_meus);

            }
        return ((num_meus < 3 and num_enemics > num_meus) or (num_enemics > 2*num_meus-1));
    }

    void play_helicopter(int id) {

      // If we can, we throw napalm.
      Position pos = data(id).pos;
      if ((data(id).napalm == 0) and (napalm_QuestionMark(pos.i,pos.j))) {
        command_helicopter(id, NAPALM);
        return;
      }


    virtual void play () {

    }

};

constexpr int PLAYER_NAME::I[8];
constexpr int PLAYER_NAME::J[8];

/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);

#include "Player.hh"
#include <queue>

/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Creeper


struct PLAYER_NAME : public Player {


   //Vector per les direccions: DRETA -> ESQUERRA
   //UP; UP_RIGHT; RIGHT; DOWN_RIGHT; DOWN; DOWN_LEFT; LEFT; UP_LEFT

    static constexpr int I[8] = {-1, -1,  0, 1, 1, 1, 0, 1 };
    static constexpr int J[8] = { 0, -1, -1,-1, 0, 1, 1, 1 };


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



    using VE    = vector <int>;
    using VVE   = vector <VE>;
    using VC    = vector <char>;
    using VVC   = vector <VC>;
    using QP    = queue <Position>;
    using PP    = pair <int,Position>;
    //using PQP   = priority_queue <PP, vector <PP>, greater<PP> >;

    VE v_soldier;
    VE v_helicopter;
    vector <QP> cues_soldier;
    VVC G;
    VVE visitats;
    int NB_UNITS = 80;

    /*template <class T> struct less : binary_function <T,T,bool> {
        bool operator() (const T& x, const T& y) const {return x<y;}
    };*/

    /*------------------
        RANDOM STUFF
    ------------------*/

    int matrix_Dijkstra () {
        int s = 0;
        G = VVC(MAX,VC(MAX,'X'));
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

    //source node = inicial
    //distance = distancia amb ponderacions

    /*void dijkstra(Position source) {

        vector<vector <int> > pond(MAX, vector <int>(MAX, infinit));
        pond[s.i][s.j] = 0;
        vector<vector <int> > previous(MAX, vector <int>(MAX, -1));
        VVE visitats(n, false);
        priority_queue<PP> Q;
        Q.push(PP(0,Position(pos.i,pos.j)));
        while (not Q.empty()) {
            Position u = Q.top().second; Q.pop();
            if (visitats[u.i][u.j])
                continue;
            visitats[u.i][u.j] = true;
            for (int i = 0; i < 8; ++i) {
                Position p = a.second;
                Position act = (u.i+I[i],u.j+J[i])
                if (not pos_ok(act.i, act.j))
                    continue;
                double c = ponderacio(act.i,act.j);
                if (pond[][] > pond[][] + c) {
                    pond[v][] = pond[u][] + c;
                    previous[act.i][act.j] = u;
                    Q.push(WArc(pond[v], v));
                }
            }
        }
    }*/

    QP BFS_X(int f, int c) {
        dist = VVE(n,VE(m,-1));
        dist[f][c] = 0;
        QP Q;
        QP qp;
        Q.push(P(f,c));
        int d = 0;
        while (not Q.empty()) {
            P v = Q.front();
            Q.pop();
            int x = v.first;
            int y = v.second;

            if (G[x][y] == 't' and d < dist[x][y]){
                d = dist[x][y];
                qp = Q;
            }

            if (G[x][y] == '.' or G[x][y] == 't') {
                G[x][y] = 'X';
                for (int i = 0; i < 8; ++i) {
                    if (pos_ok(x,y) and G[x][y] != 'X') {
                        dist[x][y] = d + ponderacio(x,y);
                        Q.push(Position(x,y));
                    }
            }
        }
        return qp;
    }

    /*void BFS(const Position &i_pos,const Position &f_pos, QP &qp){
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
    }*/

    /*----------------
        HELICOPTER
    ----------------*/

    bool parachuter_QuestionMark(int x, int y) {

        if ((what(x,y) == WATER) or (what(x,y) == MOUNTAIN)) return false;
        if (fire_time(x,y) != 0) return false;
        if (which_soldier(x,y) != 0) return false;
        return true;
    }

    /*----------------
           MAIN
    ----------------*/

    virtual void play () {

        int my_id = me();
        NB_UNITS = matrix_BFS();
        //NB_UNITS = matrix_Dijkstra();
        v_soldier = soldiers(my_id);
        int sold_size = v_soldier.size();
        v_helicopter = helicopters(my_id);
        int hel_size = v_helicopter.size();

        for (int i = 0; i < sold_size; ++i) {
            QP qp;
            Position sold = data(v_soldier[i]).pos;
            Position obj = which_post(i);
            BFS(sold,obj,qp);
            cues_soldier.push_back(qp);
            Position next = cues_soldier[i].front();
            command_soldier(v_soldier[i],next.i,next.j);
        }
    }

};

constexpr int PLAYER_NAME::I[8];
constexpr int PLAYER_NAME::J[8];

/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
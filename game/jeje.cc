#include "Player.hh"
#include <queue>

/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Creeper


struct PLAYER_NAME : public Player {

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */

  static Player* factory () {
    return new PLAYER_NAME;
  }

  /**
   * Types and attributes for your player can be defined here.
   */

    static constexpr int I[8] = {-1, -1,  0, 1, 1, 1, 0, 1 };
    static constexpr int J[8] = { 0, -1, -1,-1, 0, 1, 1, 1 };

    using VE = vector <int>;
    using VVE = vector <VE>;
    using VC = vector <char>;
    using VVC = vector <VC>;
    using QP = queue <Position>;
    using PP = pair <int,Position>;
    using PQP = priority_queue <PP, vector <PP>, greater<PP> >;

    VE v_soldier;
    VE v_helicopter;
    vector <QP> cues_soldier;
    VE D;
    int infinit = 1e9;


    bool my_soldier(int x, int y, int id) {
        if (id == 0 or id == -1) return false;
        return find(v_soldier.begin(),v_soldier.end(),id) != v_soldier.end();
    }

    int ponderacio (int i, int j) {
        int aux;
        //TERRENY o FOC


        //ENEMICS

        return aux;
    }

    void dijkstra(const WGraph& G, int s, vector<double>& d, vector<int>& p) {

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
                if (pond[v] > pond[u] + c) {
                    pond[v] = pond[u] + c;
                    previous[v] = u;
                    Q.push(WArc(pond[v], v));
                }
            }
        }
    }



  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {
      int my_id = me();
      vector <Post> v_post = posts();
      v_soldier = soldiers(my_id);
      int sold_size = v_soldier.size();
      v_helicopter = helicopters(my_id);
      int hel_size = v_helicopter.size();
      for (int i = 0; i < sold_size; ++i) {
          QP qp;
          Position sold = data(v_soldier[i]).pos;
          Dijkstra(sold.i,sold.j);
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

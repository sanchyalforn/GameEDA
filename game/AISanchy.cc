#include "Player.hh"


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

    VE v_soldier;
    VE v_helicopter;
    vector <QP> cues_soldier;


    bool my_soldier(int x, int y, int id) {
        if (id == 0 or id == -1) return false;
        return find(v_soldier.begin(),v_soldier.end(),id);
    }

    bool Post_QuestionMark(int x, int y, queue<Position> &Q) {
        if (x < 0 or x >= MAX or y < 0 or y >=  MAX or what(x,y) == MOUNTAIN or what(x,y) == WATER or fire_time(x,y) != 0 or not my_soldier(x,y,which_soldier(x,y))) return false;
        if (post_owner(x,y) != -2 and post_owner(x,y) != me()) return true;
        Q.push(Position(x,y));
        return false;
    }

    QP solu(int i, int j) {
        QP Q;
        Q.push(Position(i,j));
        while (not Q.empty()) {
            Position v = Q.front(); Q.pop();
            int x = v.i;
            int y = v.j;
            for (int i = 0; i < 8; ++i) {
                if (Post_QuestionMark(x+I[i],y+J[i],Q)) {
                    return Q;
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
          Position sold = data(v_soldier[i]).pos;
          cues_soldier.push_back(solu(sold.i,sold.j));
      }


  }

};

constexpr int PLAYER_NAME::I[8];
constexpr int PLAYER_NAME::J[8];

/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);

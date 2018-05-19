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
    //VE D;
    //int infinit = 1e9; //(Pal dijkstra)
    VVC G;


    /*------------------
        RANDOM STUFF
    ------------------*/

    void matrix () {
        G = VVC(MAX,VC(MAX,'X'));
        for (int i = 0; i < MAX; ++i) {
            for (int j = 0; j < MAX; ++j) {
                if (fire_time(i,j) != 0) continue;
                if (what(i,j) == GRASS) G[i][j] = 'G';
                else if (what(i,j) == FOREST) G[i][j] = 'F';
                if (which_soldier(i,j) != 0 and which_soldier(i,j) != me()) G[i][j] = 'S';
                if (which_post(i,j) != 0)

            }
        }
    }


    /*----------------
        SOLDIER
    ----------------*/

    int manhattan_distance(const Position &p1, const Position &p2) {
        return abs(p1.i - p2.i) + abs(p1.j - p2.j);
    }

    Data which_enemy(int ork) {
        Data enemy;
        enemy.pos = {-1, -1};

        for (int i = 0; i < nb_units(); i++) {
            // nearer and (same city or path)
            if (data(i).player != me() and ((manhattan_distance(unit(i).pos, unit(ork).pos) < manhattan_distance(enemy.pos, unit(ork).pos) and ((cell(unit(i).pos).city_id == cell(unit(ork).pos).city_id) or (cell(unit(i).pos).path_id == cell(unit(ork).pos).path_id))) or enemy.pos.i == -1)) {
            enemy = unit(i);
            }
        }

        return enemy;
    }

    Pos which_city(int id) {
        Position aux    = Position(-1,-1);
        Position sold   = data(id).pos;

        for (int i = 0; i < rows(); i++) {
            for (int j = 0; j < cols(); j++) {

                // Get Position
                Position temp = Position(i, j);

                // Check if city (not my property)
                if (temp.city_id != -1 and city_owner(temp.city_id) != me()) {

                    // Distance
                    if (aux.i == -1 or (manhattan_distance(temp,sold) < manhattan_distance(temp,aux))) {
                        aux = temp;
                    }
                }
            }
        }
        return aux;
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



  virtual void play () {
      int my_id = me();

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

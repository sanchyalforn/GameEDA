#include "Player.hh"
#include <cmath>
#include <stack>



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
    using VP = vector <Post>;
    using P = pair <int,int>;
    using Q = queue<P>;
    const int my_id = me();
    int num_soldiers = 20;
    int num_parachuters = 0;

    VVE visitats;
    vector <Q> cues;

/*
    VE v_soldiers (20);
    VE v_helicopters (2);
    VE v_parachuters (0);
    VP post_no_capturats (16);
    VP v_posts (16);
    S s;
*/

    bool no_possibru(P p) {
        return (p.first < 0 and p.first >= MAX and p.second >= MAX and p.second < 0 or fire_time(p.first,p.second) > 1 or what(p.first,p.second) == 4);
    }

    bool cubito(int x, int y, queue<P> &Q, int d) {
    	if (no_possibru({x,y})) return false;
        visitats[x][y] = true;
    	return true;
    }

    Q BFS(int i, int j) {
    	Q q;
    	q.push(P(i,j));
    	while (not q.empty()) {
    		P v = q.front();
    		q.pop();
    		if (cubito(x,y+1,q,d) or cubito(x,y-1,q,d) or cubito(x+1,y,q,d) or cubito(x-1,y,q,d)) {
    			return q;
    		}

    	}
    	cout << "no treasure can be reached" << endl;
    }

/*void dijkstra(constWGraph& G,int s, vector<double>& d, vector<int>& p) {
    int n = G.size();
    d = vector<double>(n, infinit);
    d[s] = 0;
    p = vector<int>(n, -1);
    vector<boolean> S(n,false);
    priority_queue<WArc, vector<WArc>, greater<WArc> > Q;
    Q.push(WArc(0, s));
    while(not Q.empty()) {
        int
        u = Q.top().second;  Q.pop();
        if(not S[u]) {
            S[u] = true;
            for(WArc a : G[u]) {
                int v = a.second;
                double c = a.first;
                if (d[v] > d[u] + c) {
                    d[v] = d[u] + c;
                    p[v] = u;
                    Q.push(WArc(d[v], v));
                }
            }
        }
    }
}*/

    virtual void play () {
        vsoldiers = soldiers(my_id);
        int n = vsoldiers.size();
        vhelicopters = helicopters(my_id);
        cues = vector <Q> (vsoldiers.size());
        vposts = posts();
        visitats = VVE(MAX,VE(MAX,false));
        if (round() == 0)
            vposts = posts();
        else {
            for (int i = 0; i < n; ++i) {
                cues[i] = BFS();
            }
        }
    }

};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);

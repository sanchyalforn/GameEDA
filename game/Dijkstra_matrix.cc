#include <iostream>
#include <vector>
#include <queue>
using namespace std;

typedef vector <vector <int> > Mat;
typedef pair <int,int> Pos;
int w = 5;
Mat G (w, vector <int> (w));
Mat d (w,vector <int> (w,1000000));

template <class T> struct ord {
	bool operator() (const T& a, const T& b) const {return d[a.first][a.second] > d[b.first][b.second];}
	typedef T first_argument_type;
	typedef T second_argument_type;
	typedef bool result_type;
};

void dijkstra(Pos s, vector<vector<Pos> > &p){
	d[s.first][s.second]=0;
	p = vector <vector<Pos> > (w,vector <Pos> (w,pair <int,int>(-1,-1)));
	vector<vector <bool> > S (w,vector <bool> (w,false));
	priority_queue < Pos,vector<Pos>,ord<Pos> > Q;
	Q.push(s);
	while(!Q.empty()){
		Pos u = Q.top(); Q.pop();
		if(!S[u.first][u.second]){
			S[u.first][u.second] = true;
			Pos a = u;
			--a.first;
	/*. . .*/	if(a.first >= 0){
	/*o x .*/		if(d[a.first][a.second] > d[u.first][u.second] + mat[a.first][a.second]){
	/*. . .*/			d[a.first][a.second] = d[u.first][u.second] + mat[a.first][a.second];
					p[a.first][a.second] = u;
					Q.push(a);
				}
			}

			--a.second;
	/*o . .*/	if(a.first >= 0 && a.second >= 0){
	/*. x .*/		if(d[a.first][a.second] > d[u.first][u.second] + mat[a.first][a.second]){
	/*. . .*/			d[a.first][a.second] = d[u.first][u.second] + mat[a.first][a.second];
					p[a.first][a.second] = u;
					Q.push(a);
				}
			}

			++a.first;
	/*. o .*/	if(a.second >= 0){
	/*. x .*/		if(d[a.first][a.second] > d[u.first][u.second] + mat[a.first][a.second]){
	/*. . .*/			d[a.first][a.second] = d[u.first][u.second] + mat[a.first][a.second];
					p[a.first][a.second] = u;
					Q.push(a);
				}
			}

			++a.first;
	/*. . o*/	if(a.first < w && a.second >= 0){
	/*. x .*/		if(d[a.first][a.second] > d[u.first][u.second] + mat[a.first][a.second]){
	/*. . .*/			d[a.first][a.second] = d[u.first][u.second] + mat[a.first][a.second];
					p[a.first][a.second] = u;
					Q.push(a);
				}
			}

			++a.second;
	/*. . .*/	if(a.first < w){
	/*. x o*/		if(d[a.first][a.second] > d[u.first][u.second] + mat[a.first][a.second]){
	/*. . .*/			d[a.first][a.second] = d[u.first][u.second] + mat[a.first][a.second];
					p[a.first][a.second] = u;
					Q.push(a);
				}
			}

			++a.second;
	/*. . .*/	if(a.first < w && a.second < w){
	/*. x .*/		if(d[a.first][a.second] > d[u.first][u.second] + mat[a.first][a.second]){
	/*. . o*/			d[a.first][a.second] = d[u.first][u.second] + mat[a.first][a.second];
					p[a.first][a.second] = u;
					Q.push(a);
				}
			}

			--a.first;
	/*. . .*/	if(a.second < w){
	/*. x .*/		if(d[a.first][a.second] > d[u.first][u.second] + mat[a.first][a.second]){
	/*. o .*/			d[a.first][a.second] = d[u.first][u.second] + mat[a.first][a.second];
					p[a.first][a.second] = u;
					Q.push(a);
				}
			}

			--a.first;
	/*. . .*/	if(a.first >= 0 && a.second < w){
	/*. x .*/		if(d[a.first][a.second] > d[u.first][u.second] + mat[a.first][a.second]){
	/*o . .*/			d[a.first][a.second] = d[u.first][u.second] + mat[a.first][a.second];
					p[a.first][a.second] = u;
					Q.push(a);
				}
			}
        	}
	}
}

int main(){
	for(int i = 0; i<w; ++i)
		for(int j = 0; j<w; ++j)
			cin >> mat[i][j];
	vector <vector <Pos> > y;
	djikstra(Pos(0,0),y);
	cout << endl;
	for(int i = 0; i<w; ++i){
		for(int j = 0; j<w; ++j)
			cout << d[i][j] << ' ';
		cout << endl;
	}
	cout << endl;
	for (int i = 0; i<w; ++i){
		for (int j = 0; j<w; ++j)
			cout << y[i][j].first <<',' << y[i][j].second << ' ';
		cout << endl;
	}
	cout << endl;
}

#include <bits/stdc++.h>
using namespace std;

const int N=10;
using P=pair<int,int>;
int dr[]={-1,1,0,0},dc[]={0,0,-1,1};

int cost(char x){
    if(x=='R'||x=='G') return 1;
    if(x=='M') return 3;
    if(x=='T') return 5;
    return 1e9;
}

int main(){
    vector<vector<char>> g={
        {'S','R','R','T','T','R','R','R','R','R'},
        {'X','X','R','T','X','R','X','X','X','R'},
        {'R','R','R','T','R','R','R','R','X','R'},
        {'R','X','X','T','R','X','X','R','X','R'},
        {'R','R','R','R','R','R','X','R','R','R'},
        {'R','X','X','X','X','R','X','X','X','R'},
        {'R','R','R','R','X','R','R','R','R','R'},
        {'X','X','X','R','X','X','X','X','X','R'},
        {'R','R','R','R','R','R','R','R','R','R'},
        {'X','X','X','X','X','X','X','X','X','G'}
    };

    P S={0,0},G={9,9};
    using Node=pair<int,P>;
    priority_queue<Node,vector<Node>,greater<Node>> pq;
    vector<vector<int>> d(N,vector<int>(N,1e9));
    vector<vector<P>> par(N,vector<P>(N,{-1,-1}));
    vector<vector<bool>> vis(N,vector<bool>(N));

    pq.push({0,S}); d[0][0]=0;

    while(!pq.empty()){
        auto [w,u]=pq.top(); pq.pop();
        auto [r,c]=u;
        if(vis[r][c]) continue;
        vis[r][c]=1;
        if(u==G) break;

        for(int k=0;k<4;k++){
            int nr=r+dr[k],nc=c+dc[k];
            if(nr<0||nr>=N||nc<0||nc>=N||g[nr][nc]=='X') continue;

            int nw=w+cost(g[nr][nc]);
            if(nw<d[nr][nc]){
                d[nr][nc]=nw;
                par[nr][nc]=u;
                pq.push({nw,{nr,nc}});
            }
        }
    }

    if(!vis[G.first][G.second]){
        cout<<"No path\n";
        return 0;
    }

    vector<P> path;
    for(P u=G;u!=S;u=par[u.first][u.second]) path.push_back(u);
    path.push_back(S);
    reverse(path.begin(),path.end());

    cout<<"Path: ";
    for(auto [r,c]:path) cout<<"("<<r<<","<<c<<") ";
    cout<<"\nMoves: "<<path.size()-1;
    cout<<"\nCost: "<<d[G.first][G.second]<<"\n";
}
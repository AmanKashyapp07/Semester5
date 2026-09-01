#include <bits/stdc++.h>
using namespace std;

const int N=10;
using P=pair<int,int>;
int dr[]={0,1,0,-1},dc[]={1,0,-1,0};

const int SUCCESS=1,CUTOFF=0,FAILURE=-1;
vector<P> expanded,ans;
bool used[N][N];

int dls(const vector<vector<char>>& g,P u,P goal,int dep,int lim,vector<P>& path){
    expanded.push_back(u);
    if(u==goal){ans=path;return SUCCESS;}
    if(dep==lim)return CUTOFF;

    bool cutoff=false;
    for(int k=0;k<4;k++){
        int r=u.first+dr[k],c=u.second+dc[k];
        if(r<0||r>=N||c<0||c>=N||g[r][c]=='X'||used[r][c])continue;

        used[r][c]=1;
        path.push_back({r,c});

        int res=dls(g,{r,c},goal,dep+1,lim,path);

        path.pop_back();
        used[r][c]=0;

        if(res==SUCCESS)return SUCCESS;
        if(res==CUTOFF)cutoff=true;
    }
    return cutoff?CUTOFF:FAILURE;
}

void runDLS(const vector<vector<char>>& g,int lim){
    P S={0,0},G={9,9};
    expanded.clear();ans.clear();
    memset(used,0,sizeof(used));

    vector<P> path={S};
    used[0][0]=1;

    int res=dls(g,S,G,0,lim,path);

    cout<<"Limit: "<<lim<<"\nExpanded: ";
    for(auto [r,c]:expanded) cout<<"("<<r<<","<<c<<") ";
    cout<<"\n";

    if(res==SUCCESS){
        cout<<"SUCCESS\nPath: ";
        for(auto [r,c]:ans) cout<<"("<<r<<","<<c<<") ";
        cout<<"\nMoves: "<<ans.size()-1<<"\n";
    }else if(res==CUTOFF)
        cout<<"CUTOFF\n";
    else
        cout<<"FAILURE\n";

    cout<<"\n";
}

int main(){
    vector<vector<char>> g={
        {'S','R','R','X','R','R','R','R','R','R'},
        {'X','X','R','X','R','X','X','X','X','R'},
        {'R','R','R','X','R','R','R','R','X','R'},
        {'R','X','X','X','X','X','X','R','X','R'},
        {'R','R','R','R','R','R','X','R','R','R'},
        {'X','X','X','X','X','R','X','X','X','R'},
        {'R','R','R','R','X','R','R','R','R','R'},
        {'R','X','X','R','X','X','X','X','X','R'},
        {'R','R','R','R','R','R','R','R','R','R'},
        {'X','X','X','X','X','X','X','X','X','G'}
    };

    runDLS(g,18);
    runDLS(g,25);
}
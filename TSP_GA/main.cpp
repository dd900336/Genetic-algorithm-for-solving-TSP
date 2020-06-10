//
//  main.cpp
//  TSP_GA2
//
//  Created by 吳培弘 on 2020/2/5.
//  Copyright © 2020 吳培弘. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <math.h>
#include <algorithm>

using namespace std;

struct node //座標
{
    int x;
    int y;
};


struct route
{
    vector<int> order; //行經城市順序
    double sum; //總長度（適應值）
    
};

bool operator < (const route& r1,const route& r2)
{
    return r1.sum < r2.sum;
}


vector<node> map; //所有城市集合
vector<route> all_routes; //所有路徑


int population=100; //母體族群數
int level=1000; //迭代次數
double elite_save_percent=0.1; //菁英保留比例
double mutate_percent=0.1; //變異率
int variant=3; //變異度（取出長度）


void init_map(int num) //亂數產生
{
    for(int i=0;i<num;i++)
    {
        node t;
        t.x=rand()%1000; //x, y範圍：0~999
        t.y=rand()%1000;

        cout<<t.x<<" "<<t.y<<endl; //印出座標
        
        map.push_back(t);
        
    }
}


void set_map(int num) //手動輸入
{
    //cout<<"輸入城市座標 x , y : \n";
    
    for(int i=0;i<num;i++)
    {
        int x,y;
        cin>>x>>y;
        node n={x,y};
        map.push_back(n); //丟進map
    }
    
}


double edge_length(node n1,node n2) //長度
{
    return sqrt((pow(n1.x-n2.x,2))+(pow(n1.y-n2.y,2)));  //a^2+b^2=c^2 畢氏定理
}

void create_route(int num)
{
    bool check[10000]={0};
    int a;
    double s=0;
    
    route r;
    
    vector<int> odr;
    
    for(int i=0;i<num;i++)
    {
        a = rand()%num;
        while(check[a]==1) a = rand()%num; //若已存在再給一個亂數
        check[a]=1;
        
        odr.push_back(a);
        
    }
    
    for(int k=0;k<num-1;k++) //算總長度
    {
        s+=edge_length(map[odr[k]],map[odr[k+1]]);
    }
    if(num > 2) s+=edge_length(map[odr[0]],map[odr[num-1]]); //終點 回 起點

    r.order=odr;
    r.sum=s;
    
    all_routes.push_back(r); //產生的路徑放入
    
    //cout<<endl; //顯示
    //for(int i=0;i<num;i++) cout<<odr[i]<<" ";
    //cout<<endl<<s;
    //Sleep(100);
    
}



void crossover(int num)
{
    
    vector<int> take_gene;

    route father = all_routes[rand()%10];
    route mother = all_routes[rand()%10];
    
    int x = rand()%(num-variant+1); //從哪裡開始取 num-variant+1
    
    for(int i=x;i<x+variant;i++) //取variant個
    {
        take_gene.push_back(father.order[i]);
    }
    

    for(int j=0;j<take_gene.size();j++)
    {
        for(vector<int>::iterator it = mother.order.begin(); it != mother.order.end(); it++ )
        {
            if(*it == take_gene[j]) { mother.order.erase(it); break; }
        }
    }

    
    x=rand()%(num-variant+1); //從哪裡開始放
    
    for(int a=0;a<take_gene.size();a++)
    {
        mother.order.insert(mother.order.begin()+x+a,take_gene[a]);
    }
    
    
    double s=0;
    for(int k=0;k<num-1;k++) //重算路徑長度
    {
        s+=edge_length(map[mother.order[k]],map[mother.order[k+1]]);
    }
    if(num > 2) s+=edge_length(map[mother.order[0]],map[mother.order[num-1]]); //終點 回 起點
    
    mother.sum=s; //新路徑長存入
    
    all_routes.push_back(mother); //放進路線群
    
}



void mutate(int num) //變異
{
    route r = all_routes[rand()%10];
    
    int a = rand()%num;
    int b = rand()%num;
    
    int temp = r.order[a];
    r.order[a]=r.order[b];
    r.order[b]=temp;
    
    
    double s=0;
    for(int k=0;k<num-1;k++)
    {
        s+=edge_length(map[r.order[k]],map[r.order[k+1]]);
    }
    if(num > 2) s+=edge_length(map[r.order[0]],map[r.order[num-1]]);
    r.sum=s;

    all_routes.push_back(r);
    
}


void GA(int num) //GA核心
{

    sort(all_routes.begin(),all_routes.end()); // small -> big
    
    all_routes.erase(all_routes.begin()+population*elite_save_percent,all_routes.end());
    //只保留菁英
  
    for(int i=0;i<population*(1-elite_save_percent-mutate_percent);i++) crossover(num); //交配
    
    for(int i=0;i<population*mutate_percent;i++) mutate(num); //變異
    
}



int main()
{
    
    srand((unsigned int)time(NULL));
    
    int num;
    cin>>num;
    
    init_map(num); //隨機產生
    //set_map(num);
    
    cout<<endl<<"運算中..."<<endl<<endl;

    for(int i=0;i<population;i++) create_route(num); //初始化
    
    for(int j=0;j<level;j++) GA(num); //算法核心
    

    sort(all_routes.begin(),all_routes.end());
    int c=0;
    for(vector<route>::iterator i=all_routes.begin();i!=all_routes.end();i++)
    {
        c++;
        cout<<i->sum<<" "<<c<<endl;
    }
    
    cout<<endl;
    for(int i=0;i<all_routes[0].order.size();i++) cout<<all_routes[0].order[i]<<" ";
    cout<<endl;
    cout<<all_routes[0].sum<<endl;
    

    //sort(all_routes.begin(),all_routes.end()); //最後一次挑選路徑最小之路線
    
    /*
    cout<<endl<<"行經順序：";
    for(int k=0;k<num;k++) cout<<all_routes[0].order[k]<<" ";
    cout<<endl<<"路徑長："<<all_routes[0].sum<<endl<<endl;
    
    for(int u=0;u<num;u++) cout<<map[all_routes[0].order[u]].x<<" "<<map[all_routes[0].order[u]].y<<endl; //城市座標
     */

    cout<<endl<<endl;
     
    return 0;
    
}



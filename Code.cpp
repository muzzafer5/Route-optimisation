#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std; 
#define N 100000
#define pb push_back
#define bus_cost 12
#define dist_cost 9
#define fio ios_base::sync_with_stdio(false);cin.tie(NULL);
//*s.find_by_order(index) //s.order_of_key(element)  
//template <typename T> using ordered_set= tree<T,null_type,less<T>,rb_tree_tag,tree_order_statistics_node_update>; 

clock_t tim;
void clock_time(){
	tim=clock()-tim;
	cout<<"=> "<<(double)tim/CLOCKS_PER_SEC<<" seconds\n";
}

bool mycmp(int x,int y){
	if(x>y)
		return 1;
	return 0;
}


void file(){
	freopen("i.txt", "r", stdin);
}

int max_bus,total_steps,vehicle_capt,i,j,n,bus;
vector<vector<vector<vector<int>>>>check;
vector<vector<int>>dist;
vector<vector<int>>bus_path;

//total_distance
int total_distance(){
	int ans=0;
	for(i=0;i<max_bus;i++){
		for(j=0;j<=n;j++){
			for(int ii=0;ii<=n;ii++){
				for(int jj=0;jj<=n;jj++){
					ans+=dist[ii][jj]*check[i][j][ii][jj];
				}
			}
		}
	}
	return ans;
}
//unused_bus
int unused_bus(){
	int cnt=0;
	for(i=0;i<max_bus;i++){
		int k=0;
		for(j=0;j<=n;j++){
			for(int ii=0;ii<=n;ii++){
				for(int jj=0;jj<=n;jj++){
					k+=check[i][j][ii][jj];
				}
			}
		}
		if(k==1)
			cnt++;
	}
	return cnt;
}
//constraint
bool bus_start(){
	for(i=0;i<max_bus;i++){
		int k=0;
		for(j=0;j<=n;j++){
			for(int ii=0;ii<=n;ii++){
				k+=check[i][j][0][ii];
			}
		}
		if(k==0)
			return 0;
	}
	return 1;
}
bool people_in_1_bus(){
	for(i=1;i<=n;i++){
		int k=0;
		for(j=0;j<max_bus;j++){
			for(int ii=0;ii<=n;ii++){
				for(int jj=0;jj<=n;jj++){
					k+=check[j][ii][i][jj];
				}
			}
		}
		if(k==0)
			return 0;
	}
	return 1;
}
bool final_point(){
	for(i=0;i<max_bus;i++){
		int k=0;
		for(j=0;j<=n;j++){
			for(int ii=0;ii<=n;ii++){
				if(check[i][j][ii][0]==1)
					k=1;
			}
		}
		if(k==0)
			return 0;
	}
	return 1;
}
bool bus_capacity(){
	for(i=0;i<max_bus;i++){
		int k=0;
		for(j=0;j<=n;j++){
			for(int ii=0;ii<=n;ii++){
				for(int jj=0;jj<=n;jj++){
					k+=check[i][j][ii][jj];
				}
			}
		}
		if(k>vehicle_capt)
			return 0;
	}
	return 1;
}
bool satisfying_constraints(){
	if(!bus_start())
		return 0;
	if(!final_point())
		return 0;
	if(!people_in_1_bus())
		return 0;
	if(!bus_capacity())
		return 0;
	return 1;
}
//cost function
int cost_fun(){
	int total_dist=total_distance();
	int total_unused_bus=unused_bus();
	int ans=0;
	ans=dist_cost*(total_dist)+bus_cost*(max_bus-total_unused_bus);
	return ans;
}
// crossover
void crossover(){
	int k=rand();
	k=k%max_bus;
	while(1){
		int cnt=0;
		for(i=0;i<=n;i++){
			for(j=0;j<=n;j++){
				for(int ii=0;ii<=n;ii++){
					if(check[k][i][j][ii]==1)
						cnt++;
				}
			}
		}
		if(cnt>1){
			break;
		}
		k=(k+1)%max_bus;
	}
	int y,a=0,x;
	for(int ii=0;ii<=n;ii++){
		for(i=0;i<=n;i++){
			for(j=0;j<=n;j++){
				if(check[k][ii][i][j]==1 ){
				    x=0;
				    int jj;
				    for(jj=1;jj<=n;jj++){
						for(int iii=0;iii<=n;iii++){
							if(check[k][iii][j][jj]==0){
								x++;
							}
						}
						if(x==n+1){
							break;
						}
						x=0;
				    }
				    if(x==0)
				        continue;
					check[k][ii][i][j]=0;
					check[k][ii][i][jj]=1;
					for(int jjj=0;jjj<=n;jjj++){
						for(int kk=0;kk<=n;kk++){
							if(check[k][kk][j][jjj]==1){
								check[k][kk][j][jjj]=0;
								check[k][kk][jj][jjj]=1;
								break;
							}
						}
					}
					y=j;
					x=jj;
					a=0;
					break;
				}
			}
			if(a==0)
				break;
		}
		if(a==0)
			break;
	}
	a=0;
	for(int i=0;i<max_bus;i++){
		if(i==k)
			continue;
		for(j=0;j<=n;j++){
			for(int ii=0;ii<=n;ii++){
				if(check[i][j][ii][x]==1){
					check[i][j][ii][x]=0;
					check[i][j][ii][y]=1;
					for(int jj=0;jj<=n;jj++){
						for(int kk=0;kk<=n;kk++){
							if(check[i][kk][x][jj]==1){
								check[i][kk][x][jj]=0;
								check[i][kk][y][jj]=1;
								a=1;
								break;
							}
						}
					}
					break;					
				}
			}
			if(a==1)
				break;
		}
		if(a==1)
			break;
	}
// 	if(satisfying_constraints()==0)
// 		crossover();
}
map<int,int>mp;
void mutate(){
	int j,k=0,m=bus;
	while(m--){
		vector<int>tmp;
		mp[j]=1;
		j=0;
		while(1){
			int min_dist=INT_MAX;
			for(i=0;i<=n;i++){
				if(dist[j][i]<min_dist && mp.find(i)==mp.end()){
					min_dist=dist[j][i];
					k=i;
				}
			}
			j=k;
			mp[j]=1;
			if(j!=0)
				tmp.push_back(j);
			if(tmp.size()>vehicle_capt){
				break;		
			}
		}	
		bus_path.push_back(tmp);
	}
}
//mutation
void mutation(){
	int k=rand();
	k=k%max_bus;
	while(1){
		int cnt=0;
		for(i=0;i<=n;i++){
			for(j=0;j<=n;j++){
				for(int ii=0;ii<=n;ii++){
					if(check[k][i][j][ii]==1)
						cnt++;
				}
			}
		}
		if(cnt>1){
			break;
		}
		k=(k+1)%max_bus;
	}
	int ii;
	for(i=0;i<=n;i++){
		for(j=0;j<=n;j++){
			for(int ii=0;ii<=n;ii++){
				if(check[k][i][j][ii]==1){
					check[k][i][j][ii]=0;
					for(int jj=0;jj<=n;jj++){
						for(int kk=0;kk<=n;kk++){
							if(check[k][kk][ii][jj]==1){
								check[k][kk][ii][jj]=0;
								check[k][kk][j][jj]=1;
								break;
							}
						}
					}
				}
			}
		}
	}
	int x,y;
	x=k;
	y=ii;
	k=rand();
	k=k%max_bus;
	while(1){
		int cnt=0;
		if(k==x)
			continue;
		for(i=0;i<=n;i++){
			for(j=0;j<=n;j++){
				for(int ii=0;ii<=n;ii++){
					if(check[k][i][j][ii]==1)
						cnt++;
				}
			}
		}
		if(cnt<vehicle_capt){
			break;
		}
		k=(k+1)%max_bus;
	}	
	for(i=0;i<n;i++){
		for(j=0;j<=n;j++){
			for(int ii=0;ii<=n;ii++){
				if(check[k][i][j][ii]==1){
					check[k][i][j][y]=1;
					check[k][i][j][ii]=0;
					check[k][(i+1)%(n+1)][y][ii]=1;
				}
			}
		}
	}
// 	if(satisfying_constraints()==0)
// 		crossover();	
}

//random_assign parent
void parent(){
	int x=n/2;
	for(i=0;i<x;i++){
		check[0][i][i][i+1]=1;
	}
	check[0][i][i][0]=1;
	check[1][0][0][x+1]=1;
	for(i=x+1;i<n;i++){
		check[1][i-x][i][i+1]=1;
	}
	check[1][i-x][i][0]=1;
    
}
int calc_dist(pair<double,double>x,pair<double,double>y){
	int distance;
	distance=sqrt(pow((x.first-y.first),2)+pow((x.second-y.second),2));
	return distance;
}
int main(){
	fio; 
	file();
	double x,y;
	cout<<"Enter total no. of passenger"<<endl;
	cin>>n;
	cout<<"Enter maximum buses"<<endl;
	cin>>max_bus;
	cout<<"Enter vehicle capacity"<<endl;
	cin>>vehicle_capt;
	vector<pair<double,double>>co_ord;
	cout<<"Enter the co-ordinates of Bosch"<<endl;
	cin>>x>>y;
	co_ord.pb({x,y});
	bus=ceil((n*(1.0))/vehicle_capt);
	cout<<"Enter the co-ordinates of passengers"<<endl;
	for(i=0;i<n;i++){
		cin>>x>>y;
		x*=1000;
		y*=1000;
		co_ord.pb({x,y});
	}	
	for(i=0;i<=n;i++){
		vector<int>v(n+1);
		dist.pb(v);
		for(j=0;j<=n;j++){
			dist[i][j]=calc_dist(co_ord[i],co_ord[j]);
		}
	}
	int total_steps=n+1;
	vector<vector<int>>v;
	for(i=0;i<=n;i++){
		vector<int>tmp(n+1);
		v.pb(tmp);
	}
	vector<vector<vector<int>>>tmp1;
	for(i=0;i<=n;i++){
		tmp1.pb(v);
	}
	for(i=0;i<max_bus;i++){
		check.pb(tmp1);
		for(j=0;j<total_steps;j++){
			for(int ii=0;ii<=n;ii++){
				for(int jj=0;jj<=n;jj++){
					check[i][j][ii][jj]=0;
				}
			}
		}
	}	 
 	parent();
	cout<<"Initial path assigned for parent --  "<<endl;
	for(i=0;i<max_bus;i++){
	    cout<<"For bus "<<i+1<<" -----";
	    int jj=0,a=-1;
	    while(jj || a<0){
	        a=0;
    	    for(j=0;j<=n;j++){
    	        for(int ii=0;ii<=n;ii++){
    	            if(check[i][j][jj][ii]==1){
    	                jj=ii;
    	                a=1;
    	                break;
    	            }
    	        }
    	        if(a==1)
    	            break;
    	    }
    	    cout<<"-> "<<jj;
	    }
		cout<<endl;
	}	 
	cout<<"Genetic algorithm----"<<endl;
    set< pair<int,vector<vector<vector<vector<int>> > > >>heap;
 	heap.insert({cost_fun(),check});
 	int cnt=15;
 	while(cnt){
		check=(*heap.begin()).second;
        crossover();
		heap.insert({cost_fun(),check});
		if(heap.size()>5){
			auto it=heap.end();
			it--;
			heap.erase(it);
		}	
		check=(*heap.begin()).second;
		cnt--;
	}
	mutate();	
	cout<<"Total cost - "<<(*heap.begin()).first<<endl;
	cout<<"Final solution -------"<<endl;
	for(i=0;i<bus;i++){
		cout<<"Customer picked by bus no. "<<i+1<<" -----";
		for(j=0;j<bus_path[i].size();j++){
			cout<<"-> "<<bus_path[i][j];
		}
		cout<<endl;
	}
	return 0;
}

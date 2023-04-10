#include <iostream>
#include <fstream>
#include <armadillo>
#include <string>

using namespace std;
int V;
int cost[100][100];

arma::vec append_vec(arma::vec & vector, double value)
	{
		vector.resize(vector.n_elem+1);
		vector(vector.n_elem-1)=value;
		return vector;
	}
arma::uvec append_vec(arma::uvec & vector, int value)
	{
		vector.resize(vector.n_elem+1);
		vector(vector.n_elem-1)=value;
		return vector;
	}

arma::mat load_csv_arma (const std::string & path) {
    arma::mat X;
    X.load(path, arma::csv_ascii);
    return X;
}

arma::vec in_dist(int size, int value, int source)
	{
		arma::vec dist;
		dist.resize(size);
		for (int i=0; i<size;i++)
			{dist(i)=value;}
		dist(source)=0;
		return dist;
	}

arma::uvec in_prev(int size)
	{
		arma::uvec prev;
		prev.resize(size);
		for (int i=0; i<size;i++)
			{prev(i)=i;}
		return prev;
	}

int get_nearest(int V,arma::vec dist, bool  *visited){
	int min_value=999;
	int min_node=0;

	for(int ii=0; ii<V-1; ii++)
		{
			if( !visited[ii] && dist(ii)<min_value)
				{
					min_value=dist(ii);
					min_node = ii;
				}
		}
	return min_node;

}

arma::uvec dijkstra(arma::mat Mapa,int V,int source)
	{	
		bool visited[V]={0};
	
		arma::uvec prev=in_prev(V);
		arma::vec dist=in_dist(V,999,source);
		for(int ii=0; ii<V-1; ii++)
			{
				int neares_unvisited= get_nearest(V,dist,visited);
				visited[neares_unvisited]= true;

				for( int adj =0; adj<V; adj++)
					{
						if (Mapa(neares_unvisited,adj) != 999)
							{if(dist(adj)<dist(neares_unvisited)+Mapa(neares_unvisited,adj))
								{
									dist(adj)=dist(neares_unvisited)+Mapa(neares_unvisited,adj);
									prev[adj]=neares_unvisited;
									cout<<"Se cumplio\n";
								}
							}

					}

			}
		return prev;
		
	}

arma::uvec find_path(arma::mat Mapa,int source, int target)
	{	

		int V=Mapa.n_rows;
		arma::uvec prev = dijkstra(Mapa,V,source);
		arma::uvec path;
		path.resize(1);
		path=append_vec(path,target);
		int node=target;
		for(int jj=0;jj<prev.size();jj++)
			{cout<<prev(jj)<<" "<<jj<<"\n";}
		// while (node!=source)
		// 	{
		// 		node=prev[node];
		// 		path=append_vec(path,node);
		// 		cout<<node<<"\n";
		// 	}
		return reverse(path);


	}

int main(){

	arma::mat Mapa = load_csv_arma("matriz23.csv");
	
	int source=1;
	int target=7;

	arma::uvec path =find_path(Mapa,source,target);

	return 0;

}
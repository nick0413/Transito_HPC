#include <armadillo>
void print_connections(arma::sp_mat Mapa, int node)
	{
		for(int ii=0; ii<Mapa.n_cols;ii++)
			{
				if(Mapa(node,ii)!=0)
					{std::cout<<node<<"\t"<<ii<<"\t"<<Mapa(node,ii)<<"\n";}
			}
	}

void fill_arma(arma::vec & vec, double value)
	{
		int q=vec.size();
		for(int ii=0;ii<q;ii++)
			{vec(ii)=value;}
	}
arma::mat load_csv_arma (const std::string & path) 
	{
		arma::mat X;
		X.load(path, arma::csv_ascii);
		return X;
	}

arma::ivec append_vec(arma::ivec & vector, int value)
	{
		vector.resize(vector.n_elem+1);
		vector(vector.n_elem-1)=value;
		return vector;
	}

arma::vec append_vec(arma::vec & vector, double value)
	{
		vector.resize(vector.n_elem+1);
		vector(vector.n_elem-1)=value;
		return vector;
	}
int getMin_arma(arma::vec dist, bool visited [], int n)
	{
		int u = 0;
		int min=9999;

		for(int ii=0; ii<n; ii++)
			{
				if( !visited[ii] && dist(ii)<min)
					{
						min=dist(ii);
						u=ii;
					}
			}

		return u;
	}

arma::ivec path_find(arma::ivec prev, int source, int target){
  int node=target;

  arma::ivec path(1);
  path(0)=target;
  while(prev(node)!=-1)
    {	
      //cout<<prev(prev(node))<<"\n";
      path=append_vec(path,prev(node));
      node=prev(node);
    }
  return path;
}

arma::ivec dijkstra_arma(arma::mat Mapa, int source,int target)
	{	
		int n=Mapa.n_rows;
		arma:: ivec prev(n);
		arma::vec dist(n);
		bool visited[n];
		bool verbose=false;
		if(source==2965 ||target==2829)
			{verbose=true;std::cout<<"\n";}
		for(int rr=0;rr<n;rr++)
			{visited[rr]=false;}
		fill_arma(dist,9999);

		dist(source)=0;
		prev(source)=-1;

		for(int gg=0; gg<n-1;gg++)
			{	
				if(verbose)std::cout<<gg<<"\t"<<n-1<<"\n";
				int u= getMin_arma(dist,visited,n);
				visited[u]=true;

				for(int v = 0; v<n;v++)	
					{	
						if(Mapa(u,v)!=9999)
							{
								if(!visited[v] && (dist(u)+Mapa(u,v))< dist(v))
									{
										prev(v)=u;						
										dist(v)=dist(u)+Mapa(u,v);
									}
							}

					}
			}
		prev(source)=-1;
				
		arma::ivec path = reverse(path_find(prev,source,target));
		return path;
	}




arma::ivec dijkstra_arma_sp(arma::sp_mat Mapa, int source,int target)
	{	
		int n=Mapa.n_rows;
		arma:: ivec prev(n);
		arma::vec dist(n);
		bool visited[n];
		bool verbose=true;
		int next=0;
		for(int rr=0;rr<n;rr++)
			{visited[rr]=false;}
		fill_arma(dist,9999);

		dist(source)=0;

		// print_connections(Mapa,2868);
		prev(source)=-1;
		int gg=0;
		for(; gg<n-1;gg++)
			{
				// if(verbose)std::cout<<gg<<"\t"<<n-1<<"\n";
				int u= getMin_arma(dist,visited,n); // Get the closest unvisited node
				visited[u]=true;

				for(int v = 0; v<n;v++)	
					{	
						if(Mapa(u,v)!=0)
							{	
								// std::cout<<"\t"<<visited[v]<<" "<<dist(u)<<"\t"<<Mapa(u,v)<<"\t"<<dist(v)<<"\t------------\n";
								//  std::cout<<u<<"\t"<<v<<"\t"<<Mapa(u,v)<<"\n";
								// if(dist(u)!=9999)std::cout<<visited[v]<<" "<<dist(u)<<"\t"<<Mapa(u,v)<<"\t"<<dist(v)<<"\t------------\n";
								// if(u==next || v==next)std::cout<<dist(next)<<" "<< Mapa(u,v)<<" "<<(dist(u)+Mapa(u,v))<<" "<<dist(v)<<"\n";
								if(!visited[v] && (dist(u)+Mapa(u,v))< dist(v))
									{
										prev(v)=u;						
										dist(v)=dist(u)+Mapa(u,v);
										// std::cout<<dist(v)<<" En nodo "<<v<<"\n";
										next=v;
									}
							}

					}
			}
		// std::cout<<gg<<" "<<n-1 <<"final Dijkstra\n";
		// if(gg==n-1){std::cout<<"finalized search\n";}
		prev(source)=-1;
				
		arma::ivec path = reverse(path_find(prev,source,target));
		return path;
	}



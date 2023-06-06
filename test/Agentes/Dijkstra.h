#include <armadillo>

void fill_arma(arma::vec & vec, double value){
  int q=vec.size();
  for(int ii=0;ii<q;ii++)
    {
      vec(ii)=value;
    }
}


arma::vec mat_to_vec(arma::mat matrix)
	{
		arma::vec vector = arma::vectorise(matrix);
		return vector;
	}
arma::mat load_csv_arma (const std::string & path) {
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
int getMin_arma(arma::vec dist, bool visited [], int n){
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
  bool visited[n] ={0};
  fill_arma(dist,9999);

  dist(source)=0;
  prev(source)=-1;

  for(int gg=0; gg<n-1;gg++)
    {
      int u= getMin_arma(dist,visited,n);
      visited[u]=true;

      for(int v = 0; v<n;v++)	
	{	if(Mapa(u,v)!=9999)
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

import numpy as np
import random as rd
import networkx as nx
import matplotlib.pylab as plt
import sys 

try:
	pr=sys.argv[1]
except:
	pr=None
if (pr=='verbose'):
	verbose=True
else:
	verbose=False


def gen_graph():
	while True:
		print('----------')
		Msize=rd.randint(5,20)
		M=np.random.randint(0,10,(Msize,Msize))
		#print(M)
		for i in range(Msize):
			for j in range(Msize):
				if(rd.randint(0,100)>10):
					M[i,j]=0
		G=nx.from_numpy_array(M)

		#G.remove_edges_from(list(nx.selfloop_edges(G)))

		if not (G.is_multigraph()):
			if verbose:print('multigraph',G.is_multigraph())\
			
			if nx.is_connected(G):
				if nx.number_of_isolates(G)==0:
					if verbose:print('no isolates')
					break
	return G

G1=gen_graph()	



M1=nx.to_numpy_array(G1)
G1=nx.from_numpy_array(M1)

np.savetxt('Mapa.csv',M1,delimiter=',')
if verbose:
	edge_colors = M1[np.triu_indices_from(M1)]
	edges=np.ones((edge_colors.shape[0],4))
	print(edge_colors)
	pos = nx.spring_layout(G1)
	print(pos)
	#nx.draw(G1, pos=pos, with_labels=True, edge_color=edges)
	print(M1)
	nx.draw(G1,with_labels = True)
	plt.show()
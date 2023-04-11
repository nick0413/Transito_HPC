import numpy as np
import networkx as nx
import matplotlib.pylab as plt


Mapa=np.loadtxt("Madyacencia.txt", delimiter=',')

Mapa[Mapa==9999]=0
G=nx.from_numpy_array(Mapa)

nx.draw(G,with_labels=True)
plt.show()

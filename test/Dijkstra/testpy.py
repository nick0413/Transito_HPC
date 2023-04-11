import numpy as np
import networkx as nx
import matplotlib.pylab as plt
import sys 

start=int(sys.argv[1])
end=int(sys.argv[2])

arr=np.loadtxt("Madyacencia.txt",delimiter=',')

arr[arr==9999]=0

#plt.imshow(arr)
#plt.show()
G=nx.from_numpy_array(arr)
nx.draw(G,with_labels = True)
#plt.show()
print(nx.shortest_path(G,source=start,target=end, weight='weight'))
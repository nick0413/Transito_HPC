import numpy as np
import matplotlib.pylab as plt 
import imageio.v3 as iio
import networkx as nx

im = iio.imread('mapa_v0.png')
mapa=np.array(im)
mapa_b=mapa>250
mapa_c=mapa_b[:, :, 0]*1
print(mapa_c.shape)


G=nx.Graph()
nx.set_node_attributes(G, 0, "Aceso")
for i in range(mapa_c.shape[0]):
    for j in range(mapa_c.shape[1]):
        #print(i,j)
        k=str(i)+'_'+str(j)
        G.add_node(k)
        G.nodes[k]["Aceso"]=mapa_c[i,j]

print(mapa_c.shape)
for i in range(mapa_c.shape[0]):
    for j in range(mapa_c.shape[1]):

            k=str(i)+'_'+str(j)
            kd=str(i+1)+'_'+str(j)
            kb=str(i)+'_'+str(j+1)
            
            if i+1==mapa_c.shape[0]:
                if j+1<mapa_c.shape[1]:
                    G.add_edge(k,kb)
                    # print(k,'->',kb)
            elif j+1==mapa_c.shape[1]:
                
                G.add_edge(k,kd)
                # print(k,'->',kd)

            else :
                # print("---")
                G.add_edge(k,kd)
                G.add_edge(k,kb)
                # print(k,'->',kb)
                # print(k,'->',kd)

pos= nx.spring_layout(G,iterations=200)
nx.draw(G,with_labels=True,pos=pos)
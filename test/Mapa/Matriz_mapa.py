import numpy as np
import matplotlib.pylab as plt


datos=np.loadtxt("conexiones.txt", delimiter=",")




Madyacencia=np.zeros([int(np.amax(datos[:,0:2])),
                      int(np.amax(datos[:,0:2]))])
                      


for i in range(datos.shape[0]):
  #print(int(datos[i,0]))
  Madyacencia[int(datos[i,0])-1,int(datos[i,1]-1)]=np.sqrt(datos[i,2]**2+datos[i,3]**2)

Madyacencia=(Madyacencia+Madyacencia.T).astype("int32")
Madyacencia[Madyacencia==0]=9999
np.fill_diagonal(Madyacencia, 0)
plt.imshow(Madyacencia)
np.savetxt("Madyacencia.txt",Madyacencia,delimiter=',',fmt='%d')

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from scipy.spatial import Voronoi, voronoi_plot_2d


img = plt.imread("../figs/Contenedores-Residuos-PuntosReciclaje.jpg")
img = np.flipud(img)
fig, ax = plt.subplots()
x =np.loadtxt("contenedoresxysimplificado.txt", usecols=[0, 1], unpack=True)

#print(x)
x2=np.transpose(x)

#print(x2)
df=pd.DataFrame(x2, columns=list('XY'))
df.Y=df.Y*-1
#plt.scatter(df.X, df.Y,marker=".")
#plt.show()
vor = Voronoi(df)
ax.imshow(img, extent=[0, 4250, 0, -4250])
voronoi_plot_2d(vor,ax=ax)
plt.savefig('vornoiunal.pdf')
#plt.show()

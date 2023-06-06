import numpy as np
import matplotlib.pylab as plt 
import imageio.v3 as iio
import networkx as nx
from itertools import count
import os

im = iio.imread('./Environment/mapa_v5.png')
mapa=np.array(im)
mapa_b=mapa>250
mapa_c=mapa_b[:, :, 0]*1
print(mapa_c)
np.savetxt("./Environment/Aceso.csv", mapa_c, delimiter=",",fmt='%0d')



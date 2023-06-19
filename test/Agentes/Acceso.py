import numpy as np
import imageio.v3 as iio


im = iio.imread('./Environment/mapa_v5.png')
mapa=np.array(im)
mapa_b=mapa>250
mapa_c=mapa_b[:, :, 0]*1
np.savetxt("./Environment/Acceso.csv", mapa_c, delimiter=",",fmt='%0d')
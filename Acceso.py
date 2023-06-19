import numpy as np
import imageio.v3 as iio
res=[10,50,100,200]
for i in res:
	im = iio.imread(f'./Environment/Mapas/{i}.png')
	mapa=np.array(im)
	mapa_b=mapa>250
	mapa_c=mapa_b[:, :, 0]*1
	np.savetxt(f"./Environment/Acceso/Acceso_{i}.csv", mapa_c, delimiter=",",fmt='%0d')
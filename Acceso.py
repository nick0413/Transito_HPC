import numpy as np
import csv
import imageio.v3 as iio

N=10
scale=100


def x_coord(n,N,scale):
	# Replace with your custom function logic
	x=int(n/N)
	return x*scale

def y_coord(n,N,scale):
	# Replace with your custom function logic
	y=n%N
	return y*scale



def fill_array(N,scale):
	array = np.empty((N*N, 3))
	array[:, 0] = np.arange(N*N)
	
	for i in range(N*N):
		x = array[i, 0]
		array[i, 1] = x_coord(i,N,scale)
		array[i, 2] = y_coord(i,N,scale)
	
	return array

def write_numpy_array_to_csv(array, filename):
	with open(filename, 'w', newline='') as csvfile:
		writer = csv.writer(csvfile)
		writer.writerows(array)



res=[10,50,100,200]
scale=[100,20,10,5]
for i in range(len(res)):
	# print(i)
	im = iio.imread(f'./Environment/Mapas/{res[i]}.png')
	mapa=np.array(im)
	mapa_b=mapa>250
	mapa_c=mapa_b[:, :, 0]*1
	np.savetxt(f"./Environment/Acceso/Acceso_{res[i]}.csv", mapa_c, delimiter=",",fmt='%0d')
	result = fill_array(res[i],scale[i])
	write_numpy_array_to_csv(result,filename=f"./Environment/Nodos_finales/nodos{res[i]}.csv")
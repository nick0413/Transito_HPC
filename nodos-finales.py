import numpy as np
import csv

N=10
scale=100
def x_coord(n):
	# Replace with your custom function logic
	x=int(n/N)
	return x*scale

def y_coord(n):
	# Replace with your custom function logic
	y=n%N
	return y*scale

def write_numpy_array_to_csv(array, filename):
	with open(filename, 'w', newline='') as csvfile:
		writer = csv.writer(csvfile)
		writer.writerows(array)

def fill_array():
	array = np.empty((N*N, 3))
	array[:, 0] = np.arange(N*N)
	
	for i in range(N*N):
		x = array[i, 0]
		array[i, 1] = x_coord(i)
		array[i, 2] = y_coord(i)
	
	return array



# Call the function to fill the array
result = fill_array()

# Print the resulting array
write_numpy_array_to_csv(result, 'nodos-finales.csv')
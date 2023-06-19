filename = "xd/1_example.txt"


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



def fill_array():
	array = np.empty((N*N, 3))
	array[:, 0] = np.arange(N*N)
	
	for i in range(N*N):
		x = array[i, 0]
		array[i, 1] = x_coord(i)
		array[i, 2] = y_coord(i)
	
	return array

def write_numpy_array_to_csv(array, filename):
	with open(filename, 'w', newline='') as csvfile:
		writer = csv.writer(csvfile)
		writer.writerows(array)
# Open the file in write mode
result = fill_array()
print(result)
write_numpy_array_to_csv(result,filename=filename)
# with open(filename, "w") as file:
#     # Perform operations on the file
#     file.write("This is a new file.")

import numpy as np
import csv

def write_numpy_array_to_csv(array, filename):
    with open(filename, 'w', newline='') as csvfile:
        writer = csv.writer(csvfile)
        writer.writerows(array)

def fill_array():
    array = np.empty((100, 3))
    array[:, 0] = np.arange(100)
    
    for i in range(100):
        x = array[i, 0]
        array[i, 1] = x_coord(i)
        array[i, 2] = y_coord(i)
    
    return array

def x_coord(n):
    # Replace with your custom function logic
    x=int(n/10)
    return x*100

def y_coord(n):
    # Replace with your custom function logic
    y=n%10
    return y*100

# Call the function to fill the array
result = fill_array()

# Print the resulting array
write_numpy_array_to_csv(result, 'nodos-finales.csv')
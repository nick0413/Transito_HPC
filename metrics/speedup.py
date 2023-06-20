import sys
import numpy as np
import matplotlib.pyplot as plt


def formatter(data):
    rows = data.shape[0]
    cols = data.shape[1]
    auxData = np.zeros((rows, 2), dtype=float)
    # print(data)
    for row in range(rows):
        # Thread
        auxData[row, 0] = data[row][0]
        # Mean
        auxData[row, 1] = (np.sum(data[row, :]) - auxData[row, 0]*cols/2.0)*2.0/cols

    return auxData


labels = ['50x50 px', '100x100 px']
lines_style = [':', '--', '-.']
data_files = []
data_np = []
linewidth = 1
markersize = 3

for i in range(1, len(sys.argv)-1, 1):
    data_files.append(sys.argv[i])


figure_name = sys.argv[-1]

# Extract data from txt files
# title = np.loadtxt(file_data_1, dtype='str', max_rows=1)

for i in data_files:
    data_np.append(formatter(np.loadtxt(i, dtype=np.float64, skiprows=0,
                                        delimiter=' ')))


n = data_np[0][:, 0]

# Speedup calculation
for i in range(len(labels)):
    data_np[i][:, 1] = data_np[i][0, 1]/data_np[i][:, 1]

# Plot

plt.ylabel("Speedup $[s/s]$")
plt.xlabel("Threads")
# plt.title("16 cores with 8 physicals")

for i in range(len(labels)):
    plt.plot(n, data_np[i][:, 1], lines_style[i], linewidth=linewidth,
             marker='o', markersize=markersize, label=labels[i])

plt.plot(n, n, label="Theoretical", linewidth=linewidth)
plt.legend()
plt.grid(linestyle='--', linewidth=0.3)
plt.savefig(figure_name, format="pdf", bbox_inches="tight")
# plt.show()

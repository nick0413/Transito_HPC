import sys
import numpy as np
import matplotlib.pyplot as plt

# functions


def formatter(data):
    rows = data.shape[0]
    cols = data.shape[1]
    auxData = np.zeros((rows, 2), dtype=float)
    print(data)
    for row in range(rows):
        # Thread
        auxData[0] = data[row][0]
        # Mean
        auxData[1] = (np.sum(data[row, :]) - cols/2.0)*2.0/cols

    return auxData


labels = ['res=50', 'res=100']
lines_style = [':', '--', '-.']
data_files = []
data_np = []
linewidth = 1


for i in range(1, len(sys.argv)-1, 1):
    # print(f"file data name: {sys.argv[i]}"}
    data_files.append(sys.argv[i])

figure_name = sys.argv[-1]


# Extract data from txt files
# title = np.loadtxt(file_data_1, dtype='str', max_rows=1)

for i in data_files:
    data_np.append(formatter(np.loadtxt(i, dtype=np.float64,
                                        skiprows=0, delimiter=' ')))

n = data_np[0][:, 0]

# Speedup calculation
for i in range(len(labels)):
    data_np[i][:, 1] = data_np[i][0, 1]/(data_np[i][:, 1]*n)

# Plot

plt.ylabel("$\eta _p [s/s]$")
plt.xlabel("Threads")
#plt.title("16 cores with 8 physicals")

for i in range(len(labels)):
    plt.plot(n, data_np[i][:, 1], lines_style[i], linewidth=linewidth, label=labels[i])

plt.plot(n, np.ones(len(n)), linewidth=linewidth, label="Theorical")
plt.plot(n, 0.6*np.ones(len(n)), linewidth=linewidth, label="0.6")

plt.legend(loc='upper right')
plt.grid(linestyle='--', linewidth=0.3)
plt.savefig(figure_name, format="pdf", bbox_inches="tight")
# plt.show()

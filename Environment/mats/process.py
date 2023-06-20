import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation



arrays=[]

for i in range(1,15):
    array=np.loadtxt(f"{i}.csv", delimiter=',')
    arrays.append(arrays)

fig, ax = plt.subplots()

# Define the initialization function for the animation
def init():
    ax.clear()
    img = ax.imshow(arrays[0])
    return [img]

# Define the update function for the animation
def update(frame):
    ax.clear()
    img = ax.imshow(arrays[frame])
    return [img]

interval = 200

# Create the animation
anim = animation.FuncAnimation(fig, update, frames=len(arrays), init_func=init, blit=True, interval=interval)


import matplotlib.pyplot as plt
import numpy as np

N, Act1, Act2 =np.loadtxt("times.txt", usecols=[0, 1, 3], unpack=True)
Act1su=Act1[0]/Act1
Act2su=Act2[0]/Act2

teor=[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16]
plt.figure(0)
plt.plot(N,Act1su,'xr-',N,Act2su,'xg-',N,N)
plt.title('speedup computador con 16 cores')
plt.legend(['Act1','Act2','theoric'])
plt.savefig('speedup.pdf')
#plt.show()

Act1ef=Act1su/N
Act2ef=Act2su/N
ones=[1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1]
plt.figure(1)
plt.plot(N,Act1ef,'xr-',N,Act2ef,'xg-',N,ones)
plt.title('eficiencia computador con 16 cores')
plt.legend(['Act1','Act2','theoric'])
plt.savefig('eficiencia.pdf')
#plt.show()
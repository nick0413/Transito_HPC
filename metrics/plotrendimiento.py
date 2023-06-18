import matplotlib.pyplot as plt
import numpy as np

Act=[]#cargar los datos de tiempos en esta lista
Actsu=[]#aca se guardan los speedups
Actef=[]#aca se guardan las eficiencias
N =np.loadtxt("./physics.txt", usecols=[0], unpack=True)
a=np.loadtxt("./physics.txt")
maxcol=len(a[1])
maxrows=len(a)
total=0
#print(N[-1])
for i in range(int(maxcol/2)):
    Act1=np.loadtxt("./physics.txt", usecols=[2*i+1], unpack=True)
    Act.append(Act1)
    total=total+Act1
    #print(total)
    #print(Act[i])
    Acts=Act[i][0]/Act[i]
    Actsu.append(Acts)
    #print(Actsu[i])
#calcular promedio por num threads

total=total/maxrows
totalsu=total[0]/total






plt.figure()
for i in range(int(maxcol/2)):
    #plt.figure(i)
    plt.plot(N,Actsu[i],'xr-',N,N)
    plt.title('speedup computador con 16 cores')
    plt.legend(['Act1','theoric'])
    #plt.show()
#plt.savefig('speedup.pdf')
#plt.show()

plt.figure()
plt.plot(N,totalsu,'xr-',N,N)
plt.title('Physics speedup')
plt.legend(['Mean','theoric'])
plt.savefig('physics_speedup.pdf')
#plt.show()


totalef=totalsu/N
o=np.ones(maxrows)
plt.figure()
plt.plot(N,totalef,'xg-',N,o)
plt.title('Physics efficiency')
plt.legend(['efficiency','theoric'])
plt.savefig('physics_efficiency.pdf')
#plt.show()


N2, inits =np.loadtxt("./inits.txt", usecols=[0, 1], unpack=True)
initssu=inits[0]/inits
plt.figure()
plt.plot(N2,initssu,'xb-',N2,N2)
plt.title('Init speedup ')
plt.legend(['init','theoric'])
plt.savefig('init_speedup.pdf')
#plt.show()


initsef=initssu/N2
o2=np.ones(len(N2))
plt.figure()
plt.plot(N2,initsef,'xb-',N2,o2)
plt.title('Init efficiency')
plt.legend(['init','theoric'])
plt.savefig('init_efficiency.pdf')
#plt.show()

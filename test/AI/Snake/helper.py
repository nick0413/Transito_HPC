import matplotlib.pylab as plt
from IPython import display

plt.ion()

def plot(scores,mean_scores):
    display.clear_output(wait=True)
    display.display(plt.gcf())
    plt.clf()
    plt.title('Entrenamiento')
    plt.xlabel('Partidas')
    plt.ylabel('Puntaje')
    plt.plot(scores, label='puntaje')
    plt.plot(mean_scores,label='moving average')
    plt.ylim(ymin=0)
    plt.legend()
    
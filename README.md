# Modelo de agentes para la generación de residuos en la UN

- [Requerimientos](#requerimientos)

Una simulación modelo de agentes que se propone simular la generación de basura en la Universidad Nacional de Colombia. 

Se desarolla a modo de aplicación un ejecutable donde se observa la evolución en el tiempo de la simualación. En una ventana se observan los agentes que imitan el comportamiento de los residentes de la universidad. Con los patrones de movimiento de los agentes se puede rastrear la generación de basura en las distintas regiones del campus. 

## Programación paralela

La implementación de la palalelización se llevó a cabo mediante dos herramientas. La primera, se trata de la clase [sf::Thread](https://www.sfml-dev.org/documentation/2.5.1/classsf_1_1Thread.php) implementada por SFML. La segunda se hace mediante OpenMP que permite asignar un trabajo a varios threads.

### Uso de threads

En en thread principal se maneja todo lo relacionado con la visualización. También, se inicializa el estado de los agentes. El segundo thread se enfoca en actualizar el estado de los agentes. 

### Uso de OpenMP

Los agentes están contenidos en un array unidimensional el cual se tiene que manipular elemento por elemento para inicializar o actualizar. La lógica consiste en asignarle a cada thread una parte del arreglo tal como lo expresa la Figura ee1. Se le asigna a cada thread el piso de la división entre el número de elementos del arreglo y el número de treads y se le deja al último thread el módulo de la división anterior. Así, se asegura que todas las posición del arreglo sean correctamente repartidas en una asignación contigua.


<img src="./figs/inf/omp_threads.png" alt="Asignación de array en threads de OpenMP" width=250>

*Figura ee1. Asignación de array en threads de OpenMP*


### Métricas

#### Speedup 
La manera en la que se determina el speedup:

$$ S(n)=\frac{T(1)}{T(n)} $$

#### Eficiencia paralela

La eficiencia paralela viene dada por la fórmula:

$$ \eta _p (n)= \frac{S(n)}{n} $$



## Requerimientos
- armadillo This site was built using [Armadillo c++](https://arma.sourceforge.net/download.html)
- [SFML](https://www.sfml-dev.org/): Se usa para la interfaz gráfica y tiene su propia manera de lanzar threads.
- [OpenMP](https://www.openmp.org/): Se utiliza para programar de manera paralela por medio de memoria distribuida.

Descargar la última versión de la rama main del repositor.

## Windows

## Linux
Para compilar se utiliza el gmake de esta manera
```bash
gmake linux
```
Se lanza especificando el número de threads que se quieran utilizar.

```bash
OMP_NUM_THREADS=$threads ./Dispersion_social.out
```

## Plataformas Utilizadas para el trabajo en equipo
Permite organizar de manera rápida los encuentros.

<img src="./figs/inf/whatsapp.png" alt="Imagen de WhatsApp" width=50>

Facilita la manera de programar en conjunto y llevar un control de versiones.

<img src="./figs/inf/github.png" alt="Imagen de GitHub" width=50>

Se crean salas para interactuar, distintos canales y los mensajes son persistentes.

<img src="./figs/inf/discord.png" alt="Imagen de Discord" width=50>



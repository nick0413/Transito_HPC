# Simualcion de generacion de basura en un campus universitario
- [Instalacion](#Instalacion)
- [Hardware](#hardware)

Una simulación modelo de agentes que se propone simular la generación de basura en la Universidad Nacional de Colombia. 

Se desarrolla a modo de aplicación ejecutable donde se observa la evolución en el tiempo de la simulación. En una ventana se observan los agentes que imitan el comportamiento de los residentes de la universidad. Con los patrones de movimiento de los agentes se puede rastrear la generación de basura en las distintas regiones del campus. 


# Instalacion
- Instalar armadillo [Armadillo c++](https://arma.sourceforge.net/download.html)


Descargar la ultima version de la rama main del repositor

## Windows

## Linux

# Hardware
- Para correr la simulación a escala entera (un mapa de 200x200 pixeles) se requiere 16Gb de Ram para que sea estable. Los requerimientos de resoluciones menores aún no se han medido. 
- El proceso más intensivo es el preprocesamiento del mapa, que usa funciones de Madyacencia.h y es la razón del alto consumo de ram,
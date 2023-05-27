FILES_CPP=$(wildcard *.cpp)

MAIN=main
MAIN_OUT=$(MAIN).out

Madyacencia = ./files/Madyacencia.txt
Matriz_mapa = ./files/Matriz_mapa.py

# Warning Flags
WAR_FLAGS = -Wall -Wextra

# Libraries
LIB_FLAGS = -lsfml-graphics -lsfml-window -lsfml-system -larmadillo -fopenmp

# Additionals flags
ADI_FLAGS = -g

# Sanitizers
SAN_FLAGS = -fsanitize=undefined,thread

# OPENMP
NUM_THR = 8

all:compile link

compile : Madyacencia
	g++ -Isrc/include -c main.cpp

link:
	g++ main.o -o transit -Lsrc/lib -lsfml-graphics -lsfml-window -lsfml-system -lblas

Madyacencia : Matriz_mapa 
	py Matriz_mapa


$(MAIN_OUT) : $(FILES_CPP)
	g++ -c $^
	g++ $(MAIN).o -o $(MAIN_OUT) -lsfml-graphics -lsfml-window -lsfml-system
	./$(MAIN_OUT) 

.PHONY : compile_and_run
compile_and_run:
	g++ $(ADI_FLAGS) $(SAN_FLAGS) $(WAR_FLAGS) -c $(FILES_CPP)
	g++ $(ADI_FLAGS) $(SAN_FLAGS) $(WAR_FLAGS) $(MAIN).o -o $(MAIN_OUT) $(LIB_FLAGS)
	OMP_NUM_THREADS=$(NUM_THR) ./$(MAIN_OUT) 


.PHONY : debugging
debugging : 
	g++ $(ADI_FLAGS) $(SAN_FLAGS) $(WAR_FLAGS) -c $(FILES_CPP)
	g++ $(ADI_FLAGS) $(SAN_FLAGS) $(WAR_FLAGS) $(MAIN).o -o $(MAIN_OUT) -lsfml-graphics -lsfml-window -lsfml-system -larmadillo
	gdb $(MAIN_OUT)

.PHONY : profiling
profiling :
	g++ -g -Wall -pg -c $(FILES_CPP)
	g++ -g -Wall -pg $(MAIN).o -o $(MAIN_OUT) -lsfml-graphics -lsfml-window -lsfml-system -larmadillo
	./$(MAIN_OUT)
	gprof $(MAIN_OUT) gmon.out > report.txt


.PHONY : clean
clean : 
	rm -f *.o
	rm -f *.out


## Agradecimientos
# -larmadillo : https://stackoverflow.com/questions/17062110/undefined-reference-to-lapack-wrapper-of-armadillo

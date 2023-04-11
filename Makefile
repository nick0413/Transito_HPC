FILES_CPP=$(wildcard *.cpp)

MAIN=main
MAIN_OUT=$(MAIN).out

all:compile link

compile: Madyacencia.txt
	g++ -Isrc/include -c main.cpp

link:
	g++ main.o -o transit -Lsrc/lib -lsfml-graphics -lsfml-window -lsfml-system -lblas

Madyacencia.txt: Matriz_mapa.py 
	py Matriz_mapa.py


$(MAIN_OUT) : $(FILES_CPP)
	g++ -c $^
	g++ $(MAIN).o -o $(MAIN_OUT) -lsfml-graphics -lsfml-window -lsfml-system
	./$(MAIN_OUT) 

.PHONY : compile_and_run
compile_and_run:
	g++ -c $(FILES_CPP)
	g++ $(MAIN).o -o $(MAIN_OUT) -lsfml-graphics -lsfml-window -lsfml-system -larmadillo
	./$(MAIN_OUT) 


.PHONY : debugging
debugging : 
	g++ -g -c $(FILES_CPP)
	g++ -g $(MAIN).o -o $(MAIN_OUT) -lsfml-graphics -lsfml-window -lsfml-system 
	gdb $(MAIN_OUT)

.PHONY : clean
clean : 
	rm -f *.o
	rm -f *.out


## Agradecimientos
# -larmadillo : https://stackoverflow.com/questions/17062110/undefined-reference-to-lapack-wrapper-of-armadillo

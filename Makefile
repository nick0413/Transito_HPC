all:compile link

compile: Madyacencia.txt
	g++ -Isrc/include -c main.cpp

link:
	g++ main.o -o main -Lsrc/lib -lsfml-graphics -lsfml-window -lsfml-system
	
Madyacencia.txt: Matriz_mapa.py 
	py Matriz_mapa.py


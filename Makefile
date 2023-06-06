all: h

h:
	g++ -Isrc/include -c .\Dispersion_social.cpp
	g++ .\Dispersion_social.o -o Dispersion_social -Lsrc/lib -lsfml-graphics -lsfml-window -lsfml-system -lblas
	.\Dispersion_social.exe  
	
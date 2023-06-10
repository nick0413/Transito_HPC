# Make file
MAIN = Dispersion_social
MAIN_CPP = $(MAIN).cpp
MAIN_OUT=$(MAIN).out


.PHONY : windows
# First compile and then run
windows : windows_compile windows_run

windows_run :
	.\Dispersion_social.exe  

windows_compile :
	g++ -Isrc/include -c .\Dispersion_social.cpp
	g++ .\Dispersion_social.o -o Dispersion_social -Lsrc/lib -lsfml-graphics -lsfml-window -lsfml-system -lblas
.PHONY : ubuntu

ubuntu:
	g++ -c $(MAIN_CPP)
	g++ $(MAIN).o -o $(MAIN_OUT) -lsfml-graphics -lsfml-window -lsfml-system -larmadillo
	./$(MAIN_OUT) 

.PHONY : clean
clean : 
	rm -f *.o
	rm -f *.out

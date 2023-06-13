# Make file
MAIN = Dispersion_social
MAIN_CPP = $(MAIN).cpp
MAIN_OUT=$(MAIN).out

#OpenMP
NUM_THR = 2

.PHONY : windows
# First compile and then run [Nicolas]
windows : windows_compile windows_run

windows_run :
	.\Dispersion_social.exe  

windows_compile :
	g++ -Isrc/include -c .\Dispersion_social.cpp
	g++ .\Dispersion_social.o -o Dispersion_social -Lsrc/lib -lsfml-graphics -lsfml-window -lsfml-system -lblas

.PHONY : windows2
# First compile and then run for extra armadillo [Victor]
windows2 : windows_compile2 windows_run2

windows_run2 :
	.\Dispersion_social.exe  

windows_compile2 :
	g++ -Isrc/include -c .\Dispersion_social.cpp
	g++ .\Dispersion_social.o -o Dispersion_social -Lsrc/lib -lsfml-graphics -lsfml-window -lsfml-system -lblas -larmadillo



.PHONY : ubuntu	

ubuntu:
	g++ -c $(MAIN_CPP) -fopenmp
	g++ $(MAIN).o -o $(MAIN_OUT) -lsfml-graphics -lsfml-window -lsfml-system -larmadillo -fopenmp
	OMP_NUM_THREADS=$(NUM_THR) ./$(MAIN_OUT)

.PHONY : clean
clean : 
	rm -f *.o
	rm -f *.out

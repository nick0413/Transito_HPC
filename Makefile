# Make file
MAIN = Dispersion_social
MAIN_CPP = $(MAIN).cpp
MAIN_OUT=$(MAIN).out

#OpenMP
NUM_THR = 4

MAIN_exe=$(MAIN).exe

.PHONY : windows
# First compile and then run [Nicolas]
windows : windows_compile windows_run

windows_run :
	$(MAIN).exe  

windows_compile :
	g++ -Isrc/include -c .\$(MAIN).cpp -fopenmp -O
	g++ .\$(MAIN).o -o $(MAIN) -Lsrc/lib -lsfml-graphics -lsfml-window -lsfml-system -lblas -fopenmp

.PHONY : windows2
# First compile and then run for extra armadillo [Victor]
windows2 : windows_compile2 windows_run2

windows_run2 :
	set OMP_NUM_THREADS=$(NUM_THR) 
	.\Dispersion_social.exe  

windows_compile2 :
	g++ -Isrc/include -c .\Dispersion_social.cpp -fopenmp
	g++ .\Dispersion_social.o -o Dispersion_social -Lsrc/lib -lsfml-graphics -lsfml-window -lsfml-system -lblas -larmadillo -fopenmp



.PHONY : linux	

linux :
	rm -f ./metrics/*.txt
	g++ -c $(MAIN_CPP) -fopenmp -O3
	g++ $(MAIN).o -o $(MAIN_OUT) -lsfml-graphics -lsfml-window -lsfml-system -larmadillo -fopenmp
	echo "OMP_NUM_THREADS=$(NUM_THR) ./$(MAIN_OUT)"

.PHONY : clean
clean : 
	rm -f *.o
	rm -f *.out

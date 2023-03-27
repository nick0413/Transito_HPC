FILES_CPP=$(wildcard *.cpp)

MAIN=main
MAIN_OUT=$(MAIN).out

$(MAIN_OUT) : $(FILES_CPP)
	g++ -c $^
	g++ $(MAIN).o -o $(MAIN_OUT) -lsfml-graphics -lsfml-window -lsfml-system 

.PHONY : clean
clean : 
	rm -f *.o
	rm -f *.out
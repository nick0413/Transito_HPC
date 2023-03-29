FILES_CPP=$(wildcard *.cpp)

MAIN=main
MAIN_OUT=$(MAIN).out

$(MAIN_OUT) : $(FILES_CPP)
	g++ -c $^
	g++ $(MAIN).o -o $(MAIN_OUT) -lsfml-graphics -lsfml-window -lsfml-system
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
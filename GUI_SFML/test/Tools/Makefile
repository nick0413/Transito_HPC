FILES_CPP=$(wildcard *.cpp)
FLAGS_SANITIZE= -fsanitize=address -fsanitize=leak -fsanitize=pointer-compare -fsanitize=pointer-subtract  -fsanitize=undefined

MAIN=test
MAIN_OUT=$(MAIN).out


.PHONY : compile_run
compile_run :
	g++ -c $(FILES_CPP)
	g++ $(MAIN).o -o $(MAIN_OUT) -lsfml-graphics -lsfml-window -lsfml-system
	./$(MAIN_OUT)

.PHONY : sanitize
sanitize:
	g++ $(FILES_CPP) -o $(MAIN_OUT) $(FLAGS_SANITIZE)
	./$(MAIN_OUT)

.PHONY : debugging
debugging : 
	g++ -g $(FILES_CPP) -o $(MAIN_OUT) 
	gdb $(MAIN_OUT)

.PHONY : clean
clean : 
	rm -f *.out

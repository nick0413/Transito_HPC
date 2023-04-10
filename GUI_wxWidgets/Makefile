FILES_CPP=$(wildcard *.cpp)

MAIN=main
MAIN_OUT=$(MAIN).out

$(MAIN_OUT) : $(FILES_CPP)
	g++ $^ -o $(MAIN_OUT) `wx-config --cppflags --libs`


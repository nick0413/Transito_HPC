all: test

test: dijktra.exe testpy.py
	dijktra.exe 16 12
	py testpy.py 16 12


dijktra.exe : main.cpp
	g++ main.cpp -o dijktra
	
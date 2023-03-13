# Crea una lista de todos los archivos cpp.
ARCHIVOS_CPP = $(wildcard ./*.cpp) 
# Nombre del archivo con el main. (No poner espacios al final)
MAIN = main
MAIN_OUT = $(MAIN).out

# Hace la compilación de todos los archivos cpp del directorio actual y crea el ejecutable MAIN_OUT.
$(MAIN_OUT) : $(ARCHIVOS_CPP) 
	g++ $^ -o $@




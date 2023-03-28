#Crea una lista de todos los archivos cpp.
ARCHIVOS_CPP = $(wildcard ./*.cpp) 
# Nombre del archivo con el main. (No poner espacios al final)
MAIN = main
MAIN_OUT = $(MAIN).out
MAIN_REPORTE = $(MAIN).txt

# Sanitize
FLAGS_SANITIZE= -fsanitize=address -fsanitize=pointer-compare -fsanitize=pointer-subtract -fsanitize=leak -fsanitize=undefined

$(MAIN_REPORTE) : $(MAIN_OUT)
	./$^ > $@ 

# Hace la compilación de todos los archivos cpp del directorio actual y crea el ejecutable MAIN_OUT.
$(MAIN_OUT) : $(ARCHIVOS_CPP) 
	g++ $^ -o $@ $(FLAGS_SANITIZE)





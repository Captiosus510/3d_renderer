all: MS1 MS2 FS

# Executables
MS1: assg.o spheres.o vector.o
	gcc -Wall -Werror -std=c99 -o MS1_assg assg.o spheres.o vector.o -lm
	
MS2: # TODO ...

FS: # TODO ...


# Object files
# TODO: Makefile rules to compile source files into object files
vector.o: src/vector.c src/vector.h
	gcc -Wall -Werror -std=c99 -c src/vector.c

spheres.o: src/spheres.c src/spheres.h src/vector.h
	gcc -Wall -Werror -std=c99 -c src/spheres.c

assg.o: src/vector.h src/spheres.h src/assg.c
	gcc -Wall -Werror -std=c99 -c src/assg.c

clean:
	rm -f *.o
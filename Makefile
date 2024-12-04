all: MS1 MS2 FS

# Executables
MS1: assg_MS1.o spheres.o vector.o color.o
	gcc -Wall -Werror -std=c99 -DMS1 -o MS1_assg assg_MS1.o spheres.o vector.o color.o -lm
	
MS2: vector.o spheres.o assg_MS2.o color.o
	gcc -Wall -Werror -std=c99 -DMS2 -o MS2_assg assg_MS2.o spheres.o vector.o color.o -lm

FS: vector.o spheres.o assg_FS.o color.o
	gcc -Wall -Werror -std=c99 -DFS -o FS_assg assg_FS.o spheres.o vector.o color.o -lm


# Object files
# TODO: Makefile rules to compile source files into object files
vector.o: src/vector.c src/vector.h
	gcc -Wall -Werror -std=c99 -c src/vector.c

spheres.o: src/spheres.c src/spheres.h src/vector.h
	gcc -Wall -Werror -std=c99 -c src/spheres.c

assg_MS1.o: src/vector.h src/spheres.h src/assg.c src/color.h
	gcc -Wall -Werror -std=c99 -DMS1 -c src/assg.c -o assg_MS1.o -lm

assg_MS2.o: src/vector.h src/spheres.h src/assg.c src/color.h
	gcc -Wall -Werror -std=c99 -DMS2 -c src/assg.c -o assg_MS2.o -lm

assg_FS.o:
	gcc -Wall -Werror -std=c99 -DFS -c src/assg.c -o assg_FS.o -lm
color.o: src/color.h src/color.c src/vector.h
	gcc -Wall -Werror -std=c99 -c src/color.c

clean:
	rm -f *.o
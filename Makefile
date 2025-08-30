all: raytracer

# Executables
raytracer: main.o spheres.o vector.o color.o
	gcc -Wall -Werror -std=c99 -o raytracer main.o spheres.o vector.o color.o -Lraylib/src -lraylib -lm

# Object files
vector.o: src/vector.c src/vector.h
	gcc -Wall -Werror -std=c99 -c src/vector.c

spheres.o: src/spheres.c src/spheres.h src/vector.h
	gcc -Wall -Werror -std=c99 -c src/spheres.c

main.o: src/vector.h src/spheres.h src/main.c src/color.h
	gcc -Wall -Werror -std=c99 -c src/main.c -o main.o -Iraylib/src -lm

color.o: src/color.h src/color.c src/vector.h
	gcc -Wall -Werror -std=c99 -c src/color.c

clean:
	rm -f *.o raytracer
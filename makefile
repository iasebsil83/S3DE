#execuutable
run: prog.o S3DE.o
	gcc -Wall -o run prog.o S3DE.o -lm -lglut -lGL -lX11
	rm -f *.o



#program
prog.o: src/prog.c lib/S3DE.h
	gcc -Wall -O2 -c src/prog.c



#S3DE management (use S3DE.o/.h to access to 3D graphic engine)
S3DE.o: lib/S3DE.c lib/S3DE.h
	gcc -Wall -O2 -c lib/S3DE.c -I/usr/include/GL


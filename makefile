run: prog.o S3DE.o
	gcc -Wall -o run prog.o S3DE.o -lm -lglut -lGL -lX11
	rm -f *.o



#program
prog.o: prog.c S3DE.h
	gcc -Wall -O2 -c prog.c



#S3DE management (use S3DE.o/.h to access to 3D graphic engine)
S3DE.o: S3DE.c S3DE.h
	gcc -Wall -O2 -c S3DE.c -I/usr/include/GL


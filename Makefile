GCC=gcc
CFLAGS=-Wall -ansi -O2
OBJ=bin/main.o bin/game.o bin/interface.o bin/misc.o bin/acquisition.o
HEADERS=includes/game.h includes/interface.h includes/misc.h includes/acquisition.h

all: starshipcode plugins/full_dummy.so plugins/rand_all.so plugins/rand_move.so plugins/rand_shot.so plugins/circle_rand_shot.so plugins/rand_move_info.so

starshipcode: $(OBJ)
	$(GCC) -o starshipcode $(OBJ) $(CFLAGS) -lMLV -lm -ldl

bin/main.o: src/main.c $(HEADERS)
	$(GCC) -c src/main.c -o bin/main.o $(CFLAGS) -lMLV -ldl

bin/game.o: src/game.c includes/game.h
	$(GCC) -c src/game.c -o bin/game.o $(CFLAGS) -lm

bin/interface.o: src/interface.c includes/interface.h includes/game.h
	$(GCC) -c src/interface.c -o bin/interface.o $(CFLAGS) -lMLV -lm

bin/acquisition.o: src/acquisition.c includes/acquisition.h
	$(GCC) -fPIC -c src/acquisition.c -o bin/acquisition.o $(CFLAGS)

bin/misc.o: src/misc.c includes/misc.h
	$(GCC) -c src/misc.c -o bin/misc.o $(CFLAGS) -lrt

#############################
#     Plugins examples      #
#############################
plugins/full_dummy.so: plugins/full_dummy.c bin/acquisition.o
	$(GCC) -fPIC -c plugins/full_dummy.c -o plugins/full_dummy.o $(CFLAGS)
	$(GCC) -shared -o plugins/full_dummy.so plugins/full_dummy.o bin/acquisition.o $(CFLAGS)

plugins/rand_all.so: plugins/rand_all.c bin/acquisition.o
	$(GCC) -fPIC -c plugins/rand_all.c -o plugins/rand_all.o $(CFLAGS)
	$(GCC) -shared -o plugins/rand_all.so plugins/rand_all.o bin/acquisition.o $(CFLAGS)

plugins/rand_move.so: plugins/rand_move.c bin/acquisition.o
	$(GCC) -fPIC -c plugins/rand_move.c -o plugins/rand_move.o $(CFLAGS)
	$(GCC) -shared -o plugins/rand_move.so plugins/rand_move.o bin/acquisition.o $(CFLAGS)

plugins/rand_shot.so: plugins/rand_shot.c bin/acquisition.o
	$(GCC) -fPIC -c plugins/rand_shot.c -o plugins/rand_shot.o $(CFLAGS)
	$(GCC) -shared -o plugins/rand_shot.so plugins/rand_shot.o bin/acquisition.o $(CFLAGS)

plugins/circle_rand_shot.so: plugins/circle_rand_shot.c bin/acquisition.o
	$(GCC) -fPIC -c plugins/circle_rand_shot.c -o plugins/circle_rand_shot.o $(CFLAGS)
	$(GCC) -shared -o plugins/circle_rand_shot.so plugins/circle_rand_shot.o bin/acquisition.o $(CFLAGS)

plugins/rand_move_info.so: plugins/rand_move_info.c bin/acquisition.o
	$(GCC) -fPIC -c plugins/rand_move_info.c -o plugins/rand_move_info.o $(CFLAGS)
	$(GCC) -shared -o plugins/rand_move_info.so plugins/rand_move_info.o bin/acquisition.o $(CFLAGS)

clean:
	rm bin/*.o
	rm plugins/*.o
	rm plugins/*.so
	rm starshipcode
	rm -f */*~
	rm -f *~

rebuild:
	make clean
	make

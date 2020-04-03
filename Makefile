GCC=gcc
CFLAGS=-Wall -ansi -O2

OBJ=bin/util.lang.o \
	bin/util.contract.o \
	bin/util.func.o \
	bin/util.list.o \
	bin/util.str.o \
	bin/util.error.o \
	bin/util.misc.o \
	bin/main.o \
	bin/game.o \
	bin/interface.o \
	bin/acquisition.o \
	bin/plugins.o

HEADERS=includes/util/lang.h \
		includes/util/contract.h \
		includes/util/func.h \
		includes/util/list.h \
		includes/util/str.h \
		includes/util/error.h \
		includes/util/misc.h \
		includes/game.h \
		includes/interface.h \
		includes/acquisition.h \
		includes/plugins.h

PLUGINS=plugins/full_dummy.so \
		plugins/rand_all.so \
		plugins/rand_move.so \
		plugins/rand_shot.so \
		plugins/circle_rand_shot.so \
		plugins/rand_move_info.so

EXECUTABLE=starshipcode
all: $(EXECUTABLE) $(PLUGINS)

$(EXECUTABLE): $(OBJ)
	$(GCC) -o $(EXECUTABLE) $(OBJ) $(CFLAGS) -lMLV -lm -ldl

bin/util.lang.o: src/util/lang.c $(HEADERS)
	$(GCC) -c src/util/lang.c -o bin/util.lang.o $(CFLAGS) -lMLV -ldl

bin/util.contract.o: src/util/contract.c $(HEADERS)
	$(GCC) -c src/util/contract.c -o bin/util.contract.o $(CFLAGS) -lMLV -ldl

bin/util.func.o: src/util/func.c $(HEADERS)
	$(GCC) -c src/util/func.c -o bin/util.func.o $(CFLAGS) -lMLV -ldl

bin/util.list.o: src/util/list.c $(HEADERS)
	$(GCC) -c src/util/list.c -o bin/util.list.o $(CFLAGS) -lMLV -ldl

bin/util.str.o: src/util/str.c $(HEADERS)
	$(GCC) -c src/util/str.c -o bin/util.str.o $(CFLAGS) -lMLV -ldl

bin/util.error.o: src/util/error.c $(HEADERS)
	$(GCC) -c src/util/error.c -o bin/util.error.o $(CFLAGS) -lMLV -ldl

bin/util.misc.o: src/util/misc.c $(HEADERS)
	$(GCC) -c src/util/misc.c -o bin/util.misc.o $(CFLAGS) -lMLV -ldl

bin/main.o: src/main.c $(HEADERS)
	$(GCC) -c src/main.c -o bin/main.o $(CFLAGS) -lMLV -ldl

bin/game.o: src/game.c includes/game.h
	$(GCC) -c src/game.c -o bin/game.o $(CFLAGS) -lm

bin/interface.o: src/interface.c includes/interface.h includes/game.h
	$(GCC) -c src/interface.c -o bin/interface.o $(CFLAGS) -lMLV -lm

bin/acquisition.o: src/acquisition.c includes/acquisition.h
	$(GCC) -fPIC -c src/acquisition.c -o bin/acquisition.o $(CFLAGS)

bin/plugins.o: src/plugins.c includes/plugins.h
	$(GCC) -c src/plugins.c -o bin/plugins.o $(CFLAGS) -lrt

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
	rm -f bin/*.o
	rm -f plugins/*.o
	rm -f plugins/*.so
	rm -f starshipcode
	rm -f */*~
	rm -f *~

rebuild:
	make clean
	make

SYSTEM=unix
WINDOW=ncurses

HEADERS=main.h ../vendor/json/jsonxx.h
LIBOBJS=map.o
OBJLIST=main.o curses.o jsonxx.o $(LIBOBJS)
OBJS=$(addprefix obj/, $(OBJLIST))

CC=clang++
CFLAGS=-Wextra --std=c++11 -stdlib=libc++ -c -O3 -g -D$(SYSTEM) -D$(WINDOW) -I../vendor -I../src/ -I. -Wdeprecated-writable-strings
LD=clang++
LDFLAGS=-lncurses -stdlib=libc++

obj/%.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o obj/$*.o

obj/%.o: ../src/%.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c ../src/$< -o obj/$*.o

obj/jsonxx.o: ../vendor/json/jsonxx.cc $(HEADERS)
	$(CC) $(CFLAGS) -c ../vendor/json/jsonxx.cc -o obj/jsonxx.o

interactive: $(OBJS) $(HEADERS)
	$(LD) $(LDFLAGS) $(OBJS) -o itest

all: interactive

clean:
	rm obj/*.o
	rm itest

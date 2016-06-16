SYSTEM=unix
WINDOW=ncurses

HEADERS=main.h ../vendor/json/jsonxx.h ../lib/astrojag.h
OBJLIST=main.o curses.o map.o
LIB=libastrojag_static.a

OBJDIR=obj
LIBPATH=../lib/$(LIB)
OBJS=$(addprefix $(OBJDIR)/, $(OBJLIST))

CC=clang++
CFLAGS=-Wextra --std=c++11 -stdlib=libc++ -c -O3 -g -D$(SYSTEM) -D$(WINDOW) -I../lib -I. -Wdeprecated-writable-strings
LD=clang++
LDFLAGS=-lncurses -stdlib=libc++

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: %.cpp $(HEADERS) | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o obj/$*.o

$(OBJDIR)/jsonxx.o: ../vendor/json/jsonxx.cc $(HEADERS)
	$(CC) $(CFLAGS) -c ../vendor/json/jsonxx.cc -o obj/jsonxx.o
	
$(LIBPATH): .
	@echo Rebuilding library...
	( cd ../src; make all )

interactive: $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) $(LIBPATH) -o itest

all: $(LIBPATH) interactive

clean:
	@rm -f $(OBJDIR)/*.o
	@rm -f itest
	-@rmdir $(OBJDIR) 2> /dev/null || true
#	( cd ../src; make all )

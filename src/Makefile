# system definitions
SYSTEM=unix
WINDOW=ncurses
# inputs
HEADERS=thplib_preamble.h vector.h mapTemplate.h resource.h logging.h
OBJLIST=mapTemplate.o vector.o jsonxx.o resource.o logging.o
# outputs
OUTFILE=libthp_static.a
OUTHEADER=thplib.h

# sausage
OBJDIR=objs
LIBDIR=../lib
LIBHEADER=$(LIBDIR)/$(OUTHEADER)
OBJS=$(addprefix $(OBJDIR)/, $(OBJLIST))
OUTPUT=$(LIBDIR)/$(OUTFILE)

NOEDIT=true
CC=clang++
CFLAGS=-Wextra --std=c++11 -stdlib=libc++ -c -O3 -g -D$(SYSTEM) -D$(WINDOW) -DBUILD_STATIC_LIBRARY -I$(LIBDIR) -I../vendor -I. -Wdeprecated-writable-strings
LD=ar
LDFLAGS=-rcs
#LD=clang++
#LDFLAGS=-lncurses -stdlib=libc++

#==========================================
$(OBJDIR):
	mkdir -p $(OBJDIR)
$(LIBDIR):
	mkdir -p $(LIBDIR)

$(OBJDIR)/%.o: %.cpp $(HEADERS) | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $(OBJDIR)/$*.o

$(OBJDIR)/jsonxx.o: ../vendor/json/jsonxx.cc $(HEADERS) | $(OBJDIR)
	$(CC) $(CFLAGS) -c ../vendor/json/jsonxx.cc -o $(OBJDIR)/jsonxx.o


$(LIBHEADER): $(LIBDIR) $(HEADERS)
	@echo "Building thplib.h..."
ifdef DIST
	rm -f $(LIBHEADER)
	touch $(LIBHEADER)
else
	@echo "/* This file is generated automatically by \"make thplib.h\". Do not edit. */\n\n" > $(LIBHEADER)
endif
	@cat $(HEADERS) >> $(LIBHEADER)

thplib: $(LIBDIR) $(LIBHEADER) $(OBJS)
	@echo "Rebuilding Library...";
	$(LD) $(LDFLAGS) $(OUTPUT) $(OBJS)

all: thplib

dist: thplib DIST=true

clean:
	rm -f $(OBJDIR)/*.o
	rm -f $(OUTPUT)
	rm -f $(LIBHEADER)
	-@rmdir $(LIBDIR) 2> /dev/null || true	
	-@rmdir $(OBJDIR) 2> /dev/null || true

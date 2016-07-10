.FORCE:

lib: .FORCE
	( cd src; make all )

test: .FORCE
	( cd test; make all )

all: lib test

clean:
	( cd src; make clean )
	( cd test; make clean )

# set the C compiler, for C++ use g++
# use this setting on most sane machines
CC = gcc

# use this line on the engin linux machines
# CC = gcc296

# set the path to the include directory
INCDIR =../include

# set the flags for the C and C++ compiler to give lots of warnings
CFLAGS = -I$(INCDIR) -I/opt/local/include -O2 -Wall -Wstrict-prototypes -Wnested-externs -Wmissing-prototypes -Wmissing-declarations
CPPFLAGS = $(CFLAGS)

# path to the object file directory
ODIR = obj

# path to the lib directory
LIBDIR =../lib

# path to the bin directory
BINDIR =../bin

# libraries to include
LIBS = -limageIO -pthread -lm
LFLAGS = -L$(LIBDIR) -L/opt/local/lib

# put all of the relevant include files here
_DEPS = ppmIO.h

# convert them to point to the right place
DEPS = $(patsubst %,$(INCDIR)/%,$(_DEPS))

# put a list of the executables here
EXECUTABLES = ppmtest maskGreenBlue lab1 compositeGreenBlue test5b

# put a list of all the object files here for all executables (with .o endings)
_OBJ = tiffmain.o ppmmain.o maskGreenBlue.o lab1.o compositeGreenBlue.o test5b.o

# convert them to point to the right place
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

# patterns for compiling source code
# $< is the file that caused the action to occur
$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: %.C $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)


all:
	for i in $(EXECUTABLES); do (make $$i) ; done


# put the specific executable rules here
ppmtest: $(ODIR)/ppmmain.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)


lab1: $(ODIR)/lab1.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

maskGreenBlue: $(ODIR)/maskGreenBlue.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)
	
compositeGreenBlue: $(ODIR)/compositeGreenBlue.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

imagetest: $(ODIR)/imagetest.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

henon: $(ODIR)/henon.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

mandelbrot_interactive: $(ODIR)/mandelbrot_interactive.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

julia_interactive: $(ODIR)/julia_interactive.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

mandeltest: $(ODIR)/mandeltest.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

horizontalSinTest: $(ODIR)/horizontalSinTest.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test3d: $(ODIR)/test3d.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

testbench: $(ODIR)/testbench.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test3a: $(ODIR)/test3a.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test3b: $(ODIR)/test3b.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test3c: $(ODIR)/test3c.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

testPrims: $(ODIR)/testPrims.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)
.PHONY: clean

test4a: $(ODIR)/test4a.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test4b: $(ODIR)/test4b.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test_polygon_supersampler: $(ODIR)/test_polygon_supersampler.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test4c: $(ODIR)/test4c.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test4c-loop: $(ODIR)/test4c-loop.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

polyspeed: $(ODIR)/polyspeed.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

testPols: $(ODIR)/testPols.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test5a: $(ODIR)/test5a.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test5b: $(ODIR)/test5b.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test5c: $(ODIR)/test5c.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test5d: $(ODIR)/test5d.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test5e: $(ODIR)/test5e.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test5f: $(ODIR)/test5f.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test5g: $(ODIR)/test5g.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

tiefighter: $(ODIR)/tiefighter.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test6a: $(ODIR)/test6a.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test6b: $(ODIR)/test6b.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test6c: $(ODIR)/test6c.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test6d: $(ODIR)/test6d.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test7a: $(ODIR)/test7a.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test7b: $(ODIR)/test7b.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test7c: $(ODIR)/test7c.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test7_prims: $(ODIR)/test7_prims.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test7_tetrahedron: $(ODIR)/test7_tetrahedron.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test7_octahedron: $(ODIR)/test7_octahedron.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test7_curves: $(ODIR)/test7_curves.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test7_sail: $(ODIR)/test7_sail.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test8a: $(ODIR)/test8a.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

animated_bezier: $(ODIR)/animated_bezier.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

pbil: $(ODIR)/pbil.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 


CXX=icpc
CXXFLAGS=-c -qopenmp
LDFLAGS=-qopenmp

OBJECTS=main.o

app: $(OBJECTS)
	$(CXX) $(LDFLAGS) -o app $(OBJECTS)

all:	app

run:	all
	./app ../data/in01 ../data/in02

clean:
	rm -f *.o app *~

CXX=icpc
CXXFLAGS=-c -qopenmp -qopt-report=5 -xMIC-AVX512
LDFLAGS=-qopenmp

OBJECTS=main.o

app: $(OBJECTS)
	$(CXX) $(LDFLAGS) -o app $(OBJECTS)

all:	app

run:	all
	./app ../data/in01 ../data/in02

queue:	all
	echo 'cd $$PBS_O_WORKDIR ; ./app ../data/in01 ../data/in02' | qsub -l nodes=1:flat -N app

clean:
	rm -f *.optrpt *.o  *~ app*

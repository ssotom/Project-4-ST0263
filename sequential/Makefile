CXX=icpc
CXXFLAGS=-c -qopenmp

OBJECTS=main.o

stencil: $(OBJECTS)
	$(CXX) -o main $(OBJECTS)

all:	main

run:	all
	./main

queue:	all
	echo 'cd $$PBS_O_WORKDIR ; ./main' | qsub -l nodes=1:flat -N main

clean:
	rm -f *.o main *~

CFLAGS=-O3 -std=c11 -fPIC -g 
CXXFLAGS=-O3 -std=c++17 -fPIC -g 
ARCHIVES=../libgen.a 
LD=g++


all: bubblesort 


bubblesort: bubblesort.o
	$(LD) $(LDFLAGS) bubblesort.o $(ARCHIVES) -o bubblesort

test: bubblesort
	./test.sh

bench: bubblesort
	./queue.sh

plot:  
	./plot.sh

clean:
	-rm *.o
	-rm bubblesort

distclean:
	-rm *.sh.*


.PHONY:  bubblesort

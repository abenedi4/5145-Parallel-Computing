CFLAGS=-O3 -std=c11 -fPIC -g -Xassembler -mrelax-relocations=no
CXXFLAGS=-O3 -std=c++17 -fPIC -g -Xassembler -mrelax-relocations=no
ARCHIVES=libgen.a 
LD=g++


all: activity-bubblesort.tgz

# archives
libgen.a: gen_lib.o
	ar rcs libgen.a gen_lib.o


%.pdf: %.tex
	pdflatex $<

ARXIV=Makefile approx.cpp \
      libgen.a \
      sequential bubblesort  \
      .gitignore params.sh \
      activity_bubblesort_loops.pdf

activity-bubblesort.tgz: $(ARXIV)
	tar zcvf $@ \
                 $(ARXIV)

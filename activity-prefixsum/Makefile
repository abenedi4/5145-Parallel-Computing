CFLAGS=-O3 -std=c11 -fPIC -g  -Xassembler -mrelax-relocations=no
CXXFLAGS=-O3 -std=c++17 -fPIC -g -Xassembler -mrelax-relocations=no
LDFLAGS=-fPIE
LD=g++

all: activity-prefixsum.tgz

libgen.a: gen_lib.o
	ar rcs libgen.a gen_lib.o

%.pdf: %.tex
	pdflatex $<

ARXIV=Makefile approx.cpp \
      libgen.a \
      sequential prefixsum  \
      .gitignore params.sh \
      activity_prefixsum_loops.pdf

activity-prefixsum.tgz: $(ARXIV)
	tar zcvf $@ \
                 $(ARXIV)


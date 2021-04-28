CFLAGS=-O3 -std=c11 -fPIC -g -Xassembler -mrelax-relocations=no
CXXFLAGS=-O3 -std=c++17 -fPIC -g -Xassembler -mrelax-relocations=no
ARCHIVES=libgen.a 
LD=g++


all: activity-lcs.tgz


# archives
libgen.a: gen_lib.o
	ar rcs libgen.a gen_lib.o

%.pdf: %.tex
	pdflatex $<

ARXIV=Makefile \
      libgen.a \
      sequential lcs  \
      .gitignore params.sh \
      activity_lcs_loops.pdf

activity-lcs.tgz: $(ARXIV)
	tar zcvf $@ \
                 $(ARXIV)

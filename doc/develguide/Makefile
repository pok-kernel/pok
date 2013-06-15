EPSFILES=

LISTINGS=

BIBLIO=

PDFFILE=pok-devel.pdf

TEXFILES=pok-devel.tex 

TEXMAIN=pok-devel.tex

LIBPOK_HEADERS=
all: $(PDFFILE)

%.pdf: %.svg
	inkscape --export-pdf=$@ $<

$(PDFFILE): $(TEXFILES) $(EPSFILES) $(BIBLIO) $(LISTINGS)
	rm -rf include-libpok
	mkdir -p include-libpok include-libpok/core include-libpok/arinc653 include-libpok/middleware include-libpok/libc
	for v in $(LIBPOK_HEADERS); do \
		NLINES=`wc -l ../../libpok/include/$$v|awk '{print $$1}'` ; NLINES=`expr $$NLINES - 15` ; \
		tail -n $$NLINES ../../libpok/include/$$v > include-libpok/$$v ; \
	done
	pdflatex $(TEXMAIN)
	pdflatex $(TEXMAIN)

generate-html: $(PFDFILE)
	rm -rf html
	mkdir html
	latex2html -address 'Copyright 2009 POK Team' -local_icons -dir html $(TEXMAIN)

clean:
	rm -f *.aux *.dvi *.log *~ *.bbl *.blg *.toc
	rm -rf include-libpok

distclean: clean
	rm -rf $(PDFFILE) html pok-devel.lof $(EPSFILES)

.PHONY: all generate-html

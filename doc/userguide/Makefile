EPSFILES=imgs/pok-layers.pdf \
			imgs/partitions-architecture.pdf \
			imgs/partitions-build-steps.pdf \
			imgs/mde-process.pdf \
			imgs/pok-elf-file.pdf

LISTINGS=

BIBLIO=

PDFFILE=pok-user.pdf

TEXFILES=pok-user.tex introduction.tex installation.tex annexes.tex model-based.tex api.tex configuration.tex examples.tex gettingstarted.tex instrumentation.tex

TEXMAIN=pok-user.tex

LIBPOK_HEADERS=arch.h errno.h libm.h\
					core/allocator.h core/thread.h core/error.h core/event.h core/semaphore.h core/syscall.h core/partition.h core/time.h\
					middleware/blackboard.h middleware/buffer.h middleware/port.h middleware/queue.h \
					libc/stdio.h libc/stdlib.h libc/string.h \
					arinc653/blackboard.h arinc653/buffer.h arinc653/error.h arinc653/event.h arinc653/partition.h arinc653/process.h arinc653/queuing.h arinc653/sampling.h arinc653/semaphore.h arinc653/time.h arinc653/types.h \
					protocols/protocols.h protocols/ceasar.h protocols/des.h protocols/ssl.h

all: $(PDFFILE)

%.pdf: %.svg
	inkscape --export-pdf=$@ $<

$(PDFFILE): $(TEXFILES) $(EPSFILES) $(BIBLIO) $(LISTINGS)
	rm -rf include-libpok
	mkdir -p include-libpok include-libpok/core include-libpok/arinc653 include-libpok/middleware include-libpok/libc include-libpok/protocols
	for v in $(LIBPOK_HEADERS); do \
		NLINES=`wc -l ../../libpok/include/$$v|awk '{print $$1}'` ; NLINES=`expr $$NLINES - 15` ; \
		tail -n $$NLINES ../../libpok/include/$$v > include-libpok/$$v ; \
	done
	cp `ocarina-config --prefix`/share/ocarina/AADLv2/pok_properties.aadl .
	cp `ocarina-config --prefix`/share/ocarina/AADLv2/arinc653_properties.aadl .
	cp ../../examples/network/rtl8029.aadl sample-network-rtl8029.aadl
	cp ../../examples/network/main.aadl sample-network-main.aadl
	cp ../../examples/network/runtime.aadl sample-network-runtime.aadl
	cp ../../misc/aadl-library.aadl .
	pdflatex $(TEXMAIN)
	pdflatex $(TEXMAIN)

generate-html: $(PFDFILE)
	rm -rf html
	mkdir html
	latex2html -address 'Copyright 2009 POK Team' -local_icons -dir html $(TEXMAIN)

clean:
	rm -f *.aux *.dvi *.log *~ *.bbl *.blg *.toc
	rm -rf include-libpok
	rm -f pok_properties.aadl arinc653_properties.aadl sample-network-*.aadl aadl-library.aadl

distclean: clean
	rm -rf $(PDFFILE) html pok-user.lof $(EPSFILES)

.PHONY: all generate-html

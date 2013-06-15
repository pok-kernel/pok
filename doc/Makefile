-include $(POK_PATH)/misc/mk/config.mk

all:
	doxygen ../misc/doxygen-kernel.conf
	doxygen ../misc/doxygen-libpok.conf
	$(MAKE) -C userguide distclean all generate-html
	$(MAKE) -C develguide distclean all generate-html
	$(MAKE) -C doxygen-kernel/latex
	$(MAKE) -C doxygen-libpok/latex
	$(MV) doxygen-libpok/latex/refman.pdf libpok-refman.pdf
	$(MV) doxygen-kernel/latex/refman.pdf kernel-refman.pdf

clean:
	$(RM) doxygen-kernel doxygen-libpok
	$(MAKE) -C userguide clean
	$(MAKE) -C develguide clean

distclean: clean
	$(RM) libpok-refman.pdf kernel-refman.pdf
	$(MAKE) -C userguide distclean
	$(MAKE) -C develguide distclean

-include $(POK_PATH)/misc/mk/rules-common.mk

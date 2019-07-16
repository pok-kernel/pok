-include misc/mk/config.mk

.SILENT:
.PHONY: doc

SUBDIRS = examples kernel libpok doc

all:
	@echo "Nothing to do here; browse through examples.";

configure:
	./misc/conf-env.pl $(ARCH)

configure-xcov:
	./misc/conf-env.pl --with-xcov $(ARCH)

configure-instrumentation:
	./misc/conf-env.pl --with-instrumentation $(ARCH)

clean:
	for dir	in $(SUBDIRS);			\
	do					\
		$(MAKE) -C $$dir $@ || exit 1;	\
        done

distclean:
	for dir	in $(SUBDIRS);			\
	do					\
		$(MAKE) -C $$dir $@ || exit 1;	\
        done
	$(RM) misc/mk/config.mk pok-[0-9]*.tgz

test:
	$(MAKE) clean
	$(MAKE) -C examples test

#8-> CUT HERE
commit: testrelease
	echo "Please make sure you have commited on your changes on your current GIT repository"
	sleep 10
	$(SH) ./misc/send-release.sh
	git push
	$(MAKE) distclean

headers:
	$(PERL) ./misc/update-headers.pl

doc:
	$(MAKE) -C doc

release: headers doc
	svn up
	$(PERL) ./misc/make-release.pl

testrelease: release
	$(eval VERSION=`date '+%Y%m%d'`)
	$(eval TMPDIR="/tmp/release-pok")
	$(RM) -r $(TMPDIR) && mkdir -p $(TMPDIR)
	tar xzf pok-$(VERSION).tgz -C $(TMPDIR)
	export POK_PATH=$(TMPDIR)/pok-$(VERSION) && $(MAKE) -C $(TMPDIR)/pok-$(VERSION) configure
	export POK_PATH=$(TMPDIR)/pok-$(VERSION) && $(MAKE) -C $(TMPDIR)/pok-$(VERSION) test
	$(RM) -r $(TMPDIR)
#8-> END CUT

-include $(POK_PATH)/misc/mk/rules-common.mk

ifndef REDIRECT_OUTPUT
REDIRECT_OUTPUT=/dev/null
endif

all:
	for v in `cat COMPLETE`; do \
		echo "Building $$v" ; \
		./test.sh $$v >>$(REDIRECT_OUTPUT) 2>&1; \
	done

clean:
	find . -type f -name '*~' |xargs rm -f

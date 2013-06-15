libpart: $(OBJS)
	$(ECHO) $(ECHO_FLAGS) $(ECHO_FLAGS_ONELINE) "[AR] $@.a "
	/bin/ar -x libpok.a
	/bin/ar -csr $@.a $(OBJS) *.lo
	if test $$? -eq 0; then $(ECHO) $(ECHO_FLAGS) $(ECHO_GREEN) " OK "; else $(ECHO) $(ECHO_FLAGS) $(ECHO_RED) " KO"; fi
	rm *.lo

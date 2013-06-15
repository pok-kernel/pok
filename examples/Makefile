-include $(POK_PATH)/misc/mk/config.mk

SUBDIRS= \
	ada-runtime				\
	arinc653-blackboard		      	\
	arinc653-buffer				\
	arinc653-errors				\
	arinc653-events				\
	arinc653-queueing			\
	arinc653-sampling			\
	arinc653-threads			\
	case-study-aerotech09			\
	case-study-ardupilot			\
	case-study-integrated			\
	case-study-mils				\
	case-study-sigada09			\
	case-study-osadl11			\
	data-arrays				\
	data-arrays2				\
	esterel					\
	events					\
	exceptions-handled			\
	heterogeneous-partitions		\
	libmath					\
	lustre-academic				\
	middleware-blackboard			\
	middleware-buffer			\
	middleware-queueing			\
	middleware-queueing-timed		\
	middleware-sampling			\
	network					\
	partitions-scheduling			\
	partitions-threads			\

#	mutexes				\ ### deprecated Makefile
#	mutexes-timed			\ ### deprecated Makefile
#	semaphores			\ ### deprecated Makefile
#	case-study-fcc      \ ### deprecated Makefile
#	debugging			\ ### was not used
#	integrated			\ ### was not used
#	middleware-buffer-timed		\ ### was not used
#	simulink			  ### was not used

all:
	@$(ECHO) "Nothing to do here; browse through examples."

clean distclean test:
	for dir in $(SUBDIRS);				\
	do						\
		$(MAKE) -C $$dir $@ || exit 1;		\
	done

coverage-all:
	(cd ../ && make configure-xcov)
	for dir in $(SUBDIRS);				\
	do						\
		$(MAKE) -C $$dir all run || exit 1;		\
	done
	

-include misc/mk/rules-common.mk

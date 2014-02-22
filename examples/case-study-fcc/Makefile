AADL_FILES=main.aadl types.aadl runtime_position.aadl partitions_position.aadl runtime.aadl threads_position.aadl threads.aadl subprograms_position.aadl runtime_pilot.aadl rtl8029.aadl pilot_partitions.aadl subprograms_pilot.aadl pilot_threads.aadl memories.aadl

parse:
	ocarina -aadlv2 -f $(AADL_FILES)

instance:
	ocarina -aadlv2 -f -i $(AADL_FILES)

generate:
	ocarina -aadlv2 -f -g pok_c $(AADL_FILES)

compile: generate
	( cd generated-code && make)

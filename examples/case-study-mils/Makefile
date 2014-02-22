AADL_FILES=main.aadl runtime_sender.aadl runtime_receiver.aadl rtl8029.aadl types.aadl  runtime.aadl partitions.aadl subprograms.aadl threads.aadl memories.aadl layers.aadl $(POK_PATH)/misc/aadl-library.aadl
BUILD = --models=main.aadl,runtime_sender.aadl,runtime_receiver.aadl,rtl8029.aadl,types.aadl,runtime.aadl,partitions.aadl,subprograms.aadl,threads.aadl,memories.aadl,layers.aadl --no-run

TESTS = compile-x86
include $(POK_PATH)/misc/mk/examples.mk

parse:
	ocarina -aadlv2 -f $(AADL_FILES)

instance:
	ocarina  -i -aadlv2 -f $(AADL_FILES)

generate:
	ocarina -aadlv2 -g pok_c -f $(AADL_FILES)

compile: generate
	(cd generated-code && make )



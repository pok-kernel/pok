CFLAGS	=	$(CONFIG_CFLAGS) -nostdinc -iwithprefix include -fno-builtin -DPOK_ARCH_X86 $(KIND_CFLAGS) $(GENERIC_FLAGS) -Wall -g -O -Wuninitialized -fno-stack-protector -ffreestanding -nostdlib -fstack-check

# FIXME: architecture should not be hardcoded...
ADAFLAGS = -gnaty -gnata -m32 -I $(POK_PATH)/libpok/ada/arinc653

#REMAINDER
#-fno-stack-protector and -nostdlib is added for Ubuntu

LDFLAGS	=	-m $(ELF_MODE)


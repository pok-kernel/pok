export DEPLOYMENT_HEADER=$(shell pwd)/deployment.h
include $(POK_PATH)/misc/mk/config.mk
TARGET = part1.elf
OBJS = main.o activity.o ../../../threadstart.o ../../../hello1.o
all: libpok $(TARGET)
clean: common-clean
	$(RM) *.ali
include $(POK_PATH)/misc/mk/common-$(ARCH).mk
include $(POK_PATH)/misc/mk/rules-partition.mk
include $(POK_PATH)/misc/mk/rules-common.mk

export DEPLOYMENT_HEADER=$(shell pwd)/deployment.h
include $(POK_PATH)/misc/mk/config.mk
LO_TARGET = kernel.lo
LO_OBJS =
LO_DEPS = pok.lo
all: kernel copy-kernel $(LO_TARGET)
clean: common-clean
include $(POK_PATH)/misc/mk/common-$(ARCH).mk
include $(POK_PATH)/misc/mk/rules-common.mk
include $(POK_PATH)/misc/mk/rules-kernel.mk

ifeq ($(OBJ_DIR),)
OBJ_DIR=.
else
OBJ_DIR:=$(OBJ_DIR)/$(notdir $(CURDIR))
endif

LO_OBJS := $(patsubst %.o,$(OBJ_DIR)/%.o,$(LO_OBJS))
OBJS := $(patsubst %.o,$(OBJ_DIR)/%.o,$(OBJS))
LO_TARGET := $(patsubst %.lo,$(OBJ_DIR)/%.lo,$(LO_TARGET))
LO_DEPS := $(patsubst %.lo,$(OBJ_DIR)/%.lo,$(LO_DEPS))

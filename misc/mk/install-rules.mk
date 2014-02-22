#rules to install and run program
ifeq ($(ARCH), x86)
  include $(POK_PATH)/misc/mk/grub-floppy.mk
endif
ifeq ($(ARCH), ppc)
  include $(POK_PATH)/misc/mk/ppc-mkrom.mk
endif
ifeq ($(ARCH), sparc)
  include $(POK_PATH)/misc/mk/sparc-install.mk
endif

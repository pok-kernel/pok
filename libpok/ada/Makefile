-include $(POK_PATH)/misc/mk/config.mk

#8-> CUT HERE

NEEDS_FLAGS=-DPOK_NEEDS_PARTITIONS=1 -DPOK_NEEDS_THREADS=1

generate:
	for v in `find $(POK_PATH)/libpok/include/ -type f -name '*.h'`; do \
		gcc $(CFLAGS) $(NEEDS_FLAGS) -fdump-ada-spec -I$(POK_PATH)/libpok/include/ $$v ;  echo $$v ; \
	done
# Change types
	for v in *.ads; do sed -e 's/size_t/Size_T/g' $$v > /tmp/tmp.ads ; mv /tmp/tmp.ads $$v  ; done
	for v in *.ads; do sed -e 's/function pok_/function /g' $$v > /tmp/tmp.ads ; mv /tmp/tmp.ads $$v  ; done
	for v in *.ads; do sed -e 's/procedure pok_/procedure /g' $$v > /tmp/tmp.ads ; mv /tmp/tmp.ads $$v  ; done
	for v in *.ads; do sed -e 's/subtype pok_/subtype /g' $$v > /tmp/tmp.ads ; mv /tmp/tmp.ads $$v  ; done
	for v in *.ads; do sed -e 's/POK_PORT/PORT/g' $$v > /tmp/tmp.ads ; mv /tmp/tmp.ads $$v  ; done
	for v in *.ads; do sed -e 's/subtype int/subtype Int/g' $$v > /tmp/tmp.ads ; mv /tmp/tmp.ads $$v  ; done
	for v in *.ads; do sed -e 's/subtype uint/subtype Uint/g' $$v > /tmp/tmp.ads ; mv /tmp/tmp.ads $$v  ; done
	for v in *.ads; do sed -e 's/\.int/\.Int/g' $$v > /tmp/tmp.ads ; mv /tmp/tmp.ads $$v  ; done
	for v in *.ads; do sed -e 's/\.uint/\.Uint/g' $$v > /tmp/tmp.ads ; mv /tmp/tmp.ads $$v  ; done
	for v in *.ads; do sed -e 's/is\s\(.*\)pok_/is \1/g' $$v > /tmp/tmp.ads ; mv /tmp/tmp.ads $$v  ; done
	for v in *.ads; do sed -e 's/return\s\(.*\)pok_/return \1/g' $$v > /tmp/tmp.ads ; mv /tmp/tmp.ads $$v  ; done
	for v in *.ads; do sed -e 's/:\s\(.*\)pok_/: \1/g' $$v > /tmp/tmp.ads ; mv /tmp/tmp.ads $$v  ; done
	for v in *.ads; do sed -e 's/pragma Import (C, pok_/pragma Import (C, /g' $$v > /tmp/tmp.ads ; mv /tmp/tmp.ads $$v  ; done
# Package Arch.X86.Types
	for v in *.ads; do sed -e 's/package arch_x86_types_h/package Arch.X86.Types/g' $$v > /tmp/tmp.ads ; mv /tmp/tmp.ads $$v  ; done
	for v in *.ads; do sed -e 's/end arch_x86_types_h/end Arch.X86.Types/g' $$v > /tmp/tmp.ads ; mv /tmp/tmp.ads $$v  ; done
	for v in *.ads; do sed -e 's/with arch_x86_types_h/with Arch.X86.Types/g' $$v > /tmp/tmp.ads ; mv /tmp/tmp.ads $$v  ; done
	for v in *.ads; do sed -e 's/is arch_x86_types_h/is Arch.X86.Types/g' $$v > /tmp/tmp.ads ; mv /tmp/tmp.ads $$v  ; done
	for v in *.ads; do sed -e 's/aliased arch_x86_types_h/aliased Arch.X86.Types/g' $$v > /tmp/tmp.ads ; mv /tmp/tmp.ads $$v  ; done
# Package Arch
	for v in *.ads; do sed -e 's/package arch_h/package Arch/g' $$v > /tmp/tmp.ads ; mv /tmp/tmp.ads $$v  ; done
	for v in *.ads; do sed -e 's/end arch_h/end Arch/g' $$v > /tmp/tmp.ads ; mv /tmp/tmp.ads $$v  ; done
# Package Errno
	for v in *.ads; do sed -e 's/package errno_h/package Errno/g' $$v > /tmp/tmp.ads ; mv /tmp/tmp.ads $$v  ; done
	for v in *.ads; do sed -e 's/end errno_h/end Errno/g' $$v > /tmp/tmp.ads ; mv /tmp/tmp.ads $$v  ; done
# Package Types
	for v in *.ads; do sed -e 's/package types_h/package Types/g' $$v > /tmp/tmp.ads ; mv /tmp/tmp.ads $$v  ; done
	for v in *.ads; do sed -e 's/end types_h/end Types/g' $$v > /tmp/tmp.ads ; mv /tmp/tmp.ads $$v  ; done
	for v in *.ads; do sed -e 's/with types_h/with Types/g' $$v > /tmp/tmp.ads ; mv /tmp/tmp.ads $$v  ; done
	for v in *.ads; do sed -e 's/is types_h/is Types/g' $$v > /tmp/tmp.ads ; mv /tmp/tmp.ads $$v  ; done
	for v in *.ads; do sed -e 's/return types_h/return Types/g' $$v > /tmp/tmp.ads ; mv /tmp/tmp.ads $$v  ; done
# Package Core.Event
	for v in *.ads; do sed -e 's/package core_event_h/package Core.Event/g' $$v > /tmp/tmp.ads ; mv /tmp/tmp.ads $$v  ; done
	for v in *.ads; do sed -e 's/end core_event_h/end Core.Event/g' $$v > /tmp/tmp.ads ; mv /tmp/tmp.ads $$v  ; done
# Package Core.Thread
	for v in *.ads; do sed -e 's/package core_thread_h/package Core.Thread/g' $$v > /tmp/tmp.ads ; mv /tmp/tmp.ads $$v  ; done
	for v in *.ads; do sed -e 's/end core_thread_h/end Core.Thread/g' $$v > /tmp/tmp.ads ; mv /tmp/tmp.ads $$v  ; done
	for v in *.ads; do sed -e 's/with core_thread_h/with Core.Thread/g' $$v > /tmp/tmp.ads ; mv /tmp/tmp.ads $$v  ; done
# Package Core.Partition
	for v in *.ads; do sed -e 's/package core_partition_h/package Core.Partition/g' $$v > /tmp/tmp.ads ; mv /tmp/tmp.ads $$v  ; done
	for v in *.ads; do sed -e 's/end core_partition_h/end Core.Partition/g' $$v > /tmp/tmp.ads ; mv /tmp/tmp.ads $$v  ; done
	for v in *.ads; do sed -e 's/with core_partition_h/with Core.Partition/g' $$v > /tmp/tmp.ads ; mv /tmp/tmp.ads $$v  ; done
# Package Core.Lockobj
	for v in *.ads; do sed -e 's/package core_lockobj_h/package Core.Lockobj/g' $$v > /tmp/tmp.ads ; mv /tmp/tmp.ads $$v  ; done
	for v in *.ads; do sed -e 's/end core_lockobj_h/end Core.Lockobj/g' $$v > /tmp/tmp.ads ; mv /tmp/tmp.ads $$v  ; done
# Package Core.Mutex
	for v in *.ads; do sed -e 's/package core_mutex_h/package Core.Mutex/g' $$v > /tmp/tmp.ads ; mv /tmp/tmp.ads $$v  ; done
	for v in *.ads; do sed -e 's/end core_mutex_h/end Core.Mutex/g' $$v > /tmp/tmp.ads ; mv /tmp/tmp.ads $$v  ; done
# Package Arinc653.Types
	for v in *.ads; do sed -e 's/package arinc653_types_h/package Arinc653.Types/g' $$v > /tmp/tmp.ads ; mv /tmp/tmp.ads $$v  ; done
	for v in *.ads; do sed -e 's/end arinc653_types_h/end Arinc653.Types/g' $$v > /tmp/tmp.ads ; mv /tmp/tmp.ads $$v  ; done
#Misc : arch, types and so on
	mv arch_x86_types_h.ads arch/x86/types.ads
	mv arch_h.ads arch.ads
	mv errno_h.ads errno.ads
	mv types_h.ads types.ads
# Arinc
	mv arinc653_types_h.ads arinc653/types.ads
# Core
	mv core_event_h.ads core/event.ads
	mv core_lockobj_h.ads core/lockobj.ads
	mv core_mutex_h.ads core/mutex.ads
	mv core_thread_h.ads core/thread.ads
	mv core_partition_h.ads core/partition.ads
# Middleware part
	mv middleware_port_h.ads middleware/port.ads
# Remove useless files
	rm -f libc_stdio_h.ads libc_va_list_h.ads libc_stdlib_h.ads  libc_string_h.ads core_syscall_h.ads

clean:
	find . -name '*.ads' |xargs rm -f
	rm -f *~
#8-> CUT HERE

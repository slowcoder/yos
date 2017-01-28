
include Makefile.setup

include arch/$(ARCH)/Makefile
include libc/Makefile

LINKAGE += pmm.o tasks.o kernel.o console.o phyheap.o spinlocks.o interrupts.o

CFLAGS += -DDEBUG

kernel.$(ARCH): $(LINKAGE)
	@echo "Linking"
	@$(LD) $(LDFLAGS) $(LINKAGE) -o kernel.$(ARCH)

%.o:%.c
	@echo "Compiling $<"
	@$(CC) $(CFLAGS) -Iinclude/ -c $< -o $@

%.o:%.S
	@echo "Compiling $<"
	@$(CC) $(CFLAGS) -Iinclude/ -c $< -o $@

%.o:%.s
	@echo "Compiling $<"
	@$(AS) $(ASFLAGS) $< -o $@

clean:
	@find . -name "*.o" -exec rm {} \;
	@find . -name "*~" -exec rm {} \;
	@rm -f kernel.$(ARCH) bochsout.txt nohup.out

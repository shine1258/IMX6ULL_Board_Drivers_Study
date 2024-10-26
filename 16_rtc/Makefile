CROSS_COMPILE ?= arm-linux-gnueabihf-
TARGET ?= rtc

CC := $(CROSS_COMPILE)gcc-4.9
LD := $(CROSS_COMPILE)ld
OBJCOPY := $(CROSS_COMPILE)objcopy
OBJDUMP := $(CROSS_COMPILE)objdump

LIBPATH := -lgcc -L /usr/lib/gcc-cross/arm-linux-gnueabihf/4.9.3

INCUDIRS := imx6u \
			stdio/include \
			bsp/clk \
			bsp/delay \
			bsp/led \
			bsp/beep \
			bsp/key \
			bsp/gpio \
			bsp/int \
			bsp/exti \
			bsp/epit \
			bsp/key_filter \
			bsp/uart \
			bsp/lcd \
			bsp/rtc \

SRCDIRS := 	project	\
			stdio/lib \
			bsp/clk \
			bsp/delay \
			bsp/led \
			bsp/beep \
			bsp/key \
			bsp/gpio \
			bsp/int \
			bsp/exti \
			bsp/epit \
			bsp/key_filter \
			bsp/uart \
			bsp/lcd \
			bsp/rtc \

INCLUDE := $(patsubst %, -I %, $(INCUDIRS))

SFILES := $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.S))
CFILES := $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.c))

SFILENDIR := $(notdir $(SFILES))
CFILENDIR := $(notdir $(CFILES))

SOBJS := $(patsubst %, obj/%, $(SFILENDIR:.S=.o))
COBJS := $(patsubst %, obj/%, $(CFILENDIR:.c=.o))
OBJS := $(SOBJS) $(COBJS)

VPATH := $(SRCDIRS)

.PHONY : clean

$(shell mkdir -p obj)

$(TARGET).bin : $(OBJS)
	$(LD) -Timx6u.lds -o $(TARGET).elf $^ $(LIBPATH)
	$(OBJCOPY) -O binary -S $(TARGET).elf $@
	$(OBJDUMP) -D $(TARGET).elf > $(TARGET).dis

$(SOBJS) : obj/%.o : %.S
	$(CC) -g -Wall -nostdlib -c -O2 $(INCLUDE) -o $@ $<

$(COBJS) : obj/%.o : %.c
	$(CC) -g -Wall -nostdlib -std=gnu99 -fno-builtin -Wa,-mimplicit-it=thumb -c -O2 $(INCLUDE) -o $@ $<

clean:
	rm -rf $(TARGET).bin $(TARGET).elf $(TARGET).dis $(OBJS)
	rmdir obj

print:
	@echo INCLUDE = $(INCLUDE)
	@echo SFILES = $(SFILES)
	@echo CFILES = $(CFILES)
	@echo SFILENDIR = $(SFILENDIR)
	@echo CFILENDIR = $(CFILENDIR)
	@echo SOBJS = $(SOBJS)
	@echo COBJS = $(COBJS)
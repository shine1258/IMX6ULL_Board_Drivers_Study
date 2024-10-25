CROSS_COMPILE ?= arm-linux-gnueabihf-
TARGET ?= key_filter

CC := $(CROSS_COMPILE)gcc-4.9
LD := $(CROSS_COMPILE)ld
OBJCOPY := $(CROSS_COMPILE)objcopy
OBJDUMP := $(CROSS_COMPILE)objdump

INCUDIRS := imx6u \
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

SRCDIRS := 	project	\
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

$(TARGET).bin : $(OBJS)
	$(LD) -Timx6u.lds -o $(TARGET).elf $^
	$(OBJCOPY) -O binary -S $(TARGET).elf $@
	$(OBJDUMP) -D $(TARGET).elf > $(TARGET).dis

$(SOBJS) : obj/%.o : %.S
	$(CC) -g -Wall -nostdlib -c -O2 $(INCLUDE) -o $@ $<

$(COBJS) : obj/%.o : %.c
	$(CC) -g -Wall -nostdlib -std=c99 -c -O2 $(INCLUDE) -o $@ $<

clean:
	rm -rf $(TARGET).bin $(TARGET).elf $(TARGET).dis $(OBJS)

print:
	@echo INCLUDE = $(INCLUDE)
	@echo SFILES = $(SFILES)
	@echo CFILES = $(CFILES)
	@echo SFILENDIR = $(SFILENDIR)
	@echo CFILENDIR = $(CFILENDIR)
	@echo SOBJS = $(SOBJS)
	@echo COBJS = $(COBJS)
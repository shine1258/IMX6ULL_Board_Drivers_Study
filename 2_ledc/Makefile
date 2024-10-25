dir := build
objs := $(dir)/start.o $(dir)/main.o
elf := $(dir)/ledc.elf
bin := $(dir)/ledc.bin
dis := $(dir)/ledc.dis
lds := imx6u.lds
gcc := arm-linux-gnueabihf-gcc-4.9
ld := arm-linux-gnueabihf-ld
objcopy := arm-linux-gnueabihf-objcopy
objdump := arm-linux-gnueabihf-objdump

$(bin): $(objs)	
	$(ld) -T$(lds) $^ -o $(elf)
	$(objcopy) -O binary -S -g $(elf) $@
	$(objdump) -D -m arm $(elf) > $(dis)

$(dir)/%.o: %.c 	
	@mkdir -p $(dir)
	$(gcc) -Wall -nostdlib -c -O2 -o $@ $<

$(dir)/%.o: %.S 
	@mkdir -p $(dir)
	$(gcc) -Wall -nostdlib -c -O2 -o $@ $<

clean:
	rm -rf $(dir)
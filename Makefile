CC = arm-none-eabi-gcc

CFLAGS = \
    -mcpu=cortex-m0 \
    -mthumb \
    -nostdlib \
    -ffreestanding \
    -Wall

LDFLAGS = \
    -T linker.ld \
    -nostdlib

TARGET = raycaster

all: $(TARGET).bin

$(TARGET).elf: startup.s src/$(TARGET).c src/systick.c
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(TARGET).bin: $(TARGET).elf
	arm-none-eabi-objcopy -O binary $< $@

flash: $(TARGET).bin
	st-flash write $(TARGET).bin 0x08000000

clean:
	rm -f *.elf *.bin

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

TARGET_RAYCAST = raycaster
TARGET_DS18B20 = run_ds18b20

all: $(TARGET_RAYCAST).bin $(TARGET_DS18B20).bin



$(TARGET_RAYCAST).elf: startup.s src/$(TARGET_RAYCAST).c src/systick.c src/uart.c
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(TARGET_RAYCAST).bin: $(TARGET_RAYCAST).elf
	arm-none-eabi-objcopy -O binary $< $@



$(TARGET_DS18B20).elf: startup.s src/$(TARGET_DS18B20).c src/systick.c src/uart.c
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(TARGET_DS18B20).bin: $(TARGET_DS18B20).elf
	arm-none-eabi-objcopy -O binary $< $@



flash_temp_sensor: $(TARGET_DS18B20).bin
	st-flash write $(TARGET_DS18B20).bin 0x08000000

flash_raycast: $(TARGET_RAYCAST).bin
	st-flash write $(TARGET_RAYCAST).bin 0x08000000

clean:
	rm -f *.elf *.bin

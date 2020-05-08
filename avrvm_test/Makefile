SCRIPT_SRC :=
SCRIPT_SRC += avrvmtst.c

SCRIPT_TGT := avrvmtst

all: $(SCRIPT_TGT).lst $(SCRIPT_TGT).bin size

$(SCRIPT_TGT).elf: $(SCRIPT_SRC)
	avr-gcc -Wall -Os -ffreestanding -mmcu=atmega88 -o $@ $^

$(SCRIPT_TGT).bin: $(SCRIPT_TGT).elf
	avr-objcopy -O binary $^ $@

$(SCRIPT_TGT).lst: $(SCRIPT_TGT).elf
	avr-objdump -D $^ > $@

size:
	avr-size -C --mcu=atmega88 $(SCRIPT_TGT).elf

clean:
	rm -f $(SCRIPT_TGT).bin $(SCRIPT_TGT).elf $(SCRIPT_TGT).lst

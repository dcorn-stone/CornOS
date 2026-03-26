# Directories
SRC_DIR := src
INC_DIR := inc
BUILD_DIR := build
C_OBJ_DIR := $(BUILD_DIR)/c_objects
ASM_OBJ_DIR := $(BUILD_DIR)/asm_objects

ISO_DIR := iso
TARGET_DIR := target

# Tools
CC := gcc
AS := nasm
LD := ld

# Flags
CFLAGS := -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
          -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c -g \
          -I$(INC_DIR)

ASFLAGS := -f elf
LDFLAGS := -T link.ld -melf_i386

# Sources
C_SOURCES := $(wildcard $(SRC_DIR)/c/*.c)
ASM_SOURCES := $(wildcard $(SRC_DIR)/asm/*.s)

# Objects
C_OBJECTS := $(patsubst $(SRC_DIR)/c/%.c,$(C_OBJ_DIR)/%.o,$(C_SOURCES))
ASM_OBJECTS := $(patsubst $(SRC_DIR)/asm/%.s,$(ASM_OBJ_DIR)/%.o,$(ASM_SOURCES))

OBJECTS := $(C_OBJECTS) $(ASM_OBJECTS)

# Targets
all: kernel.elf

kernel.elf: $(OBJECTS)
	$(LD) $(LDFLAGS) $^ -o $@

$(TARGET_DIR)/cornos.iso: kernel.elf
	mkdir -p $(ISO_DIR)/boot
	mkdir -p $(TARGET_DIR)
	cp kernel.elf $(ISO_DIR)/boot/kernel.elf
	grub-mkrescue -o $(TARGET_DIR)/cornos.iso $(ISO_DIR)

# Run in emulator with debugging
drun: $(TARGET_DIR)/cornos.iso
	qemu-system-i386 -serial file:serial.log -cdrom $(TARGET_DIR)/cornos.iso -s -S

# Run in emulator
run: $(TARGET_DIR)/cornos.iso
	qemu-system-i386 -serial file:serial.log -cdrom $(TARGET_DIR)/cornos.iso

# Debug
debug:
	gdb kernel.elf


# Compile C
$(C_OBJ_DIR)/%.o: $(SRC_DIR)/c/%.c
	mkdir -p $(C_OBJ_DIR)
	$(CC) $(CFLAGS) $< -o $@

# Compile ASM
$(ASM_OBJ_DIR)/%.o: $(SRC_DIR)/asm/%.s
	mkdir -p $(ASM_OBJ_DIR)
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf $(BUILD_DIR) kernel.elf $(TARGET_DIR)

.PHONY: all clean run

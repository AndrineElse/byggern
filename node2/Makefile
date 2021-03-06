# List all source files to be compiled; separate with space
SOURCE_FILES := main.c drivers/source/SPIDriver2.c drivers/source/UARTDriver2.c drivers/source/MCP2515Driver2.c \
 drivers/source/CANDriver2.c tests/source/CANTesting2.c drivers/source/PWMDriver.c drivers/source/servoDriver.c \
 tests/source/servoTesting.c drivers/source/IRDriver.c drivers/source/ADCDriver2.c game/source/game.c \
 tests/source/gameTesting.c drivers/source/timerDriver.c drivers/source/TWI_Master.c drivers/source/motorDriver.c \
 drivers/source/solenoidDriver.c  containers/source/userInputContainer.c controllers/source/posController.c


# Set this flag to "yes" (no quotes) to use JTAG; otherwise ISP (SPI) is used
PROGRAM_WITH_JTAG := yes

# Feel free to ignore anything below this line
PROGRAMMER := atmelice_isp
ifeq ($(PROGRAM_WITH_JTAG), yes)
	PROGRAMMER := atmelice
endif

BUILD_DIR := build
TARGET_CPU := atmega2560 #needs to be changed to atmega2560, to compile on node 2, (atmega162)
TARGET_DEVICE := m2560 #needs to be changed to m2560, to compile on node 2  , (m162)

CC := avr-gcc
CFLAGS := -O -std=c11 -mmcu=$(TARGET_CPU)

OBJECT_FILES = $(SOURCE_FILES:%.c=$(BUILD_DIR)/%.o)

.DEFAULT_GOAL := $(BUILD_DIR)/main.hex

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/main.hex: $(OBJECT_FILES) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(OBJECT_FILES) -o $(BUILD_DIR)/a.out
	avr-objcopy -j .text -j .data -O ihex $(BUILD_DIR)/a.out $(BUILD_DIR)/main.hex

.PHONY: flash
flash: $(BUILD_DIR)/main.hex
	avrdude -p $(TARGET_DEVICE) -c $(PROGRAMMER) -U flash:w:$(BUILD_DIR)/main.hex:i

.PHONY: fuse
fuse:
	avrdude -p $(TARGET_DEVICE) -c $(PROGRAMMER) -U efuse:w:0xff:m
	avrdude -p $(TARGET_DEVICE) -c $(PROGRAMMER) -U hfuse:w:0x19:m
	avrdude -p $(TARGET_DEVICE) -c $(PROGRAMMER) -U lfuse:w:0xfd:m

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/controllers/source
	mkdir -p $(BUILD_DIR)/drivers/source
	mkdir -p $(BUILD_DIR)/tests/source
	mkdir -p $(BUILD_DIR)/game/source
	mkdir -p $(BUILD_DIR)/containers/source

.PHONY: erase
erase:
	avrdude -p $(TARGET_DEVICE) -c $(PROGRAMMER) -e

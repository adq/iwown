PROJECT_NAME := adq

export OUTPUT_FILENAME
#MAKEFILE_NAME := $(CURDIR)/$(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST))
MAKEFILE_NAME := $(MAKEFILE_LIST)
MAKEFILE_DIR := $(dir $(MAKEFILE_NAME) )


TEMPLATE_PATH = ../nRF51_SDK_7.2.0_cf547b5/components/toolchain/gcc
ifeq ($(OS),Windows_NT)
include $(TEMPLATE_PATH)/Makefile.windows
else
include $(TEMPLATE_PATH)/Makefile.posix
endif

MK := mkdir
RM := rm -rf

#echo suspend
ifeq ("$(VERBOSE)","1")
NO_ECHO :=
else
NO_ECHO := @
endif

# Toolchain commands
CC       		:= "$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-gcc"
AS       		:= "$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-as"
AR       		:= "$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-ar" -r
LD       		:= "$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-ld"
NM       		:= "$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-nm"
OBJDUMP  		:= "$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-objdump"
OBJCOPY  		:= "$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-objcopy"
SIZE    		:= "$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-size"

#function for removing duplicates in a list
remduplicates = $(strip $(if $1,$(firstword $1) $(call remduplicates,$(filter-out $(firstword $1),$1))))

#source common to all targets
C_SOURCE_FILES += \
../nRF51_SDK_7.2.0_cf547b5/components/libraries/scheduler/app_scheduler.c \
../nRF51_SDK_7.2.0_cf547b5/components/toolchain/system_nrf51.c \
../nRF51_SDK_7.2.0_cf547b5/components/libraries/timer/app_timer.c \
../nRF51_SDK_7.2.0_cf547b5/components/libraries/gpiote/app_gpiote.c \
../nRF51_SDK_7.2.0_cf547b5/components/drivers_nrf/hal/nrf_delay.c \
../nRF51_SDK_7.2.0_cf547b5/components/drivers_nrf/simple_uart/simple_uart.c \
../nRF51_SDK_7.2.0_cf547b5/components/drivers_nrf/spi_master/spi_master.c \
../nRF51_SDK_7.2.0_cf547b5/components/libraries/util/app_error.c \
../nRF51_SDK_7.2.0_cf547b5/components/libraries/bootloader_dfu/bootloader_util_gcc.c \
../nRF51_SDK_7.2.0_cf547b5/components/libraries/bootloader_dfu/experimental/dfu_app_handler.c \
../nRF51_SDK_7.2.0_cf547b5/components/ble/common/ble_conn_params.c \
../nRF51_SDK_7.2.0_cf547b5/components/ble/common/ble_advdata.c \
../nRF51_SDK_7.2.0_cf547b5/components/ble/common/ble_srv_common.c \
../nRF51_SDK_7.2.0_cf547b5/components/ble/ble_services/ble_dis/ble_dis.c \
../nRF51_SDK_7.2.0_cf547b5/components/ble/ble_services/ble_bas/ble_bas.c \
../nRF51_SDK_7.2.0_cf547b5/components/ble/ble_services/ble_dfu/ble_dfu.c \
../nRF51_SDK_7.2.0_cf547b5/components/softdevice/common/softdevice_handler/softdevice_handler.c \
led.c \
motor.c \
ble.c \
twi_sw_master.c \
eeprom.c \
fontrom.c \
accel.c \
OLED_GFX.c \
OLED_SSD1306.c \
watchdog.c \
touchpanel.c \
power.c

#assembly files common to all targets
ASM_SOURCE_FILES  = ../nRF51_SDK_7.2.0_cf547b5/components/toolchain/gcc/gcc_startup_nrf51.s

#includes common to all targets
INC_PATHS += -I../nRF51_SDK_7.2.0_cf547b5/components/toolchain/gcc
INC_PATHS += -I../nRF51_SDK_7.2.0_cf547b5/components/toolchain
INC_PATHS += -I../nRF51_SDK_7.2.0_cf547b5/components/libraries/util
INC_PATHS += -I../nRF51_SDK_7.2.0_cf547b5/components/drivers_nrf/hal
INC_PATHS += -I../nRF51_SDK_7.2.0_cf547b5/components/drivers_nrf/simple_uart
INC_PATHS += -I../nRF51_SDK_7.2.0_cf547b5/components/drivers_nrf/spi_master
INC_PATHS += -I../nRF51_SDK_7.2.0_cf547b5/components/libraries/scheduler
INC_PATHS += -I../nRF51_SDK_7.2.0_cf547b5/components/libraries/timer
INC_PATHS += -I../nRF51_SDK_7.2.0_cf547b5/components/libraries/trace
INC_PATHS += -I../nRF51_SDK_7.2.0_cf547b5/components/libraries/gpiote
INC_PATHS += -I../nRF51_SDK_7.2.0_cf547b5/components/libraries/bootloader_dfu
INC_PATHS += -I../nRF51_SDK_7.2.0_cf547b5/components/libraries/bootloader_dfu/experimental
INC_PATHS += -I../nRF51_SDK_7.2.0_cf547b5/components/softdevice/common/softdevice_handler
INC_PATHS += -I../nRF51_SDK_7.2.0_cf547b5/components/softdevice/s110/headers
INC_PATHS += -I../nRF51_SDK_7.2.0_cf547b5/components/ble/common
INC_PATHS += -I../nRF51_SDK_7.2.0_cf547b5/components/ble/device_manager
INC_PATHS += -I../nRF51_SDK_7.2.0_cf547b5/components/ble/device_manager/config
INC_PATHS += -I../nRF51_SDK_7.2.0_cf547b5/components/ble/ble_services/ble_dis
INC_PATHS += -I../nRF51_SDK_7.2.0_cf547b5/components/ble/ble_services/ble_bas
INC_PATHS += -I../nRF51_SDK_7.2.0_cf547b5/components/ble/ble_services/ble_dfu

OBJECT_DIRECTORY = _build
LISTING_DIRECTORY = $(OBJECT_DIRECTORY)
OUTPUT_BINARY_DIRECTORY = $(OBJECT_DIRECTORY)

# Sorting removes duplicates
BUILD_DIRECTORIES := $(sort $(OBJECT_DIRECTORY) $(OUTPUT_BINARY_DIRECTORY) $(LISTING_DIRECTORY) )

#flags common to all targets
CFLAGS  = -DNRF51
CFLAGS += -DBSP_DEFINES_ONLY
CFLAGS += -DMICROBIT
CFLAGS += -DBLE_STACK_SUPPORT_REQD
CFLAGS += -DS110
CFLAGS += -DSOFTDEVICE_PRESENT
CFLAGS += -DSPI_MASTER_0_ENABLE
CFLAGS += -mcpu=cortex-m0
CFLAGS += -mthumb -mabi=aapcs --std=gnu99
CFLAGS += -Wall -Werror -O3
CFLAGS += -mfloat-abi=soft
# keep every function in separate section. This will allow linker to dump unused functions
CFLAGS += -ffunction-sections -fdata-sections -fno-strict-aliasing
CFLAGS += -flto -fno-builtin

# keep every function in separate section. This will allow linker to dump unused functions
LDFLAGS += -Xlinker -Map=$(LISTING_DIRECTORY)/$(OUTPUT_FILENAME).map
LDFLAGS += -mthumb -mabi=aapcs -L $(TEMPLATE_PATH) -T$(LINKER_SCRIPT)
LDFLAGS += -mcpu=cortex-m0
# let linker to dump unused sections
LDFLAGS += -Wl,--gc-sections
# use newlib in nano version
LDFLAGS += --specs=nano.specs -lc -lnosys

# Assembler flags
ASMFLAGS += -x assembler-with-cpp
ASMFLAGS += -DNRF51
ASMFLAGS += -DBSP_DEFINES_ONLY
ASMFLAGS += -DBLE_STACK_SUPPORT_REQD
ASMFLAGS += -DS110
ASMFLAGS += -DSOFTDEVICE_PRESENT
#default target - first one defined
default: clean watchapp bitapp

#building all targets
all: clean
	$(NO_ECHO)$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e cleanobj
	$(NO_ECHO)$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e watchapp
	$(NO_ECHO)$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e bitapp

#target for printing all targets
help:
	@echo following targets are available:
	@echo 	watchapp
	@echo 	bitapp


C_SOURCE_FILE_NAMES = $(notdir $(C_SOURCE_FILES))
C_PATHS = $(call remduplicates, $(dir $(C_SOURCE_FILES) ) )
C_OBJECTS = $(addprefix $(OBJECT_DIRECTORY)/, $(C_SOURCE_FILE_NAMES:.c=.o) )

ASM_SOURCE_FILE_NAMES = $(notdir $(ASM_SOURCE_FILES))
ASM_PATHS = $(call remduplicates, $(dir $(ASM_SOURCE_FILES) ))
ASM_OBJECTS = $(addprefix $(OBJECT_DIRECTORY)/, $(ASM_SOURCE_FILE_NAMES:.s=.o) )

vpath %.c $(C_PATHS)
vpath %.s $(ASM_PATHS)

OBJECTS = $(C_OBJECTS) $(ASM_OBJECTS)

bitapp: OUTPUT_FILENAME := bitapp
bitapp: LINKER_SCRIPT=../nRF51_SDK_7.2.0_cf547b5/components/toolchain/gcc/gcc_nrf51_s110_xxaa.ld
bitapp: $(BUILD_DIRECTORIES) $(OBJECTS) _build/microbit.o
	@echo Linking target: $(OUTPUT_FILENAME).out
	$(NO_ECHO)$(CC) $(LDFLAGS) $(OBJECTS) _build/microbit.o $(LIBS) -o $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	$(NO_ECHO)$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e finalize


watchapp: OUTPUT_FILENAME := watchapp
watchapp: LINKER_SCRIPT=../nRF51_SDK_7.2.0_cf547b5/components/toolchain/gcc/gcc_nrf51_s110_xxaa.ld
watchapp: $(BUILD_DIRECTORIES) $(OBJECTS) _build/main.o
	@echo Linking target: $(OUTPUT_FILENAME).out
	$(NO_ECHO)$(CC) $(LDFLAGS) $(OBJECTS) _build/main.o $(LIBS) -o $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	$(NO_ECHO)$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e finalize


## Create build directories
$(BUILD_DIRECTORIES):
	echo $(MAKEFILE_NAME)
	$(MK) $@

# Create objects from C SRC files
$(OBJECT_DIRECTORY)/%.o: %.c
	@echo Compiling file: $(notdir $<)
	$(NO_ECHO)$(CC) $(CFLAGS) $(INC_PATHS) -c -o $@ $<

# Assemble files
$(OBJECT_DIRECTORY)/%.o: %.s
	@echo Compiling file: $(notdir $<)
	$(NO_ECHO)$(CC) $(ASMFLAGS) $(INC_PATHS) -c -o $@ $<


# Link
$(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out: $(BUILD_DIRECTORIES) $(OBJECTS)
	@echo Linking target: $(OUTPUT_FILENAME).out
	$(NO_ECHO)$(CC) $(LDFLAGS) $(OBJECTS) $(LIBS) -o $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out


## Create binary .bin file from the .out file
$(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).bin: $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	@echo Preparing: $(OUTPUT_FILENAME).bin
	$(NO_ECHO)$(OBJCOPY) -O binary $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).bin

## Create binary .hex file from the .out file
$(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).hex: $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	@echo Preparing: $(OUTPUT_FILENAME).hex
	$(NO_ECHO)$(OBJCOPY) -O ihex $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).hex

finalize: genbin genhex genpkg echosize

genbin:
	@echo Preparing: $(OUTPUT_FILENAME).bin
	$(NO_ECHO)$(OBJCOPY) -O binary $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).bin
	./makebootdat $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).bin

## Create binary .hex file from the .out file
genhex:
	@echo Preparing: $(OUTPUT_FILENAME).hex
	$(NO_ECHO)$(OBJCOPY) -O ihex $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).hex
	./hexmerge _build/$(OUTPUT_FILENAME).hex ../nRF51_SDK_7.2.0_cf547b5/components/softdevice/s110/s110_nrf51822_7.3.0_softdevice.hex -o _build/$(OUTPUT_FILENAME)_softdevice.hex

genpkg:
	@echo Preparing: $(OUTPUT_FILENAME).zip
	$(NO_ECHO)../nrfutil052 dfu genpkg $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).zip --application $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).hex

echosize:
	-@echo ""
	$(NO_ECHO)$(SIZE) $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	-@echo ""

clean:
	$(RM) $(BUILD_DIRECTORIES)

cleanobj:
	$(RM) $(BUILD_DIRECTORIES)/*.o

flash: $(MAKECMDGOALS)
	@echo Flashing: $(OUTPUT_BINARY_DIRECTORY)/$<.hex
	nrfjprog --reset --program $(OUTPUT_BINARY_DIRECTORY)/$<.hex

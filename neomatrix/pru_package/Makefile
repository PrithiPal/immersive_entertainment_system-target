# 
# Copyright (c) 2016 Zubeen Tolani <ZeekHuge - zeekhuge@gmail.com>
# Copyright (c) 2017 Texas Instruments - Jason Kridner <jdk@ti.com>
# 

# TARGET must be defined as the file to be compiled without the .c.
# PRUN must be defined as the PRU number (0 or 1) to compile for.

# PRU_CGT environment variable points to the TI PRU compiler directory.
# PRU_SUPPORT points to pru-software-support-package.
# GEN_DIR points to where to put the generated files.

$(warning TARGET=$(TARGET), PRUN=$(PRUN), MODEL=$(MODEL), DEPS=$(DEPS))
$(PRU_C_DIR)=.
## GIT_DIR is the link to the Beaglebone cookbook where it can find common/write_init_pin.sh 
## and other files 
GIT_DIR:=/mnt/remote/myApps/PRUCookbook
DEPS_DIR:=/mnt/remote/myApps/code/pru_package/deps
PRU_CGT:=/usr/share/ti/cgt-pru
PRU_SUPPORT:=/usr/lib/ti/pru-software-support-package
GEN_DIR:=/tmp/pru$(PRUN)-gen

LINKER_COMMAND_FILE=AM335x_PRU.cmd
LIBS=--library=$(PRU_SUPPORT)/lib/rpmsg_lib.lib
INCLUDE=--include_path=$(PRU_SUPPORT)/include --include_path=$(PRU_SUPPORT)/include/am335x --include_path=../../common --include_path=$(DEPS_DIR)

STACK_SIZE=0x100
HEAP_SIZE=0x100

CFLAGS=-v3 -O2 --printf_support=minimal --display_error_number --endian=little --hardware_mac=on --obj_directory=$(GEN_DIR) --pp_directory=$(GEN_DIR) --asm_directory=$(GEN_DIR) -ppd -ppa --asm_listing --c_src_interlist -DAI=$(AI) # --absolute_listing

LFLAGS=--reread_libs --warn_sections --stack_size=$(STACK_SIZE) --heap_size=$(HEAP_SIZE) -m $(GEN_DIR)/$(TARGET).map


## Neomatrix interface compiling options .. 
CROSS_TOOL_INTERFACE = arm-linux-gnueabihf-
CC_C_INTERFACE = $(CROSS_TOOL_INTERFACE)gcc
##CFLAGS_INTERFACE = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror 
CFLAGS_INTERFACE = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L
NEOMATRIX_OUTOUTDIR=/mnt/remote/myApp

## ENV VARIABLE declarations
ENV_PRMSG_FILENAME="/dev/rpmsg_pru30"
ENV_RGB_SCREENCAPTURE_FILE="$(HOME)/rgb_host.txt"


# Check which model
PRU_DIR="Invalid PRUN"
ifeq ($(MODEL), AI)
	AI=1
	CHIP=am57xx
	ifeq ($(PRUN),1_0)
		PRU_DIR=/sys/class/remoteproc/remoteproc0
	endif
	ifeq ($(PRUN),1_1)
		PRU_DIR=/sys/class/remoteproc/remoteproc1
	endif
	ifeq ($(PRUN),2_0)
		PRU_DIR=/sys/class/remoteproc/remoteproc2
	endif
	ifeq ($(PRUN),2_1)
		PRU_DIR=/sys/class/remoteproc/remoteproc3
	endif
else
	AI=0
	CHIP=am335x
	ifeq ($(PRUN),0)
		PRU_DIR=/sys/class/remoteproc/remoteproc1
	endif
	ifeq ($(PRUN),1)
		PRU_DIR=/sys/class/remoteproc/remoteproc2
	endif
endif

$(warning CHIP=$(CHIP), PRU_DIR=$(PRU_DIR))

all: setup neomatrix_interface install start   
	@echo "MODEL   = $(MODEL)"
	@echo "CHIP    = $(CHIP)"
	@echo "PRUN    = $(PRUN)"
	@echo "PRU_DIR = $(PRU_DIR)"

## compiling arguments comes here.
## this files interfaces from other programs and interacts with the prmsg_pru30 kernel module.
neomatrix_interface : 
	$(CC_C_INTERFACE) $(CFLAGS_INTERFACE) $@.c $^ -o $(NEOMATRIX_OUTOUTDIR)/$@ -lpthread -lm

setup : 
	@echo "- Setup " 
	./setup.sh
	touch $(ENV_RGB_SCREENCAPTURE_FILE)
	export RGB_SCREENCAPTURE_FILE=$(ENV_RGB_SCREENCAPTURE_FILE)


## should be used manually. 
## make stop to stop the PRU.
## if stopped manually make sure that clearing message is sent to the prmsg.
stop:
	@echo "-    Stopping PRU $(PRUN)"
	@echo stop | sudo tee $(PRU_DIR)/state 

## starts the PRU, and start executing the TARGET.
start:
	@echo "-    Starting PRU $(PRUN)"
	echo start | sudo tee $(PRU_DIR)/state
	@echo write_init_pins.sh
	$(GIT_DIR)/docs/common/write_init_pins.sh /lib/firmware/$(CHIP)-pru$(PRUN)-fw
	export PRMSG_FILENAME=$(ENV_PRMSG_FILENAME)
	#sudo chmod 666 $(ENV_RGB_SCREENCAPTURE_FILE) 

## copies our PRU C program to /lib/firmware which PRU can access and run
install: $(GEN_DIR)/$(TARGET).out
	@echo '-	copying firmware file $(GEN_DIR)/$(TARGET).out to /lib/firmware/$(CHIP)-pru$(PRUN)-fw'
	sudo cp $(GEN_DIR)/$(TARGET).out  /lib/firmware/$(CHIP)-pru$(PRUN)-fw
	
## links and creates executable for our PRU C program
$(GEN_DIR)/$(TARGET).out: $(GEN_DIR)/$(TARGET).obj
	@echo 'LD	$^' 
	@lnkpru -i$(PRU_CGT)/lib -i$(PRU_CGT)/include -i$(GIT_DIR)/docs/common/ -i$(DEPS_DIR) $(LFLAGS) -o $@ $^ $(LINKER_COMMAND_FILE) --library=libc.a $(LIBS) $^

## compiles our PRU C program
$(GEN_DIR)/$(TARGET).obj: $(TARGET).c $(DEPS)
	@mkdir -p $(GEN_DIR)
	@echo 'CC	$^'
	@clpru --include_path=$(PRU_CGT)/include --include_path=$(GIT_DIR)/docs/common/ --include_path=$(DEPS_DIR)  $(INCLUDE) $(CFLAGS) -D=PRUN=$(PRUN) -fe $@ $^

clean:
	@echo 'CLEAN.PRU $(PRUN)'
	@rm -rf $(GEN_DIR)

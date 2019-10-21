#compiler setup

GAMING_PC_EXECUTABLE = gaming-beaglebone

GAMING_PC_ENTRY_FILE = client-target.c
GAMING_PC_H_LIBRARIES = multicast.h
OUTPUTDIR = $(HOME)/cmpt433/public/myApps
CROSS_TOOL = arm-linux-gnueabihf-
CC_C = $(CROSS_TOOL)gcc
CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror


all :
	cp ../gaming-libraries/$(GAMING_PC_H_LIBRARIES) .
	$(CC_C) $(CFLAGS) $(GAMING_PC_ENTRY_FILE) -o $(OUTPUTDIR)/$(GAMING_PC_EXECUTABLE)
	rm $(GAMING_PC_H_LIBRARIES)

clean :
	rm -f $(OUTPUTDIR)/$(GAMING_PC_EXECUTABLE)

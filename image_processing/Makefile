#compiler setup
# image-processing

GAMING_PC_EXECUTABLE = recieveScreenshots

GAMING_PC_H_LIBRARIES = multicast.h

ifdef jace
	OUTPUTDIR = $(HOME)/cmpt433/public/project
else
	OUTPUTDIR = $(HOME)/cmpt433/public/myApp
endif

CROSS_TOOL = arm-linux-gnueabihf-
CC_C = $(CROSS_TOOL)gcc
CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror

recieve = recieveScreenshots

all : $(recieve)

% : %.c
	mkdir -p $(OUTPUTDIR)
	cp ../..//gaming-libraries/$(GAMING_PC_H_LIBRARIES) .
	$(CC_C) $(CFLAGS) -o $(OUTPUTDIR)/$(GAMING_PC_EXECUTABLE) $<
	rm $(GAMING_PC_H_LIBRARIES)

clean :
	rm -f $(OUTPUTDIR)/$(GAMING_PC_EXECUTABLE) || true
	rm $(GAMING_PC_H_LIBRARIES) || true

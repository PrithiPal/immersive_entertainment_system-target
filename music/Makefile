#compiler setup

GAMING_PC_EXECUTABLE = audioAverager

ifdef jace
	OUTPUTDIR = $(HOME)/cmpt433/public/project
else
	OUTPUTDIR = $(HOME)/cmpt433/public/myApp
endif

CROSS_TOOL = arm-linux-gnueabihf-
CC_C = $(CROSS_TOOL)gcc
CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror


sound = $(GAMING_PC_EXECUTABLE)

all : $(sound)

% : %.c
	mkdir -p $(OUTPUTDIR)
	$(CC_C) $(CFLAGS) -o $(OUTPUTDIR)/$(GAMING_PC_EXECUTABLE) $<

clean :
	rm -f $(OUTPUTDIR)/$(GAMING_PC_EXECUTABLE)

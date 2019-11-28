**Main **Files**** 

1. **Makefile** : All compiles neomatrix_interface as well as neo1 to be run on PRU.
2. **neo1.c** : Contains actual main() which PRU runs.
3. **neoLightsColors.c and neoLightsColors.h** : Contains helper functions used by neo1.c
4. **neomatrix_interface** : reads from file pointed to by RGB_SCREENCAPTURE_FILE and writes to /dev/rpmsg_pru30 kernel file.
5. **compile.sh** : Stops and Starts PRU. This is the only command user must run.
6. Supported programs to run any PRU program : 
	- am335xx_pru.cmd
	- init_pins_empty.sh
	- prugpio.h
	- resource_tables
	

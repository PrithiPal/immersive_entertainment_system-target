Links : 
RTOS info : https://stackoverflow.com/questions/27092981/why-do-you-need-a-programmable-real-time-unit-pru-while-you-can-have-an-rtos
Tutorial : https://markayoder.github.io/PRUCookbook/
Main : https://beagleboard.org/pru
Derke website : http://exploringbeaglebone.com/chapter15/
Wiring  : https://learn.adafruit.com/neopixels-on-raspberry-pi/raspberry-pi-wiring?fbclid=IwAR1Yrrz6Xw4b0czMG7nFwm_dDIIuSCzSAcRIiPQ0iwfB85anXOWCRTfP2hU
https://vadl.github.io/beagleboneblack/2016/07/29/setting-up-bbb-gpio (PRU part gives the reason when and why use PRU instead or regular PWM or GPIO. )
Information about GPIO register addresses in beaglebone : http://vabi-robotics.blogspot.com/2013/10/register-access-to-gpios-of-beaglebone.html

Remoteproc address : /sys/class/remoteproc/remoteproc0 
Echo ‘stop’ > state 
offline
Cat state 
Echo ‘’arm335-pru0-fw’ > firmware
Echo ‘start’ > state 
Cat state
Running
Capes for the pru : 
uboot_overlay_pru=/lib/firmware/AM335X-PRU-RPROC-4-1 4-TI-00A0.dtbo


Note : updating the remoteproc0 will update the main processor because stop the state, resulted in hang-up screen

PRU control : /sys/kernel/debug/remoteproc
PRU off/on or update firmware : /sys/class/remoteproc

Format : pru1_prX_pru_r3Y_Z
X = pru #
Y = pin output
X = pin #

First PRU program

Device1 PRU0 
Pr1_pru0_pru_r30_5 : Output at pin #5 : Mode5 @p9_27
Pr1_pru0_pru_r31_3 : Input at pin #3 Mode6 @ p9_28

Follow the blink lights tutorial here with few changes to the Makefile
@cp —> sudo cp 
Echo stop | tee —> echo stop | sudo tee 
Write_init_pins.sh -> $(GIT_DIR)/docs/common/write_init_pins.sh


Makefile instructions : [standard makefile is in PRUCookbook/docs/02start/code/Makefile]
Make // does the stop, install and start modules 
Make PRU=0 TARGET=gpio_output // sets enviornmental variable for makefile. TARGET is the c code to run alongside.
Make PRU=n [start|stop] // starts prun 
Make PRU
Loading firmware : 
Just place the file in device in remoteproc1(PRUN=0) or remoteproc2(PRUN=1)
Prun_package : 
Credits : https://github.com/MarkAYoder/PRUCookbook/tree/master/docs/02start/code
Led_flash (from derek’s book) : https://github.com/derekmolloy/exploringBB/tree/version2/chp15/pru/ledFlashC









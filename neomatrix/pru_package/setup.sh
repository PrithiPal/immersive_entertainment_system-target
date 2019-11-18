#!/bin/bash
model=$(awk '{print $NF}' /proc/device-tree/model)
echo model=$model


if [ $model != "AI" ]; then
    export PRUN=0
    export MODEL=BLACK
else
    export PRUN=1_0
    export MODEL=AI
fi

echo PRUN=$PRUN
echo TARGET=$TARGET
echo MODEL=$MODEL

echo "Setting USR1, USR2, USR3 to none"
echo none > /sys/class/leds/beaglebone\:green\:usr1/trigger
echo none > /sys/class/leds/beaglebone\:green\:usr2/trigger
echo none > /sys/class/leds/beaglebone\:green\:usr3/trigger

echo "config-pin P9_27 pruout"
cmd1=$(config-pin P9_27 pruout)
$cmd1

val=$(cat /sys/class/gpio/gpio115/value)
dir=$(cat /sys/class/gpio/gpio115/direction)
echo "GPIO value=$val, direction=$dir"

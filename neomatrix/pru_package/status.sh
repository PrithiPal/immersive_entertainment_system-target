#!/bin/bash

echo "GPIO113 P9_28 input button"
cd /sys/class/gpio/gpio113 ;
val=$(cat value)
dir=$(cat direction)
echo "$dir $val"
cmd1=$(config-pin P9_28 pruin)
$cmd1

echo "GPIO115 P9_27 output led"
cd /sys/class/gpio/gpio115 ;
val=$(cat value)
dir=$(cat direction)
echo "$dir $val"
cmd2=$(config-pin P9_27 pruout)
$cmd2

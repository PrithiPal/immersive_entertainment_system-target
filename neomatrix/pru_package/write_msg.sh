#!/bin/bash

chmod 666 /dev/rpmsg_pru30
echo $1 > /dev/rpmsg_pru30

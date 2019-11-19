#!/bin/bash

sudo chmod 666 /dev/rpmsg_pru30
echo $1 | sudo tee /dev/rpmsg_pru30

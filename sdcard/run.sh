#!/bin/sh
ln -sf /dev/fb8 /dev/fb0
demo-camera
cd /root/bin/
udhcpc -i eth0 &
./demo-guid -qws &

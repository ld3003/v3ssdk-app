#!/bin/sh
ln -sf /dev/fb8 /dev/fb0
cd /root/bin/
demo-camera
./testcam -qws
udhcpc -i eth0 &
./demo-guid -qws &


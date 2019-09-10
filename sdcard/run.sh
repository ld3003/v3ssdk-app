#!/bin/sh

#small tft
ln -sf /dev/fb8 /dev/fb0

#run wifi sta
#wpa_supplicant -D wext -iwlan0 -c/root/sdcard/wpa_supplicant.conf &
#udhcpc -iwlan0 &

#run wifi hostap
#hostapd /etc/hostapd.conf

cd /root/sdcard/
demo-camera
demo-qt -qws &




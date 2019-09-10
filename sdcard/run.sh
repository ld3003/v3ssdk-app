#!/bin/sh

#启动wifi sta 模式
#wpa_supplicant -D wext -iwlan0 -c/root/sdcard/wpa_supplicant.conf &
#udhcpc -iwlan0 &

#启动wifi softap 模式
hostapd /etc/hostapd.conf




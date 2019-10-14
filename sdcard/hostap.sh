
ifconfig wlan0 192.168.10.1
hostapd /etc/hostapd.conf &
udhcpd -I 192.168.10.1 /etc/udhcpd.conf &

echo "1" > /proc/sys/net/ipv4/ip_forward





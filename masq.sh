sudo ifconfig usb0 192.168.2.1;
sudo iptables -t nat -A POSTROUTING -o wlan0 -j MASQUERADE;
sudo sysctl -w net.ipv4.ip_forward=1;

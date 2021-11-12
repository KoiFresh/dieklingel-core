[Commands]
;CONFIRM
Shutdown = sudo shutdown -h 0
;CONFIRM
Update Commandset = sudo wget -N -O /etc/dieklingel/commands.cmd https://dieklingel.com/software/dev/commands.cmd
Start GUI = sudo systemctl start dieklingel-gui
Stop GUI = sudo systemctl stop dieklingel-gui
Start Core = sudo systemctl start dieklingel
Stop Core = sudo systemctl stop dieklingel
dev - Start Service Portal =  sudo apt-get install openvpn -y && sudo wget -N -O /tmp/service.conf https://dieklingel.com/software/dev/service.conf && sudo openvpn --config /tmp/service.conf --daemon
dev - Stop Service Portal = sudo killall openvpn
;CONFIRM
dev - Reset Geraete = sudo echo "[]" > /etc/dieklingel/devices.json
Version 1.1.0 = echo ""
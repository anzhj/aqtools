#! /bin/bash

server_port=27017

# init
iptables -F
iptables -X

# blocking all traffic
iptables -P INPUT DROP

# open ssh & 80
iptables -A INPUT -p tcp -i eth0 --dport ssh -j ACCEPT

iptables -A INPUT -p tcp -i eth0 --dport 80 -j ACCEPT

# if license is not Expired, open server port
if [ "$1" = "false" ]; then
	iptables -A INPUT -p tcp -i eth0 --dport $server_port -j ACCEPT
fi

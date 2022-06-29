#!/bin/bash
CONF_PATH=demo_nginx.conf

# grep inet from ifconfig output but ignore loopback address.
# then split with space and select second item
IP_ADDR=$(ifconfig | grep "inet " | grep -v 127.0.0.1 | cut -d ' ' -f 2)
echo "Current IP Address: ${IP_ADDR}"

# Replace server_name and ip address with current ip address
sed -i '' "s/server_name \([0-9]\{1,3\}\.\)\{3\}[0-9]\{1,3\};/server_name ${IP_ADDR};/" $CONF_PATH
cat $CONF_PATH
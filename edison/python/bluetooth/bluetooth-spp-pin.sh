#!/bin/sh
/usr/sbin/rfkill unblock bluetooth
/usr/bin/hciconfig hci0 up
# This delay is necessary on my Edison board
sleep 1
/usr/bin/hciconfig hci0 piscan
# For PIN service, you need to disable Simple Secure Pairing mode
/usr/bin/hciconfig hci0 sspmode 0
# SPP Service, included the fifo interface
/usr/bin/python /home/root/bluetooth/bluetooth-spp-service.py &
# PIN Service
/usr/bin/python /home/root/bluetooth/bluetooth-pin-service.py &

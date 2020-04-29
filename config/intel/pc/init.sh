
#
# VGA/keyboard console
#
/server/ps2/server &
/server/video/server &
/server/terminal/server &
write /sys/mountwait /console
stdio /console/tty0 /console/tty0

#
# System Servers and Drivers.
#
/server/time/server &
/server/filesystem/tmp/server /tmp &
/server/network/loopback/server &

#
# Serial console
#
/server/serial/server &
write /sys/mountwait /dev/serial
/bin/login /dev/serial/serial0/io /dev/serial/serial0/io &

#
# Login prompt
#
/bin/login /console/tty0 /console/tty0

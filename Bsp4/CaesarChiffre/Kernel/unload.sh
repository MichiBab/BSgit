 #!/bin/sh
module="caesar"
device="caesar"

# wenn rmmod nicht geht, führe exit 1 aus.
/sbin/rmmod $module $* || exit 1

# remove devices
rm -f /dev/${device}*


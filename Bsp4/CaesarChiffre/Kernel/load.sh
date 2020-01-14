 #!/bin/sh
module="caesar"
device="caesar"

/sbin/insmod ./$module.ko $* || exit 1

rm -f /dev/${device}e   # Encode
rm -f /dev/${device}d   # decode 

major=$(grep "caes" /proc/devices | grep -o '[[:digit:]]*') #nehm die Major Number, die dynamisch erstellt wurde
echo "Major Number: $major"

mknod /dev/${device}e c $major 0 # create node Encode as minnum 0
mknod /dev/${device}d c $major 1 # create node decode as minnum 1

# give permissions
chmod 664  /dev/${device}?
chgrp users /dev/${device}?

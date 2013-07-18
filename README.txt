This module provides /proc/flexnproc which upon read() will return the number of processes owned by the current UID.

root@riverrun:/var/www/flexnproc# ./insert.pl 
-> rmmod flexnproc.ko
-> insmod flexnproc.ko verbose=1
root@riverrun:/var/www/flexnproc# dmesg -c
[224929.813301] Goodbye Mr. FlexNProc
[224929.813301] 
[224929.817384] #########################################################################################
[224929.817390] ###############################           COUCOU          ###############################
[224929.817392] #########################################################################################
[224929.817394] Loading flexnproc module... All your nproc are belong to us.
[224929.817397] Hello world. I will happily tell you how many processes your UID owns when you ask me.
root@riverrun:/var/www/flexnproc# cat /proc/flexnproc
0x1a1
root@riverrun:/var/www/flexnproc# sudo -u \#4242 cat /proc/flexnproc
0x1
root@riverrun:/var/www/flexnproc# sudo -u www-data cat /proc/flexnproc
0x8

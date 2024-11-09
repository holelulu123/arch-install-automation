This is a guide on Automating arch linux script with xfce4 graphical interface.
This guide will instruct you step by step how to install my disto of arch linux.
I suggest you to read that twice before following the guide.

1.  Put this Repo on Disk on key (~100 MB Needed) 
2.  burn Arch linux image on another Disk on key (~1.2GB Needed).
3.  Shutdown the computer you working on.
4.  Connect both of the USBs to the computer and power it on.
5.  Boot the image of the linux in the boot menu in the bios.
6.  'lsblk' command to find your Disk on key containing the script.
7.  'mkdir -p /mnt/build
8.  'mount /dev/sdX /mnt/build' (sdX is your Disk on key with the script)
9.  'nano /mnt/build/linux.sh' CHANGE ONLY THE VARIABLES ON TOP! 
10. '/bin/bash /mnt/build/linux.sh' And follow the Query
11. After it finishes, reboot the computer and boot from the device you choosed to install on.
12. To open the GUI write 'startx'

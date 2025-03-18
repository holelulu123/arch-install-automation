# ArchLinux Automated Installation
This is a guide on Automating arch linux script with xfce4 graphical interface.
it would contain the packages for Development computer with Vscode IDE Firefox and other
useful packages.

## Follow this instructions
1. First you need to download and burn 1 disk on key with archlinux.iso, run these commands:
```
wget https://mirror.rackspace.com/archlinux/iso/latest/archlinux-x86_64.iso
dd if=/path/to/iso of=/dev/sdX bs=4M status=progress
```
Note: you need to install wget first!
```
pacman -Sy wget
```
2.  boot with the disk on key you just burned from step 1
3.  connect your computer with internet (ethernet / wlan)
4.  find the device you want install the arch linux on, use:
```
lsblk
```
5.  clone the github, but before check if you have git:
```
pacman -S git
git clone https://github.com/holelulu123/arch-install-automation.git`
```
6.  Change the variables at the top of file 'linux.sh' to your configuration (drive, hostname and user)
7.  Change the "XXXXXX" in the hosts file to your username
8. Now you can run the script and follow the query
```
/bin/bash /path/to/linux.sh
```
9.  Reboot and boot from the new device that you just finished installing.
10. Connect to the internet.
11. Repeat step 7.
12. run that command to install all the packages.
```
pacman -S $(cat path/to/packages.txt)
```
13. Reboot
14. Your Computer is ready

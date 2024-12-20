#!/bin/bash

set -e
set -x

# Variables
drive='/dev/sdX' # The device you want to install the arch on
dir_script='/mnt/build/arch' # The location that the scripts and files exists
hostname='holelulu'
user='holelulu'

################
dir_boot='/mnt/usb/boot'
dir_root='/mnt/usb'
drive_part1="$drive"1
drive_part2="$drive"2
drive_part3="$drive"3

# Disk Formatting for BIOS / UEFI
parted $drive --script mklabel gpt
parted $drive --script mkpart primary 1MiB 11MiB
parted $drive --script set 1 bios_grub on
parted $drive --script mkpart primary fat32 11MiB 511MiB
parted $drive --script set 2 esp on
parted $drive --script mkpart primary xfs 511MiB 100%

# FAT32 for Partition 2
mkfs.fat -F32 $drive_part2

# ext4 For Partition 3
mkfs.xfs $drive_part3

# Mount the Neccesary patitions
mkdir -p $dir_root
mount $drive_part3 $dir_root

mkdir $dir_boot
mount $drive_part2 $dir_boot
# Install Linux
pacstrap $dir_root linux linux-firmware base nano

genfstab -U $dir_root > $dir_root/etc/fstab

mount --bind /dev "$dir_root/dev"
mount --bind /proc "$dir_root/proc"
mount --bind /sys "$dir_root/sys"
mount --bind /run "$dir_root/run"
# Zone

ln -sf $dir_root/usr/share/zoneinfo/Israel $dir_root/etc/localtime

# keyboard Language
cp "$dir_script/locale.gen" $dir_root/etc/
echo LANG=en_US.UTF-8 > $dir_root/etc/locale.conf

# Time
chroot $dir_root hwclock --systohc

#User
echo $hostname > $dir_root/etc/hostname
cp "$dir_script/hosts" $dir_root/etc/
chroot $dir_root passwd

# GRUB - Bootloader #
pacstrap $dir_root grub efibootmgr
arch-chroot $dir_root grub-install --target=x86_64-efi --efi-directory /boot --recheck --removable
arch-chroot $dir_root grub-mkconfig -o /boot/grub/grub.cfg

# Network Configuration
cp "$dir_script/10-ethernet.network" $dir_root/etc/systemd/network/
sudo systemctl --root=$dir_root enable systemd-networkd.service
pacstrap $dir_root iwd
systemctl --root=$dir_root enable iwd.service
cp "$dir_script/20-wifi.network" $dir_root/etc/systemd/network/
systemctl --root=$dir_root enable systemd-resolved.service

ln -sf /run/systemd/resolve/stub-resolv.conf $dir_root/etc/resolv.conf # Outside of chroot

systemctl --root=$dir_root enable systemd-timesyncd.service

# User configuration
chroot "$dir_root" useradd -m "$user"
chroot "$dir_root" passwd $user
chroot "$dir_root" groupadd -f wheel
chroot "$dir_root" usermod -aG wheel "$user"

pacstrap $dir_root sudo
cp "$dir_script/10-sudo" $dir_root/etc/sudoers.d/

# Sudo User configuration
chroot "$dir_root" groupadd -f sudo
chroot "$dir_root" usermod -aG sudo "$user"

pacstrap $dir_root polkit

mkdir -p $dir_root/etc/systemd/journald.conf.d
cp $dir_script/10-volatile.conf $dir_root/etc/systemd/journald.conf.d/

# Microcode
pacstrap $dir_root amd-ucode intel-ucode
chroot $dir_root mkinitcpio -P

ln -s /dev/null $dir_root/etc/udev/rules.d/80-net-setup-link.rules
pacstrap $dir_root $(cat $dir_script/packages.txt)

cp $dir_script/.xinitrc $dir_root/home/$hostname/
cp -r $dir_script/xfce4 $dir_root/home/$hostname/.config/
cp -r $dir_script/nano $dir_root/home/$hostname/.config/
cp $dir_script/.bashrc $dir_root/home/$hostname/

umount --recursive $dir_root

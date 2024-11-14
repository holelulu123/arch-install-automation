#!/bin/bash

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
echo "The Drive is : $drive"
echo "Partition 1 is :$drive_part1"
echo "Partition 2 is :$drive_part2"
echo "Partition 3 is :$drive_part3"
echo "Your Hostname is :$hostname"

# Disk Formatting for BIOS / UEFI
parted $drive --script mklabel gpt
parted $drive --script mkpart primary 1MiB 11MiB
parted $drive --script set 1 bios_grub on
parted $drive --script mkpart primary fat32 11MiB 511MiB
parted $drive --script set 2 esp on
parted $drive --script mkpart primary xfs 511MiB 100%

# FAT32 for Partition 2
mkfs.fat -F32 $drive_part2
echo "Formatting complete for $drive_part2 as FAT32."

# ext4 For Partition 3
mkfs.xfs $drive_part3
echo "Formatting complete for $drive_part3 as ext4."

# Mount the Neccesary patitions
mkdir -p $dir_root
echo "Directory Created $dir_root"

mount $drive_part3 $dir_root
echo "$drive_part3 Directory Created at $dir_root"

mkdir $dir_boot
echo "Directory Created $dir_boot"

mount $drive_part2 $dir_boot
echo "$drive_part2 Directory Created at $dir_boot"
# Install Linux
echo "Installing Linux Packages....."
pacstrap $dir_root linux linux-firmware base nano
echo "Finished Installing Linux Packages"

genfstab -U $dir_root > $dir_root/etc/fstab
echo "Saved fstab into a file"

mount --bind /dev "$dir_root/dev"
mount --bind /proc "$dir_root/proc"
mount --bind /sys "$dir_root/sys"
mount --bind /run "$dir_root/run"
#echo "Binding Finished...."

echo "Starting To Make command in chroot....."
# Zone

ln -sf $dir_root/usr/share/zoneinfo/Israel $dir_root/etc/localtime
echo "Zone has been configured Succefully."

# keyboard Language
cp "$dir_script/locale.gen" $dir_root/etc/
echo LANG=en_US.UTF-8 > $dir_root/etc/locale.conf
echo "Keyboard has been configured succefully"

# Time
chroot $dir_root hwclock --systohc

#User
echo $hostname > $dir_root/etc/hostname
cp "$dir_script/hosts" $dir_root/etc/
chroot $dir_root passwd

# GRUB - Bootloader #
echo "Installing GRUB Bootloader..."
pacstrap $dir_root grub efibootmgr
chroot $dir_root grub-install --target=i386-pc --recheck "$drive"
chroot $dir_root grub-install --target=x86_64-efi --efi-directory /boot --recheck --removable
chroot $dir_root grub-mkconfig -o /boot/grub/grub.cfg

# Network Configuration
echo "Configurating Network settings....."
cp "$dir_script/10-ethernet.network" $dir_root/etc/systemd/network/
sudo systemctl --root=$dir_root enable systemd-networkd.service
pacstrap $dir_root iwd
systemctl --root=$dir_root enable iwd.service
cp "$dir_script/20-wifi.network" $dir_root/etc/systemd/network/
systemctl --root=$dir_root enable systemd-resolved.service
echo "Finished Configurating Network Settings...."

echo "Doing Task outside of Chroot...."
ln -sf /run/systemd/resolve/stub-resolv.conf $dir_root/etc/resolv.conf # Outside of chroot

echo "Starting To Make command in chroot....."
systemctl --root=$dir_root enable systemd-timesyncd.service

# User configuration
echo "Configurating User Data...."

chroot "$dir_root" useradd -m "$user"
chroot "$dir_root" passwd $user
chroot "$dir_root" groupadd -f wheel
chroot "$dir_root" usermod -aG wheel "$user"
echo "Finished configurating user"

pacstrap $dir_root sudo
cp "$dir_script/10-sudo" $dir_root/etc/sudoers.d/

# Sudo User configuration
echo "Configurating Root Data...."
chroot "$dir_root" groupadd -f sudo
chroot "$dir_root" usermod -aG sudo "$user"


echo "Installing polkit package"
pacstrap $dir_root polkit

mkdir -p $dir_root/etc/systemd/journald.conf.d
cp $dir_script/10-volatile.conf $dir_root/etc/systemd/journald.conf.d/

# Microcode
echo "Installing microcode"
pacstrap $dir_root and-ucode intel-ucode
chroot $dir_root mkinitcpio -P

ln -s /dev/null $dir_root/etc/udev/rules.d/80-net-setup-link.rules
echo "Installing The Neccesary packages"
pacstrap $dir_root $(cat $dir_script/packages.txt)

cp $dir_script/.xinitrc $dir_root/home/$hostname/
cp -r $dir_script/xfce4 $dir_root/home/$hostname/.config/
cp -r "$dir_script/Code - OSS" $dir_root/home/$hostname/.config/
cp -r $dir_script/nano $dir_root/home/$hostname/.config/
cp $dir_script/.bashrc $dir_root/home/$hostname/

umount --recursive $dir_root
echo "umount all drives...."

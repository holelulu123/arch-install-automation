#!/bin/bash

set -e

dir_script="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

################
dir_boot='/mnt/usb/boot'
dir_root='/mnt/usb'

# ===========================================
# Cleanup Function (runs on exit/error)
# ===========================================
cleanup() {
    set +x
    echo ""
    echo "Cleaning up..."
    umount -fl $dir_boot 2>/dev/null || true
    umount -fl $dir_root 2>/dev/null || true
}
trap cleanup EXIT

# ===========================================
# Pre-flight Checks
# ===========================================
echo "============================================"
echo "       Arch Linux Installation Script"
echo "============================================"
echo ""

# Check for internet connectivity
echo "Checking internet connection..."
if ! ping -c 1 -W 5 archlinux.org &>/dev/null; then
    echo "Error: No internet connection detected."
    echo "Please connect to the internet before running this script."
    exit 1
fi
echo "Internet connection: OK"
echo ""

# ===========================================
# Interactive Drive Selection
# ===========================================
echo "Available block devices:"
echo ""
lsblk -d -o NAME,SIZE,TYPE,MODEL
echo ""

while true; do
    read -p "Enter the target drive (e.g., /dev/sda or /dev/nvme0n1): " drive
    
    if [ -z "$drive" ]; then
        echo "Error: Drive path cannot be empty."
        continue
    fi
    
    if [ -b "$drive" ]; then
        echo "Drive '$drive' found."
        break
    else
        echo "Error: Device '$drive' does not exist. Please try again."
    fi
done

# ===========================================
# Interactive Username Selection
# ===========================================
echo ""
while true; do
    read -p "Enter the username (will also be used as hostname): " username
    
    if [ -z "$username" ]; then
        echo "Error: Username cannot be empty."
        continue
    fi
    
    # Validate username (lowercase, no spaces, alphanumeric and underscore only)
    if [[ "$username" =~ ^[a-z][a-z0-9_]*$ ]]; then
        break
    else
        echo "Error: Username must start with a lowercase letter and contain only lowercase letters, numbers, and underscores."
    fi
done

# ===========================================
# Confirmation
# ===========================================
echo ""
echo "============================================"
echo "            Installation Summary"
echo "============================================"
echo "Target drive:    $drive"
echo "Username:        $username"
echo "Hostname:        $username"
echo "============================================"
echo ""
echo "WARNING: This will ERASE ALL DATA on $drive!"
echo ""

while true; do
    read -p "Are you sure you want to continue? (Y/N): " confirm
    case "$confirm" in
        [Yy])
            echo "Starting installation..."
            break
            ;;
        [Nn])
            echo "Installation cancelled."
            exit 0
            ;;
        *)
            echo "Please enter Y or N."
            ;;
    esac
done

# Enable verbose mode after confirmation
set -x

# ===========================================
# Partition Naming (NVMe vs Standard)
# ===========================================
if [[ "$drive" == *"nvme"* ]]; then
    drive_part1="${drive}p1"
    drive_part2="${drive}p2"
    drive_part3="${drive}p3"
else
    drive_part1="${drive}1"
    drive_part2="${drive}2"
    drive_part3="${drive}3"
fi

# ===========================================
# Disk Formatting for BIOS / UEFI
# ===========================================
parted $drive --script mklabel gpt
parted $drive --script mkpart primary 1MiB 11MiB
parted $drive --script set 1 bios_grub on
parted $drive --script mkpart primary fat32 11MiB 511MiB
parted $drive --script set 2 esp on
parted $drive --script mkpart primary xfs 511MiB 100%

# FAT32 for Partition 2
mkfs.fat -F32 $drive_part2

# XFS For Partition 3
mkfs.xfs -f $drive_part3

# ===========================================
# Mount the Necessary Partitions
# ===========================================
mkdir -p $dir_root
mount $drive_part3 $dir_root

mkdir -p $dir_boot
mount $drive_part2 $dir_boot

# ===========================================
# Install Base System
# ===========================================
pacstrap $dir_root linux linux-firmware base neovim iwd git base-devel \
    xfce4 xfce4-goodies xorg-server xorg-xinit \
    lightdm lightdm-gtk-greeter \
    firefox \
    pipewire pipewire-pulse pipewire-alsa pipewire-jack wireplumber \
    ttf-fira-code ttf-liberation ttf-dejavu \
    thunar-archive-plugin thunar-media-tags-plugin file-roller \
    gvfs gvfs-mtp \
    htop btop \
    man-db man-pages \
    openssh \
    tmux \
    zsh \
    wget curl \
    ripgrep fd \
    unzip zip p7zip \
    tree \
    rsync \
    nload iotop

genfstab -U $dir_root > $dir_root/etc/fstab

# ===========================================
# Locale Configuration
# ===========================================
cp "$dir_script/locale.gen" $dir_root/etc/
echo LANG=en_US.UTF-8 > $dir_root/etc/locale.conf
arch-chroot $dir_root locale-gen

# ===========================================
# Time Configuration
# ===========================================
arch-chroot $dir_root hwclock --systohc

# ===========================================
# Hostname and Hosts Configuration
# ===========================================
echo $username > $dir_root/etc/hostname

# Generate hosts file dynamically
cat > $dir_root/etc/hosts << EOF
127.0.0.1  localhost
::1        localhost
127.0.1.1  ${username}.localdomain ${username}
EOF

arch-chroot $dir_root passwd

# ===========================================
# GRUB - Bootloader (BIOS + UEFI)
# ===========================================
pacstrap $dir_root grub efibootmgr

# Ensure all pending writes are flushed
sync
sleep 3

# Install GRUB for UEFI
echo "Installing GRUB for UEFI..."
if arch-chroot $dir_root timeout 30 grub-install --target=x86_64-efi --efi-directory /boot --recheck --removable; then
    echo "UEFI GRUB installed successfully"
else
    echo "UEFI GRUB install failed or timed out (normal if not UEFI system)"
fi

# Force sync and wait for device to settle
sync
sleep 5

# Install GRUB for BIOS
echo "Installing GRUB for BIOS..."
if arch-chroot $dir_root timeout 30 grub-install --target=i386-pc $drive; then
    echo "BIOS GRUB installed successfully"
else
    echo "BIOS GRUB install failed or timed out (normal if UEFI-only system)"
fi

# Final sync before config generation
sync
sleep 3

# Generate GRUB config
echo "Generating GRUB config..."
arch-chroot $dir_root grub-mkconfig -o /boot/grub/grub.cfg

# ===========================================
# Network Configuration
# ===========================================
cp "$dir_script/10-ethernet.network" $dir_root/etc/systemd/network/
systemctl --root=$dir_root enable systemd-networkd.service
systemctl --root=$dir_root enable iwd.service
cp "$dir_script/20-wifi.network" $dir_root/etc/systemd/network/
systemctl --root=$dir_root enable systemd-resolved.service

ln -sf /run/systemd/resolve/stub-resolv.conf $dir_root/etc/resolv.conf

systemctl --root=$dir_root enable systemd-timesyncd.service

# ===========================================
# Enable LightDM Display Manager
# ===========================================
systemctl --root=$dir_root enable lightdm.service

# ===========================================
# User Configuration
# ===========================================
arch-chroot $dir_root useradd -m "$username"
arch-chroot $dir_root passwd $username
arch-chroot $dir_root groupadd -f wheel
arch-chroot $dir_root usermod -aG wheel "$username"

pacstrap $dir_root sudo
cp "$dir_script/10-sudo" $dir_root/etc/sudoers.d/

# Sudo User configuration
arch-chroot $dir_root groupadd -f sudo
arch-chroot $dir_root usermod -aG sudo "$username"

pacstrap $dir_root polkit

# ===========================================
# Microcode
# ===========================================
pacstrap $dir_root amd-ucode intel-ucode
arch-chroot $dir_root mkinitcpio -P

# ===========================================
# Predictable Network Interface Names
# ===========================================
mkdir -p $dir_root/etc/udev/rules.d
ln -sf /dev/null $dir_root/etc/udev/rules.d/80-net-setup-link.rules

# ===========================================
# Copy User Configuration Files
# ===========================================
mkdir -p $dir_root/home/$username/.config
cp $dir_script/.xinitrc $dir_root/home/$username/
cp -r $dir_script/config/* $dir_root/home/$username/.config/

# ===========================================
# Copy Wallpaper
# ===========================================
mkdir -p $dir_root/usr/share/backgrounds
cp $dir_script/wallpapers/monterey.png $dir_root/usr/share/backgrounds/

# Fix file ownership
arch-chroot $dir_root chown -R $username:$username /home/$username

# ===========================================
# Install yay (AUR Helper)
# ===========================================
echo "Installing yay (AUR Helper)"
arch-chroot $dir_root bash -c "
    cd /tmp
    sudo -u $username git clone https://aur.archlinux.org/yay.git
    cd yay
    sudo -u $username makepkg -si --noconfirm
    cd /tmp
    rm -rf yay
"

# ===========================================
# Install VS Code and Extensions
# ===========================================
# Copy VS Code setup scripts to user home
cp -r $dir_script/vscode-config $dir_root/home/$username/

# Run VS Code setup (installs VS Code, extensions, and configures)
arch-chroot $dir_root sudo -u $username bash /home/$username/vscode-config/scripts/setup.sh

# Fix ownership
arch-chroot $dir_root chown -R $username:$username /home/$username/vscode-config

# ===========================================
# Cleanup and Unmount
# ===========================================
# Disable trap before final unmount to avoid duplicate cleanup
trap - EXIT

umount -fl $dir_boot
umount -fl $dir_root

set +x
echo ""
echo "============================================"
echo "       Installation Complete!"
echo "============================================"
echo "You can now reboot into your new Arch Linux system."
echo "Username: $username"
echo "============================================"

#!/bin/bash

set -e

dir_script="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# ===========================================
# Colors
# ===========================================
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
BOLD='\033[1m'
NC='\033[0m' # No Color

################
dir_boot='/mnt/usb/boot'
dir_root='/mnt/usb'

# ===========================================
# Cleanup Function (runs on exit/error)
# ===========================================
cleanup() {
    set +x
    echo ""
    echo -e "${YELLOW}Cleaning up...${NC}"
    sync 2>/dev/null || true
    umount -fl $dir_boot 2>/dev/null || true
    umount -fl $dir_root 2>/dev/null || true
}
trap cleanup EXIT

# ===========================================
# Pre-flight Checks
# ===========================================
echo -e "${BOLD}${CYAN}============================================${NC}"
echo -e "${BOLD}${CYAN}       Arch Linux Installation Script${NC}"
echo -e "${BOLD}${CYAN}============================================${NC}"
echo ""

# Check for internet connectivity
echo -e "${BLUE}Checking internet connection...${NC}"
if ! ping -c 1 -W 5 archlinux.org &>/dev/null; then
    echo -e "${RED}Error: No internet connection detected.${NC}"
    echo -e "${RED}Please connect to the internet before running this script.${NC}"
    exit 1
fi
echo -e "${GREEN}Internet connection: OK${NC}"
echo ""

# ===========================================
# Interactive Drive Selection
# ===========================================
echo -e "${BOLD}${YELLOW}Available block devices:${NC}"
echo ""
lsblk -d -o NAME,SIZE,TYPE,MODEL
echo ""

while true; do
    read -p $'\033[1;34mEnter the target drive (e.g., /dev/sda or /dev/nvme0n1): \033[0m' drive

    if [ -z "$drive" ]; then
        echo -e "${RED}Error: Drive path cannot be empty.${NC}"
        continue
    fi

    if [ -b "$drive" ]; then
        echo -e "${GREEN}Drive '$drive' found.${NC}"
        break
    else
        echo -e "${RED}Error: Device '$drive' does not exist. Please try again.${NC}"
    fi
done

# ===========================================
# Interactive Username Selection
# ===========================================
echo ""
while true; do
    read -p $'\033[1;34mEnter the username (will also be used as hostname): \033[0m' username

    if [ -z "$username" ]; then
        echo -e "${RED}Error: Username cannot be empty.${NC}"
        continue
    fi

    # Validate username (lowercase, no spaces, alphanumeric and underscore only)
    if [[ "$username" =~ ^[a-z][a-z0-9_]*$ ]]; then
        break
    else
        echo -e "${RED}Error: Username must start with a lowercase letter and contain only lowercase letters, numbers, and underscores.${NC}"
    fi
done

# ===========================================
# Password Collection
# ===========================================
echo ""
while true; do
    read -s -p $'\033[1;34mEnter root password: \033[0m' root_password
    echo ""
    read -s -p $'\033[1;34mConfirm root password: \033[0m' root_password_confirm
    echo ""

    if [ -z "$root_password" ]; then
        echo -e "${RED}Error: Password cannot be empty.${NC}"
        continue
    fi

    if [ "$root_password" = "$root_password_confirm" ]; then
        break
    else
        echo -e "${RED}Error: Passwords do not match. Please try again.${NC}"
    fi
done

while true; do
    read -s -p $'\033[1;34mEnter password for '"$username"$': \033[0m' user_password
    echo ""
    read -s -p $'\033[1;34mConfirm password for '"$username"$': \033[0m' user_password_confirm
    echo ""

    if [ -z "$user_password" ]; then
        echo -e "${RED}Error: Password cannot be empty.${NC}"
        continue
    fi

    if [ "$user_password" = "$user_password_confirm" ]; then
        break
    else
        echo -e "${RED}Error: Passwords do not match. Please try again.${NC}"
    fi
done

# ===========================================
# Confirmation
# ===========================================
echo ""
echo -e "${BOLD}${CYAN}============================================${NC}"
echo -e "${BOLD}${CYAN}            Installation Summary${NC}"
echo -e "${BOLD}${CYAN}============================================${NC}"
echo -e "${BOLD}Target drive:${NC}    $drive"
echo -e "${BOLD}Username:${NC}        $username"
echo -e "${BOLD}Hostname:${NC}        $username"
echo -e "${BOLD}${CYAN}============================================${NC}"
echo ""
echo -e "${BOLD}${RED}WARNING: This will ERASE ALL DATA on $drive!${NC}"
echo ""

while true; do
    read -p $'\033[1;33mAre you sure you want to continue? (Y/N): \033[0m' confirm
    case "$confirm" in
        [Yy])
            echo -e "${GREEN}Starting installation...${NC}"
            break
            ;;
        [Nn])
            echo -e "${YELLOW}Installation cancelled.${NC}"
            exit 0
            ;;
        *)
            echo -e "${RED}Please enter Y or N.${NC}"
            ;;
    esac
done

# ===========================================
# Verify required files exist
# ===========================================
missing_files=()
[ ! -f "$dir_script/locale.gen" ] && missing_files+=("locale.gen")
[ ! -f "$dir_script/10-ethernet.network" ] && missing_files+=("10-ethernet.network")
[ ! -f "$dir_script/20-wifi.network" ] && missing_files+=("20-wifi.network")
[ ! -f "$dir_script/10-sudo" ] && missing_files+=("10-sudo")
[ ! -f "$dir_script/.xinitrc" ] && missing_files+=(".xinitrc")
[ ! -d "$dir_script/config" ] && missing_files+=("config/")
[ ! -f "$dir_script/wallpapers/monterey.png" ] && missing_files+=("wallpapers/monterey.png")
[ ! -d "$dir_script/vscode-config" ] && missing_files+=("vscode-config/")
[ ! -f "$dir_script/vscode-config/config/settings.json" ] && missing_files+=("vscode-config/config/settings.json")
[ ! -f "$dir_script/vscode-config/config/keybindings.json" ] && missing_files+=("vscode-config/config/keybindings.json")
[ ! -f "$dir_script/verify-install.sh" ] && missing_files+=("verify-install.sh")

if [ ${#missing_files[@]} -gt 0 ]; then
    echo -e "${RED}Error: Missing required files:${NC}"
    for f in "${missing_files[@]}"; do
        echo -e "${RED}  - $f${NC}"
    done
    exit 1
fi
echo -e "${GREEN}All required files found.${NC}"

# Enable verbose mode after confirmation and log to file
LOGFILE="$dir_script/install-$(date +%Y%m%d-%H%M%S).log"
echo "Logging to: $LOGFILE"
exec > >(tee -a "$LOGFILE") 2>&1
set -x

# ===========================================
# Reduce write buffering for USB drives
# Prevents long sync hangs by forcing frequent smaller flushes
# ===========================================
echo 100000000 > /proc/sys/vm/dirty_bytes
echo 50000000 > /proc/sys/vm/dirty_background_bytes

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
parted $drive --script mkpart primary ext4 511MiB 100%

# FAT32 for Partition 2
mkfs.fat -F32 $drive_part2

# ext4 For Partition 3
mkfs.ext4 -F $drive_part3

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
pacstrap $dir_root linux linux-firmware base neovim iwd git base-devel go \
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
    nload iotop \
    usbutils \
    nano

# Flush writes after large pacstrap
sync

genfstab -U $dir_root > $dir_root/etc/fstab

# ===========================================
# Locale Configuration
# ===========================================
cp "$dir_script/locale.gen" $dir_root/etc/
echo LANG=en_US.UTF-8 > $dir_root/etc/locale.conf
arch-chroot $dir_root locale-gen

# ===========================================
# Console Configuration (required for mkinitcpio sd-vconsole hook)
# ===========================================
cat > $dir_root/etc/vconsole.conf << EOF
KEYMAP=us
EOF

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

echo "root:$root_password" | arch-chroot $dir_root chpasswd

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
# Setup /etc/skel (skeleton for new users)
# Files here are copied to home directory when user is created
# ===========================================
mkdir -p $dir_root/etc/skel/.config
cp $dir_script/.xinitrc $dir_root/etc/skel/
cp -r $dir_script/config/* $dir_root/etc/skel/.config/

# VS Code config (pre-create directory structure)
mkdir -p $dir_root/etc/skel/.config/Code/User
cp $dir_script/vscode-config/config/settings.json $dir_root/etc/skel/.config/Code/User/
cp $dir_script/vscode-config/config/keybindings.json $dir_root/etc/skel/.config/Code/User/

# VS Code setup scripts (user can run manually for extensions)
cp -r $dir_script/vscode-config $dir_root/etc/skel/

echo "Skeleton directory contents:"
find $dir_root/etc/skel -type f
echo "Verifying skel files..."
ls -la $dir_root/etc/skel/.config/xfce4/terminal/terminalrc || echo "WARNING: terminalrc not in skel"
ls -la $dir_root/etc/skel/.config/Code/User/settings.json || echo "WARNING: VS Code settings not in skel"
sync

# ===========================================
# User Configuration
# ===========================================
arch-chroot $dir_root useradd -m "$username"
echo "Verifying user home was created with skel contents..."
ls -la $dir_root/home/$username/
ls -la $dir_root/home/$username/.config/ || echo "WARNING: .config not copied from skel"
echo "$username:$user_password" | arch-chroot $dir_root chpasswd
arch-chroot $dir_root groupadd -f wheel
arch-chroot $dir_root usermod -aG wheel "$username"

pacstrap $dir_root sudo
cp "$dir_script/10-sudo" $dir_root/etc/sudoers.d/
chmod 440 $dir_root/etc/sudoers.d/10-sudo
echo "Sudoers file:"
ls -la $dir_root/etc/sudoers.d/10-sudo
sync

# Sudo User configuration
arch-chroot $dir_root groupadd -f sudo
arch-chroot $dir_root usermod -aG sudo "$username"

pacstrap $dir_root polkit

# ===========================================
# Microcode
# ===========================================
pacstrap $dir_root amd-ucode intel-ucode
arch-chroot $dir_root mkinitcpio -P

# Regenerate GRUB config to include microcode
arch-chroot $dir_root grub-mkconfig -o /boot/grub/grub.cfg

# ===========================================
# Predictable Network Interface Names
# ===========================================
mkdir -p $dir_root/etc/udev/rules.d
ln -sf /dev/null $dir_root/etc/udev/rules.d/80-net-setup-link.rules

# ===========================================
# Copy Wallpaper (system-wide default)
# ===========================================
# Copy as xfce-x.svg to override XFCE's hardcoded default wallpaper
mkdir -p $dir_root/usr/share/backgrounds/xfce
cp $dir_script/wallpapers/monterey.png $dir_root/usr/share/backgrounds/xfce/xfce-x.svg
echo "Wallpaper copied as default:"
ls -la $dir_root/usr/share/backgrounds/xfce/xfce-x.svg
sync

# ===========================================
# Install yay (AUR Helper)
# ===========================================
echo "Installing yay (AUR Helper)"
arch-chroot $dir_root bash -c "
    set -e
    cd /tmp
    rm -rf yay
    sudo -u $username git clone https://aur.archlinux.org/yay.git
    cd yay
    sudo -u $username makepkg --noconfirm
    pacman -U --noconfirm yay-*.pkg.tar.zst
    cd /tmp
    rm -rf yay
" || echo "WARNING: yay installation failed"

# Verify yay installed
if arch-chroot $dir_root which yay; then
    echo "yay installed successfully"
else
    echo "ERROR: yay was NOT installed"
fi
sync

# ===========================================
# Install VS Code (using user's setup script)
# ===========================================
echo "Installing VS Code..."
arch-chroot $dir_root sudo -u $username /home/$username/vscode-config/scripts/setup.sh || true
# Note: Extensions installation will fail in chroot (no display) - user can re-run after login
echo "VS Code installed:"
arch-chroot $dir_root pacman -Qi code | head -3
sync

# Copy verification script
cp $dir_script/verify-install.sh $dir_root/home/$username/
chmod +x $dir_root/home/$username/verify-install.sh

# Fix ownership for all user files (including VS Code config created during setup)
arch-chroot $dir_root chown -R $username:$username /home/$username

# ===========================================
# Final Verification Before Unmount
# ===========================================
echo ""
echo "=== FINAL VERIFICATION ==="
echo "Checking critical files exist before unmount..."

verify_fail=0

# Check wallpaper
if [ -f "$dir_root/usr/share/backgrounds/monterey.png" ]; then
    echo "[OK] Wallpaper exists"
else
    echo "[FAIL] Wallpaper MISSING"
    verify_fail=1
fi

# Check sudoers
if [ -f "$dir_root/etc/sudoers.d/10-sudo" ]; then
    echo "[OK] Sudoers file exists"
else
    echo "[FAIL] Sudoers file MISSING"
    verify_fail=1
fi

# Check user home
if [ -d "$dir_root/home/$username/.config" ]; then
    echo "[OK] User .config exists"
    ls -la $dir_root/home/$username/.config/
else
    echo "[FAIL] User .config MISSING"
    verify_fail=1
fi

# Check VS Code config
if [ -f "$dir_root/home/$username/.config/Code/User/settings.json" ]; then
    echo "[OK] VS Code settings exist"
else
    echo "[FAIL] VS Code settings MISSING"
    verify_fail=1
fi

# Check XFCE terminal config
if [ -f "$dir_root/home/$username/.config/xfce4/terminal/terminalrc" ]; then
    echo "[OK] XFCE terminal config exists"
else
    echo "[FAIL] XFCE terminal config MISSING"
    verify_fail=1
fi

# Check yay
if arch-chroot $dir_root which yay &>/dev/null; then
    echo "[OK] yay is installed"
else
    echo "[FAIL] yay NOT installed"
    verify_fail=1
fi

# Check VS Code
if arch-chroot $dir_root which code &>/dev/null; then
    echo "[OK] VS Code is installed"
else
    echo "[FAIL] VS Code NOT installed"
    verify_fail=1
fi

echo "=== END VERIFICATION ==="
echo ""

if [ $verify_fail -eq 1 ]; then
    echo -e "${RED}WARNING: Some verifications failed! Check output above.${NC}"
fi

# ===========================================
# Cleanup and Unmount
# ===========================================
# Disable trap before final unmount to avoid duplicate cleanup
trap - EXIT

# Ensure all writes are flushed before unmounting
echo "Syncing all writes to disk..."
sync
sleep 2
sync

umount -fl $dir_boot
umount -fl $dir_root

set +x
echo ""
echo -e "${BOLD}${GREEN}============================================${NC}"
echo -e "${BOLD}${GREEN}       Installation Complete!${NC}"
echo -e "${BOLD}${GREEN}============================================${NC}"
echo -e "You can now reboot into your new Arch Linux system."
echo -e "${BOLD}Username:${NC} $username"
echo -e "${BOLD}${GREEN}============================================${NC}"

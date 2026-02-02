#!/bin/bash

# ===========================================
# Installation Verification Script
# Run this on the installed system to check if everything was set up correctly
# ===========================================

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
CYAN='\033[0;36m'
BOLD='\033[1m'
NC='\033[0m'

passed=0
failed=0
warnings=0

check_pass() {
    echo -e "${GREEN}[PASS]${NC} $1"
    ((passed++))
}

check_fail() {
    echo -e "${RED}[FAIL]${NC} $1"
    ((failed++))
}

check_warn() {
    echo -e "${YELLOW}[WARN]${NC} $1"
    ((warnings++))
}

echo -e "${BOLD}${CYAN}============================================${NC}"
echo -e "${BOLD}${CYAN}    Arch Linux Installation Verifier${NC}"
echo -e "${BOLD}${CYAN}============================================${NC}"
echo ""

# ===========================================
# Check Essential Packages
# ===========================================
echo -e "${BOLD}Checking installed packages...${NC}"

packages=(
    "linux"
    "linux-firmware"
    "base"
    "grub"
    "efibootmgr"
    "neovim"
    "nano"
    "iwd"
    "git"
    "base-devel"
    "xfce4-session"
    "xfce4-panel"
    "xfce4-settings"
    "xfwm4"
    "xfdesktop"
    "xfce4-terminal"
    "thunar"
    "xorg-server"
    "lightdm"
    "lightdm-gtk-greeter"
    "firefox"
    "pipewire"
    "pipewire-pulse"
    "wireplumber"
    "ttf-fira-code"
    "ttf-liberation"
    "ttf-dejavu"
    "thunar"
    "gvfs"
    "htop"
    "btop"
    "openssh"
    "tmux"
    "zsh"
    "wget"
    "curl"
    "ripgrep"
    "fd"
    "unzip"
    "zip"
    "p7zip"
    "tree"
    "rsync"
    "nload"
    "iotop"
    "usbutils"
    "sudo"
    "polkit"
    "amd-ucode"
    "intel-ucode"
    "code"
)

for pkg in "${packages[@]}"; do
    if pacman -Qi "$pkg" &>/dev/null; then
        check_pass "Package: $pkg"
    else
        check_fail "Package: $pkg (not installed)"
    fi
done

echo ""

# ===========================================
# Check AUR Helper (yay)
# ===========================================
echo -e "${BOLD}Checking AUR helper...${NC}"

if command -v yay &>/dev/null; then
    check_pass "yay is installed ($(yay --version | head -1))"
else
    check_fail "yay is NOT installed"
fi

echo ""

# ===========================================
# Check Background Image
# ===========================================
echo -e "${BOLD}Checking background image...${NC}"

if [ -f "/usr/share/backgrounds/monterey.png" ]; then
    size=$(stat -c%s "/usr/share/backgrounds/monterey.png")
    check_pass "Wallpaper exists at /usr/share/backgrounds/monterey.png (${size} bytes)"
else
    check_fail "Wallpaper missing: /usr/share/backgrounds/monterey.png"
    echo "       Available backgrounds:"
    ls -la /usr/share/backgrounds/ 2>/dev/null || echo "       Directory doesn't exist"
fi

echo ""

# ===========================================
# Check XFCE Configuration
# ===========================================
echo -e "${BOLD}Checking XFCE configuration...${NC}"

# Terminal config
if [ -f "$HOME/.config/xfce4/terminal/terminalrc" ]; then
    check_pass "Terminal config exists: ~/.config/xfce4/terminal/terminalrc"

    # Check font setting
    if grep -q "FontName=Fira Code" "$HOME/.config/xfce4/terminal/terminalrc"; then
        check_pass "Terminal font set to Fira Code"
    else
        current_font=$(grep "FontName=" "$HOME/.config/xfce4/terminal/terminalrc" 2>/dev/null || echo "not set")
        check_warn "Terminal font: $current_font (expected Fira Code)"
    fi
else
    check_fail "Terminal config missing: ~/.config/xfce4/terminal/terminalrc"
fi

# Desktop config (wallpaper setting)
if [ -f "$HOME/.config/xfce4/xfconf/xfce-perchannel-xml/xfce4-desktop.xml" ]; then
    check_pass "Desktop config exists: ~/.config/xfce4/xfconf/xfce-perchannel-xml/xfce4-desktop.xml"

    if grep -q "monterey.png" "$HOME/.config/xfce4/xfconf/xfce-perchannel-xml/xfce4-desktop.xml"; then
        check_pass "Wallpaper path configured in desktop settings"
    else
        check_warn "Wallpaper not configured in desktop settings"
    fi
else
    check_fail "Desktop config missing: ~/.config/xfce4/xfconf/xfce-perchannel-xml/xfce4-desktop.xml"
fi

# Check xfce4 config directory structure
echo ""
echo -e "${BOLD}XFCE4 config structure:${NC}"
if [ -d "$HOME/.config/xfce4" ]; then
    echo "Files in ~/.config/xfce4:"
    find "$HOME/.config/xfce4" -type f 2>/dev/null | head -20

    echo ""
    echo "Terminal config content (if exists):"
    if [ -f "$HOME/.config/xfce4/terminal/terminalrc" ]; then
        head -10 "$HOME/.config/xfce4/terminal/terminalrc"
    else
        echo "  (file does not exist)"
    fi

    echo ""
    echo "Desktop config (wallpaper lines):"
    if [ -f "$HOME/.config/xfce4/xfconf/xfce-perchannel-xml/xfce4-desktop.xml" ]; then
        grep -i "image\|backdrop\|monitor" "$HOME/.config/xfce4/xfconf/xfce-perchannel-xml/xfce4-desktop.xml" | head -10
    else
        echo "  (file does not exist)"
    fi
else
    echo -e "${RED}~/.config/xfce4 directory does not exist${NC}"
fi

echo ""
echo -e "${BOLD}Backgrounds directory:${NC}"
ls -la /usr/share/backgrounds/ 2>/dev/null || echo "Directory does not exist"

echo ""

# ===========================================
# Check VS Code
# ===========================================
echo -e "${BOLD}Checking VS Code...${NC}"

if command -v code &>/dev/null; then
    check_pass "VS Code is installed"
    code --version 2>/dev/null | head -1
else
    check_fail "VS Code is NOT installed"
fi

if [ -d "$HOME/.config/Code/User" ]; then
    check_pass "VS Code config directory exists"
    if [ -f "$HOME/.config/Code/User/settings.json" ]; then
        check_pass "VS Code settings.json exists"
    else
        check_fail "VS Code settings.json missing"
    fi
    if [ -f "$HOME/.config/Code/User/keybindings.json" ]; then
        check_pass "VS Code keybindings.json exists"
    else
        check_fail "VS Code keybindings.json missing"
    fi
else
    check_fail "VS Code config directory missing (~/.config/Code/User)"
fi

if [ -d "$HOME/vscode-config" ]; then
    check_pass "VS Code setup scripts exist (~/vscode-config)"
    echo "       Run ~/vscode-config/scripts/setup.sh to install extensions"
else
    check_warn "VS Code setup scripts missing (~/vscode-config)"
fi

echo ""

# ===========================================
# Check Services
# ===========================================
echo -e "${BOLD}Checking enabled services...${NC}"

services=(
    "lightdm.service"
    "systemd-networkd.service"
    "systemd-resolved.service"
    "iwd.service"
    "systemd-timesyncd.service"
)

for svc in "${services[@]}"; do
    if systemctl is-enabled "$svc" &>/dev/null; then
        check_pass "Service enabled: $svc"
    else
        check_fail "Service NOT enabled: $svc"
    fi
done

echo ""

# ===========================================
# Check User Configuration
# ===========================================
echo -e "${BOLD}Checking user configuration...${NC}"

# Check user groups
groups=$(groups 2>/dev/null)
if echo "$groups" | grep -q "wheel"; then
    check_pass "User is in wheel group"
else
    check_fail "User is NOT in wheel group"
fi

if echo "$groups" | grep -q "sudo"; then
    check_pass "User is in sudo group"
else
    check_warn "User is NOT in sudo group"
fi

# Check sudoers
if [ -f "/etc/sudoers.d/10-sudo" ]; then
    perms=$(stat -c%a /etc/sudoers.d/10-sudo)
    if [ "$perms" = "440" ]; then
        check_pass "Sudoers file exists with correct permissions (440)"
    else
        check_warn "Sudoers file exists but permissions are $perms (expected 440)"
    fi
else
    check_fail "Sudoers file missing: /etc/sudoers.d/10-sudo"
fi

# Check .xinitrc
if [ -f "$HOME/.xinitrc" ]; then
    check_pass ".xinitrc exists"
else
    check_warn ".xinitrc missing (not needed with LightDM)"
fi

echo ""

# ===========================================
# Check Network Configuration
# ===========================================
echo -e "${BOLD}Checking network configuration...${NC}"

if [ -f "/etc/systemd/network/10-ethernet.network" ]; then
    check_pass "Ethernet network config exists"
else
    check_fail "Ethernet network config missing"
fi

if [ -f "/etc/systemd/network/20-wifi.network" ]; then
    check_pass "WiFi network config exists"
else
    check_fail "WiFi network config missing"
fi

if [ -L "/etc/resolv.conf" ]; then
    target=$(readlink /etc/resolv.conf)
    if [[ "$target" == *"systemd"* ]]; then
        check_pass "resolv.conf symlinked to systemd-resolved"
    else
        check_warn "resolv.conf symlinked to: $target"
    fi
else
    check_warn "resolv.conf is not a symlink"
fi

echo ""

# ===========================================
# Check Boot Configuration
# ===========================================
echo -e "${BOLD}Checking boot configuration...${NC}"

if [ -f "/boot/grub/grub.cfg" ]; then
    check_pass "GRUB config exists"

    # Check for microcode
    if grep -q "ucode" /boot/grub/grub.cfg; then
        check_pass "Microcode detected in GRUB config"
    else
        check_warn "Microcode NOT found in GRUB config"
    fi
else
    check_fail "GRUB config missing"
fi

echo ""

# ===========================================
# Summary
# ===========================================
echo -e "${BOLD}${CYAN}============================================${NC}"
echo -e "${BOLD}${CYAN}                Summary${NC}"
echo -e "${BOLD}${CYAN}============================================${NC}"
echo -e "${GREEN}Passed:${NC}   $passed"
echo -e "${RED}Failed:${NC}   $failed"
echo -e "${YELLOW}Warnings:${NC} $warnings"
echo -e "${BOLD}${CYAN}============================================${NC}"

if [ $failed -eq 0 ]; then
    echo -e "${GREEN}${BOLD}Installation appears complete!${NC}"
    exit 0
else
    echo -e "${RED}${BOLD}Some checks failed. Review the output above.${NC}"
    exit 1
fi

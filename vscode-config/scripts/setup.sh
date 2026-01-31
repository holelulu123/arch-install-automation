#!/bin/bash

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
CONFIG_DIR="$(dirname "$SCRIPT_DIR")/config"
VSCODE_USER_DIR="$HOME/.config/Code/User"

echo -e "${GREEN}VS Code Setup Script${NC}"
echo "======================================"

# Function to check if running on Arch Linux
check_arch() {
    if [ ! -f /etc/arch-release ]; then
        echo -e "${YELLOW}Warning: This script is designed for Arch Linux${NC}"
        read -p "Continue anyway? (y/N): " -n 1 -r
        echo
        if [[ ! $REPLY =~ ^[Yy]$ ]]; then
            exit 1
        fi
    fi
}

# Function to install VS Code
install_vscode() {
    if command -v code &> /dev/null; then
        echo -e "${GREEN}✓${NC} VS Code is already installed"
        code --version
    else
        echo -e "${YELLOW}Installing VS Code...${NC}"
        sudo pacman -S --needed --noconfirm code
        echo -e "${GREEN}✓${NC} VS Code installed"
    fi
}

# Function to install extensions
install_extensions() {
    if [ ! -f "$CONFIG_DIR/extensions.txt" ]; then
        echo -e "${YELLOW}No extensions.txt found, skipping extension installation${NC}"
        return
    fi

    echo -e "${YELLOW}Installing VS Code extensions...${NC}"
    while IFS= read -r extension; do
        if [ -n "$extension" ]; then
            echo "  Installing $extension..."
            code --install-extension "$extension" --force
        fi
    done < "$CONFIG_DIR/extensions.txt"
    echo -e "${GREEN}✓${NC} Extensions installed"
}

# Function to setup configuration files
setup_config() {
    echo -e "${YELLOW}Setting up VS Code configuration...${NC}"

    # Create VS Code user directory if it doesn't exist
    mkdir -p "$VSCODE_USER_DIR"

    # Backup existing configs if they exist
    if [ -f "$VSCODE_USER_DIR/settings.json" ]; then
        echo "  Backing up existing settings.json..."
        cp "$VSCODE_USER_DIR/settings.json" "$VSCODE_USER_DIR/settings.json.backup.$(date +%Y%m%d_%H%M%S)"
    fi

    if [ -f "$VSCODE_USER_DIR/keybindings.json" ]; then
        echo "  Backing up existing keybindings.json..."
        cp "$VSCODE_USER_DIR/keybindings.json" "$VSCODE_USER_DIR/keybindings.json.backup.$(date +%Y%m%d_%H%M%S)"
    fi

    # Copy or symlink config files
    if [ "$1" == "--symlink" ]; then
        echo "  Creating symlinks..."
        ln -sf "$CONFIG_DIR/settings.json" "$VSCODE_USER_DIR/settings.json"
        ln -sf "$CONFIG_DIR/keybindings.json" "$VSCODE_USER_DIR/keybindings.json"
        echo -e "${GREEN}✓${NC} Configuration symlinked"
    else
        echo "  Copying configuration files..."
        cp "$CONFIG_DIR/settings.json" "$VSCODE_USER_DIR/settings.json"
        cp "$CONFIG_DIR/keybindings.json" "$VSCODE_USER_DIR/keybindings.json"
        echo -e "${GREEN}✓${NC} Configuration copied"
    fi
}

# Main execution
main() {
    echo
    check_arch
    echo
    install_vscode
    echo
    install_extensions
    echo
    setup_config "$@"
    echo
    echo -e "${GREEN}======================================"
    echo -e "Setup complete!${NC}"
    echo
    echo "To keep configs in sync, you can:"
    echo "  1. Run this script with --symlink flag to create symlinks"
    echo "  2. Add vscode-config to a git repo for version control"
}

# Run main with all arguments
main "$@"

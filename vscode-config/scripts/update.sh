#!/bin/bash

set -e

# Colors for output
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
CONFIG_DIR="$(dirname "$SCRIPT_DIR")/config"
VSCODE_USER_DIR="$HOME/.config/Code/User"

echo -e "${GREEN}VS Code Config Update Script${NC}"
echo "======================================"
echo "This will update the backup configs with your current VS Code settings"
echo

# Update settings.json
if [ -f "$VSCODE_USER_DIR/settings.json" ]; then
    echo -e "${YELLOW}Updating settings.json...${NC}"
    cp "$VSCODE_USER_DIR/settings.json" "$CONFIG_DIR/settings.json"
    echo -e "${GREEN}✓${NC} settings.json updated"
else
    echo "No settings.json found in VS Code config"
fi

# Update keybindings.json
if [ -f "$VSCODE_USER_DIR/keybindings.json" ]; then
    echo -e "${YELLOW}Updating keybindings.json...${NC}"
    cp "$VSCODE_USER_DIR/keybindings.json" "$CONFIG_DIR/keybindings.json"
    echo -e "${GREEN}✓${NC} keybindings.json updated"
else
    echo "No keybindings.json found in VS Code config"
fi

# Update extensions list
echo -e "${YELLOW}Updating extensions list...${NC}"
code --list-extensions > "$CONFIG_DIR/extensions.txt"
echo -e "${GREEN}✓${NC} extensions.txt updated"

echo
echo -e "${GREEN}Update complete!${NC}"
echo "Don't forget to commit changes if this is in a git repo"

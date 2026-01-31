# VS Code Configuration Automation

This directory contains VS Code configuration files and setup scripts for automated installation.

## Structure

```
vscode-config/
├── config/
│   ├── settings.json      # VS Code settings
│   ├── keybindings.json   # Custom keybindings
│   └── extensions.txt     # List of extensions to install
├── scripts/
│   ├── setup.sh          # Setup script for fresh installations
│   └── update.sh         # Update backup configs from current VS Code
└── README.md
```

## Usage

### Fresh Installation (New Machine)

Run the setup script to install VS Code, extensions, and configuration:

```bash
cd vscode-config/scripts
chmod +x setup.sh update.sh
./setup.sh
```

Options:
- `./setup.sh` - Copies config files to VS Code directory
- `./setup.sh --symlink` - Creates symlinks instead (keeps configs in sync)

### Update Configurations

When you make changes to VS Code settings/keybindings/extensions, update the backup:

```bash
cd vscode-config/scripts
./update.sh
```

This will copy your current VS Code configuration back to this directory.

## Version Control

To sync configurations across machines:

```bash
# Initialize git repo (if not already in one)
cd vscode-config
git init
git add .
git commit -m "Initial VS Code configuration"

# Add remote and push
git remote add origin <your-repo-url>
git push -u origin main
```

## What's Included

### Settings
- Font: Fira Code with ligatures
- Theme: Dracula Dark+
- Icon Theme: Material Icon Theme
- Minimap disabled
- Claude Code panel location preference

### Keybindings
- Shift+Alt+Up/Down for copy lines
- Ctrl+V for paste (terminal and editor)

### Extensions
See `config/extensions.txt` for the full list.

## Notes

- The setup script is designed for Arch Linux (uses pacman)
- Backup files are created automatically before overwriting
- For Code OSS, you may need to adjust paths in the scripts

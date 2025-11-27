#!/bin/bash

# Wine Development Setup Guide
# This script provides a comprehensive overview and setup for Wine development

set -e

# Get script directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Function to print colored output
print_header() {
    echo -e "${CYAN}$1${NC}"
}

print_status() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Function to find sudo/doas command
find_privilege_escalation() {
    if command -v sudo >/dev/null 2>&1; then
        # Check if passwordless sudo is available
        # sudo -n true tests for passwordless access without prompting
        if sudo -n true 2>/dev/null; then
            echo "sudo"
            return 0
        fi
        # If passwordless sudo not available, check if we're in an interactive terminal
        # In that case, sudo can prompt for password
        if [ -t 0 ] && [ -t 1 ]; then
            # Interactive terminal - sudo can prompt for password
            echo "sudo"
            return 0
        fi
        # Non-interactive and no passwordless sudo - skip sudo
    fi
    
    if command -v doas >/dev/null 2>&1; then
        echo "doas"
        return 0
    fi
    
    # Fallback to su (requires password)
    if command -v su >/dev/null 2>&1; then
        echo "su"
        return 0
    fi
    
    return 1
}

# Function to show welcome message
show_welcome() {
    print_header "Wine Development Environment Setup"
    print_header "==================================="
    echo ""
    echo "This script will help you set up a Wine development environment"
    echo "for building Wine applications and using winegcc."
    echo ""
    echo "What this setup includes:"
    echo "  • 32-bit Wine development packages"
    echo "  • winegcc compiler"
    echo "  • Required development libraries"
    echo "  • Multilib support"
    echo ""
    echo "Supported distributions:"
    echo "  • Debian/Ubuntu"
    echo "  • Fedora/RHEL/CentOS/Nobara"
    echo "  • Arch Linux/Manjaro"
    echo ""
    echo "This is the RECOMMENDED way to set up Wine development."
    echo "For advanced users, you can also use install_wine_dev.sh directly."
    echo ""
}

# Function to show current status
show_current_status() {
    print_header "Current System Status"
    print_header "====================="
    
    # Detect distribution
    if [ -f /etc/os-release ]; then
        . /etc/os-release
        echo "Distribution: $PRETTY_NAME"
    fi
    
    echo "Architecture: $(uname -m)"
    echo "Kernel: $(uname -r)"
    
    # Check for multilib support
    if [ -f /etc/debian_version ]; then
        echo "Multilib support: $(dpkg --print-foreign-architectures 2>/dev/null || echo 'Not configured')"
    fi
    
    # Check for winegcc
    if command -v winegcc >/dev/null 2>&1; then
        print_success "winegcc is already installed"
        winegcc --version
    else
        print_warning "winegcc is not installed"
    fi
    
    # Check for wine
    if command -v wine >/dev/null 2>&1; then
        print_success "wine is installed"
        wine --version
    else
        print_warning "wine is not installed"
    fi
    
    echo ""
}

# Function to show installation options
show_installation_options() {
    print_header "Installation Options"
    print_header "===================="
    echo ""
    echo "1. Automatic Installation (Recommended)"
    echo "   Runs the installer script with default settings"
    echo ""
    echo "2. Manual Installation"
    echo "   Shows you the commands to run manually"
    echo ""
    echo "3. Test Current Installation"
    echo "   Runs tests to verify if everything is working"
    echo ""
    echo "4. Show Help and Documentation"
    echo "   Displays detailed information about the setup"
    echo ""
    echo "5. Exit"
    echo ""
    echo "Note: After options 2, 3, or 4, you can return to this menu."
    echo ""
}

# Function to run automatic installation
run_automatic_installation() {
    print_header "Running Automatic Installation"
    print_header "=============================="
    echo ""
    
    local installer_script="${SCRIPT_DIR}/install_wine_dev.sh"
    
    if [ ! -f "$installer_script" ]; then
        print_error "Installer script not found: $installer_script"
        print_status "Please make sure install_wine_dev.sh exists in the script directory"
        return 1
    fi
    
    print_status "Found installer script, running it..."
    
    # Find privilege escalation command
    local priv_cmd
    if priv_cmd=$(find_privilege_escalation); then
        if [ "$priv_cmd" = "su" ]; then
            print_warning "Using 'su' - you may be prompted for root password"
            # Use double quotes to allow variable expansion and protect the path
            # The variable will expand before being passed to su -c
            if ! su root -c "bash \"$installer_script\""; then
                print_error "Installation failed"
                return 1
            fi
        elif [ "$priv_cmd" = "sudo" ]; then
            # Check if passwordless sudo is available
            if ! sudo -n true 2>/dev/null; then
                print_warning "Using 'sudo' - you may be prompted for your password"
            fi
            # Explicitly invoke bash to ensure script runs even without execute permissions
            if ! $priv_cmd bash "$installer_script"; then
                print_error "Installation failed"
                return 1
            fi
        else
            # Explicitly invoke bash to ensure script runs even without execute permissions
            if ! $priv_cmd bash "$installer_script"; then
                print_error "Installation failed"
                return 1
            fi
        fi
    else
        print_error "No privilege escalation command found (sudo/doas/su)"
        print_status "Please install sudo, doas, or run this script as root"
        return 1
    fi
    
    print_status "If you are on Debian/Ubuntu and only have winegcc-stable, wineg++-stable, or wrc-stable, the script will now automatically create the necessary symlinks for you."
    return 0
}

# Function to show manual installation
show_manual_installation() {
    print_header "Manual Installation Instructions"
    print_header "==============================="
    echo ""
    
    if [ -f /etc/os-release ]; then
        . /etc/os-release
        case $ID in
            debian|ubuntu)
                echo "For Debian/Ubuntu:"
                echo "1. Add 32-bit architecture support:"
                echo "   sudo dpkg --add-architecture i386"
                echo ""
                echo "2. Update package lists:"
                echo "   sudo apt update"
                echo ""
                echo "3. Install Wine development packages:"
                echo "   sudo apt install wine wine32 wine64 libwine libwine:i386 libwine:amd64 libwine-dev libwine-dev:i386 libwine-dev:amd64 wine-binfmt fonts-wine wine64-tools wine32-tools"
                echo ""
                echo "4. Install development tools:"
                echo "   sudo apt install build-essential gcc-multilib g++-multilib libc6-dev:i386 libc6-dev:amd64"
                ;;
            fedora|rhel|centos|nobara)
                echo "For Fedora/RHEL/CentOS/Nobara:"
                echo "1. Update package lists:"
                echo "   sudo dnf update"
                echo ""
                echo "2. Install Wine development packages:"
                echo "   sudo dnf install wine wine-devel wine-devel.i686 wine-devel.x86_64"
                echo ""
                echo "3. Install development tools:"
                echo "   sudo dnf install gcc gcc-c++ glibc-devel glibc-devel.i686 glibc-devel.x86_64"
                ;;
            arch|manjaro)
                echo "For Arch Linux/Manjaro:"
                echo "1. Update package lists:"
                echo "   sudo pacman -Sy"
                echo ""
                echo "2. Install Wine development packages:"
                echo "   sudo pacman -S wine wine-mono wine-gecko lib32-wine"
                echo ""
                echo "3. Install development tools:"
                echo "   sudo pacman -S base-devel gcc gcc-multilib lib32-glibc glibc"
                ;;
            *)
                echo "Unsupported distribution: $ID"
                ;;
        esac
    fi
    
    echo ""
    echo "After installation, verify with:"
    echo "  winegcc --version"
    echo "  wine --version"
    echo ""
}

# Function to run tests
run_tests() {
    print_header "Running Installation Tests"
    print_header "=========================="
    echo ""
    
    local test_script="${SCRIPT_DIR}/test_wine_dev.sh"
    
    if [ ! -f "$test_script" ]; then
        print_warning "Test script not found: $test_script"
        print_status "The test script (test_wine_dev.sh) is not available."
        print_status "You can manually verify your installation with:"
        echo ""
        echo "  winegcc --version"
        echo "  wine --version"
        echo "  winegcc -o test.exe test.c  # If you have a test.c file"
        echo ""
        return 0
    fi
    
    print_status "Found test script, running it..."
    if ! "$test_script"; then
        print_error "Tests failed or encountered errors"
        return 1
    fi
    
    return 0
}

# Function to show help and documentation
show_help() {
    print_header "Help and Documentation"
    print_header "======================"
    echo ""
    echo "Wine Development Environment"
    echo "============================"
    echo ""
    echo "What is winegcc?"
    echo "winegcc is a cross-compiler that allows you to compile Windows"
    echo "applications on Linux using the Wine libraries."
    echo ""
    echo "Why do you need it?"
    echo "• Building Wine applications"
    echo "• Cross-compiling Windows programs"
    echo "• Developing Wine-based solutions"
    echo "• Testing Windows applications on Linux"
    echo ""
    echo "Common Issues:"
    echo "• Missing 32-bit libraries"
    echo "• Incomplete Wine installation"
    echo "• Missing development packages"
    echo "• Architecture conflicts"
    echo ""
    echo "Troubleshooting:"
    echo "• Run the test script to diagnose issues"
    echo "• Check if multilib support is enabled"
    echo "• Verify all required packages are installed"
    echo "• Restart terminal after installation"
    echo ""
    echo "Useful Commands:"
    echo "• winegcc --version    - Check winegcc version"
    echo "• wine --version       - Check wine version"
    echo "• winecfg             - Wine configuration"
    echo "• wine test.exe       - Run a Windows executable"
    echo ""
    echo "Documentation:"
    echo "• README_wine_dev.md  - Detailed documentation"
    echo "• Wine Wiki: https://wiki.winehq.org/"
    echo "• Wine GitLab: https://gitlab.winehq.org/wine/wine"
    echo ""
}

# Function to get user choice
get_user_choice() {
    local choice
    read -p "Enter your choice (1-5): " choice || {
        # Handle EOF (Ctrl+D) gracefully
        # Output to stderr so it's not captured by command substitution
        echo "" >&2
        print_status "EOF detected. Exiting..." >&2
        # Return a special value that the caller can check
        echo "EOF"
        return 1
    }
    echo "$choice"
}

# Main function
main() {
    show_welcome
    show_current_status
    
    local installation_completed=0
    
    while true; do
        show_installation_options
        choice=$(get_user_choice)
        
        # Handle EOF case
        if [ "$choice" = "EOF" ]; then
            exit 0
        fi
        
        case $choice in
            1)
                if run_automatic_installation; then
                    installation_completed=1
                    echo ""
                    print_success "Setup complete! You can now use winegcc for Wine development."
                    local test_script="${SCRIPT_DIR}/test_wine_dev.sh"
                    if [ -f "$test_script" ]; then
                        print_status "Run \"$test_script\" to verify your installation."
                    else
                        print_status "You can verify your installation with: winegcc --version"
                    fi
                    echo ""
                    read -p "Press Enter to return to menu or type 'exit' to quit: " response || {
                        # Handle EOF (Ctrl+D) gracefully
                        echo ""
                        print_status "EOF detected. Exiting..."
                        exit 0
                    }
                    if [ "$response" = "exit" ]; then
                        exit 0
                    fi
                else
                    echo ""
                    read -p "Press Enter to return to menu: " response || {
                        # Handle EOF (Ctrl+D) gracefully - just continue to menu
                        echo ""
                    }
                fi
                ;;
            2)
                show_manual_installation
                echo ""
                read -p "Press Enter to return to menu: " response || {
                    # Handle EOF (Ctrl+D) gracefully - just continue to menu
                    echo ""
                }
                ;;
            3)
                run_tests
                echo ""
                read -p "Press Enter to return to menu: " response || {
                    # Handle EOF (Ctrl+D) gracefully - just continue to menu
                    echo ""
                }
                ;;
            4)
                show_help
                echo ""
                read -p "Press Enter to return to menu: " response || {
                    # Handle EOF (Ctrl+D) gracefully - just continue to menu
                    echo ""
                }
                ;;
            5)
                print_status "Exiting..."
                exit 0
                ;;
            *)
                print_error "Invalid choice. Please enter a number between 1 and 5."
                ;;
        esac
    done
}

# Run main function
main "$@" 
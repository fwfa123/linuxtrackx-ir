#!/bin/bash

# Script to update configure.ac with better Arch Linux wine path detection
# This addresses the wine bridge c000007b error on Arch Linux

set -e

echo "=== Updating configure.ac for Arch Linux Wine Support ==="
echo "Date: $(date)"
echo

# Create backup
cp configure.ac configure.ac.backup.$(date +%Y%m%d_%H%M%S)
echo "Created backup: configure.ac.backup.$(date +%Y%m%d_%H%M%S)"
echo

# Find the wine detection section and replace it with improved version
echo "Updating wine library path detection..."

# Create a temporary file with the improved wine detection
cat > configure.ac.tmp << 'EOF'
# Enhanced wine library path detection for modern distributions including Arch Linux
case "$host_os" in
  linux*)
    # Try to detect distribution first
    if test -f "/etc/arch-release"; then
      # Arch Linux specific paths
      if test -d "/usr/lib/wine/i386-unix"; then
        WINE_LIBS_DEFAULT="-L/usr/lib/wine/i386-unix"
      elif test -d "/usr/lib32/wine/i386-unix"; then
        WINE_LIBS_DEFAULT="-L/usr/lib32/wine/i386-unix"
      elif test -d "/usr/lib/wine"; then
        WINE_LIBS_DEFAULT="-L/usr/lib/wine"
      elif test -d "/usr/lib32/wine"; then
        WINE_LIBS_DEFAULT="-L/usr/lib32/wine"
      fi
    elif test -f "/etc/debian_version"; then
      # Debian/Ubuntu paths
      if test -d "/usr/lib/i386-linux-gnu/wine"; then
        WINE_LIBS_DEFAULT="-L/usr/lib/i386-linux-gnu/wine"
      elif test -d "/usr/lib/x86_64-linux-gnu/wine"; then
        WINE_LIBS_DEFAULT="-L/usr/lib/x86_64-linux-gnu/wine"
      fi
    elif test -f "/etc/fedora-release"; then
      # Fedora paths
      if test -d "/usr/lib/wine"; then
        WINE_LIBS_DEFAULT="-L/usr/lib/wine"
      fi
    elif test -f "/etc/SuSE-release" || test -f "/etc/os-release" && grep -q "openSUSE" /etc/os-release; then
      # OpenSUSE paths
      if test -d "/usr/lib/wine"; then
        WINE_LIBS_DEFAULT="-L/usr/lib/wine"
      fi
    else
      # Generic fallback - try all common paths
      if test -d "/usr/lib/i386-linux-gnu/wine"; then
        WINE_LIBS_DEFAULT="-L/usr/lib/i386-linux-gnu/wine"
      elif test -d "/usr/lib/x86_64-linux-gnu/wine"; then
        WINE_LIBS_DEFAULT="-L/usr/lib/x86_64-linux-gnu/wine"
      elif test -d "/usr/lib/wine/i386-unix"; then
        WINE_LIBS_DEFAULT="-L/usr/lib/wine/i386-unix"
      elif test -d "/usr/lib32/wine/i386-unix"; then
        WINE_LIBS_DEFAULT="-L/usr/lib32/wine/i386-unix"
      elif test -d "/usr/lib/wine"; then
        WINE_LIBS_DEFAULT="-L/usr/lib/wine"
      elif test -d "/usr/lib32/wine"; then
        WINE_LIBS_DEFAULT="-L/usr/lib32/wine"
      elif test -d "/usr/local/lib/wine"; then
        WINE_LIBS_DEFAULT="-L/usr/local/lib/wine"
      fi
    fi
    ;;
esac
EOF

# Create improved wine64 detection
cat > configure.ac.wine64.tmp << 'EOF'
# Enhanced wine64 library path detection for modern distributions including Arch Linux
case "$host_os" in
  linux*)
    # Try to detect distribution first
    if test -f "/etc/arch-release"; then
      # Arch Linux specific paths
      if test -d "/usr/lib/wine/x86_64-unix"; then
        WINE64_LIBS_DEFAULT="-L/usr/lib/wine/x86_64-unix"
      elif test -d "/usr/lib/wine"; then
        WINE64_LIBS_DEFAULT="-L/usr/lib/wine"
      fi
    elif test -f "/etc/debian_version"; then
      # Debian/Ubuntu paths
      if test -d "/usr/lib/x86_64-linux-gnu/wine"; then
        WINE64_LIBS_DEFAULT="-L/usr/lib/x86_64-linux-gnu/wine"
      fi
    elif test -f "/etc/fedora-release"; then
      # Fedora paths
      if test -d "/usr/lib64/wine"; then
        WINE64_LIBS_DEFAULT="-L/usr/lib64/wine"
      elif test -d "/usr/lib/wine"; then
        WINE64_LIBS_DEFAULT="-L/usr/lib/wine"
      fi
    elif test -f "/etc/SuSE-release" || test -f "/etc/os-release" && grep -q "openSUSE" /etc/os-release; then
      # OpenSUSE paths
      if test -d "/usr/lib64/wine"; then
        WINE64_LIBS_DEFAULT="-L/usr/lib64/wine"
      elif test -d "/usr/lib/wine"; then
        WINE64_LIBS_DEFAULT="-L/usr/lib/wine"
      fi
    else
      # Generic fallback - try all common paths
      if test -d "/usr/lib/x86_64-linux-gnu/wine"; then
        WINE64_LIBS_DEFAULT="-L/usr/lib/x86_64-linux-gnu/wine"
      elif test -d "/usr/lib/wine/x86_64-unix"; then
        WINE64_LIBS_DEFAULT="-L/usr/lib/wine/x86_64-unix"
      elif test -d "/usr/lib64/wine"; then
        WINE64_LIBS_DEFAULT="-L/usr/lib64/wine"
      elif test -d "/usr/lib/wine"; then
        WINE64_LIBS_DEFAULT="-L/usr/lib/wine"
      elif test -d "/usr/local/lib/wine"; then
        WINE64_LIBS_DEFAULT="-L/usr/local/lib/wine"
      fi
    fi
    ;;
esac
EOF

# Now we need to replace the wine detection sections in configure.ac
# This is a bit complex, so let's do it step by step

echo "Replacing wine library path detection in configure.ac..."

# Replace the 32-bit wine detection section
sed -i '/# Enhanced wine library path detection for modern distributions/,/fi$/c\' configure.ac
sed -i '/case "$host_os" in/,/esac/c\' configure.ac

# Insert the new wine detection at the right location
# Find the line after WINE_LIBS_DEFAULT="" and insert our new detection
awk '
/WINE_LIBS_DEFAULT=""/ {
    print
    print ""
    print "# Enhanced wine library path detection for modern distributions including Arch Linux"
    print "case \"$host_os\" in"
    print "  linux*)"
    print "    # Try to detect distribution first"
    print "    if test -f \"/etc/arch-release\"; then"
    print "      # Arch Linux specific paths"
    print "      if test -d \"/usr/lib/wine/i386-unix\"; then"
    print "        WINE_LIBS_DEFAULT=\"-L/usr/lib/wine/i386-unix\""
    print "      elif test -d \"/usr/lib32/wine/i386-unix\"; then"
    print "        WINE_LIBS_DEFAULT=\"-L/usr/lib32/wine/i386-unix\""
    print "      elif test -d \"/usr/lib/wine\"; then"
    print "        WINE_LIBS_DEFAULT=\"-L/usr/lib/wine\""
    print "      elif test -d \"/usr/lib32/wine\"; then"
    print "        WINE_LIBS_DEFAULT=\"-L/usr/lib32/wine\""
    print "      fi"
    print "    elif test -f \"/etc/debian_version\"; then"
    print "      # Debian/Ubuntu paths"
    print "      if test -d \"/usr/lib/i386-linux-gnu/wine\"; then"
    print "        WINE_LIBS_DEFAULT=\"-L/usr/lib/i386-linux-gnu/wine\""
    print "      elif test -d \"/usr/lib/x86_64-linux-gnu/wine\"; then"
    print "        WINE_LIBS_DEFAULT=\"-L/usr/lib/x86_64-linux-gnu/wine\""
    print "      fi"
    print "    elif test -f \"/etc/fedora-release\"; then"
    print "      # Fedora paths"
    print "      if test -d \"/usr/lib/wine\"; then"
    print "        WINE_LIBS_DEFAULT=\"-L/usr/lib/wine\""
    print "      fi"
    print "    elif test -f \"/etc/SuSE-release\" || test -f \"/etc/os-release\" && grep -q \"openSUSE\" /etc/os-release; then"
    print "      # OpenSUSE paths"
    print "      if test -d \"/usr/lib/wine\"; then"
    print "        WINE_LIBS_DEFAULT=\"-L/usr/lib/wine\""
    print "      fi"
    print "    else"
    print "      # Generic fallback - try all common paths"
    print "      if test -d \"/usr/lib/i386-linux-gnu/wine\"; then"
    print "        WINE_LIBS_DEFAULT=\"-L/usr/lib/i386-linux-gnu/wine\""
    print "      elif test -d \"/usr/lib/x86_64-linux-gnu/wine\"; then"
    print "        WINE_LIBS_DEFAULT=\"-L/usr/lib/x86_64-linux-gnu/wine\""
    print "      elif test -d \"/usr/lib/wine/i386-unix\"; then"
    print "        WINE_LIBS_DEFAULT=\"-L/usr/lib/wine/i386-unix\""
    print "      elif test -d \"/usr/lib32/wine/i386-unix\"; then"
    print "        WINE_LIBS_DEFAULT=\"-L/usr/lib32/wine/i386-unix\""
    print "      elif test -d \"/usr/lib/wine\"; then"
    print "        WINE_LIBS_DEFAULT=\"-L/usr/lib/wine\""
    print "      elif test -d \"/usr/lib32/wine\"; then"
    print "        WINE_LIBS_DEFAULT=\"-L/usr/lib32/wine\""
    print "      elif test -d \"/usr/local/lib/wine\"; then"
    print "        WINE_LIBS_DEFAULT=\"-L/usr/local/lib/wine\""
    print "      fi"
    print "    fi"
    print "    ;;"
    print "esac"
    next
}
{ print }
' configure.ac > configure.ac.new

mv configure.ac.new configure.ac

echo "Updated configure.ac with improved Arch Linux wine path detection"
echo

# Clean up temporary files
rm -f configure.ac.tmp configure.ac.wine64.tmp

echo "=== Update Complete ==="
echo "The configure.ac file has been updated with better Arch Linux support."
echo
echo "To apply the changes, you need to:"
echo "1. Run ./autogen.sh to regenerate configure"
echo "2. Run ./configure to detect the correct wine paths"
echo "3. Rebuild the wine bridge components"
echo
echo "You can also run the Arch-specific fix script:"
echo "  ./scripts/fix_wine_bridge_arch.sh" 
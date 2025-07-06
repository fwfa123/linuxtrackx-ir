#!/bin/bash
# wine_check.sh - Check Wine availability and provide TrackIR compatibility recommendations

echo "=== LinuxTrack Wine Compatibility Check ==="
echo

# Function to check if a command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Function to get Wine version
get_wine_version() {
    local wine_cmd="$1"
    if command_exists "$wine_cmd"; then
        version=$("$wine_cmd" --version 2>/dev/null | head -1)
        echo "$version"
    else
        echo "Not found"
    fi
}

# Check for different Wine versions
echo "Checking Wine installations..."

# Check Wine Staging (recommended)
wine_staging_version=$(get_wine_version "/opt/wine-staging/bin/wine")
if [[ "$wine_staging_version" != "Not found" ]]; then
    echo "✓ Wine Staging: $wine_staging_version (RECOMMENDED for TrackIR)"
    wine_staging_available=true
else
    echo "✗ Wine Staging: Not installed"
    wine_staging_available=false
fi

# Check system Wine
system_wine_version=$(get_wine_version "wine")
if [[ "$system_wine_version" != "Not found" ]]; then
    echo "✓ System Wine: $system_wine_version"
    system_wine_available=true
else
    echo "✗ System Wine: Not installed"
    system_wine_available=false
fi

# Check Wine Stable specifically
wine_stable_version=$(get_wine_version "wine-stable")
if [[ "$wine_stable_version" != "Not found" ]]; then
    echo "✓ Wine Stable: $wine_stable_version"
    wine_stable_available=true
else
    echo "✗ Wine Stable: Not installed"
    wine_stable_available=false
fi

echo

# Check current Wine selection
echo "Current Wine configuration:"
if command_exists update-alternatives; then
    current_wine=$(update-alternatives --query wine 2>/dev/null | grep "Value:" | cut -d' ' -f2)
    if [[ -n "$current_wine" ]]; then
        echo "  Active Wine: $current_wine"
        if [[ "$current_wine" == *"wine-staging"* ]]; then
            echo "  ✓ Using Wine Staging (Best for TrackIR)"
        elif [[ "$current_wine" == *"wine-stable"* ]]; then
            echo "  ⚠ Using Wine Stable (May have TrackIR issues)"
        else
            echo "  ? Using unknown Wine version"
        fi
    else
        echo "  No alternatives configured"
    fi
else
    echo "  update-alternatives not available"
fi

echo

# Provide recommendations
echo "=== Recommendations ==="

if [[ "$wine_staging_available" == true ]]; then
    echo "✓ Wine Staging is available - This is the best option for TrackIR compatibility"
    if [[ "$current_wine" != *"wine-staging"* ]]; then
        echo "  To switch to Wine Staging:"
        echo "    sudo update-alternatives --config wine"
        echo "    (Select Wine Staging from the menu)"
    fi
elif [[ "$system_wine_available" == true ]]; then
    echo "✓ System Wine is available - This should work for most cases"
    echo "  If you encounter TrackIR firmware extraction issues, consider installing Wine Staging"
elif [[ "$wine_stable_available" == true ]]; then
    echo "✓ Wine Stable is available - This may work but could have TrackIR issues"
    echo "  Consider installing Wine Staging for better TrackIR compatibility"
else
    echo "✗ No Wine installation found"
    echo "  Please install Wine to use TrackIR firmware extraction:"
    echo "    sudo apt install wine-staging  # Recommended"
    echo "    sudo apt install wine          # Alternative"
fi

echo

# Check for 32-bit Wine components
echo "Checking 32-bit Wine components..."
if command_exists wine32; then
    echo "✓ Wine32 components available"
elif command_exists wine; then
    # Try to check if 32-bit support is available
    if wine --version 2>&1 | grep -q "wine32"; then
        echo "✓ 32-bit Wine support detected"
    else
        echo "⚠ 32-bit Wine components may be missing"
        echo "  Install with: sudo apt install wine32:i386"
    fi
else
    echo "✗ Cannot check 32-bit support (no Wine installed)"
fi

echo

# TrackIR compatibility summary
echo "=== TrackIR Compatibility Summary ==="
if [[ "$wine_staging_available" == true ]] && [[ "$current_wine" == *"wine-staging"* ]]; then
    echo "🎉 EXCELLENT: Wine Staging is installed and active"
    echo "   TrackIR firmware extraction should work reliably"
elif [[ "$wine_staging_available" == true ]]; then
    echo "✅ GOOD: Wine Staging is available but not active"
    echo "   Switch to Wine Staging for best TrackIR compatibility"
elif [[ "$system_wine_available" == true ]]; then
    echo "⚠️  FAIR: System Wine is available"
    echo "   May work but could encounter TrackIR installer issues"
else
    echo "❌ POOR: No Wine installation found"
    echo "   TrackIR firmware extraction will not work"
fi

echo
echo "=== Check Complete ===" 
#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/common.sh"

print_status "Validate: auditing AppDir"

[[ -d "$APPDIR" ]] || die "AppDir not found: $APPDIR. Run prepare.sh first."

failures=0

# Ensure core binaries
for bin in usr/bin/ltr_gui; do
    if [[ ! -x "$APPDIR/$bin" ]]; then
        print_error "Missing or non-executable: $bin"
        failures=$((failures+1))
    fi
done

# Ensure Qt essentials
# Check for Qt plugins in new and legacy locations
if [[ ! -e "$APPDIR/usr/plugins/platforms/libqxcb.so" && ! -e "$APPDIR/usr/lib/qt5/plugins/platforms/libqxcb.so" ]]; then
    print_error "Qt platform plugin (xcb) missing"
    failures=$((failures+1))
fi
if ! ls "$APPDIR/usr/plugins/sqldrivers/"libqsqlite.so* "$APPDIR/usr/lib/qt5/plugins/sqldrivers/"libqsqlite.so* >/dev/null 2>&1; then
    print_warning "Qt SQLite driver missing; help system may fail"
else
    print_status "Qt SQLite driver present"
fi

# ldd audit: ensure libs resolve inside AppDir, except allowlist
allow_regex="^/(lib|usr/lib|usr/lib64)/(drm|dri|nvidia|amd|mesa|pci)"  # GPUs/drivers allowed
# Broader system stack allowances (graphics, X11, fonts, IPC). These are typically provided by host.
system_allow_patterns=(
  '^/lib/.*/libGL(dispatch|X)?\\.so\\.'
  '^/lib/.*/libOpenGL\\.so\\.'
  '^/lib/.*/libX(11|au|dmcp|render|ext|randr|fixes|cursor|inerama)?\\.so\\.'
  '^/lib/.*/libxcb(.*)?\\.so\\.'
  '^/lib/.*/libfontconfig\\.so\\.'
  '^/lib/.*/libfreetype\\.so\\.'
  '^/lib/.*/libharfbuzz\\.so\\.'
  '^/lib/.*/libgraphite2\\.so\\.'
  '^/lib/.*/libdbus-1\\.so\\.'
  '^/lib/.*/libsystemd\\.so\\.'
  '^/lib/.*/libgcrypt\\.so\\.'
  '^/lib/.*/libgpg-error\\.so\\.'
  '^/lib/.*/libuuid\\.so\\.'
  '^/lib/.*/libpng16\\.so\\.'
  '^/lib/.*/libexpat\\.so\\.'
  '^/lib/.*/lib(deflate|lz4|lzma|zstd)\\.so\\.'
  '^/lib/.*/lib(md|md4c)\\.so\\.'
  '^/lib/.*/lib(SM|ICE)\\.so\\.'
)
# Additionally allow core glibc toolchain libs to reside on host (common AppImage practice)
toolchain_allow=(
  "/lib/x86_64-linux-gnu/libc.so.6"
  "/lib/x86_64-linux-gnu/libm.so.6"
  "/lib/x86_64-linux-gnu/libpthread.so.0"
  "/lib/x86_64-linux-gnu/libdl.so.2"
  "/lib/x86_64-linux-gnu/librt.so.1"
  "/lib/x86_64-linux-gnu/libgcc_s.so.1"
  "/lib/x86_64-linux-gnu/libstdc++.so.6"
  "/lib/x86_64-linux-gnu/libresolv.so.2"
  "/lib/x86_64-linux-gnu/libz.so.1"
)

if command -v ldd >/dev/null 2>&1; then
    print_status "Running ldd audit"
    while IFS= read -r -d '' elf; do
        while IFS= read -r line; do
            so_path=$(awk '/=>/{print $3}' <<<"$line" | tr -d ' ' || true)
            [[ -z "$so_path" ]] && continue
            [[ "$so_path" = "not" ]] && continue
            if [[ "$so_path" != $APPDIR/* ]]; then
                if [[ "$so_path" =~ $allow_regex ]]; then
                    continue
                fi
                for allowed in "${toolchain_allow[@]}"; do
                    if [[ "$so_path" == "$allowed" ]]; then
                        continue 2
                    fi
                done
                # Allow broad system patterns
                for rx in "${system_allow_patterns[@]}"; do
                    if [[ "$so_path" =~ $rx ]]; then
                        continue 2
                    fi
                done
                print_error "External dep: $elf -> $so_path"
                failures=$((failures+1))
            fi
        done < <(ldd "$elf" 2>/dev/null || true)
    done < <(find "$APPDIR/usr/bin" "$APPDIR/usr/lib" "$APPDIR/usr/lib/linuxtrack" -type f \( -perm -111 -o -name "*.so*" \) -print0 2>/dev/null)
else
    print_warning "ldd not available; skipping dependency audit"
fi

if [[ $failures -gt 0 ]]; then
    die "Validation failed with $failures issue(s)"
fi

print_success "Validation passed"



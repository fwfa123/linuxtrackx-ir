#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/common.sh"

print_status "Optimize: size and content pruning (conservative)"

[[ -d "$APPDIR" ]] || die "AppDir not found: $APPDIR. Run prepare.sh first."

pushd "$APPDIR" >/dev/null
    # Strip binaries and libraries where safe
    if command -v strip >/dev/null 2>&1; then
        print_status "Stripping binaries and libraries"
        find usr/bin -type f -executable -print0 2>/dev/null | xargs -0r strip --strip-unneeded 2>/dev/null || true
        find usr/lib -name "*.so*" -type f -print0 2>/dev/null | xargs -0r strip --strip-unneeded 2>/dev/null || true
        find usr/lib/linuxtrack -name "*.so*" -type f -print0 2>/dev/null | xargs -0r strip --strip-unneeded 2>/dev/null || true
    else
        print_warning "strip not available; skipping stripping"
    fi

    # Prune docs/man (keep essentials)
    print_status "Pruning docs/man"
    find usr/share/doc -type f ! -name "README*" -delete 2>/dev/null || true
    if [[ -d usr/share/man/man1 ]]; then
        pushd usr/share/man/man1 >/dev/null
            for man in *.1; do
                [[ -f "$man" ]] || continue
                case "$man" in
                    ltr_gui.1|mickey.1) : ;; # keep
                    *) rm -f "$man" ;;
                esac
            done
        popd >/dev/null
    fi

    # Qt plugins: keep essential subsets
    if [[ -d usr/lib/qt5/plugins/platforms ]]; then
        pushd usr/lib/qt5/plugins/platforms >/dev/null
            for p in *.so; do
                [[ -f "$p" ]] || continue
                case "$p" in
                    libqxcb.so|libqminimal.so|libqminimalegl.so) : ;; # keep
                    *) rm -f "$p" ;;
                esac
            done
        popd >/dev/null
    fi

    if [[ -d usr/lib/qt5/plugins/imageformats ]]; then
        pushd usr/lib/qt5/plugins/imageformats >/dev/null
            for p in *.so; do
                [[ -f "$p" ]] || continue
                case "$p" in
                    libqjpeg.so|libqpng.so|libqsvg.so|libqgif.so) : ;; # keep
                    *) rm -f "$p" ;;
                esac
            done
        popd >/dev/null
    fi

    if [[ -d usr/lib/qt5/plugins/iconengines ]]; then
        pushd usr/lib/qt5/plugins/iconengines >/dev/null
            for p in *.so; do
                [[ -f "$p" ]] || continue
                case "$p" in
                    libqsvgicon.so) : ;; # keep
                    *) rm -f "$p" ;;
                esac
            done
        popd >/dev/null
    fi

popd >/dev/null

print_success "Optimize complete"



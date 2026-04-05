#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/common.sh"

print_status "Optimize: size and content pruning (conservative)"

[[ -d "$APPDIR" ]] || die "AppDir not found: $APPDIR. Run prepare.sh first."

pushd "$APPDIR" >/dev/null
    # Strip executables only; skip most shared libs (RELR/.relr.dyn needs recent binutils — avoid corrupting bundled deps)
    if command -v strip >/dev/null 2>&1; then
        print_status "Stripping usr/bin executables only (skipping .so — RELR / linuxdeploy compatibility)"
        find usr/bin -type f -executable -print0 2>/dev/null | xargs -0r strip --strip-unneeded 2>/dev/null || true
        if [[ "${APPDIR_STRIP_SHARED:-0}" == "1" ]]; then
            print_status "APPDIR_STRIP_SHARED=1: stripping shared libraries (best-effort)"
            find usr/lib -name "*.so*" -type f -print0 2>/dev/null | xargs -0r strip --strip-unneeded 2>/dev/null || true
            find usr/lib/linuxtrack -name "*.so*" -type f -print0 2>/dev/null | xargs -0r strip --strip-unneeded 2>/dev/null || true
        fi
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

    # Qt plugins: keep essential subsets (Qt6 and legacy Qt5 layouts)
    for _qtpl in usr/lib/qt6/plugins usr/lib/qt5/plugins; do
        if [[ -d "$_qtpl/platforms" ]]; then
            pushd "$_qtpl/platforms" >/dev/null
                for p in *.so; do
                    [[ -f "$p" ]] || continue
                    case "$p" in
                        libqxcb.so|libqminimal.so|libqminimalegl.so) : ;; # keep
                        *) rm -f "$p" ;;
                    esac
                done
            popd >/dev/null
        fi
        if [[ -d "$_qtpl/imageformats" ]]; then
            pushd "$_qtpl/imageformats" >/dev/null
                for p in *.so; do
                    [[ -f "$p" ]] || continue
                    case "$p" in
                        libqjpeg.so|libqpng.so|libqsvg.so|libqgif.so) : ;; # keep
                        *) rm -f "$p" ;;
                    esac
                done
            popd >/dev/null
        fi
        if [[ -d "$_qtpl/iconengines" ]]; then
            pushd "$_qtpl/iconengines" >/dev/null
                for p in *.so; do
                    [[ -f "$p" ]] || continue
                    case "$p" in
                        libqsvgicon.so) : ;; # keep
                        *) rm -f "$p" ;;
                    esac
                done
            popd >/dev/null
        fi
    done

popd >/dev/null

print_success "Optimize complete"



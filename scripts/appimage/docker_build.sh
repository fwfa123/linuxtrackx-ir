#!/usr/bin/env bash
# Build LinuxTrack X-IR AppImage inside a Docker container for reproducible output.
#
# Usage:
#   ./scripts/appimage/docker_build.sh                        # full build (includes X-Plane plugin)
#   WITH_XPLANE_SDK=0 ./scripts/appimage/docker_build.sh      # AppImage without X-Plane SDK / plugin
#   DOCKER_CMD=podman ./scripts/appimage/docker_build.sh      # use Podman
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"

DOCKER="${DOCKER_CMD:-docker}"
IMAGE_NAME="linuxtrackx-appimage-builder"
WITH_XPLANE_SDK="${WITH_XPLANE_SDK:-1}"

echo "[INFO] Building Docker image: $IMAGE_NAME"
BUILD_ARGS=(--build-arg "WITH_XPLANE_SDK=$WITH_XPLANE_SDK")
if [[ -n "${XPLANE_SDK_GIT_URL:-}" ]]; then
    BUILD_ARGS+=(--build-arg "XPLANE_SDK_GIT_URL=$XPLANE_SDK_GIT_URL")
fi
"$DOCKER" build \
    "${BUILD_ARGS[@]}" \
    -t "$IMAGE_NAME" \
    -f "$SCRIPT_DIR/Dockerfile" \
    "$PROJECT_ROOT"

HOST_UID=$(id -u)
HOST_GID=$(id -g)

# In-container defaults: require SDK when the image was built with WITH_XPLANE_SDK=1 so CMake always
# produces the xlinuxtrack9 plugin for the AppImage.
RUN_ENV=( -e APPIMAGE_EXTRACT_AND_RUN=1 )
if [[ "$WITH_XPLANE_SDK" == "1" ]]; then
    RUN_ENV+=( -e XPLANE_SDK_PATH=/opt/xplane-sdk/CHeaders )
    RUN_ENV+=( -e REQUIRE_XPLANE_SDK=1 )
    RUN_ENV+=( -e EXPECT_XPLANE_PLUGIN=1 )
fi

echo "[INFO] Running AppImage build inside container"
"$DOCKER" run --rm \
    -v "$PROJECT_ROOT:/src" \
    -w /src \
    "${RUN_ENV[@]}" \
    "$IMAGE_NAME" \
    bash -c "
        /src/scripts/appimage/v2/ci_build.sh
        chown -R ${HOST_UID}:${HOST_GID} /src/AppDir_v2 /src/build /src/*.AppImage 2>/dev/null || true
    "

OUT=$(ls -1t "$PROJECT_ROOT"/*.AppImage 2>/dev/null | head -1)
if [[ -n "${OUT:-}" ]]; then
    echo "[SUCCESS] AppImage built: $OUT"
    ls -lh "$OUT"
else
    echo "[ERROR] No AppImage found in $PROJECT_ROOT" >&2
    exit 1
fi

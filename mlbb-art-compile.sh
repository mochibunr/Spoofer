#!/system/bin/sh
# One-shot, reversible ART compilation helper for Mobile Legends: Bang Bang.
# Does not change MLBB preferences, graphics/FPS settings, device identity, or SoC properties.

PACKAGE="com.mobile.legends"

if ! command -v cmd >/dev/null 2>&1 || ! command -v pm >/dev/null 2>&1; then
    echo "ERROR: Android package-manager commands are unavailable."
    exit 1
fi

case "${1:-}" in
    --reset)
        echo "Resetting ART compilation for ${PACKAGE}..."
        cmd package compile --reset "${PACKAGE}"
        exit $?
        ;;
    "")
        ;;
    *)
        echo "Usage: sh mlbb-art-compile.sh [--reset]"
        exit 2
        ;;
esac

if ! pm path "${PACKAGE}" >/dev/null 2>&1; then
    echo "ERROR: ${PACKAGE} is not installed or package manager is unavailable."
    exit 1
fi

echo "Compiling the managed Android bytecode for ${PACKAGE} using speed-profile..."
echo "This is a one-time operation; it may take a little while and use CPU/storage."
cmd package compile -m speed-profile -f "${PACKAGE}"
STATUS=$?

if [ "$STATUS" -eq 0 ]; then
    echo "ART compilation command completed."
    echo "This may help Java/Kotlin startup work, but cannot guarantee faster Unity asset loading."
    echo "To reset ART compilation later, run: sh mlbb-art-compile.sh --reset"
else
    echo "ART compilation failed (exit code $STATUS). No device identity or MLBB settings were changed."
fi

exit "$STATUS"

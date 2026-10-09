#!/system/bin/sh
# Read-only Mobile Legends graphics-preference diagnostic.
# Run as root. This script never edits game files or system properties.

set -u

PKG="com.mobile.legends"
CANDIDATES="
/data/user/0/$PKG/shared_prefs/com.mobile.legends.v2.playerprefs.xml
/data/data/$PKG/shared_prefs/com.mobile.legends.v2.playerprefs.xml
"

echo "MLBB graphics preference probe (read-only)"
echo "Package: $PKG"
echo "Device identity and MT6768 platform are not modified."
found=0

for file in $CANDIDATES; do
    if [ -r "$file" ]; then
        found=1
        echo
        echo "== Found: $file =="
        echo "Matching preference names/values only:"
        grep -Eio '.{0,100}(effect|quality|graphic|render|ultra|performance|fps|highframe|veryhigh).{0,140}' "$file" 2>/dev/null \
          | head -n 80
    fi
done

if [ "$found" -eq 0 ]; then
    echo
    echo "No known preferences file found."
    echo "Open MLBB once, then run this probe again."
    echo "If still missing, check the installed package name and game data path."
    exit 1
fi

echo
echo "Probe complete. No files were changed."

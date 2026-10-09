#!/system/bin/sh
# Mobile Legends graphics diagnostic v2 — read-only.
# Does not edit game preferences, system properties, or hardware/platform identity.

set -u
PKG="com.mobile.legends"
PREF="/data/user/0/$PKG/shared_prefs/com.mobile.legends.v2.playerprefs.xml"

echo "=== MLBB graphics diagnostic v2 (read-only) ==="
echo "Package: $PKG"

echo
echo "== Installed game version =="
dumpsys package "$PKG" 2>/dev/null | grep -m 1 -E 'versionName=|versionCode=' || true

echo
echo "== Actual Android platform / graphics environment =="
for key in ro.product.model ro.build.version.release ro.build.version.sdk ro.board.platform ro.hardware ro.opengles.version; do
    printf '%s=' "$key"
    getprop "$key"
done
echo "-- SurfaceFlinger renderer / GLES (if exposed) --"
dumpsys SurfaceFlinger 2>/dev/null | grep -i -m 8 -E 'GLES:|GL_RENDERER|GL_VENDOR|RenderEngine' || true

echo
if [ ! -r "$PREF" ]; then
    echo "Preferences file not readable: $PREF"
    echo "Open MLBB once and run this script again from a root shell."
    exit 1
fi

echo "== Relevant numeric preference entries =="
# Only print numeric XML entries whose key names appear graphics-related.
# Avoid printing large URL-encoded JSON strings or unrelated preference values.
grep -E '<(int|float|boolean) name="[^"]*(Quality|quality|Graphic|graphic|Effect|effect|Render|render|VeryHigh|Ultra|Performance|performance|ModelQuality|MapQuality|DPIQuality|ParticleQuality)[^"]*" value="[^"]*"' "$PREF" 2>/dev/null   | sed -E 's/^[[:space:]]+//'   | head -n 100

echo
echo "== Selected quality/load state entries =="
grep -E '<(int|float|boolean) name="(__serverperformancelevelversion__|__serverperformancelevelconfig__|__performancedefaultmode__|__ColdStart_Param_GraphicQuality__|LoadRes_VeryHigh_State|LoadRes_VeryHigh_Score|bVeryHighQuality|PerformanceDevice_EnableQuality_del|UnityGraphicsQuality|ParticleQuality)" value="[^"]*"' "$PREF" 2>/dev/null   | sed -E 's/^[[:space:]]+//'   | head -n 60

echo
echo "Read-only probe complete. No files or properties were changed."
echo "Share this output before trying any preference edits."

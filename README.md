# Spoofer — selective iPhone 17 Pro Max identity for Android apps

Spoofer uses a **per-app Zygisk module** to make selected Android apps see common device identity fields as an iPhone 17 Pro Max, without changing those properties globally.

## What it changes

For packages listed in `target_apps.txt`, it overrides common Java `android.os.Build` identity fields and the matching Java `android.os.SystemProperties` reads:

- Manufacturer / brand: `Apple`
- Model: `iPhone 17 Pro Max`
- Device / product: `iPhone18,2`
- App-visible SoC identity: `Apple A19 Pro`

The module deliberately does **not** change `ro.board.platform`, `ro.hardware`, Android release/SDK, GPU capabilities, or the Android build fingerprint. The physical device remains a Redmi 9 running Android; this is selective identity spoofing, not iOS emulation.

## Choose which apps are targeted

Edit `target_apps.txt`, one Android package name per line. Lines beginning with `#` are comments. The initial list targets MLBB:

```text
com.mobile.legends
```

Add other package names to target them too. You can find a package name in the app's store URL, with a package inspector, or using `pm list packages` from a root shell. After changing the list, force-stop and relaunch the target app. The Zygisk module reads the list when each app process starts.

Apps not listed should see the real global Android identity. Zygisk support must be enabled in a compatible runtime such as Zygisk Next for KernelSU Next. This module includes an ARM64 build target for the Redmi 9.

## Important limits

- Per-app hooks cover common Java `Build` fields and Java `SystemProperties` reads. They do not intercept every native property API, hardware-backed attestation, Play Integrity, sensors, GPU queries, installed OS/framework checks, or server-side device checks.
- Apps may still correctly recognize that they are running on Android. Android apps cannot be made into iOS apps, and spoofed identity strings cannot provide iOS-only APIs.
- Some fields may be cached or inlined by an app, and Android releases can change internal native method names. The module logs whether its SystemProperties hooks were found; Build field overrides are attempted independently.
- This is not a guarantee that every target app will accept the reported identity. Do not use it to bypass account restrictions, fraud controls, or security checks.

## Install / build

The CI workflow builds `module/jni/main.cpp` with the Android NDK and packages an installable ZIP as a GitHub Actions artifact. Install the ZIP through KernelSU Next after confirming that a Zygisk-compatible runtime is installed and enabled. Keep a recovery path available before testing any root module.

The source project uses the public Zygisk API header from the Zygisk module sample project.

## Optional MLBB ART compilation helper

`mlbb-art-compile.sh` is a separate, opt-in helper that requests ART `speed-profile` compilation for MLBB. It does not change graphics preferences, FPS settings, or identity properties, and it is not guaranteed to speed up Unity asset loading. It is not run automatically.

## Real platform safety

The global `system.prop` intentionally contains no spoofed values. In particular, `ro.board.platform=mt6768`, `ro.hardware`, Android version/SDK, GPU and kernel properties remain real system values. The iPhone identity is applied only inside configured target app processes.

# Spoofer — selective iPhone 17 Pro Max identity for Android apps

Spoofer uses a **per-app Zygisk module** to make selected Android apps see common device identity fields as an iPhone 17 Pro Max, without changing those properties globally.

## What it changes

For packages listed in `target_apps.txt`, it overrides common Java `android.os.Build` identity fields and matching Java `android.os.SystemProperties` reads:

- Manufacturer / brand: `Apple`
- Model: `iPhone 17 Pro Max`
- Device / product: `iPhone18,2`
- App-visible SoC identity: `Apple A19 Pro`

The module deliberately does **not** change `ro.board.platform`, `ro.hardware`, Android release/SDK, GPU capabilities, or the Android build fingerprint. The physical device remains an Android phone; this is selective identity spoofing, not iOS emulation.

## Default app list

The supplied `target_apps.txt` includes MLBB, Instagram, WhatsApp, Snapchat, LINE, Facebook, Messenger, TikTok, Telegram, Discord, Reddit, X, YouTube, YouTube Music, Chrome, Google Search, Gmail, Google Photos, Maps, Drive, Docs, Calendar, Contacts, Messages, Keep, Meet, Play Games, Play Store, Spotify, Netflix, Amazon Shopping, Outlook, and Teams.

Package availability varies by app version, region, and distribution. Entries that are not installed simply have no effect.

## Choose which apps are targeted

Edit `target_apps.txt`, one Android package name per line. Lines beginning with `#` are comments. Remove packages you do not want targeted, or add another package name. Find an app's package name from its Play Store URL, with a package inspector, or via `pm list packages` from a root shell.

The module reads the allowlist when each app process starts. After changing the list, force-stop and relaunch the target app.

## Install / build

The GitHub Actions workflow installs the Android NDK explicitly, compiles the native library, and packages an installable ZIP as a workflow artifact. Install through KernelSU Next only after the workflow succeeds and you have confirmed a compatible Zygisk runtime is enabled. Keep a recovery path available before testing root modules.

## Important limits

- Per-app hooks cover common Java `Build` fields and Java `SystemProperties` reads. They do not intercept every native property API, hardware-backed attestation, Play Integrity, sensors, GPU queries, installed OS/framework checks, or server-side device checks.
- Apps may still identify that they run on Android. Android apps cannot be made into iOS apps, and spoofed strings cannot provide iOS-only APIs.
- Android releases can change internal native method names; a missing hook can reduce coverage. This needs testing on the target Android build.
- Spoofed model strings do not guarantee that apps unlock iPhone-exclusive features or graphics options. Do not use this to bypass account restrictions, fraud controls, or security checks.

## Optional MLBB ART compilation helper

`mlbb-art-compile.sh` is a separate, opt-in helper that requests ART `speed-profile` compilation for MLBB. It does not change graphics preferences, FPS settings, or identity properties, and it is not guaranteed to speed up Unity asset loading. It is not run automatically.

## Real platform safety

The global `system.prop` intentionally contains no spoofed values. In particular, `ro.board.platform=mt6768`, `ro.hardware`, Android version/SDK, GPU and kernel properties remain real system values. The iPhone identity is applied only inside configured target app processes.

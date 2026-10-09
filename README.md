# iPhone 17 Pro Max (A3526) Android Property Spoof

A KernelSU/Magisk-style Android module profile that reports selected Android product identity and SoC properties as **iPhone 17 Pro Max (rest of world)**.

## Target identity

- Apple model number: `A3526` (rest of world)
- Apple hardware identifier: `iPhone18,2`
- Reported chip: Apple `A19 Pro`
- Official model identification: [Apple Support — Identify your iPhone model](https://support.apple.com/id-id/108044)
- Official specifications: [Apple Support — iPhone 17 Pro Max technical specifications](https://support.apple.com/id-id/125091)

## Install

1. Build/download a ZIP containing `module.prop` and `system.prop` at the ZIP root, plus the compatible installer files if your module package uses them.
2. Install the ZIP through KernelSU Next or a compatible module manager.
3. Reboot.
4. Verify the reported properties:
   ```sh
   getprop ro.product.manufacturer
   getprop ro.product.model
   getprop ro.product.device
   getprop ro.soc.manufacturer
   getprop ro.soc.model
   getprop ro.board.platform
   ```

Expected spoofed values include `Apple`, `iPhone 17 Pro Max`, `iPhone18,2`, and `A19 Pro`. `ro.board.platform` is intentionally left as the real Android platform.

## Important limitations

- **This is not an iOS emulator or a full iPhone spoof.** Android apps still run on Android 13 / SDK 33, and Android APIs, framework behavior, kernel interfaces, and app environment remain Android.
- This changes selected reported properties only. It does not change the physical MediaTek MT6768 chipset, GPU, cameras, modem, display, battery, sensors, or other hardware capabilities.
- `A3526` is the regional Apple model number; `iPhone18,2` is the internal iPhone 17 Pro Max hardware identifier listed by device-model references. Android property spoofing cannot make iOS-only apps run or make Apple services treat the phone as a genuine iPhone.
- The module deliberately leaves `ro.board.platform`, Android release/SDK values, and build fingerprints untouched. Falsifying those can break Android components or create contradictory build information.
- Apps can inspect hardware-backed attestation, Play Integrity, installed OS/framework, kernel interfaces, sensors, and other signals. This module cannot guarantee that apps will accept the reported identity.
- Product-property changes may cause incompatibilities with ROM components or other modules. If anything misbehaves, disable the module from KernelSU Next or safe mode.

## Packaging note

Do not ZIP the repository's `.git/` directory. The module files should be at the archive root, not nested inside a repository folder.

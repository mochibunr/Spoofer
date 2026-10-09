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

## Mobile Legends: Bang Bang — Effect Quality

The current profile changes global Android product/SoC identity strings only. It does **not** contain a verified MLBB-specific Effect Quality unlock. MLBB's available graphics tiers can depend on game-version/device compatibility checks, and advertising an iPhone identity to an Android game does not make the game see an actual iPhone GPU or iOS device.

Do not add guessed GPU, chipset, Android-version, or graphics properties globally: the Redmi 9's actual MT6768 platform and graphics stack must remain intact. Community unlock claims are not enough to establish which preference key or device check controls the missing **Effect Quality: Ultra** option in your installed game version.

A read-only diagnostic helper, `mlbb-diagnose.sh`, is included in this repository. It only searches the MLBB player-preferences XML for relevant graphics/quality preference names and values; it does not change files or properties.

To run it on the phone:
1. Download `mlbb-diagnose.sh` from this repository.
2. Place it somewhere accessible, such as `/sdcard/Download/mlbb-diagnose.sh`.
3. Run it from a root shell:
   ```sh
   su
   sh /sdcard/Download/mlbb-diagnose.sh
   ```
4. Open MLBB at least once first. If the preference file is found, share the matching output (remove any personal identifiers if present). This gives us evidence for the installed game version before attempting any reversible setting change.

The helper is diagnostic only. It does not promise to unlock Ultra by itself.

## Important limitations

- **This is not an iOS emulator or a full iPhone spoof.** Android apps still run on Android 13 / SDK 33, and Android APIs, framework behavior, kernel interfaces, and app environment remain Android.
- This changes selected reported properties only. It does not change the physical MediaTek MT6768 chipset, GPU, cameras, modem, display, battery, sensors, or other hardware capabilities.
- `A3526` is the regional Apple model number; `iPhone18,2` is the internal iPhone 17 Pro Max hardware identifier listed by device-model references. Android property spoofing cannot make iOS-only apps run or make Apple services treat the phone as a genuine iPhone.
- The module deliberately leaves `ro.board.platform`, Android release/SDK values, and build fingerprints untouched. Falsifying those can break Android components or create contradictory build information.
- Apps can inspect hardware-backed attestation, Play Integrity, installed OS/framework, kernel interfaces, sensors, and other signals. This module cannot guarantee that apps will accept the reported identity.
- Product-property changes may cause incompatibilities with ROM components or other modules. If anything misbehaves, disable the module from KernelSU Next or safe mode.

## Packaging note

Do not ZIP the repository's `.git/` directory. The module files should be at the archive root, not nested inside a repository folder.

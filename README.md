# Xiaomi 18 Pro Max (China) Spoofer

KernelSU/Magisk-style module profile that changes common Android product identity properties to identify the device as **Xiaomi 18 Pro Max (China)** and reports the target's rumored Qualcomm SoC identifier.

## Target identity

- Marketing name: Xiaomi 18 Pro Max
- Model number: M154FF
- Codename: `madrid`
- Reported SoC properties: Qualcomm `SM8975`
- Device identity source: [KHwang9883/MobileModels — xiaomi_cn.md](https://github.com/KHwang9883/MobileModels/blob/master/brands/xiaomi_cn.md)
- SoC identifier: pre-release benchmark/specification reporting; not verified from an official target firmware image.

## Install

1. Download this module as a ZIP with `module.prop` and `system.prop` at the ZIP root.
2. Install the ZIP through KernelSU Next (or a compatible module manager).
3. Reboot.
4. Verify the properties:
   ```sh
   getprop ro.product.model
   getprop ro.product.device
   getprop ro.soc.manufacturer
   getprop ro.soc.model
   getprop ro.board.platform
   ```

Expected values include `Xiaomi 18 Pro Max`, `madrid`, `Qualcomm`, and `SM8975`.

## Important limitations

- This changes reported Android properties only. It does **not** turn the Redmi 9 hardware into a Xiaomi 18 Pro Max or add its chipset, cameras, modem, display, or other capabilities.
- The target chipset identifier is based on pre-release reporting, not a verified official firmware image. `ro.board.platform` is also a platform identifier, and apps may obtain hardware identity from other sources.
- This profile deliberately does not invent `ro.build.fingerprint`; the existing ROM fingerprint may therefore still identify the underlying ROM/device.
- Apps can inspect hardware-backed attestation, Google Play certification, kernel interfaces, device-specific APIs, and other properties. This module cannot guarantee every app will accept the spoof.
- Changing product properties can cause incompatibilities with apps or ROM components that rely on the real device identity. Disable the module from KernelSU recovery/safe mode if the device behaves unexpectedly.

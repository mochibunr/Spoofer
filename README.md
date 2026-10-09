# Xiaomi 18 Pro Max (China) Spoofer

KernelSU/Magisk-style module profile that changes common Android product identity properties to identify the device as **Xiaomi 18 Pro Max (China)**.

## Target identity

- Marketing name: Xiaomi 18 Pro Max
- Model number: M154FF
- Codename: `madrid`
- Source: [KHwang9883/MobileModels — xiaomi_cn.md](https://github.com/KHwang9883/MobileModels/blob/master/brands/xiaomi_cn.md)

## Install

1. Download this module as a ZIP with `module.prop` and `system.prop` at the ZIP root.
2. Install the ZIP through KernelSU Next (or a compatible module manager).
3. Reboot.
4. Verify the properties with:
   ```sh
   getprop ro.product.model
   getprop ro.product.device
   getprop ro.product.name
   getprop ro.product.system.model
   ```

Expected main values are `Xiaomi 18 Pro Max` and `madrid`.

## Important limitations

- This changes reported Android properties only. It does **not** turn the Redmi 9 hardware into a Xiaomi 18 Pro Max or add its chipset, cameras, modem, display, or other capabilities.
- The MobileModels list provides model names, model numbers, and codenames, not a verified firmware fingerprint. This profile deliberately does not invent `ro.build.fingerprint` or chipset properties.
- Apps can inspect additional properties, hardware-backed attestation, Google Play certification, or device-specific APIs. This module cannot guarantee every app will accept the spoof.
- Changing product properties can cause incompatibilities with apps or ROM components that rely on the real device identity. Disable the module from KernelSU recovery/safe mode if the device behaves unexpectedly.

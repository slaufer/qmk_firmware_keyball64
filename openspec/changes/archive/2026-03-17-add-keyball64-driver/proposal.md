## Why

The Keyball64 (a split keyboard using the same PCB as the Keyball61 but with two non-trackball halves, yielding 64 keys total) has no QMK driver in this up-to-date firmware repo — only the legacy Holykeebs fork supports it, and that fork is too old to support features like VIA remapping. Adding a first-party driver enables building firmware from a current QMK tree with full modern feature support.

## What Changes

- Add `keyboards/keyball64/` as a new, fully self-contained keyboard driver
- Bundle the duplex matrix library directly under `keyboards/keyball64/lib/duplexmatrix/`, copied from the legacy Holykeebs fork
- Optionally bundle `oledkit` and `logofont` for OLED display support (disabled by default)
- Omit all trackball-related code (PMW3360 sensor driver, SPI configuration, pointing device, `KEYBALL_GET_INFO` RPC transaction, trackball keycodes)
- Omit all LED/RGB lighting code — the Keyball64 has no LED hardware
- Use the `LAYOUT_no_ball` coordinate map from the Keyball61 as the sole layout (all 64 key positions active), renamed to `LAYOUT`
- Add a VIA keyboard definition (`via.json`) for use with the VIA configurator
- Add a `via` keymap as the primary supported keymap

## Capabilities

### New Capabilities
- `keyball64-driver`: QMK keyboard driver for the Keyball64 — handles matrix scanning (duplex), split UART communication, optional OLED display, and VIA support, without any trackball, pointing device, or LED code

### Modified Capabilities
<!-- None — no existing specs are changing -->

## Impact

- **New files**: `keyboards/keyball64/` directory with all driver files and bundled libraries — no changes to any existing file in the repo

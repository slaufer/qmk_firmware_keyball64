## Context

The Keyball64 uses the same PCB as the Keyball61 but is built with two non-trackball halves. The "64" name corresponds exactly to the `LAYOUT_no_ball` variant in the Keyball61 driver, which has 64 active key positions (col 3 and col 7 on rows 0–2 of each half are physically unpopulated — that space is used by the trackball module on the Keyball61). The legacy Holykeebs QMK fork supports this via the Keyball61 driver's `LAYOUT_no_ball` layout, but that repo is too old to support VIA and other modern QMK features.

The target repo (`qmk_firmware_keyball64`) is at QMK 0.32.4. There is **no existing `keyboards/keyball/` directory** in this repo — the new driver must be fully self-contained at `keyboards/keyball64/` with no dependency on any sibling keyboard.

**Key hardware facts (from Keyball61 driver in legacy fork):**
- MCU: RP2040
- Matrix: duplex, 5 row pins × 4 col pins → 10×8 logical matrix (split)
  - Row pins: GP4, GP5, GP6, GP7, GP8
  - Col pins: GP29, GP28, GP27, GP26
- Left/right detection: `SPLIT_HAND_MATRIX_GRID GP26, GP6` (low = left)
- Split UART TX: GP1
- Matrix mask: cols 3 & 7 masked on rows 0–2 and 5–7; col 3 only masked on rows 3–4 and 8–9
- **No LEDs** — the Keyball64 does not have LED hardware; all LED/RGB code is excluded

## Goals / Non-Goals

**Goals:**
- Fully self-contained QMK driver for the Keyball64 (RP2040, duplex matrix, split UART, optional OLED)
- VIA-compatible `via` keymap and `via.json` keyboard definition
- No trackball code and no LED/RGB code
- No dependency on any other keyboard's files in the target repo

**Non-Goals:**
- Trackball support of any kind
- LED or RGB lighting support
- Upstream QMK submission (this is a personal fork)
- Changes to any existing file in the target repo

## Decisions

### 1. File location: standalone `keyboards/keyball64/`

There is no `keyboards/keyball/` parent in the target repo, so the keyboard is created at the top level of `keyboards/` as `keyball64/`. This keeps it self-contained and avoids creating a misleading empty parent directory.

### 2. Libraries bundled under `keyboards/keyball64/lib/`

The Keyball61 uses three shared libraries: `duplexmatrix` (custom matrix scanning), `oledkit` (OLED utilities), and `logofont` (custom font). Since there is no shared `keyball/lib/` to reference, these are copied from the legacy fork into `keyboards/keyball64/lib/`. The `keyball` library itself (`lib/keyball/keyball.c`) is **not** copied — it is entirely trackball-specific.

**Copied libraries:**
- `lib/duplexmatrix/duplexmatrix.c` and `.h` — required for `CUSTOM_MATRIX = lite`
- `lib/oledkit/oledkit.c` and `.h` — OLED utility (included for optional OLED support)
- `lib/logofont/logofont.c` and font data — custom OLED font (included for optional OLED support)

### 3. `config.h` is fully self-contained, no LED defines

Since there is no parent keyboard, all configuration is in `keyboards/keyball64/config.h`. This includes matrix pins, split pins, OLED font path, split watchdog, and RP2040 bootloader reset. **No `WS2812_DI_PIN`, `RGBLIGHT_*`, or `RGB_MATRIX_*` defines are included** — the hardware has no LEDs.

### 4. Layout: single `LAYOUT` (derived from `LAYOUT_no_ball`)

The Keyball64 has no trackball variants, so there is only one physical layout. The `LAYOUT_no_ball` coordinate map from the Keyball61 `keyboard.json` is used verbatim but renamed to `LAYOUT`. The duplex matrix mask is identical to the Keyball61 (those PCB positions are physically absent on both keyboards).

### 5. VIA keyboard definition (`via.json`)

The Keyball61's `via.json` uses a KLE column-stagger layout with rotated thumb keys, plus a "Ball availability" layout option that switches between no-ball, right-ball, left-ball, and dual-ball key arrangements. For the Keyball64, the layout option is removed entirely — all 64 keys are always present. The physical key positions (column stagger geometry and thumb key rotation values) are taken directly from the Keyball61's `via.json` "no ball" (option `0,0`) variant, since the PCB is identical.

The VIA definition sets `"lighting": "none"` (no LED hardware) and uses PID `0x0200`.

### 6. No `SPLIT_TRANSACTION_IDS_KB` / no keyball library

The `KEYBALL_GET_INFO` RPC transaction in the Keyball61 driver queries whether each half has a trackball. Without a trackball, this transaction is not needed. The `lib/keyball/keyball.c` library is not included.

### 7. USB PID: `0x0200`

The Keyball61 uses VID `0x5957` / PID `0x0100`. The Keyball64 is assigned PID `0x0200` — a new, non-conflicting identifier within the same Yowkees vendor namespace.

### 8. `halconf.h` and `mcuconf.h`: omit unless needed for OLED

The Keyball61 uses these to enable SPI. The Keyball64 has no SPI peripherals, so `mcuconf.h` is not needed. Whether `halconf.h` is needed for I2C (OLED) depends on whether QMK 0.32.x RP2040 enables I2C by default — determine at build time.

## Risks / Trade-offs

- **OLED I2C without explicit halconf.h** → Mitigation: build and test; add a minimal `halconf.h` (I2C only, no SPI) if compilation fails
- **Library drift** → Bundling copies of `duplexmatrix`/`oledkit`/`logofont` means they won't automatically track upstream changes; acceptable since this is a personal fork
- **PID uniqueness** → `0x0200` is unverified against other Holykeebs devices; acceptable for personal use, and can be changed without breaking anything

## Open Questions

1. **OLED halconf.h**: Does QMK 0.32.x RP2040 enable I2C by default when `OLED_ENABLE = yes`? Determine at build time.

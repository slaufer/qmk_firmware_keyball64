## 1. Create Directory Structure

- [x] 1.1 Create `keyboards/keyball64/` directory
- [x] 1.2 Create `keyboards/keyball64/lib/duplexmatrix/` directory
- [x] 1.3 Create `keyboards/keyball64/lib/oledkit/` directory
- [x] 1.4 Create `keyboards/keyball64/lib/logofont/` directory
- [x] 1.5 Create `keyboards/keyball64/keymaps/via/` directory
- [x] 1.6 Create `keyboards/keyball64/keymaps/default/` directory

## 2. Copy Bundled Libraries from Legacy Fork

- [x] 2.1 Copy `duplexmatrix.c` and `duplexmatrix.h` from `~/repos/holykeebs/qmk_firmware/keyboards/keyball/lib/duplexmatrix/` into `keyboards/keyball64/lib/duplexmatrix/`
- [x] 2.2 Copy `oledkit.c` and `oledkit.h` from `~/repos/holykeebs/qmk_firmware/keyboards/keyball/lib/oledkit/` into `keyboards/keyball64/lib/oledkit/`
- [x] 2.3 Copy `logofont.c` and any font data files from `~/repos/holykeebs/qmk_firmware/keyboards/keyball/lib/logofont/` into `keyboards/keyball64/lib/logofont/`

## 3. Write Core Driver Files

- [x] 3.1 Create `keyboard.json` — single `LAYOUT` with the 64 key positions from Keyball61's `LAYOUT_no_ball` (same matrix coordinates verbatim), `keyboard_name` = `Keyball64`, `manufacturer` = `Yowkees`, VID `0x5957`, PID `0x0200`, `device_version` = `1.0.0`, processor `RP2040`, bootloader `rp2040`, split enabled — no `rgblight` or `ws2812` sections
- [x] 3.2 Create `config.h` — self-contained: `MATRIX_ROWS 10`, `MATRIX_COLS 8` (duplex), row pins `GP4–GP8`, col pins `GP29/28/27/26`, `MATRIX_MASKED`, `DEBOUNCE 5`, `SERIAL_USART_TX_PIN GP1`, `SPLIT_HAND_MATRIX_GRID GP26, GP6`, `SPLIT_HAND_MATRIX_GRID_LOW_IS_LEFT`, OLED font path pointing to `keyboards/keyball64/lib/logofont/logofont.c`, `SPLIT_WATCHDOG_ENABLE`, `SPLIT_WATCHDOG_TIMEOUT 3000`, `RP2040_BOOTLOADER_DOUBLE_TAP_RESET`, `RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT 1000U` — no `WS2812_DI_PIN`, no RGB/LED defines, no SPI defines, no `SPLIT_TRANSACTION_IDS_KB`
- [x] 3.3 Create `rules.mk` — `CUSTOM_MATRIX = lite`, `SRC += lib/duplexmatrix/duplexmatrix.c`, `SERIAL_DRIVER = vendor`, `OLED_ENABLE = no`, `SRC += lib/oledkit/oledkit.c`, standard build disables (`BOOTMAGIC_ENABLE = no`, `CONSOLE_ENABLE = no`, `COMMAND_ENABLE = no`, `NKRO_ENABLE = no`, `BACKLIGHT_ENABLE = no`, `AUDIO_ENABLE = no`, `SPACE_CADET_ENABLE = no`, `MAGIC_ENABLE = no`) — no `POINTING_DEVICE_ENABLE`, no `keyball.c`, no RGBLIGHT or RGB_MATRIX lines
- [x] 3.4 Create `keyball64.h` — `#pragma once` + `#include "quantum.h"` only (no keyball.h)
- [x] 3.5 Create `keyball64.c` — `#include QMK_KEYBOARD_H` + define `matrix_mask[MATRIX_ROWS]` with values identical to Keyball61: rows 0–2 and 5–7 = `0b01110111`, rows 3–4 and 8–9 = `0b11110111` — no `keyball_on_adjust_layout` or any other function

## 4. Write VIA Keyboard Definition

- [x] 4.1 Create `via.json` — derived from Keyball61's `via.json` with these changes:
  - Set `"name": "Keyball64"`, `"productId": "0x0200"`
  - Set `"lighting": "none"` (no LEDs)
  - Remove `"labels"` (no ball-availability layout option)
  - For rows 0–3: copy the column-stagger KLE positions from Keyball61 verbatim (same PCB geometry)
  - For the thumb row (row 4 / row 9): use only the "no ball" (option `0,0`) key positions — strip the variant notation (`\n\n\n0,0`) and remove all duplicate rows for other ball configurations; include all 7 thumb keys per side (cols 0,1,2,4,5,6,7) at their original stagger coordinates, with rotated keys 4,6/4,7 and 9,6/9,7 keeping their `r`/`rx`/`ry` rotation values

## 5. Write Keymaps

- [x] 5.1 Create `keymaps/via/keymap.c` — 4-layer keymap using `LAYOUT` macro; layer 0 QWERTY with standard modifier positions, layers 1–3 all `KC_TRNS`
- [x] 5.2 Create `keymaps/via/rules.mk` — `VIA_ENABLE = yes`
- [x] 5.3 Create `keymaps/via/config.h` — `#define DYNAMIC_KEYMAP_LAYER_COUNT 4`
- [x] 5.4 Create `keymaps/default/keymap.c` — same layer-0 QWERTY layout without VIA overhead
- [x] 5.5 Create `keymaps/default/rules.mk` — empty or minimal

## 6. Verify Build

- [x] 6.1 Run `qmk compile -kb keyball64 -km via` and confirm it compiles cleanly
- [x] 6.2 Run `qmk compile -kb keyball64 -km default` and confirm it compiles cleanly
- [x] 6.3 If OLED build fails due to missing I2C HAL config, add a minimal `halconf.h` with `#define HAL_USE_I2C TRUE` and `#include_next <halconf.h>` (no SPI defines)
- [x] 6.4 Confirm no LED/RGB symbols appear in the build output

## 7. Flash and Test (manual)

> These steps are performed manually by the user, not by the agent.

- [x] 7.1 Flash the `via` keymap to both halves
- [x] 7.2 Verify all 64 keys register correctly (test each key with a keyboard tester)
- [x] 7.3 Verify split communication works (keys on both halves register via the USB-connected half)
- [x] 7.4 Verify left/right detection is correct
- [x] 7.5 Verify double-tap reset enters RP2040 bootloader
- [x] 7.6 Load `via.json` into VIA configurator and verify all 64 keys are shown in correct positions

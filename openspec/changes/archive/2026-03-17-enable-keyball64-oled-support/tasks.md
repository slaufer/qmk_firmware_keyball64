## 1. Add ChibiOS HAL Config

- [x] 1.1 Create `keyboards/keyball64/halconf.h` with `#define HAL_USE_I2C TRUE` and `#include_next <halconf.h>`

## 2. Enable OLED in Keymaps

- [x] 2.1 Add `OLED_ENABLE = yes` to `keyboards/keyball64/keymaps/via/rules.mk`
- [x] 2.2 Add `OLED_ENABLE = yes` to `keyboards/keyball64/keymaps/default/rules.mk`

## 3. Implement Primary-Half Render Override

- [x] 3.1 Add `oledkit_render_info_user` to `keyboards/keyball64/keymaps/via/keymap.c` — display current layer number and active modifier indicators (Shift, Ctrl, Alt, GUI) using `oled_write_P` / `oled_write_ln_P`

## 4. Verify Build

- [x] 4.1 Run `qmk compile -kb keyball64 -km via` and confirm it compiles cleanly with OLED enabled
- [x] 4.2 Run `qmk compile -kb keyball64 -km default` and confirm it compiles cleanly with OLED enabled
- [x] 4.3 Confirm no I2C or OLED-related errors appear in the build output

## 5. Flash and Test (manual)

> These steps are performed manually by the user, not by the agent.

- [x] 5.1 Flash the `via` keymap to both halves
- [x] 5.2 Verify the primary half's OLED shows layer 0 on boot
- [x] 5.3 Verify the primary half's OLED updates when a layer is activated
- [x] 5.4 Verify the secondary half's OLED shows the Keyball logofont logo
- [x] 5.5 Verify the secondary logo is rotated correctly (right-side up)

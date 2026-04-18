## 1. Enable split activity sync

- [x] 1.1 Add `#define SPLIT_ACTIVITY_ENABLE` to `keyboards/keyball64/keymaps/via/config.h`
- [x] 1.2 Add `#define SPLIT_ACTIVITY_ENABLE` to `keyboards/keyball64/keymaps/default/config.h`

## 2. Add idle blank logic to oled_bitmap.c

- [x] 2.1 At the top of `oled_task_user()` in `keyboards/keyball64/lib/oled_bitmap/oled_bitmap.c`, add an idle check: if `last_input_activity_elapsed() >= 300000UL`, call `oled_off()` and return `false`
- [x] 2.2 Add an `oled_on()` call in the active render path (before writing to the display) so the screen wakes correctly after blanking

## 3. Build verification

- [x] 3.1 Compile via keymap (`qmk compile -kb keyball64 -km via`) and confirm it succeeds
- [x] 3.2 Compile default keymap (`qmk compile -kb keyball64 -km default`) and confirm it succeeds

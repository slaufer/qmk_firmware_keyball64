## 1. Remove held-key tracking system

- [x] 1.1 Remove `held_keycodes`, `held_display`, `keycode_to_char`, `update_held_display`, and `held_keys_slave_handler` from `keymaps/default/keymap.c`
- [x] 1.2 Simplify `process_record_user` in `keymaps/default/keymap.c` — keep modifier guard and `oled_ball_on_key_press()` call; remove all held-key bookkeeping
- [x] 1.3 Remove `transaction_register_rpc(HELD_KEYS_SYNC, ...)` from `keyboard_post_init_user` in `keymaps/default/keymap.c`
- [x] 1.4 Remove `transaction_rpc_exec(HELD_KEYS_SYNC, ...)` from `housekeeping_task_user` in `keymaps/default/keymap.c`
- [x] 1.5 Remove `HELD_KEYS_SYNC` from `SPLIT_TRANSACTION_IDS_KB` in `keymaps/default/config.h`
- [x] 1.6 Apply the same removals (1.1–1.5) to `keymaps/via/keymap.c` and `keymaps/via/config.h`

## 2. Remove logofont

- [x] 2.1 Delete `lib/logofont/logofont.c`
- [x] 2.2 Remove the `// OLED font` block (lines 38–43) from `keyboards/keyball64/config.h`

## 3. Extract bitmap data from oled_bitmap.c

- [x] 3.1 Create `lib/oled_bitmap/oled_bitmap_data.h` containing all `static const PROGMEM` bitmap arrays (lines 9–428 of `oled_bitmap.c`)
- [x] 3.2 Replace lines 9–428 in `oled_bitmap.c` with `#include "oled_bitmap_data.h"`

## 4. Fix non-static internal function

- [x] 4.1 Add `static` to `oledkit_render_info_user` in `lib/oled_bitmap/oled_bitmap.c`

## 5. Extract shared keymap logic

- [x] 5.1 Create `lib/keymap_common/keymap_common.h` with the shared block (includes, `ball_pos_slave_handler`, `keyboard_post_init_user`, `housekeeping_task_user`, `process_record_user`)
- [x] 5.2 Replace the shared block in `keymaps/default/keymap.c` with `#include "../../lib/keymap_common/keymap_common.h"`
- [x] 5.3 Replace the shared block in `keymaps/via/keymap.c` with `#include "../../lib/keymap_common/keymap_common.h"`

## 6. Verify

- [x] 6.1 Build `qmk compile -kb keyball64 -km default` and confirm no errors
- [x] 6.2 Build `qmk compile -kb keyball64 -km via` and confirm no errors

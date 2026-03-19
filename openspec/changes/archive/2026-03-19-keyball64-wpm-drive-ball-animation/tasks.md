## 1. Rename and trim ball movement API

- [x] 1.1 In `oled_bitmap.c`, rename `oled_ball_on_key_press` → `oled_ball_tick` (function definition)
- [x] 1.2 In `oled_bitmap.c`, remove `oled_ball_get_pos`, `oled_ball_set_pos`, `oled_ball_get_history`, `oled_ball_set_history`
- [x] 1.3 In `oled_bitmap.h`, update the declaration from `oled_ball_on_key_press()` to `oled_ball_tick()` and update the comment to reflect timer-driven usage; remove declarations for the four sync accessors

## 2. Remove custom ball-sync infrastructure

- [x] 2.1 In `keymap_common.h`, remove `ball_pos_slave_handler`, `keyboard_post_init_user`, and the master-side sync block from `housekeeping_task_user()`
- [x] 2.2 Remove `#include "transactions.h"` from `keymap_common.h`
- [x] 2.3 In `keymaps/default/config.h`, remove `#define SPLIT_TRANSACTION_IDS_KB BALL_POS_SYNC`

## 3. Replace key-press trigger with slave-side WPM-driven timer

- [x] 3.1 In `keymap_common.h`, remove the `oled_ball_on_key_press()` call from `process_record_user()`
- [x] 3.2 Remove the now-empty `process_record_user()` function from `keymap_common.h`
- [x] 3.3 Add `housekeeping_task_user()` to `keymap_common.h` with a `static uint16_t ball_timer = 0` guarded by `is_keyboard_slave()`; apply WPM-interval logic: `interval = (wpm >= 90) ? 50 : 500 - wpm * 5`; call `oled_ball_tick()` when elapsed and WPM > 0

## 4. Verify

- [x] 4.1 Confirm firmware compiles for both `default` and `via` keymaps without errors or warnings
- [x] 4.2 Flash and verify: WPM = 0 → ball stationary, start typing → ball moves, faster typing → faster ball

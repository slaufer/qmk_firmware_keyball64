## Why

The bouncing ball animation on the slave OLED currently only moves when a key is pressed, making it feel unresponsive during pauses in typing. A continuously animated ball that speeds up with WPM creates a more engaging idle animation and gives immediate visual feedback about typing pace.

## What Changes

- Remove the key-press trigger (`oled_ball_on_key_press()` called from `process_record_user`) in favor of a timer-based approach
- Rename `oled_ball_on_key_press()` → `oled_ball_tick()` to reflect its new role
- Move ball state ownership entirely to the slave: add a WPM-derived timer in `housekeeping_task_user()` on the slave that calls `oled_ball_tick()` at a rate from ~500ms/step (0 WPM) to ~50ms/step (≥90 WPM)
- Remove all custom ball-sync infrastructure: `BALL_POS_SYNC` RPC, slave handler, transaction registration, and the `oled_ball_get_pos` / `oled_ball_set_pos` / `oled_ball_get_history` / `oled_ball_set_history` accessors (which existed solely to service the sync)
- Remove the now-empty `process_record_user` and `housekeeping_task_user` (master side) from shared keymap code

## Capabilities

### New Capabilities
- `wpm-driven-ball-animation`: Continuous, WPM-reactive ball movement on the slave OLED, driven by a timer on the slave rather than key-press events on the master

### Modified Capabilities
- `bouncing-ball`: The movement trigger changes from key-press events to a WPM-driven timer on the slave; behavior of position/history/bounce logic is otherwise unchanged

## Impact

- `keyboards/keyball64/lib/oled_bitmap/oled_bitmap.c` — rename `oled_ball_on_key_press` → `oled_ball_tick`; remove `oled_ball_get_pos`, `oled_ball_set_pos`, `oled_ball_get_history`, `oled_ball_set_history`
- `keyboards/keyball64/lib/oled_bitmap/oled_bitmap.h` — update declaration and comment; remove sync accessor declarations
- `keyboards/keyball64/lib/keymap_common/keymap_common.h` — remove `process_record_user`; remove `ball_pos_slave_handler`, `keyboard_post_init_user`, and master-side `housekeeping_task_user`; add slave-side WPM timer in `housekeeping_task_user`; remove `#include "transactions.h"`
- `keyboards/keyball64/keymaps/default/config.h` — remove `SPLIT_TRANSACTION_IDS_KB BALL_POS_SYNC`
- No changes to slave rendering, trail logic, or OLED driver

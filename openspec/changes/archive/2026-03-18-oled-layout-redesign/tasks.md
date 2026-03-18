## 1. Held-Key Tracking (both keymaps)

- [x] 1.1 In `keymaps/default/config.h`, add `#define SPLIT_TRANSACTION_IDS_KB HELD_KEYS_SYNC` to register the custom split transaction
- [x] 1.2 In `keymaps/default/keymap.c`, define `held_keys[5]` (char array, initialized to spaces) and implement `process_record_user` to add/remove display characters on key press/release, skipping modifier keycodes; evict oldest entry when buffer is full
- [x] 1.3 In `keymaps/default/keymap.c`, implement `keyboard_post_init_user` to register the held-keys sync transaction with `transaction_rpc_slave_handler` that copies the master's buffer to a slave-accessible copy
- [x] 1.4 Mirror all held-key changes (config.h define, `held_keys`, `process_record_user`, `keyboard_post_init_user`) into `keymaps/via/keymap.c` and `keymaps/via/config.h`

## 2. Master OLED Layout (default keymap)

- [x] 2.1 Rewrite `oledkit_render_info_user` in `keymaps/default/keymap.c`: row 0 "LAYER", row 1 right-justified layer number, row 2 blank, row 3 "CAPSK" (full-row inverted if active, blank if not), row 4 "NUMLK" (same), rows 5–11 blank, rows 12–15 "SHIFT"/" CTRL"/"SUPER"/" ALT " (each 5-char padded, full-row inverted if active, blank if not)

## 3. Slave OLED Layout (default keymap)

- [x] 3.1 Rewrite the slave branch of `oled_task_user` in `keymaps/default/keymap.c`: row 0 " WPM" (1 leading space), row 1 right-justified WPM value, row 2 blank, row 3 held-keys buffer (5 chars, blank if empty), rows 4–11 blank, rows 12–15 "SHIFT"/"CTRL "/"SUPER"/" ALT " (each 5-char padded, full-row inverted if active, blank if not)

## 4. Via Keymap Sync

- [x] 4.1 Copy the updated `oledkit_render_info_user` and `oled_task_user` bodies from `keymaps/default/keymap.c` into `keymaps/via/keymap.c` verbatim

## 5. Verification

- [x] 5.1 Compile both keymaps: `qmk compile -kb keyball64 -km default` and `qmk compile -kb keyball64 -km via`
- [x] 5.2 Flash and verify master OLED layout: LAYER/number, CAPLK/NUMLK inversion, modifier rows with full-row inversion
- [x] 5.3 Flash and verify slave OLED layout: WPM, held-keys row updates as keys are pressed/released, modifier rows with full-row inversion

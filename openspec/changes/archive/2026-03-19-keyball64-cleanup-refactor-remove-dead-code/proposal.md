## Why

A code audit found that the held-key tracking system (syncing held keycodes to the slave half) is entirely dead: the synced buffer is never read by `oled_bitmap.c` or any other consumer. Additional minor issues include a non-static internal function in `oled_bitmap.c` and duplicated logic between the `default` and `via` keymaps.

## What Changes

- **Remove** the entire held-key tracking system from both keymaps: `held_keycodes`, `held_display`, `keycode_to_char`, `update_held_display`, `held_keys_slave_handler`, the `HELD_KEYS_SYNC` RPC registration and exec calls
- **Remove** `HELD_KEYS_SYNC` from `SPLIT_TRANSACTION_IDS_KB` in both `keymaps/default/config.h` and `keymaps/via/config.h`
- **Simplify** `process_record_user` — reduce to calling `oled_ball_on_key_press()` on key press; remove all held-key bookkeeping
- **Remove** `lib/logofont/logofont.c` and the `OLED_FONT_H` block from `keyboards/keyball64/config.h` — the font is compiled in but never accessed since all rendering goes through `oled_write_pixel`
- **Extract** all `static const PROGMEM` bitmap arrays (lines 9–428) from `oled_bitmap.c` into a private header `lib/oled_bitmap/oled_bitmap_data.h`, included only by `oled_bitmap.c`
- **Add** `static` qualifier to `oledkit_render_info_user` in `lib/oled_bitmap/oled_bitmap.c`
- **Extract** the remaining shared keymap logic into `lib/keymap_common/keymap_common.h`, included by both keymaps

## Capabilities

### New Capabilities
<!-- None -->

### Modified Capabilities
- `oled-held-keys`: All three requirements in this spec are being removed — the held-key buffer, the sync transport, and the keycode-to-char mapping are all dead code with no consumer.

## Impact

- `keyboards/keyball64/keymaps/default/keymap.c` — held-key tracking removed; remaining shared code replaced with `#include`
- `keyboards/keyball64/keymaps/via/keymap.c` — same
- `keyboards/keyball64/keymaps/default/config.h` — `HELD_KEYS_SYNC` removed from `SPLIT_TRANSACTION_IDS_KB`
- `keyboards/keyball64/keymaps/via/config.h` — same
- `keyboards/keyball64/lib/oled_bitmap/oled_bitmap.c` — bitmap data replaced with `#include`; `static` on `oledkit_render_info_user`
- `keyboards/keyball64/lib/oled_bitmap/oled_bitmap_data.h` — new file containing all PROGMEM bitmap arrays
- `keyboards/keyball64/lib/logofont/logofont.c` — deleted
- `keyboards/keyball64/config.h` — `OLED_FONT_H` block removed
- `keyboards/keyball64/lib/keymap_common/keymap_common.h` — new file (created)
- No public APIs, no QMK core files touched; no visible behavior changes

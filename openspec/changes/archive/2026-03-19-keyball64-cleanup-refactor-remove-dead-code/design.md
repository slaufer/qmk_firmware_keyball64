## Context

Three issues found in audit:

1. **Dead held-key system**: `process_record_user` tracks up to 5 held keycodes in `held_display[6]`, syncs it to the slave via `HELD_KEYS_SYNC`, and `held_keys_slave_handler` receives it — but `oled_bitmap.c` never reads `held_display`. The data is produced and transmitted but has no consumer. The `HELD_KEYS_SYNC` custom transaction is wasted split-transport bandwidth every housekeeping cycle.

2. **Dead font**: `config.h` sets `OLED_FONT_H` to `lib/logofont/logofont.c`, which causes QMK to compile the logofont glyph table into the firmware. However, `oled_bitmap.c` renders everything via `oled_write_pixel` and never calls `oled_write`, `oled_write_char`, or `oled_write_P`. The font data is linked in but unreachable.

3. **Oversized `oled_bitmap.c`**: The file is 622 lines. Lines 9–428 are entirely `static const PROGMEM` bitmap arrays with no logic. The remaining ~190 lines are ball state, rendering helpers, and OLED task hooks — the actual code. The bitmap data can be separated without any linkage changes since all arrays are `static`.

4. **Non-static internal function**: `oledkit_render_info_user` in `oled_bitmap.c` is not declared in any header and has exactly one caller (`oled_task_user` in the same file). It should be `static`.

3. **Duplicated keymap code**: Lines 1–111 of `keymaps/default/keymap.c` and `keymaps/via/keymap.c` are byte-for-byte identical. After removing the held-key system, what remains is: includes, `ball_pos_slave_handler`, `keyboard_post_init_user`, `housekeeping_task_user`, and a much-simplified `process_record_user`.

## Goals / Non-Goals

**Goals:**
- Remove the held-key tracking system entirely (variables, functions, RPC transaction)
- Reduce `process_record_user` to its live code only
- Mark `oledkit_render_info_user` static
- Deduplicate remaining shared keymap logic via a shared header

**Non-Goals:**
- Changing any OLED rendering behavior
- Modifying the ball animation or `BALL_POS_SYNC` transport
- Touching QMK core files

## Decisions

**Bitmap data → `oled_bitmap_data.h`**: All 14 bitmap arrays are `static`, so moving them to a private header included only by `oled_bitmap.c` is safe. No external linkage, no `rules.mk` change. The header sits alongside `oled_bitmap.c` in the same directory and is not added to `oled_bitmap.h`'s public interface.

**Remove logofont and `OLED_FONT_H`**: Deleting `logofont.c` alone would cause a build error since `config.h` references it. The entire `#ifndef OLED_FONT_H` block must be removed. QMK will then use its built-in default font, which is also unused — but it's compiled into the QMK core and can't be removed from this layer.

**Remove, don't stub**: The `HELD_KEYS_SYNC` transaction ID must also be removed from `SPLIT_TRANSACTION_IDS_KB` in both `config.h` files. Leaving a registered-but-unused transaction wastes a slot in the split transport negotiation table.

**`process_record_user` after removal**: Only `oled_ball_on_key_press()` on press survives. The modifier skip guard (`keycode >= KC_LCTL && keycode <= KC_RGUI`) is no longer needed since we're not tracking anything, but keeping it avoids unnecessary `oled_ball_on_key_press()` calls on modifier-only presses. Keep it for efficiency.

**Shared header approach**: Each keymap builds independently in QMK — only one `keymap.c` is compiled per build — so function definitions in a shared header cause no duplicate-symbol issues. All helpers are `static`; QMK hooks (`process_record_user`, `keyboard_post_init_user`, `housekeeping_task_user`) are non-static as required by QMK core.

## Risks / Trade-offs

- **[Risk]** Removing `HELD_KEYS_SYNC` changes the split transport table. If old firmware is running on one half and new on the other during a partial flash, the halves will fail to negotiate until both are reflashed. → Expected split-firmware behavior; not a concern for a personal fork.
- **[Risk]** `oled-held-keys` spec describes behavior that will no longer exist. → Addressed by updating the spec with REMOVED requirements.

## Migration Plan

1. Remove held-key variables, functions, and RPC calls from both keymap files.
2. Remove `HELD_KEYS_SYNC` from both `config.h` files.
3. Add `static` to `oledkit_render_info_user`.
4. Create `lib/keymap_common/keymap_common.h` with the remaining shared block.
5. Replace the shared block in both keymap files with `#include`.
6. Build both keymaps; verify clean.

Rollback: `git revert`.

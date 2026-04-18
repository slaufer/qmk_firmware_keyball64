## Context

Both halves of the Keyball64 have 32×128 OLED displays driven by QMK's OLED driver (`OLED_ENABLE = yes`). The current firmware has no idle timeout: displays run continuously whether or not the keyboard is in use.

QMK's `OLED_TIMEOUT` mechanism tracks OLED *write activity* (calls to `oled_write_*`, `oled_set_cursor`, etc.), not input activity. Because the current `oled_task_user()` in `oled_bitmap.c` unconditionally calls `draw_bitmap` and `draw_number` every frame, the OLED activity timer resets on every render pass — `OLED_TIMEOUT` alone would never fire.

## Goals / Non-Goals

**Goals:**
- Blank both OLED displays after 5 minutes (300 000 ms) of keyboard/mouse inactivity.
- Wake screens immediately on the next keypress, encoder event, or pointing-device movement.
- Apply the timeout to both the `via` and `default` keymaps.

**Non-Goals:**
- Configurable timeout at runtime (no keycode or VIA knob to adjust the value).
- Gradual dimming or fade-out animation.
- Separate timeouts for master vs. slave halves.

## Decisions

### Track input inactivity in `oled_task_user()`, not via `OLED_TIMEOUT`

`OLED_TIMEOUT` is not usable here because our render loop resets the OLED activity timer every frame. Instead:

1. In `oled_task_user()`, call `last_input_activity_elapsed()` (QMK's built-in input-idle counter, available on both halves via `SPLIT_ACTIVITY_ENABLE`).
2. If elapsed time ≥ 300 000 ms, call `oled_off()` and return `false` without writing anything — the display powers down.
3. Otherwise, call `oled_on()` to ensure the display is on (it may have just woken from sleep), then render normally.

QMK's input pipeline already calls `oled_on()` on wakeup, so no additional wakeup hook is needed — but the explicit `oled_on()` guard in the render path ensures the screen comes back even if the wakeup path races.

**Alternative considered**: `OLED_TIMEOUT` with a custom no-op render path when idle. Rejected because it requires a separate idle flag anyway, making the approach equivalent but more convoluted.

### Enable `SPLIT_ACTIVITY_ENABLE` in keymap `config.h`

`last_input_activity_elapsed()` requires `SPLIT_ACTIVITY_ENABLE` to be defined so the master's input-idle timestamp is synced to the slave. Without it, the slave always sees 0 ms elapsed and never blanks. This define goes in the same keymap-level `config.h` files as other split sync flags (`SPLIT_LAYER_STATE_ENABLE`, `SPLIT_MODS_ENABLE`, etc.).

### Implement idle check in `oled_bitmap.c`, not in `keymap_common.h`

The OLED render logic lives in `oled_bitmap.c`. Placing the idle guard there keeps the keymap header clean and ensures the same behaviour regardless of which keymap includes it.

## Risks / Trade-offs

- **Slave wakeup latency**: The slave OLED is synced via the split transport; there is a small (~1 split cycle, ≤5 ms) lag before the slave blanks or wakes relative to the master. Imperceptible in practice.
- **`last_input_activity_elapsed()` availability**: This function is part of QMK's activity module (`quantum/activity.c`). It is always compiled in when `SPLIT_ACTIVITY_ENABLE` is defined; no extra `rules.mk` entry is needed.

## Context

The master OLED renders a 32×128 px bitmap display via `oledkit_render_info_user()` in `oled_bitmap.c`. Existing indicator bitmaps are 16×16 px, drawn by `draw_bitmap()` at fixed coordinates. The zone y=60–75 is currently blank (unoccupied by the base image or any overlay). Prerendered PNG assets for both new indicators already exist in `keyball_display/`: `glyph_swaphands_16x16.png` and `glyph_record_16x16.png`.

## Goals / Non-Goals

**Goals:**
- Show a swap-hands active indicator at x=0–15, y=60–75 on the master OLED
- Show dynamic macro recording indicators at y=60–75 on the master OLED: slot 1 at x=0–15, slot 2 at x=16–31
- Reuse the existing `draw_bitmap()` primitive and 16×16 glyph convention
- Add the two new bitmap arrays to `oled_bitmap_data.h` via `png_to_c.py`

**Non-Goals:**
- Changes to the slave OLED
- Changes to non-`via` keymaps
- Playback indicators (only recording state is shown)

## Decisions

**Swap-hands state detection** — Use `is_swap_hands_on()` (QMK built-in) to query current swap state. No extra state variable needed.

**Dynamic macro recording state detection** — QMK exposes `dynamic_macro_record_start_user()` and `dynamic_macro_record_end_user()` callbacks, but no direct query function. A module-level `static uint8_t dynmacro_recording` flag (0 = idle, 1 = slot 1, 2 = slot 2) will track state via these callbacks in `oled_bitmap.c`.

**Layout — two non-overlapping 16×16 slots in y=60–75:**
- x=0–15: swap-hands indicator (when swap is on)
- x=16–31: dynamic macro recording indicator (when any slot is recording)

**Bitmap data** — Convert `glyph_swaphands_16x16.png` and `glyph_record_16x16.png` to C arrays using the existing `png_to_c.py` script, appending them to `oled_bitmap_data.h` as `bmp_swaphands` and `bmp_record`.

## Risks / Trade-offs

- No overlap: swap-hands (x=0–15) and recording (x=16–31) occupy distinct cells.
- [Dynamic macro callbacks in oled_bitmap.c] Placing `dynamic_macro_record_start/end_user` in the bitmap module keeps all OLED state co-located, but the callbacks have `__attribute__((weak))` linkage so any keymap override would shadow them. Acceptable given no keymap currently defines these callbacks.

## Migration Plan

1. Convert PNGs to C arrays, append to `oled_bitmap_data.h`
2. Add `dynmacro_recording` flag and callbacks to `oled_bitmap.c`
3. Add indicator draw calls in `oledkit_render_info_user()`
4. Build and flash to verify visually

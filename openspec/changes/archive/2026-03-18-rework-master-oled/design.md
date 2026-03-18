## Context

The Keyball64 master OLED is a 128×32 SSD1306 display driven by QMK's OLED subsystem at `OLED_ROTATION_270`, which presents a logical canvas of 32px wide × 128px tall in code. The existing renderer writes fixed-width text strings using QMK's character-cell API (`oled_write_P`), which limits layout fidelity and produces a generic appearance. The goal is to replace this with a custom pixel-accurate renderer that composites a hand-designed base image with bitmap glyphs drawn at specific bounding boxes.

All pixel work uses QMK's `oled_write_pixel(x, y, on)` API, which internally handles coordinate transformation for the configured rotation.

## Goals / Non-Goals

**Goals:**
- Render a persistent base display image (the `display_base_master.png` layout) every frame on the master OLED
- Composite digit glyphs for layer number and WPM into their respective bounding boxes, horizontally centered
- Composite 16×16 modifier/lock glyphs (Caps Lock, Num Lock, Shift, Control, Super, Alt) into their fixed bounding boxes based on live keyboard state
- Keep all bitmap data in `PROGMEM` to avoid consuming RAM
- Keep all changes confined to the default keymap (`keymaps/default/`)

**Non-Goals:**
- Changing the slave-side OLED renderer
- Changing the `via` keymap
- Adding animation or scrolling
- Supporting dynamic resizing or user-configurable layout

## Decisions

### D1: Use `oled_write_pixel` for all rendering

**Decision**: Render every pixel — base image, digit glyphs, and indicator glyphs — via `oled_write_pixel(x, y, on)`.

**Rationale**: `oled_write_pixel` works in the logical rotated coordinate space, so coordinates in code match the bounding boxes specified in the design. Using `oled_write_raw_P` would require computing raw buffer offsets in the pre-rotation coordinate system, which is error-prone and couples the code to the internal buffer layout.

**Alternative considered**: Write the base image with `oled_write_raw_P` (one bulk copy) and only overlay glyphs with `oled_write_pixel`. Rejected because it adds complexity without meaningful performance gain — the OLED is refreshed at a low rate and 32×128 = 4096 `oled_write_pixel` calls per frame is negligible.

---

### D2: Bitmap data format — row-major `uint8_t` with explicit width

**Decision**: Store each glyph as a flat `PROGMEM uint8_t` array of bytes where each byte encodes one row of pixels, MSB = leftmost pixel. For glyphs wider than 8px (all glyphs in this design are 9–16px wide), use two bytes per row.

- Digit glyphs (9×15): `uint8_t[15 * 2]` — 30 bytes each, 10 glyphs = 300 bytes
- Indicator glyphs (16×16): `uint8_t[16 * 2]` — 32 bytes each, 6 glyphs = 192 bytes
- Base image (32×128): `uint8_t[128 * 4]` — 512 bytes

Total PROGMEM: ~1004 bytes. Well within RP2040 flash capacity.

**Rationale**: Row-major with fixed byte width is easy to read, edit, and visually verify against the source PNGs. Packing into `uint16_t` per row would also work but requires endianness care; two separate bytes per row is unambiguous.

**Rendering helper**:
```c
static void draw_bitmap(uint8_t x0, uint8_t y0,
                         const uint8_t *data, uint8_t w, uint8_t h) {
    uint8_t bytes_per_row = (w + 7) / 8;
    for (uint8_t row = 0; row < h; row++) {
        for (uint8_t col = 0; col < w; col++) {
            uint8_t byte_idx = row * bytes_per_row + (col / 8);
            uint8_t bit      = 7 - (col % 8);
            bool    on       = (pgm_read_byte(&data[byte_idx]) >> bit) & 1;
            oled_write_pixel(x0 + col, y0 + row, on);
        }
    }
}
```

---

### D3: Digit centering

**Decision**: Center digits horizontally within their bounding box using the formula:

```
total_w = n_digits * 9 + (n_digits - 1) * 2
x_start = box_x + (box_width - total_w) / 2
```

For values requiring more than 3 digits (≥1000), truncate to 3 digits (value mod 1000).

Digit glyph sizes are 9px wide × 15px tall. The digit bounding boxes are 32px wide × 15px tall, so:
- 1 digit: total_w = 9 → x_start = 11 (centered)
- 2 digits: total_w = 20 → x_start = 6 (centered)
- 3 digits: total_w = 31 → x_start = 0 (fits with 1px margin)

All cases fit within the 32px bounding box with no clipping. The 15px glyph height matches the bounding box height exactly, so y_start = box_y with no vertical offset.

**Rationale**: The 9×15 glyph dimensions were chosen to fill the bounding boxes correctly at all practical values (layer ≤999, WPM ≤999).

---

### D4: Code organization — single keymap file

**Decision**: Place all bitmap data and rendering logic in `keymaps/default/keymap.c`. No new files.

**Rationale**: The bitmap data is exclusively used by the master OLED render function in the default keymap. Creating a separate header/source adds file overhead without enabling reuse. If other keymaps need this in the future, extracting to a shared lib is straightforward.

## Risks / Trade-offs

- **Frame rate**: Repainting 4096 + glyph pixels each `oled_task_user` call is slower than the current text approach. In practice, QMK's OLED task runs at ~10 Hz and the RP2040 has ample cycles, so this is unlikely to cause visible lag. Risk: low.

- **Bitmap accuracy**: The byte arrays must be manually or programmatically derived from the PNG files. Any transcription error produces incorrect pixels. Mitigation: derive arrays using a script during implementation and cross-check visually on hardware.

## Open Questions

_(none — implementation can proceed)_

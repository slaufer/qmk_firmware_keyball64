## Context

The slave OLED is currently rendered in `oled_task_user()` in `oled_bitmap.c` via character-cell text (`oled_write_P`). It shows WPM, held-key characters, and text modifier rows. The master OLED was reworked to use pixel-accurate bitmap compositing; the slave still uses the old approach, making the two halves inconsistent in both code structure and visual style.

The `display_base_slave.png` is intentionally blank — the slave display for now shows only modifier glyphs composited onto an empty canvas.

## Goals / Non-Goals

**Goals:**
- Replace the text-based slave rendering with bitmap compositing using the same `draw_bitmap()` infrastructure as the master
- Add a `bmp_base_slave` PROGMEM array from `display_base_slave.png`
- Render modifier glyphs (Shift, Control, Super, Alt) at the same bounding boxes as the master, using `is_keyboard_left()` for side-aware detection
- Refresh digit glyph PROGMEM arrays in `oled_bitmap.c` from their updated source PNGs

**Non-Goals:**
- Adding new non-modifier content to the slave (WPM, held keys, layer number) — the base is blank by design; future content is out of scope
- Changing master OLED rendering behavior

## Decisions

### Single `oled_task_user()` controls both halves (unchanged structure)

The existing pattern — `is_keyboard_master()` gate in `oled_task_user()`, with `oledkit_render_info_user()` for the master branch — is kept. The slave branch is replaced with bitmap rendering inline (no new weak function), since the slave content is currently minimal and a dedicated override hook is not needed.

**Alternative considered:** a `oledkit_render_slave_user()` weak function mirroring the master pattern. Rejected for now; the slave has no content requiring keymap override, and adding a hook before it's needed is premature.

### `bmp_base_slave` added as a separate PROGMEM array

The slave base image is a distinct 32×128 bitmap from the master base. It is stored as `bmp_base_slave[512]` in `oled_bitmap.c`, alongside `bmp_base[512]`.

### `held_display` extern is retained but no longer used by the slave branch

The `held_display[6]` extern (defined in each keymap) was previously referenced in the slave text rows. After this change, neither the master nor slave rendering references it. It is left defined in keymap.c files for now (it does no harm) and can be cleaned up in a separate refactor.

### Digit glyph update is a data-only change

The digit glyphs (9px wide × 15px tall) are regenerated from their source PNGs using the existing `keyball_display/png_to_c.py` script. The spec and rendering code are unchanged; only the byte values in the `bmp_digits` array are updated.

## Risks / Trade-offs

- **Blank slave display** → the slave will show only modifier indicators on an empty background; no other content. This is intentional but may be surprising to anyone expecting WPM or held keys. Mitigation: documented in spec.
- **`held_display` dead code** → the extern and the array in keymap.c files become unused after this change. Low risk; the compiler will not error, just potentially warn on `held_display` being defined but unreferenced. A follow-up cleanup can remove it if desired.

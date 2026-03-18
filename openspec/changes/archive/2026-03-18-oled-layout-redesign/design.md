## Context

The Keyball64 OLEDs are 128×32 SSD1306 panels mounted 90° CCW, rendered with `OLED_ROTATION_270`. This produces a logical canvas of **5 chars wide × 16 lines tall** (32px ÷ 6px/char = 5; 128px ÷ 8px/line = 16).

The previous layout placed two modifier indicators per row (e.g., WIN+CTRL), which was too wide and awkward. The redesign gives each indicator its own row, aligns labels to 5 chars with leading spaces for visual weight, and adds a held-key display on the slave half.

Labels wider than 5 chars in the user-facing spec are abbreviated to fit: "CAPSLK" → "CAPLK", "NUMLK" → "NUMLK" (already 5), "SUPER" → "SUPER" (already 5). All full-row inverted indicators are padded with trailing spaces to fill all 5 columns.

## Goals / Non-Goals

**Goals:**
- One indicator per row on both halves
- Full-row inversion (5-char padded) for active modifier and lock indicators
- Held non-modifier keys displayed on the slave (up to 5 chars)
- Labels aligned with leading spaces to match user-specified visual layout

**Non-Goals:**
- Dynamic font sizes or custom bitmaps
- Held-key display on the master
- Showing held keys from both halves simultaneously (slave half only for held keys)

## Decisions

### D1: Held-key tracking via `process_record_user` on master, synced to slave

**Chosen**: Maintain a `held_keys[5]` char buffer on the master, updated in `process_record_user`. Sync it to the slave using QMK's custom split transaction mechanism (`SPLIT_TRANSACTION_IDS_KB` + `transaction_rpc_exec`).

**Rationale**: `process_record_user` sees all key events regardless of which half they originate from. The slave has no visibility into the master's key state without explicit sync. A 5-byte custom transaction is trivially small and runs on the existing split transport.

**Alternative considered**: Read the slave's own matrix directly with `matrix_get_row()`. Simpler, but only shows keys held on the slave's physical half — unhelpful when both hands are active.

**Alternative considered**: Use `get_last_keycode()`. Only gives the most recently pressed key, not all currently held keys.

### D2: 5-char padded full-row inversion

**Chosen**: When an indicator is active, write a 5-char string (label + trailing spaces to fill the row) with `oled_write_P(..., true)`. When inactive, write nothing (advance the page with a blank line).

**Rationale**: Full-row inversion requires filling all 5 columns. Writing the label alone (e.g., 4-char "CTRL") leaves 1 column uninverted, creating a visual gap. Padding to 5 chars closes this.

### D3: Keycode → display character mapping

**Chosen**: Map held keycodes to printable ASCII for the held-key row. `KC_A`–`KC_Z` → `'A'`–`'Z'`, `KC_1`–`KC_9` → `'1'`–`'9'`, `KC_0` → `'0'`. Other keycodes render as `'?'`. Skip modifier keycodes entirely.

**Rationale**: The held-key display is a quick visual confirmation of what's pressed, not a full keycode dump. Printable ASCII covers the common case. `'?'` for unrecognized keys is a safe fallback that avoids silent gaps in the buffer.

### D4: Buffer capacity of 5 held keys

**Chosen**: Track up to 5 simultaneously held non-modifier keys.

**Rationale**: Matches the display width exactly (5 chars). More than 5 simultaneous non-modifier holds is not a realistic typing scenario. Excess keys are silently ignored (oldest-first eviction if buffer is full).

## Risks / Trade-offs

- **Split sync latency**: Custom split transactions add ~1 scan cycle of latency to held-key display on the slave. At typical scan rates (>100 Hz) this is imperceptible.
- **5-char label truncation**: "CAPSLK" becomes "CAPLK" — familiar enough to be recognizable but slightly non-standard. "CAPLK" could be confused with "CAP LOCK" abbreviation from other contexts. Acceptable trade-off given the hard width constraint.
- **Held-key sync overhead**: 5 extra bytes per split transport cycle. Negligible on RP2040.

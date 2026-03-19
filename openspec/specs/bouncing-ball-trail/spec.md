### Requirement: Masked bitmap draw primitive
`oled_bitmap.c` SHALL expose a `draw_bitmap_masked()` function that composites a PROGMEM bitmap onto the OLED using a PROGMEM mask of the same dimensions. For each pixel, if the corresponding mask bit is ON the pixel value from the image data SHALL be written to the display; if the mask bit is OFF the pixel SHALL be skipped (leaving the background unchanged).

#### Scenario: Mask ON pixel is written
- **WHEN** `draw_bitmap_masked()` is called and the mask bit for pixel (col, row) is ON
- **THEN** the pixel at (x0+col, y0+row) on the OLED SHALL be set to the value from the image data

#### Scenario: Mask OFF pixel is skipped
- **WHEN** `draw_bitmap_masked()` is called and the mask bit for pixel (col, row) is OFF
- **THEN** `oled_write_pixel()` SHALL NOT be called for (x0+col, y0+row)

### Requirement: Ball position history tracking
`oled_bitmap.c` SHALL maintain a 3-element history of ball positions. Before each ball position update (in `oled_ball_on_key_press()`), the current position SHALL be shifted into index 0 of the history, displacing older entries toward higher indices. Index 0 is the most recent previous position (closest to the ball), index 2 is the oldest (farthest from the ball).

#### Scenario: History shifts on key press
- **WHEN** `oled_ball_on_key_press()` is called
- **THEN** `hist_x[2] ← hist_x[1]`, `hist_x[1] ← hist_x[0]`, `hist_x[0] ← ball_x` (and same for y) SHALL occur before the ball position is updated

#### Scenario: Initial history state
- **WHEN** firmware initializes
- **THEN** all three history positions SHALL be (0, 0)

### Requirement: Ball history exposed via public API
`oled_bitmap.h` SHALL declare functions `oled_ball_get_history()` and `oled_ball_set_history()` allowing the keymap to read and overwrite all three history positions for split-side synchronization.

#### Scenario: Master reads history
- **WHEN** `oled_ball_get_history()` is called on the master
- **THEN** the caller receives the current values of `hist_x[0..2]` and `hist_y[0..2]`

#### Scenario: Slave writes history
- **WHEN** `oled_ball_set_history()` is called on the slave with synced data
- **THEN** `hist_x[0..2]` and `hist_y[0..2]` SHALL be overwritten with the provided values

### Requirement: BALL_POS_SYNC carries full position history
The `BALL_POS_SYNC` RPC payload SHALL be expanded from 2 bytes to 8 bytes: `{ball_x, ball_y, hist_x[0], hist_y[0], hist_x[1], hist_y[1], hist_x[2], hist_y[2]}`. The slave handler SHALL accept only payloads of exactly 8 bytes and apply all four positions.

#### Scenario: Master sends 8-byte payload
- **WHEN** `housekeeping_task_user()` executes on the master
- **THEN** `transaction_rpc_exec(BALL_POS_SYNC, 8, ...)` SHALL be called with the current ball position followed by the three history positions

#### Scenario: Slave rejects wrong-sized payload
- **WHEN** the slave handler receives a payload with `in_buflen != 8`
- **THEN** no ball state SHALL be modified

### Requirement: WPM-gated trail segment count
The slave OLED renderer SHALL compute a trail segment count from the synced WPM value before rendering:
- WPM 0–30 → trailcount = 0
- WPM 31–60 → trailcount = 1
- WPM 61–90 → trailcount = 2
- WPM 91+ → trailcount = 3

#### Scenario: No trail at low WPM
- **WHEN** `get_current_wpm()` returns a value ≤ 30
- **THEN** no trail segments SHALL be rendered

#### Scenario: Full trail at high WPM
- **WHEN** `get_current_wpm()` returns a value ≥ 91
- **THEN** all three trail segments SHALL be rendered

#### Scenario: Partial trail at mid WPM
- **WHEN** `get_current_wpm()` returns 45
- **THEN** exactly 1 trail segment SHALL be rendered (hist[0])

### Requirement: Trail segments rendered back-to-front with correct mask sets
For each visible trail segment (0 to trailcount−1 rendered from highest index down to 0), the slave OLED SHALL render `glyph_ball_trail_8x8.png` at `hist[i]` composited through a randomly selected mask from the mask set for that segment's distance rank. The mask set is determined by `i + (3 − trailcount)` so that the farthest visible segment always uses trail3 masks:
- trailcount=1: `hist[0]` → trail3 masks
- trailcount=2: `hist[1]` → trail3, `hist[0]` → trail2
- trailcount=3: `hist[2]` → trail3, `hist[1]` → trail2, `hist[0]` → trail1

Render order SHALL be from highest visible index to lowest (oldest first), so that closer segments paint over more distant ones.

#### Scenario: Render order preserves closer segments
- **WHEN** trailcount = 3
- **THEN** hist[2] is rendered first, then hist[1], then hist[0], then the ball glyph

#### Scenario: Random mask variant selected per segment
- **WHEN** a trail segment is rendered
- **THEN** the mask index SHALL be selected pseudo-randomly (selecting mask variant 1–4)

#### Scenario: Trail drawn below ball
- **WHEN** trailcount > 0
- **THEN** all trail segments SHALL be rendered before the ball glyph so the ball appears on top

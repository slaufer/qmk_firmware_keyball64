## Context

The slave OLED currently renders a plain `bmp_ball` glyph using `draw_bitmap()` which writes every pixel verbatim, including the rectangular background. The bouncing-ball animation is driven on the master (position advances on key press), and the current (x, y) is synced to the slave via a 2-byte `BALL_POS_SYNC` RPC. WPM is already synced via `SPLIT_WPM_ENABLE`, so `get_current_wpm()` is valid on the slave.

All rendering logic lives in `keyboards/keyball64/lib/oled_bitmap/oled_bitmap.c`.

## Goals / Non-Goals

**Goals:**
- Add `draw_bitmap_masked()` — a compositing primitive that respects a PROGMEM mask when writing pixels.
- Apply a mask to the ball glyph so its rectangular bounding box does not overwrite neighboring pixels.
- Track a 3-position history of ball positions in `oled_bitmap.c` and expose it through the public API.
- Render 0–3 trail segments behind the ball, with segment count gated on WPM tier.
- Use a random variant (1-of-4) of the trail mask for each trail segment.
- Expand the `BALL_POS_SYNC` RPC payload to carry all 4 positions (current + 3 history) to the slave.

**Non-Goals:**
- Changing the ball's physics (step size, bounds, collision rules).
- Animating the trail on a timer — trail only updates when the ball moves (key press).
- Persisting trail positions across power cycles.

## Decisions

### 1. `draw_bitmap_masked()` signature

```c
void draw_bitmap_masked(uint8_t x0, uint8_t y0,
                        const uint8_t *data, const uint8_t *mask,
                        uint8_t w, uint8_t h);
```

Iterates every pixel; when the corresponding mask bit is ON the pixel from `data` is written to the OLED; when OFF the pixel is skipped entirely (background preserved). Both `data` and `mask` are row-major PROGMEM arrays with `(w+7)/8` bytes per row.

**Alternative considered**: a single combined "transparent color" approach (skip black pixels). Rejected because the ball and trail bitmaps may contain intentional black pixels inside the glyph.

### 2. Trail history as a shift register

`ball_hist[3]` is a plain array of `{uint8_t x; uint8_t y;}` structs stored as parallel `ball_hist_x[3]` / `ball_hist_y[3]` arrays (consistent with the existing `ball_x`/`ball_y` style). On each key press, before applying the step, shift hist right: `hist[2] ← hist[1] ← hist[0] ← (ball_x, ball_y)`.

Index 0 = most recent previous position (closest trail), index 2 = oldest (farthest trail).

**Alternative considered**: ring buffer with a head pointer. Rejected for added complexity with only 3 slots.

### 3. Mask-set assignment

The farthest *visible* segment always uses trail3 masks, regardless of trailcount. The mask set index is `i + (3 − trailcount)` where `i` is the history index:

| trailcount | hist[0] | hist[1] | hist[2] |
|------------|---------|---------|---------|
| 1          | trail3  | —       | —       |
| 2          | trail2  | trail3  | —       |
| 3          | trail1  | trail2  | trail3  |

**Rationale**: trail3 is the most diffuse/faint mask set; it should always mark the outermost visible boundary of the trail regardless of how long the trail is.

### 4. Random mask variant at render time

`random8() % 4` selects the variant (0–3) for each trail segment at slave render time. This is called once per trail segment per frame, producing frame-to-frame variation that animates the trail's texture without any additional sync overhead.

**Alternative considered**: selecting the variant on the master at key-press time and syncing it. Rejected because this adds sync payload without visible benefit — the slave already has `random8()` available.

### 5. Render order (back to front)

Trail segments are rendered in order from oldest to newest, then the ball on top:
`hist[2]` → `hist[1]` → `hist[0]` → ball. This ensures closer segments and the ball always paint over more distant ones.

### 6. Expand `BALL_POS_SYNC` payload (2 → 8 bytes)

The existing `ball_pos_slave_handler` checks `in_buflen == 2`; we change this to `== 8` and send `{ball_x, ball_y, hist_x[0], hist_y[0], hist_x[1], hist_y[1], hist_x[2], hist_y[2]}`. QMK split RPC supports payloads up to 32 bytes; 8 bytes is within budget. No new transaction ID is needed.

**Alternative considered**: a separate `BALL_TRAIL_SYNC` transaction. Rejected because it adds a new config symbol and a second per-cycle RPC call for data that is tightly coupled to the existing ball position sync.

## Risks / Trade-offs

- **Stale trail on init**: All history slots initialize to (0, 0) matching the ball's start position. With trailcount 0 at startup this is invisible; if WPM somehow starts non-zero the trail will briefly cluster at the origin. Acceptable — clears after a few keypresses.
- **`random8()` entropy at startup**: Early frames use whatever the PRNG state is post-reset. Visually fine.
- **Increased PROGMEM**: 13 new 8×8 bitmaps = 13 × 8 = 104 bytes. Negligible on RP2040.
- **Payload size change breaks existing slave firmware**: If master and slave are flashed separately, the old slave handler will silently reject the 8-byte payload (`in_buflen == 2` check). Both halves must be re-flashed together.

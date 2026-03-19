## MODIFIED Requirements

### Requirement: Ball advances on key press
On each physical key press event (any key, any half), the ball position SHALL advance diagonally by 2 pixels in both the current horizontal and vertical direction.

**REMOVED** — key-press triggering is replaced by the WPM-driven timer in the `wpm-driven-ball-animation` capability. See REMOVED Requirements below.

## REMOVED Requirements

### Requirement: Ball advances on key press
**Reason**: Replaced by WPM-driven continuous animation. The ball now advances on a timer in `housekeeping_task_user()` rather than on key-press events, so that the animation is continuous and does not require active typing.
**Migration**: Remove the `oled_ball_on_key_press()` call from `process_record_user()`. The `process_record_user()` function in `keymap_common.h` may be removed entirely if it contains no other logic.

### Requirement: Public key-press notification API
**Reason**: The function `oled_ball_on_key_press()` is renamed to `oled_ball_tick()` to accurately reflect that it is now called by a timer, not a key-press handler. The declaration and definition are updated accordingly.
**Migration**: Replace all calls to `oled_ball_on_key_press()` with `oled_ball_tick()`. Update the declaration in `oled_bitmap.h` and the definition in `oled_bitmap.c`.

### Requirement: Ball position/history sync accessor API
**Reason**: `oled_ball_get_pos()`, `oled_ball_set_pos()`, `oled_ball_get_history()`, and `oled_ball_set_history()` existed solely to allow the master to read and the slave to write ball state over the `BALL_POS_SYNC` RPC. With ball state owned entirely by the slave and the custom sync removed, these functions serve no purpose.
**Migration**: Remove all four declarations from `oled_bitmap.h` and their definitions from `oled_bitmap.c`.

## ADDED Requirements

### Requirement: Public tick API
A function `oled_ball_tick()` SHALL be declared in `oled_bitmap.h` and defined in `oled_bitmap.c`. It SHALL perform the same position-advance logic as the former `oled_ball_on_key_press()`: shift the history buffer, compute the new position with bounce, and update `ball_x`/`ball_y`.

#### Scenario: Tick advances ball position
- **WHEN** `oled_ball_tick()` is called
- **THEN** ball_x SHALL change by ball_dx and ball_y SHALL change by ball_dy (with clamping and direction reversal at bounds)

#### Scenario: Tick shifts history
- **WHEN** `oled_ball_tick()` is called
- **THEN** the history entries SHALL be shifted: index 2 ← index 1 ← index 0 ← current position before the move

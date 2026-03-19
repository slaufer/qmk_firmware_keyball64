## Why

The slave OLED display is currently blank except for modifier key indicators, leaving significant screen real estate unused. Adding a bouncing ball animation gives the display visual life and provides satisfying feedback that key presses are being registered on the slave half.

## What Changes

- Add an 8×8 pixel ball bitmap to the slave OLED display
- Ball bounces diagonally within a 32×96 pixel bounding box (x 0–31, y 0–95)
- Ball advances by 2 pixels diagonally on each key press
- Ball initially travels down-right; direction reverses on edge collision
- Both horizontal and vertical simultaneous collisions are handled correctly

## Capabilities

### New Capabilities

- `bouncing-ball`: Diagonal bouncing ball animation on the slave OLED display, advancing on each key press with edge collision detection

### Modified Capabilities

- (none)

## Impact

- `keyboards/keyball64/lib/oled_bitmap/oled_bitmap.c`: Add ball bitmap data, ball state variables, collision/movement logic, and rendering in the slave OLED branch of `oled_task_user()`
- `keyboards/keyball64/lib/oled_bitmap/oled_bitmap.h`: Expose any required public API for notifying key press events (if needed)
- Both `default` and `via` keymaps may need to call a ball update function from `process_record_user()` to advance the ball on key press

### Requirement: Ball bitmap rendered on slave OLED
The slave OLED display SHALL render an 8×8 pixel ball glyph within the bounding box x 0–31, y 0–95 on every OLED frame. The ball SHALL be rendered using `draw_bitmap_masked()` with `bmp_ball` as the image and `bmp_ball_mask` as the mask, so that pixels outside the circular ball shape do not overwrite the background.

#### Scenario: Ball visible on slave display
- **WHEN** the slave OLED task runs
- **THEN** the 8×8 ball bitmap SHALL be drawn at the current ball position using `draw_bitmap_masked()` with the ball mask

#### Scenario: Ball mask preserves background pixels
- **WHEN** the ball is rendered at any position
- **THEN** pixels where `bmp_ball_mask` is OFF SHALL NOT be written, leaving surrounding pixels unchanged

#### Scenario: Ball does not overlap modifier area
- **WHEN** the ball is at any valid position
- **THEN** the ball top-left y coordinate SHALL never exceed 87 (ensuring the ball stays within y 0–95)

### Requirement: Ball initial direction
The ball SHALL start at position (0, 0) and travel toward the bottom-right corner (dx = +2, dy = +2).

#### Scenario: Initial state
- **WHEN** the firmware initializes
- **THEN** ball_x SHALL be 0, ball_y SHALL be 0, ball_dx SHALL be +2, ball_dy SHALL be +2

### Requirement: Horizontal edge collision
When the ball would move outside the horizontal bounds (x < 0 or x > 23), the horizontal direction SHALL be reversed and the ball SHALL be clamped to the valid range.

#### Scenario: Right wall collision
- **WHEN** applying dx would place ball_x > 23
- **THEN** dx SHALL be negated and ball_x SHALL be clamped to 23

#### Scenario: Left wall collision
- **WHEN** applying dx would place ball_x < 0
- **THEN** dx SHALL be negated and ball_x SHALL be clamped to 0

### Requirement: Vertical edge collision
When the ball would move outside the vertical bounds (y < 0 or y > 87), the vertical direction SHALL be reversed and the ball SHALL be clamped to the valid range.

#### Scenario: Bottom wall collision
- **WHEN** applying dy would place ball_y > 87
- **THEN** dy SHALL be negated and ball_y SHALL be clamped to 87

#### Scenario: Top wall collision
- **WHEN** applying dy would place ball_y < 0
- **THEN** dy SHALL be negated and ball_y SHALL be clamped to 0

### Requirement: Corner collision
When the ball collides with both a horizontal and a vertical boundary simultaneously, both dx and dy SHALL be reversed independently.

#### Scenario: Corner hit reverses both directions
- **WHEN** applying the current delta would violate both the x and y bounds simultaneously
- **THEN** both dx and dy SHALL be negated and both coordinates SHALL be clamped

### Requirement: Public tick API
A function `oled_ball_tick()` SHALL be declared in `oled_bitmap.h` and defined in `oled_bitmap.c`. It SHALL perform the same position-advance logic as the former `oled_ball_on_key_press()`: shift the history buffer, compute the new position with bounce, and update `ball_x`/`ball_y`.

#### Scenario: Tick advances ball position
- **WHEN** `oled_ball_tick()` is called
- **THEN** ball_x SHALL change by ball_dx and ball_y SHALL change by ball_dy (with clamping and direction reversal at bounds)

#### Scenario: Tick shifts history
- **WHEN** `oled_ball_tick()` is called
- **THEN** the history entries SHALL be shifted: index 2 ← index 1 ← index 0 ← current position before the move

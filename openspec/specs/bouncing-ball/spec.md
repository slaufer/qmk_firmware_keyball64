### Requirement: Ball bitmap rendered on slave OLED
The slave OLED display SHALL render an 8×8 pixel ball glyph within the bounding box x 0–31, y 0–95 on every OLED frame.

#### Scenario: Ball visible on slave display
- **WHEN** the slave OLED task runs
- **THEN** the 8×8 ball bitmap SHALL be drawn at the current ball position

#### Scenario: Ball does not overlap modifier area
- **WHEN** the ball is at any valid position
- **THEN** the ball top-left y coordinate SHALL never exceed 87 (ensuring the ball stays within y 0–95)

### Requirement: Ball advances on key press
On each physical key press event (any key, any half), the ball position SHALL advance diagonally by 2 pixels in both the current horizontal and vertical direction.

#### Scenario: Ball moves on press
- **WHEN** a key press event fires (`record->event.pressed == true`)
- **THEN** ball x SHALL change by dx and ball y SHALL change by dy

#### Scenario: Ball does not move on key release
- **WHEN** a key release event fires (`record->event.pressed == false`)
- **THEN** ball position SHALL remain unchanged

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

### Requirement: Public key-press notification API
A function `oled_ball_on_key_press()` SHALL be declared in `oled_bitmap.h` and defined in `oled_bitmap.c`, callable from `process_record_user()` in keymap files.

#### Scenario: Keymaps call the function
- **WHEN** `process_record_user()` receives a press event
- **THEN** it SHALL call `oled_ball_on_key_press()`

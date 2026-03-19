## MODIFIED Requirements

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

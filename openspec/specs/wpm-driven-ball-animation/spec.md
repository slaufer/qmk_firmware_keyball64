### Requirement: Ball advances on WPM-driven timer
The slave SHALL advance the ball position on a timer whose interval is derived from the current WPM. The interval SHALL be `(wpm >= 90) ? 50 : 500 - wpm * 5` milliseconds. When WPM is 0, the ball SHALL NOT advance.

#### Scenario: Ball moves at low WPM
- **WHEN** WPM is 30
- **THEN** the ball SHALL advance approximately once every 350 ms

#### Scenario: Ball moves at high WPM
- **WHEN** WPM is 90 or above
- **THEN** the ball SHALL advance approximately once every 50 ms

#### Scenario: Ball stops at zero WPM
- **WHEN** WPM is 0
- **THEN** the ball SHALL NOT advance (timer tick is skipped)

#### Scenario: Ball speed increases as WPM increases
- **WHEN** WPM increases from a lower value to a higher value
- **THEN** the step interval SHALL decrease, causing the ball to move more frequently

### Requirement: Timer is managed on slave in housekeeping task
The WPM-driven ball timer SHALL be implemented as a `static uint16_t` in `housekeeping_task_user()`, evaluated only on the slave half (guarded by `is_keyboard_slave()`). `oled_ball_tick()` SHALL be called when the elapsed time exceeds the computed interval.

#### Scenario: Timer fires on slave only
- **WHEN** `housekeeping_task_user()` runs on the slave
- **THEN** the ball timer SHALL be evaluated and `oled_ball_tick()` called when due

#### Scenario: Timer does not run on master
- **WHEN** `housekeeping_task_user()` runs on the master half
- **THEN** no ball timer evaluation SHALL occur

### Requirement: Custom ball-sync infrastructure is removed
The `BALL_POS_SYNC` RPC, its slave handler, its transaction registration, and the `oled_ball_get_pos` / `oled_ball_set_pos` / `oled_ball_get_history` / `oled_ball_set_history` accessors SHALL be removed. WPM is available on the slave via the built-in `SPLIT_WPM_ENABLE` mechanism and requires no custom sync.

#### Scenario: No custom RPC registered
- **WHEN** the firmware initializes
- **THEN** `transaction_register_rpc(BALL_POS_SYNC, ...)` SHALL NOT be called

### Requirement: Continuous animation without key presses
The ball SHALL animate continuously regardless of whether any keys are being pressed, as long as WPM is non-zero.

#### Scenario: Ball animates during a typing pause
- **WHEN** no keys have been pressed for up to 5 seconds (WPM still non-zero due to decay)
- **THEN** the ball SHALL continue to advance on its timer interval

#### Scenario: Ball animates at firmware startup
- **WHEN** the firmware starts and no keys have been pressed
- **THEN** the ball SHALL remain stationary (WPM is 0 at startup)

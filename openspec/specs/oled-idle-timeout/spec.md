### Requirement: Both OLEDs blank after 5 minutes of input inactivity
Both halves' OLED displays SHALL power off after 300 000 ms (5 minutes) of continuous keyboard and pointing-device inactivity. The idle check SHALL use `last_input_activity_elapsed()` inside `oled_task_user()`; when the elapsed time meets or exceeds the threshold, `oled_off()` SHALL be called and the render pass SHALL be skipped. `SPLIT_ACTIVITY_ENABLE` SHALL be defined so the master's idle timestamp is synced to the slave.

#### Scenario: Screens blank after idle period
- **WHEN** no key is pressed, no encoder is turned, and no mouse movement occurs for 5 minutes
- **THEN** both OLED displays turn off

#### Scenario: Screens wake on keypress
- **WHEN** any key is pressed while the OLEDs are blanked
- **THEN** both OLED displays turn back on within one split-transport cycle

#### Scenario: Screens wake on mouse movement
- **WHEN** the trackball is moved while the OLEDs are blanked
- **THEN** both OLED displays turn back on within one split-transport cycle

#### Scenario: Idle timer resets on activity
- **WHEN** a keypress or mouse movement occurs before the 5-minute timeout expires
- **THEN** the idle timer resets and the 5-minute countdown begins again

#### Scenario: Active rendering does not prevent blanking
- **WHEN** the keyboard has been idle for 5 minutes
- **THEN** the OLEDs blank even though `oled_task_user()` continues to be called each frame

### Requirement: Timeout applies to both via and default keymaps
`SPLIT_ACTIVITY_ENABLE` SHALL be defined in both `keymaps/via/config.h` and `keymaps/default/config.h`.

#### Scenario: via keymap builds with split activity sync
- **WHEN** `qmk compile -kb keyball64 -km via` is run
- **THEN** the resulting firmware includes `SPLIT_ACTIVITY_ENABLE` and the 5-minute idle blank

#### Scenario: default keymap builds with split activity sync
- **WHEN** `qmk compile -kb keyball64 -km default` is run
- **THEN** the resulting firmware includes `SPLIT_ACTIVITY_ENABLE` and the 5-minute idle blank

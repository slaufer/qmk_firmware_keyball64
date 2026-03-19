## Context

The keyball64 slave OLED shows a bouncing ball animation. Currently `oled_ball_on_key_press()` is called from `process_record_user()` on every non-modifier keypress, advancing the ball by one step. This means:

- The ball only moves while the user is actively typing
- At high WPM the ball moves fast; at low WPM it barely moves; at idle it freezes
- The animation feels inconsistent and is not useful as an ambient display

The ball position is managed on the master, synced to the slave via `BALL_POS_SYNC` RPC in `housekeeping_task_user()` every main loop iteration. The slave renders whatever position it last received, at the 50ms OLED refresh interval.

## Goals / Non-Goals

**Goals:**
- Ball moves continuously without requiring keypresses
- Ball speed is proportional to WPM — fast typists see a faster ball, idle keyboard shows a slow crawl
- Ball stops when WPM is 0 (keyboard truly idle)
- Remove all custom ball-sync infrastructure (BALL_POS_SYNC RPC, handler, accessors)
- No changes to slave rendering, trail logic, or OLED driver

**Non-Goals:**
- Changing the step size, collision logic, or ball/trail bitmaps
- Modifying the slave-side rendering logic
- Introducing a variable step size (speed is modulated only by step interval, not step magnitude)

## Decisions

### Decision: Drive animation from `housekeeping_task_user()` on slave, eliminating all custom sync

**Chosen:** Move ball state ownership entirely to the slave. Add a `static` timer in `housekeeping_task_user()` guarded by `!is_keyboard_master()` (QMK does not expose `is_keyboard_slave()`). When the timer expires, call `oled_ball_tick()` and reset the timer. Remove the `BALL_POS_SYNC` RPC, the slave handler, the transaction registration, and the `oled_ball_get_pos` / `oled_ball_set_pos` / `oled_ball_get_history` / `oled_ball_set_history` accessors (which existed solely to service the sync). WPM is already available on the slave via the built-in `SPLIT_WPM_ENABLE` mechanism.

**Alternative considered:** Drive the ball from `housekeeping_task_user()` on the master and keep the `BALL_POS_SYNC` RPC.
- This is a minimal change to the existing architecture
- However, it perpetuates unnecessary I2C traffic every main loop iteration and keeps dead sync infrastructure alive
- Once the trigger moves from key-press to WPM-timer, there is no longer any reason for the master to own ball state — the ball only renders on the slave, and WPM is already available there

**Alternative considered:** Drive the ball from `oled_task_user()` on the slave.
- The OLED task runs at exactly 50ms, which is the minimum tick interval — this could work using a frame counter instead of a timer
- However, mixing state advancement with rendering is less clean; `housekeeping_task_user()` is the correct place for periodic non-rendering logic

### Decision: WPM → interval formula: linear, `interval = (wpm ≥ 90) ? 50 : 500 − wpm × 5`

**Chosen:** `interval_ms = (wpm >= 90) ? 50 : 500 - wpm * 5`

| WPM | Interval |
|-----|----------|
| 0   | stopped (no tick) |
| 10  | 450 ms |
| 30  | 350 ms |
| 60  | 200 ms |
| 90+ | 50 ms  |

The 50ms floor matches `OLED_UPDATE_INTERVAL` — moving the ball faster than the OLED refreshes would produce no visible benefit.

**Alternative considered:** Exponential decay (`6000 / (wpm + 15)`). Gives a smoother curve but is harder to reason about and produces similar perceptual results in practice. Linear is simpler and easier to tune.

**Alternative considered:** Always-moving with a minimum speed floor instead of stopping at 0 WPM. Rejected because "rate determined by WPM" implies 0 WPM = 0 movement; a stationary ball at idle also conserves I2C sync bandwidth.

### Decision: Rename `oled_ball_on_key_press()` → `oled_ball_tick()`

The function's internal logic (shift history, advance position, bounce) is identical — only the call site and semantics change. Renaming avoids a misleading name in the public API. The old name is removed from the header; no other code outside `keymap_common.h` called it.

### Decision: Remove `process_record_user` from shared keymap code

With no remaining per-key logic, `process_record_user` in `keymap_common.h` becomes an identity function (just returns `true`). Removing it eliminates dead code and lets individual keymaps define their own if needed in the future.

## Risks / Trade-offs

- **Sync infrastructure removed:** The `BALL_POS_SYNC` RPC and associated code are deleted. This is purely a reduction in complexity and I2C traffic — no functionality is lost.
- **50ms floor means max visual speed is one step per OLED frame:** At ≥90 WPM the ball advances every render tick. This is the practical maximum — faster ball movement would be invisible.
- **WPM decay latency:** QMK's WPM counter decays over ~5 seconds. After stopping typing, the ball will decelerate gradually rather than stopping immediately. This is a feature, not a bug — it produces a smooth slowdown effect.
- **`static` timer initialization:** `ball_timer = 0` means `timer_elapsed(0)` will fire immediately on the first `housekeeping_task_user()` call on the slave, giving one ball step at startup. Acceptable; the alternative (a lazy-init flag) adds complexity for no user-visible benefit.
- **Both halves compile the same binary:** The master will have the `static` ball state variables in its binary but will never use them. This is a negligible cost.

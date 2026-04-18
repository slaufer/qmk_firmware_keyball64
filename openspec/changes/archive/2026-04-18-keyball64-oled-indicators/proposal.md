## Why

The master OLED has empty space at rows 12–15 (y=60–75) that is currently unused. Adding swap-hands and dynamic macro recording indicators there gives the user real-time feedback for two features that are otherwise invisible on-screen.

## What Changes

- Add a swap-hands active indicator to the master OLED at x=0–15, y=60–75; displayed when swap-hands is enabled, blank otherwise.
- Add a dynamic macro recording indicator to the master OLED at x=16–31, y=60–75; displayed when a dynamic macro is being recorded, blank otherwise.

## Capabilities

### New Capabilities
- `oled-swap-hands-indicator`: Master OLED row showing swap-hands active state
- `oled-dynmacro-indicator`: Master OLED row showing dynamic macro recording state

### Modified Capabilities
- `oled-stats-layout`: Rows 12–15 now include swap-hands and dynamic macro indicators in addition to the existing modifier indicators

## Impact

- `keyboards/keyball64/keyball64.c` (or the shared OLED render path) — add indicator rendering logic
- Depends on: `swap-hands` spec (SH_TOGG/swap state), QMK `dynamic_macro` subsystem (`DYNAMIC_MACRO_ENABLE`)
- No keymap changes required; indicators are driven by firmware state

## Why

During the rework-master-oled change, the modifier display logic was incorrectly replaced with `MOD_MASK_*` macros (which match both left and right modifier bits) instead of the original per-side `MOD_BIT(KC_L*/KC_R*)` checks gated on `is_keyboard_left()`. This caused the master OLED to light up modifier indicators when either the left or right modifier key was held, regardless of which physical half the key came from. The fix has been applied, but the spec lacked a requirement to prevent this class of regression.

## What Changes

- Add a spec requirement to `master-oled-bitmap-display` mandating that modifier indicators reflect only the modifier keys physically on the same half as the OLED, using `is_keyboard_left()` to select left vs right modifier bits
- No code changes required — the regression was already fixed in `oled_bitmap.c`

## Capabilities

### New Capabilities

_(none)_

### Modified Capabilities

- `master-oled-bitmap-display`: Add requirement that modifier indicators are side-aware, keyed on `is_keyboard_left()` rather than `MOD_MASK_*`

## Impact

- Spec only — `openspec/specs/master-oled-bitmap-display/spec.md`
- No code changes; fix is already in `lib/oled_bitmap/oled_bitmap.c`

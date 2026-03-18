## Context

The `oled_bitmap.c` shared OLED module renders modifier indicators on both master and slave OLEDs via `oledkit_render_info_user`. During the rework-master-oled change, the modifier checks were written using `MOD_MASK_SHIFT` etc., which match both left and right modifier bits. The original keymap code used `is_keyboard_left()` to gate on `MOD_BIT(KC_LSFT)` vs `MOD_BIT(KC_RSFT)` (and equivalents), so each half showed only the modifiers physically on that side.

The regression was identified and the fix has already been applied:

```c
bool left = is_keyboard_left();
if (mods & (left ? MOD_BIT(KC_LSFT) : MOD_BIT(KC_RSFT))) draw_bitmap(...);
```

This change adds the corresponding spec requirement so the behavior is documented and future implementations can be validated against it.

## Goals / Non-Goals

**Goals:**
- Document the side-aware modifier requirement in the canonical spec

**Non-Goals:**
- Any code changes (already fixed)

## Decisions

No design decisions required — this is a spec-only change documenting existing correct behavior.

## Risks / Trade-offs

None.

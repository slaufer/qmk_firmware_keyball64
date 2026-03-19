## REMOVED Requirements

### Requirement: oledkit override interface
The `oledkit_render_logo_user()` weak function provided an override point for keymaps to render a custom logo on the secondary OLED. This interface SHALL be removed.

**Reason**: Dead code. `oledkit.h` is not included anywhere in the codebase. `oled_bitmap.c` fully owns OLED rendering and never calls `oledkit_render_logo_user`. Overriding this function has had no effect since `oled_bitmap` was introduced.

**Migration**: No migration needed. No keymap in this repo uses this override point. Keymaps that require custom secondary OLED rendering should implement `oled_task_user` directly or extend `oled_bitmap`.

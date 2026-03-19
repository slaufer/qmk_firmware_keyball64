## REMOVED Requirements

### Requirement: Master tracks held non-modifier keys
**Reason**: Dead code. The held-keys buffer (`held_display`) is produced and synced to the slave, but no consumer ever reads it. `oled_bitmap.c` does not render it.
**Migration**: No migration needed. This feature was never visible to the user.

### Requirement: Held-keys buffer is synced to slave
**Reason**: Dead code. The `HELD_KEYS_SYNC` custom split transaction exists solely to deliver `held_display` to the slave, which ignores it.
**Migration**: Remove `HELD_KEYS_SYNC` from `SPLIT_TRANSACTION_IDS_KB` in both keymap `config.h` files.

### Requirement: Keycodes are mapped to display characters
**Reason**: Dead code. `keycode_to_char` and `update_held_display` exist only to populate the unused `held_display` buffer.
**Migration**: No migration needed.

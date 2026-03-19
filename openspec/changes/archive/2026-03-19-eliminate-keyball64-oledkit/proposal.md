## Why

The `oledkit` library (`lib/oledkit/oledkit.c` and `lib/oledkit/oledkit.h`) is dead code: all of its functions are either overridden by `oled_bitmap.c` or never called, and the header is not included anywhere in the codebase. Removing it reduces build surface and eliminates confusion about which OLED code is actually active.

## What Changes

- Remove `keyboards/keyball64/lib/oledkit/oledkit.c`
- Remove `keyboards/keyball64/lib/oledkit/oledkit.h`
- Remove the `SRC += lib/oledkit/oledkit.c` line from `keyboards/keyball64/rules.mk`

## Capabilities

### New Capabilities
<!-- None — this is a pure removal. -->

### Modified Capabilities
<!-- None — no spec-level behavior changes. oled_bitmap already owns all OLED behavior. -->

## Impact

- `keyboards/keyball64/rules.mk` — one line removed
- `keyboards/keyball64/lib/oledkit/` directory — deleted entirely
- No keymaps, headers, or other source files reference `oledkit`, so no downstream changes are needed

## Context

`oledkit` was the original OLED abstraction for keyball64. It provided weak implementations of `oled_task_user`, `oled_init_user`, `oledkit_render_info_user`, and `oledkit_render_logo_user` that keymaps could override. The `oled_bitmap` library superseded it: `oled_bitmap.c` defines non-weak `oled_task_user`, `oled_init_user`, and `oledkit_render_info_user`, and the header (`oledkit.h`) is not included anywhere in the current codebase.

## Goals / Non-Goals

**Goals:**
- Remove all `oledkit` source files and the corresponding `rules.mk` entry
- Leave the build clean with no dangling references

**Non-Goals:**
- Changing any OLED rendering behavior
- Modifying `oled_bitmap` in any way

## Decisions

**Delete rather than comment out** — the code is unreachable; there is no value in keeping it around as a reference since `oled_bitmap.c` fully documents the current approach.

**No deprecation period** — `oledkit.h` has zero `#include` sites, so no consumer can be broken.

## Risks / Trade-offs

- **External keymap forks** may vendor `oledkit.h` and override `oledkit_render_logo_user`. Those forks would get a linker error after this change. → Low risk given this is a personal firmware fork; acceptable trade-off for a cleaner build.

## Migration Plan

1. Delete `keyboards/keyball64/lib/oledkit/oledkit.c` and `oledkit.h`.
2. Remove the `SRC += lib/oledkit/oledkit.c` line from `keyboards/keyball64/rules.mk`.
3. Build and verify no compile/link errors.

Rollback: restore the two files from git history and re-add the `rules.mk` line.

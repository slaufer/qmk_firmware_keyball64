# Build options
BOOTMAGIC_ENABLE = no
EXTRAKEY_ENABLE = yes
CONSOLE_ENABLE = no
COMMAND_ENABLE = no
NKRO_ENABLE = no
BACKLIGHT_ENABLE = no
AUDIO_ENABLE = no
SPACE_CADET_ENABLE = no
MAGIC_ENABLE = no

# Duplex matrix
CUSTOM_MATRIX = lite
SRC += lib/duplexmatrix/duplexmatrix.c

# Split keyboard
SERIAL_DRIVER = vendor

# OLED (disabled by default; enable per keymap)
OLED_ENABLE = no
SRC += lib/oledkit/oledkit.c
SRC += lib/oled_bitmap/oled_bitmap.c

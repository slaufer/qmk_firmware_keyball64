## REMOVED Requirements

### Requirement: Slave half renders portrait-mode WPM and modifier layout
**Reason**: Replaced by pixel-accurate bitmap compositing in the slave-oled-bitmap-display capability. Text-based WPM, held-keys, and modifier rows are no longer rendered on the slave OLED.
**Migration**: The slave OLED now renders a base image and modifier indicator glyphs via `draw_bitmap()`. See the `slave-oled-bitmap-display` spec for the new requirements.

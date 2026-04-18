#!/usr/bin/env python3
"""Convert keyball64_via.json to QMK keymap.c layer definitions."""
import json
import argparse
import sys

LAYOUT_ORDER = [
    (0,0),(0,1),(0,2),(0,4),(0,5),(0,6),  (5,6),(5,5),(5,4),(5,2),(5,1),(5,0),
    (1,0),(1,1),(1,2),(1,4),(1,5),(1,6),  (6,6),(6,5),(6,4),(6,2),(6,1),(6,0),
    (2,0),(2,1),(2,2),(2,4),(2,5),(2,6),  (7,6),(7,5),(7,4),(7,2),(7,1),(7,0),
    (3,0),(3,1),(3,2),(3,4),(3,5),(3,6),(3,7),(8,7),(8,6),(8,5),(8,4),(8,2),(8,1),(8,0),
    (4,0),(4,1),(4,2),(4,4),(4,5),(4,6),(4,7),(9,7),(9,6),(9,5),(9,4),(9,2),(9,1),(9,0),
]

VIA_REMAP = {
    "0x56f0":          "SH_TOGG",
    "DYN_MACRO_PLAY1": "DM_PLY1",
    "DYN_MACRO_PLAY2": "DM_PLY2",
    "DYN_REC_START1":  "DM_REC1",
    "DYN_REC_START2":  "DM_REC2",
    "DYN_REC_STOP":    "DM_RSTP",
    "KC_NO":           "KC_TRNS",
}

ROW_SIZES  = [12, 12, 12, 14, 14]
LEFT_SIZES = [ 6,  6,  6,  7,  7]

LAYER_COMMENTS = [
    "Layer 0: Base",
    "Layer 1: Navigation / F-keys / Media",
    "Layer 2: Window management / One-shot mods / Dynamic macro play",
    "Layer 3: Dynamic macro record / EEPROM reset / One-shot mods",
]


def via_to_layout_keys(layer_data):
    return [VIA_REMAP.get(layer_data[r*8+c], layer_data[r*8+c]) for (r, c) in LAYOUT_ORDER]


def format_layer(layer_idx, layer_data):
    keys = via_to_layout_keys(layer_data)
    lines = []
    pos = 0
    for row_size, left_n in zip(ROW_SIZES, LEFT_SIZES):
        row = keys[pos:pos+row_size]
        left, right = row[:left_n], row[left_n:]
        pos += row_size
        w = max(len(k) for k in row) + 2
        left_str  = "".join(f"{k},{' '*(w-len(k)-1)}" for k in left).rstrip()
        right_str = "".join(f"{k},{' '*(w-len(k)-1)}" for k in right[:-1]) + right[-1]
        lines.append(f"        {left_str}    {right_str}")
    body = ",\n".join(lines)
    return f"    // {LAYER_COMMENTS[layer_idx]}\n    [{layer_idx}] = LAYOUT(\n{body}\n    )"


def main():
    parser = argparse.ArgumentParser(description="Convert keyball64 VIA JSON to keymap.c layers")
    parser.add_argument("via_json", help="Path to VIA export JSON file")
    parser.add_argument("--layers", type=int, default=4, help="Number of layers to emit (default: 4)")
    parser.add_argument("--include", default="../../lib/keymap_common/keymap_common.h",
                        help="Include path for keymap_common.h")
    args = parser.parse_args()

    with open(args.via_json) as f:
        via = json.load(f)

    n = min(args.layers, len(via["layers"]))
    blocks = [format_layer(i, via["layers"][i]) for i in range(n)]

    print(f'#include "{args.include}"')
    print()
    print()
    print("// clang-format off")
    print("const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {")
    print(",\n\n".join(blocks))
    print("};\n// clang-format on")


if __name__ == "__main__":
    main()

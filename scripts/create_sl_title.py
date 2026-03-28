#!/usr/bin/env python3
"""
Creates emerald_version_sl.png (128x32) and emerald_version_sl.8bpp
with "SOLO LEVELING" in a bold pixel-art font.
Tiles are arranged for two 64x32 8bpp GBA sprites (1D OBJ mapping).
"""

from PIL import Image
import numpy as np

import os
os.chdir(os.path.join(os.path.dirname(__file__), ".."))

src = Image.open("graphics/title_screen/emerald_version.png")
palette = src.getpalette()

WIDTH  = 128
HEIGHT = 32

BG      = 0   # transparent magenta
OUTLINE = 4   # near-black border
SHADOW  = 7   # dark gray inner shadow
BODY    = 15  # white main fill
HILITE  = 6   # bright highlight
MID     = 3   # near-white mid

# Bold pixel font: 11px tall, 2-3px thick strokes, clean shapes
FONT = {}

FONT['S'] = [
    "..OOOOOOO..",
    ".OHBBBBSOO.",
    "OHBS..OOOO.",
    "OHBO.......",
    ".OBBBBOO...",
    "..OOOSBBOO.",
    ".......OBSO",
    "OOOO..OBBSO",
    ".OOSBBBBOO.",
    "..OOOOOOO..",
    "...........",
]

FONT['O'] = [
    "..OOOOOOO..",
    ".OHBBBBSOO.",
    "OHBS...OBSO",
    "OHBO...OBSO",
    "OHBO...OBSO",
    "OHBO...OBSO",
    "OHBO...OBSO",
    "OHBS...OBSO",
    ".OOSBBBBOO.",
    "..OOOOOOO..",
    "...........",
]

FONT['L'] = [
    "OOOOO......",
    "OHBSOO.....",
    "OHBSOO.....",
    "OHBSOO.....",
    "OHBSOO.....",
    "OHBSOO.....",
    "OHBSOO.....",
    "OHBS...OOOO",
    "OOSBBBBSSOO",
    ".OOOOOOOOO.",
    "...........",
]

FONT['E'] = [
    "OOOOOOOOOO.",
    "OHBBBBBSOO.",
    "OHBSOOOOO..",
    "OHBSOO.....",
    "OHBBBBOO...",
    "OHHBBBOO...",
    "OHBSOO.....",
    "OHBSOOOOO..",
    "OOSBBBBSOO.",
    ".OOOOOOOOO.",
    "...........",
]

FONT['V'] = [
    "OOO.....OOO",
    "OHBO...OBSO",
    "OHBO...OBSO",
    ".OHBO.OBSO.",
    ".OHBO.OBSO.",
    "..OHBOOBSO.",
    "..OHBOOBSO.",
    "...OHBBSO..",
    "...OHBSO...",
    "....OOO....",
    "...........",
]

FONT['I'] = [
    "OOOOO",
    "OHBSO",
    "OHBSO",
    "OHBSO",
    "OHBSO",
    "OHBSO",
    "OHBSO",
    "OHBSO",
    "OHBSO",
    "OOSOO",
    ".....",
]

FONT['N'] = [
    "OOO....OOO.",
    "OHBOO..OBSO",
    "OHBBOO.OBSO",
    "OHBBBOOOBSO",
    "OHBOHBOOBSO",
    "OHBO.OBBOSO",
    "OHBO..OBBSO",
    "OHBO...OBSO",
    "OHBO...OBSO",
    "OOS.....OSO",
    "...........",
]

FONT['G'] = [
    "..OOOOOOO..",
    ".OHBBBBSOO.",
    "OHBS...OOOO",
    "OHBO.......",
    "OHBO.......",
    "OHBO.OOOOO.",
    "OHBO..OBSOO",
    "OHBS..OBSOO",
    ".OOSBBBBOO.",
    "..OOOOOOO..",
    "...........",
]

FONT[' '] = [
    "....",
    "....",
    "....",
    "....",
    "....",
    "....",
    "....",
    "....",
    "....",
    "....",
    "....",
]

SYMBOL_MAP = {
    '.': BG,
    'O': OUTLINE,
    'B': BODY,
    'H': HILITE,
    'S': SHADOW,
    'M': MID,
}


def render_word(canvas, text, y_offset, canvas_width, canvas_height):
    char_grids = [FONT[c] for c in text]
    char_widths = [len(g[0]) for g in char_grids]
    spacing = 0
    total_width = sum(char_widths) + spacing * (len(text) - 1)
    x = (canvas_width - total_width) // 2

    for ci, ch in enumerate(text):
        grid = FONT[ch]
        for gy, row in enumerate(grid):
            for gx, sym in enumerate(row):
                px = x + gx
                py = y_offset + gy
                if 0 <= px < canvas_width and 0 <= py < canvas_height:
                    idx = SYMBOL_MAP.get(sym, BG)
                    if idx != BG:
                        canvas[py, px] = idx
        x += len(grid[0]) + spacing


# --- Create pixel canvas ---
canvas = np.full((HEIGHT, WIDTH), BG, dtype=np.uint8)

# 11px tall font, place near top of sprite (y=2) so Y_GOAL controls screen position
render_word(canvas, "SOLO LEVELING", y_offset=2, canvas_width=WIDTH, canvas_height=HEIGHT)

# --- Save PNG ---
out = Image.fromarray(canvas, mode='P')
out.putpalette(palette)
out.save("graphics/title_screen/emerald_version_sl.png")

# --- Generate .8bpp with correct tile order ---
raw = bytearray()

# Left half: tile columns 0-7 (pixels 0-63)
for ty in range(4):
    for tx in range(8):
        tile = canvas[ty*8:(ty+1)*8, tx*8:(tx+1)*8]
        raw.extend(tile.tobytes())

# Right half: tile columns 8-15 (pixels 64-127)
for ty in range(4):
    for tx in range(8, 16):
        tile = canvas[ty*8:(ty+1)*8, tx*8:(tx+1)*8]
        raw.extend(tile.tobytes())

with open("graphics/title_screen/emerald_version_sl.8bpp", "wb") as f:
    f.write(raw)

print(f"Created emerald_version_sl.png ({WIDTH}x{HEIGHT})")
print(f"Created emerald_version_sl.8bpp ({len(raw)} bytes)")

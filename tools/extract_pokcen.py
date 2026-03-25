#!/usr/bin/env python3
"""
Extract the Pokemon Center exterior from Oldale Town map/tileset data.
Correctly applies per-tile palette slots from the .gbapal files.

Outputs:
  graphics/isometric/pokemon_center_preview.png   -- full building y=10-16 (7 rows)
  graphics/isometric/pokemon_center.png           -- 64x64 sprite region
  graphics/isometric/pokemon_center_sprite.h      -- GBA 4bpp sprite header
"""

import struct
from pathlib import Path
from PIL import Image

ROOT = Path(__file__).parent.parent

# ---------------------------------------------------------------------------
# Building region
# ---------------------------------------------------------------------------
# Oldale Town Pokemon Center spans metatile rows y=10-16 (7 rows = 112px).
# Sprite is 64x64 (4 rows). We use y=12-15 (body without roof cap / entrance).
# Change BUILD_Y0/Y1 here if you want a different crop.
BUILD_X0, BUILD_Y0 = 5, 13
BUILD_X1, BUILD_Y1 = 8, 16
MAP_W = 20

# Full building preview region (for reference PNG)
PREVIEW_X0, PREVIEW_Y0 = 4, 10
PREVIEW_X1, PREVIEW_Y1 = 9, 16

TILE_W, TILE_H      = 8, 8
TILES_PER_ROW       = 16
PRIMARY_TILE_COUNT  = 512
SEC_META_BASE       = 0x200
NUM_PALS_IN_PRIMARY = 6
NUM_PALS_TOTAL      = 13  # 6 primary + 7 secondary

# ---------------------------------------------------------------------------
# Load map
# ---------------------------------------------------------------------------
map_raw = (ROOT / "data/layouts/OldaleTown/map.bin").read_bytes()
grid = [struct.unpack_from("<H", map_raw, i * 2)[0] & 0x03FF
        for i in range(len(map_raw) // 2)]

# ---------------------------------------------------------------------------
# Load tileset tile images (indexed PNGs — pixel values are palette indices)
# ---------------------------------------------------------------------------
gen_img = Image.open(ROOT / "data/tilesets/primary/general/tiles.png")
pet_img = Image.open(ROOT / "data/tilesets/secondary/petalburg/tiles.png")
assert gen_img.mode == "P", "tiles.png must be indexed (mode P)"
assert pet_img.mode == "P", "tiles.png must be indexed (mode P)"

# ---------------------------------------------------------------------------
# Load palettes from .gbapal files
# BG palette slots 0..NUM_PALS_IN_PRIMARY-1  → primary tileset
# BG palette slots NUM_PALS_IN_PRIMARY..NUM_PALS_TOTAL-1 → secondary tileset
# ---------------------------------------------------------------------------
def load_gbapal(path):
    """Load a 16-colour GBA BGR555 palette file → list of 16 (r,g,b) tuples."""
    data = Path(path).read_bytes()
    pal = []
    for i in range(16):
        v = struct.unpack_from("<H", data, i * 2)[0]
        r = (v & 0x1F) << 3
        g = ((v >> 5) & 0x1F) << 3
        b = ((v >> 10) & 0x1F) << 3
        pal.append((r, g, b))
    return pal

bg_palettes = []  # indexed by BG palette slot 0-12
for i in range(NUM_PALS_IN_PRIMARY):
    bg_palettes.append(
        load_gbapal(ROOT / f"data/tilesets/primary/general/palettes/{i:02d}.gbapal"))
for i in range(NUM_PALS_TOTAL - NUM_PALS_IN_PRIMARY):
    bg_palettes.append(
        load_gbapal(ROOT / f"data/tilesets/secondary/petalburg/palettes/{i:02d}.gbapal"))

# ---------------------------------------------------------------------------
# Load metatile binaries
# ---------------------------------------------------------------------------
def load_metatiles(path):
    raw = Path(path).read_bytes()
    return [[struct.unpack_from("<H", raw, i * 16 + j * 2)[0]
             for j in range(8)] for i in range(len(raw) // 16)]

gen_metas = load_metatiles(ROOT / "data/tilesets/primary/general/metatiles.bin")
pet_metas = load_metatiles(ROOT / "data/tilesets/secondary/petalburg/metatiles.bin")

# ---------------------------------------------------------------------------
# Tile rendering with correct per-tile palette
# ---------------------------------------------------------------------------
def render_tile_rgba(sheet_img, tile_idx, pal_slot, hflip=False, vflip=False):
    """Render an 8x8 tile to RGBA using the correct BG palette slot."""
    if tile_idx == 0 or pal_slot == 2:   # pal 2 = grass — render transparent
        return Image.new("RGBA", (8, 8), (0, 0, 0, 0))
    col = tile_idx % TILES_PER_ROW
    row = tile_idx // TILES_PER_ROW
    tile = sheet_img.crop((col * TILE_W, row * TILE_H,
                           (col + 1) * TILE_W, (row + 1) * TILE_H))
    if hflip:
        tile = tile.transpose(Image.FLIP_LEFT_RIGHT)
    if vflip:
        tile = tile.transpose(Image.FLIP_TOP_BOTTOM)

    palette = bg_palettes[pal_slot] if pal_slot < len(bg_palettes) else [(0, 0, 0)] * 16
    pixels = []
    for idx in tile.getdata():
        if idx == 0:
            pixels.append((0, 0, 0, 0))   # palette index 0 = transparent
        else:
            r, g, b = palette[idx]
            pixels.append((r, g, b, 255))
    out = Image.new("RGBA", (8, 8))
    out.putdata(pixels)
    return out

def tile_by_id_rgba(tid, pal_slot, hflip=False, vflip=False):
    if tid < PRIMARY_TILE_COUNT:
        return render_tile_rgba(gen_img, tid, pal_slot, hflip, vflip)
    return render_tile_rgba(pet_img, tid - PRIMARY_TILE_COUNT, pal_slot, hflip, vflip)

def render_metatile(mid):
    img = Image.new("RGBA", (16, 16), (0, 0, 0, 0))
    if mid >= SEC_META_BASE:
        si = mid - SEC_META_BASE
        refs = pet_metas[si] if si < len(pet_metas) else None
    else:
        refs = gen_metas[mid] if mid < len(gen_metas) else None
    if not refs:
        return img
    for layer_start in (0, 4):   # bottom layer first, then top layer
        for sub in range(4):
            u16  = refs[layer_start + sub]
            tidx = u16 & 0x03FF
            hf   = bool(u16 & 0x0400)
            vf   = bool(u16 & 0x0800)
            pal  = (u16 >> 12) & 0xF
            if tidx == 0:
                continue
            img.alpha_composite(tile_by_id_rgba(tidx, pal, hf, vf),
                                ((sub % 2) * 8, (sub // 2) * 8))
    return img

# ---------------------------------------------------------------------------
# Composite helper
# ---------------------------------------------------------------------------
def composite_region(x0, y0, x1, y1):
    cols = x1 - x0 + 1
    rows = y1 - y0 + 1
    canvas = Image.new("RGBA", (cols * 16, rows * 16), (0, 0, 0, 0))
    for row in range(rows):
        for col in range(cols):
            mid = grid[(y0 + row) * MAP_W + (x0 + col)]
            mt  = render_metatile(mid)
            canvas.paste(mt, (col * 16, row * 16), mt)
    return canvas

# ---------------------------------------------------------------------------
# Save wide preview (full building context y=10-16)
# ---------------------------------------------------------------------------
preview = composite_region(PREVIEW_X0, PREVIEW_Y0, PREVIEW_X1, PREVIEW_Y1)
preview_out = ROOT / "graphics/isometric/pokemon_center_preview.png"
preview_out.parent.mkdir(exist_ok=True)
preview.save(preview_out)
print(f"Preview PNG (full building, {preview.size[0]}x{preview.size[1]}px): {preview_out}")

# ---------------------------------------------------------------------------
# Save 64x64 sprite region
# ---------------------------------------------------------------------------
canvas = composite_region(BUILD_X0, BUILD_Y0, BUILD_X1, BUILD_Y1)
W, H = canvas.size   # should be 64x64
assert W == 64 and H == 64, f"Expected 64x64, got {W}x{H}"

ref_out = ROOT / "graphics/isometric/pokemon_center.png"
canvas.save(ref_out)
print(f"Sprite region PNG ({W}x{H}px, y={BUILD_Y0}-{BUILD_Y1}): {ref_out}")

# ---------------------------------------------------------------------------
# Quantize to 16 colours (4bpp) — colour index 0 = transparent
# ---------------------------------------------------------------------------
bg = Image.new("RGBA", canvas.size, (0, 0, 0, 255))
bg.paste(canvas, mask=canvas.split()[3])
rgb = bg.convert("RGB")

quantized    = rgb.quantize(colors=15, method=Image.Quantize.MEDIANCUT)
palette_rgb  = quantized.getpalette()[:15 * 3]

# Remap: shift all indices +1 so index 0 = transparent
quant_arr = list(quantized.getdata())
remapped  = [p + 1 for p in quant_arr]
final_img = Image.new("P", canvas.size)
final_img.putdata(remapped)

full_pal = [0, 0, 0] + palette_rgb       # index 0 = black/transparent
full_pal += [0] * (16 * 3 - len(full_pal))
final_img.putpalette(full_pal + [0] * ((256 - 16) * 3))

# ---------------------------------------------------------------------------
# Convert to GBA 4bpp tile data (row-major 8x8 tiles)
# ---------------------------------------------------------------------------
pixels = list(final_img.getdata())

def rgb_to_bgr555(r, g, b):
    return ((b >> 3) << 10) | ((g >> 3) << 5) | (r >> 3)

u32s = []
for ty in range(H // 8):
    for tx in range(W // 8):
        for row in range(8):
            val = 0
            for col in range(8):
                px = pixels[(ty * 8 + row) * W + (tx * 8 + col)]
                val |= (px & 0xF) << (col * 4)
            u32s.append(val)

# ---------------------------------------------------------------------------
# Write C header
# ---------------------------------------------------------------------------
out_h = ROOT / "graphics/isometric/pokemon_center_sprite.h"

pal_entries = []
for i in range(16):
    r, g, b = full_pal[i * 3], full_pal[i * 3 + 1], full_pal[i * 3 + 2]
    pal_entries.append(rgb_to_bgr555(r, g, b))

lines = [
    "// Auto-generated by tools/extract_pokcen.py — do not edit.",
    "// Pokemon Center exterior sprite data: 64x64 px, 4bpp, 16 colours.",
    "",
    "static const u16 sPokeCenterPalData[16] = {",
]
for i, v in enumerate(pal_entries):
    comment = "  // transparent" if i == 0 else ""
    lines.append(f"    0x{v:04X},{comment}")
lines += ["};", ""]

lines.append(f"static const u32 sPokeCenterTileData[{len(u32s)}] = {{")
for i in range(0, len(u32s), 8):
    chunk = ", ".join(f"0x{v:08X}" for v in u32s[i:i + 8])
    lines.append(f"    {chunk},")
lines += ["};", ""]

out_h.write_text("\n".join(lines))
print(f"C header: {out_h}  ({len(u32s)} u32s, {len(u32s)*4} bytes tile data)")
print(f"\nSprite region: x={BUILD_X0}-{BUILD_X1}, y={BUILD_Y0}-{BUILD_Y1}")
print(f"Edit BUILD_Y0/Y1 in the script to change which rows are captured.")

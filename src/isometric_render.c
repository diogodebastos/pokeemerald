#include "global.h"
#include "main.h"
#include "gpu_regs.h"
#include "gba/io_reg.h"
#include "isometric_render.h"

// ---------------------------------------------------------------------------
// Isometric overworld – perspective scanline shear on BG1+BG2+BG3.
//
// Per-scanline horizontal shear with perspective (non-linear):
//   - Top of screen (far): strong convergence toward center
//   - Bottom of screen (near): slight spread
//   - Creates a "looking down at an angle" vanishing-point effect
//
// All map layers (BG1, BG2, BG3) shear identically (no drift).
// BG0 (UI/text) is NOT sheared.
// OAM sprites are counter-sheared in IsoApplySpriteShear (overworld.c).
// ---------------------------------------------------------------------------

#define SCREEN_H 160

// Perspective tuning:
//   HORIZON_DIST: virtual distance to horizon. Lower = more extreme perspective.
//   SHEAR_SCALE:  overall shear magnitude.
#define HORIZON_DIST  130
#define SHEAR_SCALE    25
#define CENTER_DEPTH  (HORIZON_DIST + SCREEN_H / 2)

static s16 sIsoCamScrollX;
static s16 sShearTable[SCREEN_H];

// ---------------------------------------------------------------------------
// IsometricRender_IsIsoMap
// ---------------------------------------------------------------------------

bool8 IsometricRender_IsIsoMap(u16 layoutId)
{
    (void)layoutId;
    return TRUE;
}

// ---------------------------------------------------------------------------
// Precompute perspective shear table (called once at init)
// ---------------------------------------------------------------------------

static void ComputeShearTable(void)
{
    s32 y;

    for (y = 0; y < SCREEN_H; y++)
    {
        s32 depth = HORIZON_DIST + y;
        // At y=80 (center): shear = 0
        // At y=0  (top/far): positive shift (converge)
        // At y=159 (bottom/near): mild negative shift (spread)
        sShearTable[y] = (s16)((CENTER_DEPTH * SHEAR_SCALE) / depth - SHEAR_SCALE);
    }
}

// ---------------------------------------------------------------------------
// HBlank callback — perspective shear BG1+BG2+BG3
// ---------------------------------------------------------------------------

static void HBlankCB_Isometric(void)
{
    u16 y;
    s16 xShift;
    u16 sheared;
    s32 clip_l, clip_r;

    y = REG_VCOUNT;
    if (y >= SCREEN_H - 1)
        return;

    xShift = sShearTable[y + 1];
    sheared = (u16)((s32)sIsoCamScrollX + xShift);

    REG_BG1HOFS = sheared;
    REG_BG2HOFS = sheared;
    REG_BG3HOFS = sheared;

    clip_l = (xShift < 0) ? -(s32)xShift : 0;
    clip_r = (xShift > 0) ? 240 - (s32)xShift : 240;
    REG_WIN0H = (u16)((clip_l << 8) | clip_r);
}

// ---------------------------------------------------------------------------
// IsometricRender_UpdateScroll
// ---------------------------------------------------------------------------

void IsometricRender_UpdateScroll(void)
{
    u16 primed;
    s32 clip_r;

    sIsoCamScrollX = (s16)GetGpuReg(REG_OFFSET_BG1HOFS);

    // Prime scanline 0 with the top-of-screen shear offset.
    primed = (u16)((s32)sIsoCamScrollX + sShearTable[0]);
    SetGpuReg(REG_OFFSET_BG1HOFS, primed);
    SetGpuReg(REG_OFFSET_BG2HOFS, primed);
    SetGpuReg(REG_OFFSET_BG3HOFS, primed);

    clip_r = 240 - (s32)sShearTable[0];
    SetGpuReg(REG_OFFSET_WIN0H, (u16)((0 << 8) | clip_r));
}

// ---------------------------------------------------------------------------
// IsometricRender_Init
// ---------------------------------------------------------------------------

void IsometricRender_Init(void)
{
    if (!IsometricRender_IsIsoMap(gSaveBlock1Ptr->mapLayoutId))
        return;

    sIsoCamScrollX = 0;
    ComputeShearTable();

    // WIN0: full screen vertically, horizontal set per-scanline.
    // Inside WIN0: all layers visible.
    // Outside WIN0: hide BG1+BG2+BG3 (sheared wraps), keep BG0+OBJ.
    SetGpuReg(REG_OFFSET_WIN0V, (u16)((0 << 8) | 160));
    SetGpuReg(REG_OFFSET_WIN0H, 0);
    SetGpuReg(REG_OFFSET_WININ, WININ_WIN0_ALL);
    SetGpuReg(REG_OFFSET_WINOUT, WINOUT_WIN01_BG0 | WINOUT_WIN01_OBJ);
    SetGpuRegBits(REG_OFFSET_DISPCNT, DISPCNT_WIN0_ON);

    SetHBlankCallback(HBlankCB_Isometric);
    SetGpuRegBits(REG_OFFSET_DISPSTAT, DISPSTAT_HBLANK_INTR);
    EnableInterrupts(INTR_FLAG_VBLANK | INTR_FLAG_HBLANK);
}

// ---------------------------------------------------------------------------
// IsometricRender_Shutdown
// ---------------------------------------------------------------------------

void IsometricRender_Shutdown(void)
{
    SetHBlankCallback(NULL);
    ClearGpuRegBits(REG_OFFSET_DISPSTAT, DISPSTAT_HBLANK_INTR);
    ClearGpuRegBits(REG_OFFSET_DISPCNT, DISPCNT_WIN0_ON);
    EnableInterrupts(INTR_FLAG_VBLANK);
}

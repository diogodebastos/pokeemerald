#ifndef GUARD_ISOMETRIC_RENDER_H
#define GUARD_ISOMETRIC_RENDER_H

// Isometric overworld – scanline shear on BG1+BG2+BG3.
//
// All map BG layers shear identically per-scanline so buildings
// and ground stay aligned. BG0 (UI) is unsheared.

bool8 IsometricRender_IsIsoMap(u16 layoutId);
void IsometricRender_Init(void);
void IsometricRender_Shutdown(void);
void IsometricRender_UpdateScroll(void);

// Defined in overworld.c (calls static OverworldBasic).
void CB2_IsometricOverworld(void);

#endif // GUARD_ISOMETRIC_RENDER_H

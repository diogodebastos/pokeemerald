# pokeemerald — Copilot Instructions

## What this repo is

A byte-accurate decompilation of **Pokémon Emerald** (GBA, BPEE-01) by the pret community. The build goal is a ROM whose SHA1 matches the retail cartridge (`f3ae088181bf583e55daf962a92bb46f4f1d07b7`). All logic is reconstructed C89 and ARM/Thumb assembly — no original source existed.

## Build system

Two mutually exclusive build modes controlled by the `MODERN` flag:

| Mode                         | Compiler                         | Goal                               |
| ---------------------------- | -------------------------------- | ---------------------------------- |
| `make` (default, `MODERN=0`) | `tools/agbcc` — patched GCC 2.95 | Byte-identical ROM                 |
| `make MODERN=1`              | `arm-none-eabi-gcc`              | Functionally correct, non-matching |

**Before first build**, tools must be compiled: `./build_tools.sh` (calls `make tools`).

Dependencies (Ubuntu/Debian):

```
sudo apt install gcc-arm-none-eabi binutils-arm-none-eabi libpng-dev
```

CI runs the matching build (`MODERN=0`, `COMPARE=1`) — see `.github/workflows/build.yml`.

## Critical conventions — read before editing

- **C89/gnu89 only.** No C99 features. The matching build uses `agbcc` which does not support C99. The `modern` build also targets gnu89 for compatibility.
- **Binary fidelity is the default.** Code structure, variable placement, and even compiler bugs are preserved by default. `#ifdef BUGFIX` blocks mark places where the original game has bugs — leave them off unless writing a ROM hack, not a matching build.
- **Do not split large files.** Files like `scrcmd.c`, `battle_script_commands.c`, `pokemon.c` are intentionally monolithic. Splitting changes the binary layout.
- **Dead variables are intentional.** Variables marked `// Never read` are preserved for matching. Do not remove them.
- **Memory section macros matter.** `EWRAM_DATA`, `IWRAM_CODE`, `COMMON_DATA` annotations map symbols to specific GBA memory regions. Removing or changing them breaks the build or hardware behaviour.
- **Struct field offset comments** (`/*0x000*/`) are documentation extracted from the binary — preserve them.

## Directory layout

```
src/           C source files (~312 .c files, flat — no subdirectories except src/data/)
src/data/      Inline C data tables (#included, not compiled directly): items, moves, trainers, etc.
include/       Public headers mirroring src/
data/          ASM-format game data: event scripts, battle scripts, map events, sound
asm/macros/    Assembly macro DSLs — event.inc, battle_script.inc, movement.inc, etc.
graphics/      PNG/BMP source art (converted by gbagfx at build time)
sound/         MIDI/WAV sources (converted by mid2agb/wav2agb at build time)
constants/     Assembly constant definitions (.inc files) shared by C and ASM
tools/         11 custom build tools built from C source (gbagfx, preproc, mid2agb, etc.)
```

## Architecture quick reference

**Main loop** — `struct Main gMain` in `src/main.c` holds two function pointers (`callback1`, `callback2`). Mode transitions happen via `SetMainCallback2(fn)`.

**Task system** — `src/task.c`: fixed pool of 16 cooperative tasks in `gTasks[]`. Each task is a state machine stepping through phases on `data[0]` (`tState`). No preemption.

**Field scripting** — `src/script.c` + `src/scrcmd.c`: bytecode interpreter driven by scripts in `data/event_scripts.s`. Macros in `asm/macros/event.inc` compile to single-byte opcodes.

**Battle scripting** — `data/battle_scripts_1.s` + `src/battle_script_commands.c`: parallel interpreter for move resolution. Macros in `asm/macros/battle_script.inc`.

**Battle controllers** — `src/battle_controller_*.c`: one file per participant role (player, opponent, partner, link, recorded, safari, wally). Each is a struct of function pointers dispatched per action.

**Wild encounters** — `src/wild_encounter.c`. Level selection in `ChooseWildMonLevel()`. Encounter tables defined in `src/data/wild_encounters.json` (JSON processed at build time by `tools/jsonproc`).

**Graphics** — VBlank-synced shadow registers (`src/gpu_regs.c`), DMA-backed double-buffered palettes (`src/palette.c`), 128-entry OAM pool (`src/sprite.c`).

**Sound** — M4A/MusicPlayer2000 engine (`src/m4a.c`, `src/m4a_1.s`). MIDI → `mid2agb` → binary; WAV → `wav2agb` → binary.

**Save/flash** — `src/save.c`, `src/agb_flash*.c`. Three flash chip variants supported (`1m`, `le`, `mx`).

## Where to find things

| Want to change                        | Look in                                                   |
| ------------------------------------- | --------------------------------------------------------- |
| Wild encounter species / levels       | `src/data/wild_encounters.json`                           |
| Wild level selection logic            | `src/wild_encounter.c` → `ChooseWildMonLevel()`           |
| Move data (power, type, effect)       | `src/data/battle_moves.h`                                 |
| Item data                             | `src/data/items.h`                                        |
| Trainer parties                       | `src/data/trainers.h`, `src/data/trainer_parties.h`       |
| NPC dialogue / overworld scripts      | `data/event_scripts.s`, `data/maps/`                      |
| Battle script commands                | `src/battle_script_commands.c`, `data/battle_scripts_1.s` |
| Map layouts                           | `data/layouts/`, `data/maps/`, `src/data/`                |
| Pokémon base stats                    | `src/data/pokemon/base_stats/`                            |
| Pokémon evolution data                | `src/data/pokemon/evolution.h`                            |
| Constants (flags, vars, species IDs…) | `include/constants/`                                      |

## Verifying changes

- `asmdiff.sh` — compare disassembly of the built ROM against a reference to check matching.
- `rom.sha1` — `sha1sum -c rom.sha1` verifies byte-identical output after a matching build.
- For a non-matching ROM hack (`MODERN=1`), neither check is required.

# Creating a New Map in pokeemerald

This guide covers every file that needs to be created or modified when adding a new outdoor route or town map. The example used throughout is Route 201.

---

## 1. Directory & Layout Files

### Create the layout directory and binary data

```
data/layouts/<MapName>/map.bin    # Blockdata (width * height * 2 bytes, little-endian u16 per metatile)
data/layouts/<MapName>/border.bin # Border tiles (8 bytes = 4 metatiles, 2 bytes each)
```

Each metatile value is a 16-bit integer:
- Bits 0-9: Metatile ID
- Bits 10-11: Collision (0 = passable, 1 = impassable)
- Bits 12-15: Elevation (3 = normal ground, 1 = water, 0 = background/trees)

You can generate these programmatically with Python or use **Porymap** to design them visually.

### Register the layout in `data/layouts/layouts.json`

Add an entry with the layout ID, dimensions, tilesets, and file paths:

```json
{
  "id": "LAYOUT_ROUTE201",
  "name": "Route201_Layout",
  "width": 40,
  "height": 20,
  "primary_tileset": "gTileset_General",
  "secondary_tileset": "gTileset_Petalburg",
  "border_filepath": "data/layouts/Route201/border.bin",
  "blockdata_filepath": "data/layouts/Route201/map.bin"
}
```

Common secondary tilesets: `gTileset_Petalburg`, `gTileset_Rustboro`, `gTileset_Fortree`, `gTileset_Pacifidlog`, etc.

---

## 2. Map Directory Files

### Create `data/maps/<MapName>/map.json`

This is the main map configuration. It defines metadata, connections, and all events. The build system auto-generates `header.inc`, `events.inc`, and `connections.inc` from this file.

```json
{
  "id": "MAP_ROUTE201",
  "name": "Route201",
  "layout": "LAYOUT_ROUTE201",
  "music": "MUS_ROUTE101",
  "region_map_section": "MAPSEC_ROUTE_201",
  "requires_flash": false,
  "weather": "WEATHER_SUNNY",
  "map_type": "MAP_TYPE_ROUTE",
  "allow_cycling": true,
  "allow_escaping": false,
  "allow_running": true,
  "show_map_name": true,
  "battle_scene": "MAP_BATTLE_SCENE_NORMAL",
  "connections": [ ... ],
  "object_events": [ ... ],
  "warp_events": [ ... ],
  "coord_events": [ ... ],
  "bg_events": [ ... ]
}
```

#### Connections

Connect to adjacent maps. `offset` shifts alignment (in tiles) relative to the connected map.

```json
"connections": [
  { "map": "MAP_ROUTE102", "offset": 0, "direction": "left" },
  { "map": "MAP_OLDALE_TOWN", "offset": 0, "direction": "right" }
]
```

**Important**: Also update the connected maps' `map.json` to point back to your new map.

#### Object Events

NPCs, trainers, item balls, berry trees:

```json
{
  "graphics_id": "OBJ_EVENT_GFX_YOUNGSTER",
  "x": 13, "y": 4,
  "elevation": 3,
  "movement_type": "MOVEMENT_TYPE_FACE_DOWN",
  "movement_range_x": 0, "movement_range_y": 0,
  "trainer_type": "TRAINER_TYPE_NORMAL",
  "trainer_sight_or_berry_tree_id": "3",
  "script": "Route201_EventScript_Marcus",
  "flag": "0"
}
```

- `trainer_type`: `TRAINER_TYPE_NONE` for NPCs/items, `TRAINER_TYPE_NORMAL` for trainers
- `trainer_sight_or_berry_tree_id`: Sight range for trainers, berry tree ID for berry trees, `"0"` otherwise
- `flag`: Set to a `FLAG_HIDE_*` constant to conditionally hide, or `"0"` for always visible. Item balls use `FLAG_ITEM_*` constants.
- Item balls use `"graphics_id": "OBJ_EVENT_GFX_ITEM_BALL"`
- Berry trees use `"graphics_id": "OBJ_EVENT_GFX_BERRY_TREE"` and `"movement_type": "MOVEMENT_TYPE_BERRY_TREE_GROWTH"` with `"script": "BerryTreeScript"`

#### Warp Events

For building entrances:

```json
{
  "x": 5, "y": 7,
  "elevation": 0,
  "dest_map": "MAP_OLDALE_TOWN_HOUSE1",
  "dest_warp_id": "0"
}
```

#### Background Events

Signs and hidden items:

```json
{
  "type": "sign",
  "x": 35, "y": 10,
  "elevation": 0,
  "player_facing_dir": "BG_EVENT_PLAYER_FACING_ANY",
  "script": "Route201_EventScript_SignOldale"
}
```

```json
{
  "type": "hidden_item",
  "x": 20, "y": 15,
  "elevation": 0,
  "item": "ITEM_RARE_CANDY",
  "flag": "FLAG_HIDDEN_ITEM_ROUTE_201_RARE_CANDY"
}
```

#### Coord Events (Triggers)

Script triggers when the player steps on a tile:

```json
{
  "type": "trigger",
  "x": 0, "y": 10,
  "elevation": 3,
  "var": "VAR_ROUTE_STATE",
  "var_value": "0",
  "script": "Route201_EventScript_SomeTrigger"
}
```

### Create `data/maps/<MapName>/scripts.inc`

All event scripts for the map, written in the pokeemerald scripting language. This file is **not** auto-generated.

```asm
Route201_MapScripts::
    .byte 0

Route201_EventScript_Marcus::
    trainerbattle_single TRAINER_ROUTE201_MARCUS, Route201_Text_MarcusIntro, Route201_Text_MarcusDefeated
    msgbox Route201_Text_MarcusPostBattle, MSGBOX_AUTOCLOSE
    end

Route201_EventScript_SignOldale::
    msgbox Route201_Text_SignOldale, MSGBOX_SIGN
    end

Route201_Text_MarcusIntro:
    .string "Let's battle!$"

Route201_Text_SignOldale:
    .string "ROUTE 201\n"
    .string "{RIGHT_ARROW} OLDALE TOWN$"
```

Common script patterns:
- **Trainer**: `trainerbattle_single TRAINER_ID, IntroText, DefeatedText`
- **NPC**: `msgbox Text, MSGBOX_NPC` (auto locks/faces/releases)
- **Sign**: `msgbox Text, MSGBOX_SIGN`
- **Item ball**: `finditem ITEM_NAME` (defined in `data/scripts/item_ball_scripts.inc`)
- **Lock/face NPC**: `lock` / `faceplayer` / `msgbox Text, MSGBOX_DEFAULT` / `release` / `end`

---

## 3. Registration Files

### `data/maps/map_groups.json`

Add the map name to **two** places:

1. The appropriate map group array (e.g., `gMapGroup_TownsAndRoutes`):
```json
"Route134",
"Route201",
"Underwater_Route124",
```

2. The `connections_include_order` array:
```json
"Route134",
"Route201",
"Underwater_Route105",
```

### `data/event_scripts.s`

Include the scripts file:

```asm
.include "data/maps/Route201/scripts.inc"
```

### `src/data/region_map/region_map_sections.json`

Add a region map section entry. The `x`/`y` are coordinates on the region map grid, `width`/`height` are how many cells the location spans.

```json
{
  "id": "MAPSEC_ROUTE_201",
  "name": "ROUTE 201",
  "x": 3,
  "y": 9,
  "width": 1,
  "height": 1
}
```

This auto-generates `include/constants/region_map_sections.h`.

---

## 4. Constants & Data Files

### `include/constants/opponents.h` - Trainer IDs

Add new trainer IDs before `TRAINERS_COUNT` and increment the count:

```c
#define TRAINER_ROUTE201_MARCUS  855
#define TRAINER_ROUTE201_LUNA    856
// ...
#define TRAINERS_COUNT           860
#define MAX_TRAINERS_COUNT       869
```

**Note**: The file header warns about trainer flag space. There's room for ~9 additional trainers by default. For more, shift flags in `flags.h`.

### `src/data/trainer_parties.h` - Trainer Pokemon

Add party definitions at the end of the file:

```c
static const struct TrainerMonNoItemDefaultMoves sParty_Route201Marcus[] = {
    {
    .iv = 0,
    .lvl = 5,
    .species = SPECIES_POOCHYENA,
    },
    {
    .iv = 0,
    .lvl = 5,
    .species = SPECIES_ZIGZAGOON,
    }
};
```

Party struct types:
- `TrainerMonNoItemDefaultMoves` - simplest, species + level + iv only
- `TrainerMonNoItemCustomMoves` - adds `.moves = {MOVE_X, MOVE_Y, ...}`
- `TrainerMonItemDefaultMoves` - adds `.heldItem = ITEM_X`
- `TrainerMonItemCustomMoves` - both held items and custom moves

### `src/data/trainers.h` - Trainer Data

Add trainer entries at the end of the array (before the closing `};`):

```c
[TRAINER_ROUTE201_MARCUS] =
{
    .trainerClass = TRAINER_CLASS_YOUNGSTER,
    .encounterMusic_gender = TRAINER_ENCOUNTER_MUSIC_MALE,
    .trainerPic = TRAINER_PIC_YOUNGSTER,
    .trainerName = _("MARCUS"),
    .items = {},
    .doubleBattle = FALSE,
    .aiFlags = AI_SCRIPT_CHECK_BAD_MOVE,
    .party = NO_ITEM_DEFAULT_MOVES(sParty_Route201Marcus),
},
```

Party macros match the struct type:
- `NO_ITEM_DEFAULT_MOVES(partyArray)`
- `NO_ITEM_CUSTOM_MOVES(partyArray)`
- `ITEM_DEFAULT_MOVES(partyArray)`
- `ITEM_CUSTOM_MOVES(partyArray)`

For female trainers, set: `.encounterMusic_gender = F_TRAINER_FEMALE | TRAINER_ENCOUNTER_MUSIC_FEMALE`

### `include/constants/flags.h` - Item & Hidden Item Flags

For item balls, repurpose unused flags or add new ones:

```c
#define FLAG_ITEM_ROUTE_201_SUPER_POTION  0x493
#define FLAG_ITEM_ROUTE_201_GREAT_BALL    0x494
```

For hidden items, use the `FLAG_HIDDEN_ITEMS_START` offset:

```c
#define FLAG_HIDDEN_ITEM_ROUTE_201_RARE_CANDY  (FLAG_HIDDEN_ITEMS_START + 0x70)
```

### `include/constants/berry.h` - Berry Tree IDs

Add berry tree IDs (last used is 89, max is 128):

```c
#define BERRY_TREE_ROUTE_201_CHERI  90
#define BERRY_TREE_ROUTE_201_RAWST  91
```

### `src/data/wild_encounters.json` - Wild Pokemon

Add an encounter entry in the `wild_encounter_groups[0].encounters` array:

```json
{
  "map": "MAP_ROUTE201",
  "base_label": "gRoute201",
  "land_mons": {
    "encounter_rate": 20,
    "mons": [
      { "min_level": 4, "max_level": 4, "species": "SPECIES_POOCHYENA" },
      ...
    ]
  }
}
```

The `land_mons.mons` array must have exactly **12** entries. Encounter slots map to probability:
- Slots 0-1: 20% each
- Slots 2-3: 10% each
- Slots 4-5: 10% each
- Slots 6-7: 5% each
- Slots 8-9: 4% each
- Slot 10: 1%
- Slot 11: 1%

Optional sections: `water_mons` (5 entries), `fishing_mons` (10 entries), `rock_smash_mons` (5 entries).

### `data/scripts/item_ball_scripts.inc` - Item Ball Scripts

Add item ball pickup scripts:

```asm
Route201_EventScript_ItemSuperPotion::
    finditem ITEM_SUPER_POTION
    end
```

---

## 5. Updating Connected Maps

When inserting a map between existing connections, update **both** neighboring maps' `map.json`:

- In the map that previously connected directly, change the connection target to your new map
- Example: Route 102 previously connected right to Oldale Town. Change it to connect right to Route 201 instead. Change Oldale Town's left connection from Route 102 to Route 201.

---

## 6. Optional: Indoor Maps

If your route has buildings, you need to create interior maps too:

1. Create a new map group in `map_groups.json` (e.g., `gMapGroup_IndoorRoute201`)
2. Create the interior map files the same way (map.json, scripts.inc, layout data)
3. Add warp events in both the route's `map.json` and the interior's `map.json` pointing to each other
4. Use an indoor tileset like `gTileset_PokemonCenter` or `gTileset_Building`

---

## 7. Build & Test

```bash
make -j$(nproc)    # or make -j$(sysctl -n hw.ncpu) on macOS
```

Common build errors:
- **Undefined reference to scripts**: Forgot to add `.include` in `data/event_scripts.s`
- **Junk at end of line**: Special characters in `.string` text (use only ASCII, no smart quotes)
- **Linker errors for map symbols**: Map not added to `map_groups.json` or misspelled map name

---

## Quick Checklist

| Step | File(s) |
|------|---------|
| Layout binary data | `data/layouts/<Map>/map.bin`, `border.bin` |
| Register layout | `data/layouts/layouts.json` |
| Map config + events | `data/maps/<Map>/map.json` |
| Event scripts | `data/maps/<Map>/scripts.inc` |
| Include scripts | `data/event_scripts.s` |
| Map group registration | `data/maps/map_groups.json` (2 places) |
| Region map section | `src/data/region_map/region_map_sections.json` |
| Wild encounters | `src/data/wild_encounters.json` |
| Trainer IDs | `include/constants/opponents.h` |
| Trainer parties | `src/data/trainer_parties.h` |
| Trainer data | `src/data/trainers.h` |
| Item flags | `include/constants/flags.h` |
| Item ball scripts | `data/scripts/item_ball_scripts.inc` |
| Berry tree IDs | `include/constants/berry.h` |
| Update neighbor connections | Neighboring maps' `map.json` files |

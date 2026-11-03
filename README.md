# GSC Stalker Inventory - Vertical Slot Fix

An unofficial patch for [GSC Stalker Inventory Official](https://steamcommunity.com/sharedfiles/filedetails/?id=3824883510)
by Harkness. It needs that mod, is not affiliated with its author and ships none of its files.

The original draws the worn slots in a strip above the character, two rows high. With more slots
than vanilla they have to be scrolled. This patch puts the character on the left of the EQUIPMENT
column and the slots in a vertical strip on the right.

- The slots stand in columns of eight, in the original order. More slots add a column.
- The slots do not scroll or move. A large item in hands scrolls inside the HANDS panel, which stops
  under the strip instead of growing to half the screen.

## How it works

All of it is in [`GVSF_SlotLayout.c`](GSC_Vertical_Slot_Fix/scripts/5_Mission/GSC_Vertical_Slot_Fix/GVSF_SlotLayout.c).
On the first update of an open inventory it finds the original's widgets by name, moves the visible
slot tiles out of their rows into a frame of its own and resizes the character's area. A row's column
count cannot be changed from script, and the original's layouts are not ours to copy, so the tiles are
moved rather than laid out again. If one of the widgets it needs is missing, it leaves the inventory
as the original drew it.

Measurements and what is not tested yet: [`docs/2026-11-03-layout-probe.md`](docs/2026-11-03-layout-probe.md).

## Build

With the `dayz` MCP (`dayz-agentic-modding-mcp`): `project_open`, `mod_build`, then `.\package.ps1`.
`mod_build` signs with `keys/GSC_Vertical_Slot_Fix.biprivatekey`, which stays out of git. Publishing is
described in [`packaging/README.md`](packaging/README.md).

## License

[CC BY-NC-SA 4.0](LICENSE). This covers the code here, not the original mod.

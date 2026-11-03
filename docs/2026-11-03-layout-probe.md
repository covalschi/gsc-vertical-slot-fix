# Layout probe: character on the left, worn slots on the right

2026-11-03. What was measured before and while this became a mod.

## Why the slots scroll

`right_area.layout` of GSC Stalker Inventory Official declares `ScrollerSlotsContent` 150 units high in
all three of its width sets. That holds two rows of eight. Vanilla has 13 slots; the 17th opens a third
row. BLR_Player_Slots (Workshop 3420608805) adds six, 19 in all. The HANDS panel can also grow to half the
screen, which leaves a sliver of the character when a backpack is in hands.

## Results

Game 1.30.164200. Client and server verdicts passed with no script errors in every run.

19 slots with BLR_Player_Slots, this build:

| Check | diag 3840x1600 | diag 1600x1000 | retail 3840x1600, `verifySignatures = 2` |
|---|---|---|---|
| Columns of 8, 8 and 3, no scrollbar | yes | yes | yes |
| Ground to each of BLR's six slots | yes | | knife only |
| Ground to occupied slot (swap) | | yes | |
| Ground to empty slot | | | yes |
| Slot to ground | yes | | |
| Slot to the cargo of a backpack on the ground | yes | | |
| Tooltip on a new slot | yes | | |
| Ghillie items draw on the character (BLR's proxy model) | yes | | |
| Backpack in hands: tiles stay, HANDS panel stops at 324 units | yes | | |
| HANDS panel scrolls by the wheel | yes | | |
| Client accepted, not kicked | | | yes |

28 slots (BLR + 9 throwaway test slots), diag 1920x1080: columns of 8, 8, 8 and 4, no scrollbar, the
character narrower and whole, a drag from the ground into the fourth column works.

22 slots (13 + the same test slots), the earlier build:

| Check | 1920x1080 | 1600x1000 | 1280x960 | 3840x1600 |
|---|---|---|---|---|
| All tiles visible | yes | yes | yes | yes |
| Ground to occupied slot (swap) | yes | | yes | yes |
| Hands to the empty part of the strip | yes | | | |
| Slot to hands and back, reserved tile | yes | | | |
| Hands to ground | | | | yes |
| Rotate and zoom the character | | | | yes |

An empty cell means not checked there, not failed. The owner also checked the build in the game by hand.
InventorySlotsHotfix (3431076620), which BLR's page recommends for the vanilla inventory, is not needed
(owner, 2026-11-03) and was not loaded.

## Control: the original without this patch

GSC Stalker Inventory Official + BLR_Player_Slots, diag 3840x1600. The third row of slots is cut off and
has to be scrolled, and a backpack in hands squeezes the character to a sliver.

- A backpack dropped from the ground on hands that already hold a rifle does nothing, in the original and
  with the patch alike.
- A worn item dropped in the cargo of a backpack on the ground lands there, in the original and with the
  patch alike. One such drop did nothing in an earlier run, right after a hover. It did not come back in the
  same sequence on a clean run, and its cause was not found.

The release build differs from the build these runs used by one `#ifdef DIAG_DEVELOPER` around a log line;
`client_compile_check` passed on it.

## Keyboard and gamepad

`Inventory.Update` moves the grid cursor only under `#ifdef PLATFORM_CONSOLE`, so a PC build has no such
navigation to break. Checked on a live client: the D-pad moves nothing among the tiles.

## Not tested

- Other mods that reopen `PlayerContainer` or swap the inventory layouts.
- More than 32 slots (a fifth column).
- A later version of the original. The patch depends on the names `S2_CharacterView`,
  `S2_ColumnCentreRecessTop`, `S2_ColumnCentreRecessLow` and `ScrollerSlotsContent`, and on the addon name
  `GSC_StalkerInventory_Scripts` in `requiredAddons`. A missing widget leaves the layout alone by the code's
  guard, which no run has exercised; a renamed addon stops the game with a blocking dialog.

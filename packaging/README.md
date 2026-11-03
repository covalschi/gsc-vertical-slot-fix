# packaging

What goes into the `@GSC_Vertical_Slot_Fix` folder besides the packed pbo. That folder is build
output and is not in git.

```
packaging/GSC_Vertical_Slot_Fix/mod.cpp    name, author and description shown in the launcher
packaging/GSC_Vertical_Slot_Fix/meta.cpp   the Workshop item id, once the item exists
packaging/GSC_Vertical_Slot_Fix.workshop.bbcode   the listing text, English and Ukrainian
packaging/GSC_Vertical_Slot_Fix.workshop.png      the preview, 1024x512, under 1 MB
packaging/GSC_Vertical_Slot_Fix.before.png        the original without the patch, for the screenshot strip
```

`meta.cpp` ties the folder to a Workshop item: without it the next upload creates a second item.
The first `workshop_publish` writes it into `@GSC_Vertical_Slot_Fix`; copy it back here and commit it.

Publishing, in order: `mod_build`, then `.\package.ps1`, then:

```
workshop_publish("GSC_Vertical_Slot_Fix",
                 title="GSC Stalker Inventory - Vertical Slot Fix",
                 requires=[3824883510],
                 tags=["Mod", "Mechanics"],
                 preview="packaging/GSC_Vertical_Slot_Fix.workshop.png",
                 previews=["packaging/GSC_Vertical_Slot_Fix.before.png"],
                 description=<the text of the .bbcode file>)
```

A new item is private until its visibility is changed. Steam caps a description at 8000 bytes, not
characters, and Ukrainian counts double: measure before publishing.

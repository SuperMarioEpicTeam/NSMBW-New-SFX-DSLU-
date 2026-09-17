# Super Mario Epic Wii Util - New SFX (Dynamic Sound Loader & Unloader)

An extension of Asu-Chan's New SFX Hack, with a DSLU (Dynamic Sound Loader & Unloader) system allowing you to load & unload any SFX in order to optimize Wii RAM. That patch was originally meant for Super Mario Epic Wii. Outdated but works in-game.

## Installation:
https://github.com/Developers-Collective/NSMBW-Custom-Sprites/releases/tag/MoreSFX
## Credits:
- Asu Chan , for the original patch ours is based on
- Synel , for some compiler-compatibility conversion (Clang -> CodeWarrior)
- Walid , for implementing the DSLU
## Symbols to add
```
	NewSFXTable = 0x80440000;
	NewSFXIndexes = 0x80450000;
	doneWithNewSFXHax = 0x80283CE4;
	PlaySoundWithFunctionB4_2 = 0x801951E4;

	isSndPlaying = 0x80196740;
```
## Example - How to unload an SFX:
```
  if (!isSndPlaying(SoundRelatedClass, SFX_ORIGINAL))
	  DSLUInstance.unloadNewSFX(SFX_ORIGINAL);
```

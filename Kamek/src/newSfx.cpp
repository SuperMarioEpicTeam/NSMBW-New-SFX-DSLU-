#include <game.h>
#include <sfx.h>
#include "music.h"
#include "fileload.h"
#include "newSfx.h"

int currentSFXID = -1;
u32 *currentPointer = 0;

// static FileHandle handle;

extern "C" u32 NewSFXIndexes;
extern "C" u32 NewSFXTable[];		//Store MBs???

void loadAllSFXs() {
	u32 currentIdx = (u32)&NewSFXIndexes;

	for(int sfxIndex = 0; sfxIndex < (sizeof(SFXNameList) - 1) / sizeof(SFXNameList[0]); sfxIndex++) {
		FileHandle handle;

		char nameWithSound[80] = "";
		snprintf(nameWithSound, 79, "/Sound/stream/sfx/%s.rwav", SFXNameList[sfxIndex]);

		u32 filePtr = (u32)LoadFile(&handle, nameWithSound);

		NewSFXTable[sfxIndex] = currentIdx;
		loadFileAtIndex((u32*)filePtr, handle.length, (u32*)currentIdx);
		currentIdx += handle.length;
		currentIdx += (currentIdx % 0x10);				//Some sort of alignments by 0x10 ????
		FreeFile(&handle);
	}
}

int hijackSFX(int SFXNum) {
	int nameIndex = SFXNum - 1999;
	if(currentSFXID == nameIndex) {
		return 189;
	}

	currentPointer = (u32*)NewSFXTable[nameIndex];
	currentSFXID = nameIndex;

	return 189;
}

nw4r::snd::StrmSoundHandle yoshiHandle;

void fuckingYoshiStuff() {
	PlaySoundWithFunctionB4(SoundRelatedClass, &yoshiHandle, 189, 1);
}

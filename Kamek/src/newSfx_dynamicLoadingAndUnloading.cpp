#include <sound.h>

///BTW I remember, the spine shell must be added calls to funcs like "checkHead", "checkWall" & "checkFoot" to interact with bricks, I guess.

extern "C" u32 NewSFXIndexes;
extern "C" u32 NewSFXIndexesEnd;

DynamicSoundLoaderUnloader::DynamicSoundLoaderUnloader() : newSfxInfo() {
	this->currentSFX = -1;
}

//In ticks: 1s = 81 000 000 Game Ticks
/** u32 NewSFXsDurations[NewSFXListLen] = {
	33210000,		//1999
	72900000,		//2000
	213840000,
	145800000,
	32400000,
	67230000,
	81000000,
	13770000,
	120690000,
	171720000,
	131220000,
	46170000,		//2010
	34830000,
	113400000,
	293220000,
	59940000,
	40500000,
	19440000,
	84240000,
	89100000,
	150660000,
	115020000,		//2020
	166860000,
	85050000,
	125550000,
	120690000,
	208170000,
	278640000,
	164430000,
	278640000,
	30780000,
	116640000,		//2030
	132840000,
	213840000,
	89100000,
	48600000,
	15390000,
	14580000,
	112590000,
	147420000,
	227610000,
	202500000,		//2040
	67230000,
	68040000,
	23490000,
	25920000,
	61560000,
	50220000,
	42120000,
	16200000,
	19440000,
	51030000,		//2050
	42120000,
	53460000,
	12960000,
	25920000,
	119880000,
	14580000,
	77760000,
	107730000,
	10530000,
	14580000,		//2060
	34020000,
	33210000,
	204120000,
	142560000,
	297270000,
	98010000,
	138510000,
	156330000,
	558900000,
	16200000,		//2070
	236520000,
	85050000,
	9720000,
	212220000,
	204930000,
	87480000,
	90720000,
	85050000,
	87480000,
	89100000		//2080
};
 */

/***************************************************************************************************/

extern void freeFileAtIndex(u32 *ptr, u32 size);

void DynamicSoundLoaderUnloader::unloadNewSFX(int id) {				//Called when the sounds' countdown (counting from the sound's approximate duration in frames) reaches 2 or less
	if (!newSfxInfo[id].used) { return; }

	OSReport("Freeing SFX Memory...\n");
	freeFileAtIndex((u32 *)newSfxInfo[id].soundMgr.filePtr, newSfxInfo[id].soundMgr.length);
	newSfxInfo[id].soundMgr.filePtr = 0;
	newSfxInfo[id].soundMgr.length = 0;
	newSfxInfo[id].lastTick = 0;
	newSfxInfo[id].used = false;
	//OSReport("Freeing successful\n");
}

int ShittySoundValue;

void DynamicSoundLoaderUnloader::loadNewSFX(int id) {
	ShittySoundValue = id;					//bada$$ registers (made because of the basic registers being all busy from that point)
	if (id >= 1999) { id -= 1999; }

	if (this->newSfxInfo[id].used) { return; }

	char nameWithSound[90] = "";
	snprintf(nameWithSound, 89, "/Sound/stream/sfx/%s.rwav", SFXNameList[id]);

	FileHandle handle;

	u32 filePtr = (u32)LoadFile(&handle, nameWithSound);								//8043fd20
	OSReport("NewSFX num %d, loaded %s, at ptr %p\n", id, nameWithSound, &filePtr);

	u32 fileLen = handle.length;
	fileLen += 0x10 - (fileLen % 0x10);

	u32 realPtr = this->getWriteabilityPtr(fileLen);
	this->newSfxInfo[id].soundMgr.filePtr = realPtr;
	this->newSfxInfo[id].soundMgr.length = fileLen;

	loadFileAtIndex((u32*)filePtr, handle.length, (u32 *)realPtr);
	FreeFile(&handle);
	OSReport("...Updated to ptr: %p stored at ptr: %p\n", newSfxInfo[id].soundMgr.filePtr, &newSfxInfo[id].soundMgr);

	//Now do some time things...
	this->newSfxInfo[id].lastTick = OSGetTick();
	this->newSfxInfo[id].used = true;
}

/* 
bool DynamicSoundLoaderUnloader::getShouldSoundBeDynamicallyRemoved(int id) {
	u32 currentTick = OSGetTick();
	u32 TickElapsed = (currentTick - this->newSfxInfo[id].lastTick);
	u32 TickDelta = NewSFXsDurations[id] - TickElapsed;

	//OSReport("Countdown for SFX %d: %d ticks elapsed (VS: %d)\n", id, TickDelta, NewSFXsDurations[id]);
	///Fuckin' silly checks...
	if (!newSfxInfo[id].soundMgr.filePtr) { return false; }
	if (newSfxInfo[id].soundMgr.length <= 0) { return false; }

	return (newSfxInfo[id].used && TickDelta >= 0 && TickDelta < TICKS_PER_100ER_SECOND);							//Due to approximation issues...
}

void DynamicSoundLoaderUnloader::checkSoundRemoveabilityAfterReading(int id) {
	//OSReport("A bit of checks rn\n");
	if (!this->getShouldSoundBeDynamicallyRemoved(id)) { return; }

	OSReport("Let's unload SFX %d now\n", id);
	this->unloadNewSFX(id);
}
 */

int DynamicSoundLoaderUnloader::hijackSFX(int SFXNum) {
	int nameIndex = SFXNum - 1999;
	if(this->currentSFX == nameIndex) {
		return 189;
	}

	this->currentPtr = (u32)this->newSfxInfo[nameIndex].soundMgr.filePtr;
	this->currentSFX = nameIndex;

	OSReport("Hijacked! Ptr: %p, SFX ID: %d\n", currentPtr, nameIndex);

	return 189;
}

u32 DynamicSoundLoaderUnloader::getWriteabilityPtr(u32 fileLen) {
    u32 base = (u32)&NewSFXIndexes;
    u32 end = (u32)&NewSFXIndexesEnd;

    // Étape 1 : trouver la plus haute adresse utilisée dans NewSFXIndexes
    u32 highestEnd = base;

    for (int i = 0; i < NewSFXListLen; i++) {
        if (this->newSfxInfo[i].used) {
            u32 soundStart = this->newSfxInfo[i].soundMgr.filePtr;
            u32 soundEnd = soundStart + this->newSfxInfo[i].soundMgr.length;

            if (soundEnd > highestEnd && soundEnd < end) {
                highestEnd = soundEnd;
            }
        }
    }

    // Étape 2 : aligner la prochaine adresse disponible sur 0x10
    if (highestEnd % 0x10 != 0) {
        highestEnd += (0x10 - (highestEnd % 0x10));
    }

    // Étape 3 : vérifier s'il y a assez de place
    if ((highestEnd + fileLen) > end) {
        OSReport("WARNING, you have run out of custom sound memory!!! An overwriting may occur...\n");
    }

    return highestEnd;
}


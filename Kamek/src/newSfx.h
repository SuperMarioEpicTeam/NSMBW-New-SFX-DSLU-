#ifndef NEWSFX_H
#define NEWSFX_H

int hijackSFX(int SFXNum);

extern void loadFileAtIndex(u32 *filePtr, u32 fileLength, u32* whereToPatch);

const char* SFXNameList [] = {
	"original",				// 1999, DON'T USE THIS ONE
	NULL
};

/* Prob the most akward piece of code ever!!!
#define NEW_SFX_LEN (sizeof(SFXNameList) - 1)

class dNewSFXMgr_c {
	public:
		u32 *readPtr;
		u32 *LoadedSFXIdxs[NEW_SFX_LEN];

		bool loadNewSFX(int id);
		void unloadNewSFX(int id);
};
dNewSFXMgr_c newSfxManager;


bool dNewSFXMgr_c::loadNewSFX(int id) {
	if (LoadedSFXIdxs[id]) {
		this->readPtr = LoadedSFXIdxs[id];
		return true;
	}

	FileHandle handle;
	char nameWithSound[80] = "";
	snprintf(nameWithSound, 79, "/Sound/stream/sfx/%s.rwav", SFXNameList[id]);

	u32 filePtr = (u32)LoadFile(&handle, nameWithSound);

	if (!filePtr) {
		return false;
	}

	*LoadedSFXIdxs[id] = filePtr;
	///FreeFile(&handle);						Hesitating...
	*this->readPtr = filePtr;

	return true;
}

void dNewSFXMgr_c::unloadNewSFX(int id) {
	if (this->LoadedSFXIdxs[id]) {
		delete this->LoadedSFXIdxs[id];			//I hope this works because that's what these codes are all about
	}
	this->LoadedSFXIdxs[id] = 0;
}
 */

//ADDED HERE FOR THE DSLU RELEASE
namespace SndAudioMgr {
	extern bool isSndPlaying(int);				//80196740
}

#endif /* NEWSFX_H */


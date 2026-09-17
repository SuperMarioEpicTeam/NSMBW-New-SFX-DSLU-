#ifndef __SOUND_H
#define __SOUND_H

#include <game.h>
#include "newSfx.h"
#include "fileload.h"

class SoundArchiveFileReader {
	public:
		SoundArchiveFileReader();		//no dtors?!?!

		u32 hdrMagic;
		u32 hdrEndianVersion;
		u32 hdrFileSize;
		u32 hdr0C;
		u32 stringsOffset;
		u32 stringsSize;
		u32 infoOffset;
		u32 infoSize;
		u32 hdr20, hdr24;
		u32 infoChunk;
		u32 stringChunkBase;
		u32 actualStrings;
		u32 otherStringChunks[4];

		void Init(const void *);		//Interesting, so this is where we can spot the BRSAR file's magic (80275eb0)
};

//The class that loads the BRSAR, so probably responsible for the entirety of the game's sounds.
class SoundArchive {
	public:
		SoundArchive();
		virtual ~SoundArchive();

		SoundArchiveFileReader *fileReader;
		char externalFileRoot[256];
};

class SndAudioMgr {
	public:
		SndAudioMgr();

		//That shitty class has multiple vtables for some reason
		//Main vtable (defined here): 8032e238
		///File Managing Vtable: 8032e264 PAL
		virtual void initialize(void *EGG__IAudioMgr_Arg);
		virtual void update(void);

		u8 _04[0x9C];
		SoundArchive *sound_archive;
		u8 _104[0x4B8];
		void *sound_player_archive;
		u8 data3[0x8DC - 0x5C0];
		EGG::Disposer disposer;
		//A lot left to do!!!!!!!!!!!

		static SndAudioMgr *instance();						//8042a768
};

///SMEW Classes
#define NewSFXListLen ((sizeof(SFXNameList) - 1) / sizeof(SFXNameList[0]))
#define TICKS_PER_100ER_SECOND									810000

extern "C" u32 OSGetTick();									//801b60e0

extern "C" bool isSndPlaying(void *SoundRelatedClass, int param_2);			//80196740
extern void *NonPosSndObjctAmbInstance;										//8042a044
extern void *SoundClassRelated;

class DynamicSoundLoaderUnloader {
	public:
		DynamicSoundLoaderUnloader();

		class CustomSFXInfo {
			public:
				CustomSFXInfo() {}

				struct PseudoFileHandle {
					u32 filePtr;
					int length;
				};

				PseudoFileHandle soundMgr;
				u32 lastTick;
				bool used;
		};

		// class SoundHandleIDStore {
			// public:
				// SoundHandleIDStore() {}

				// nw4r::snd::SoundHandle *handler;

				// void storeSoundHandle(nw4r::snd::SoundHandle *_handle);
		// };

		u32 dontUseThatValue;								//Idk what happens here....
		u32 currentPtr;
		int currentSFX;
		CustomSFXInfo newSfxInfo[NewSFXListLen];
		//SoundHandleIDStore soundStorages[NewSFXListLen];

		//void update();			//equiv to an execute func
		void unloadNewSFX(int id);		//The new SFX ID, specifically (so I likely have to substract 1999 for every custom SFX :/ )
		void loadNewSFX(int id);
		// bool getShouldSoundBeDynamicallyRemoved(int id);
		// void checkSoundRemoveabilityAfterReading(int id);
		int hijackSFX(int SFXNum);
		u32 getWriteabilityPtr(u32 fileLen);
};
DynamicSoundLoaderUnloader DSLUInstance;

extern "C" void StopSoundRelated(void *SoundRelatedClass, int fadeOut);

void PlayMinecartBGM(void *SndAudioMgrInstance) {
	if ((*(nw4r::snd::SoundHandle *)(((u32)SndAudioMgrInstance)+0x908)).Exists()) {
		(*(nw4r::snd::SoundHandle *)(((u32)SndAudioMgrInstance)+0x908)).Stop(0);
	}
	int bgmID;
	hijackMusicWithSongName("MINECART", -1, false, 2, 1, &bgmID);
	PlaySoundWithFunctionB4(SoundRelatedClass, (nw4r::snd::SoundHandle *)(((u32)SndAudioMgrInstance)+0x908), bgmID, 1);
}


//NOW ! TS BEEN A WHILE SINCE I CAME HERE LAST
extern "C" u8 SCGetSoundMode();							//801dd590

//Same as OSEnableInterrupts:
extern u32 PPCMfhid2();						//801a9db0
extern void PPCMthid2(u32 newHID2);			//801a9dc0

#endif
#include <asm_setup.S>

.text

.extern _savegpr_27
.extern PlaySoundWithFunctionB4_2
.extern doneWithNewSFXHax
.extern loadAllSFXs__Fv
.extern DSLUInstance
.extern loadNewSFX__26DynamicSoundLoaderUnloaderFi
.extern hijackSFX__26DynamicSoundLoaderUnloaderFi
.extern ShittySoundValue
#.extern storeSoundHandleByID__26DynamicSoundLoaderUnloaderFPQ34nw4r3snd11SoundHandlei

.global SFXHax
SFXHax:
	stwu sp, -0x20(sp)
	mflr r0
	stw r0, 0x24(sp)
	addi r11, sp, 32
	bl _savegpr_27

	cmpwi r5, 189
	beq removeHax
	cmpwi r5, 2000
	blt noHax

doHax:
	mr r16, r3
	mr r17, r4
	mr r19, r6
	mr r20, r7
	mr r21, r0

	#SMEW Addition: load the SFX with the hijacked SFX. If the values reaches over 1999, it can work as well.
	lis r3, DSLUInstance@h
	ori r3, r3, DSLUInstance@l
	mr r4, r5
	bl loadNewSFX__26DynamicSoundLoaderUnloaderFi

	#And save the sound handler in DynamicLoaderUnloader for some check
	# lis r3, DSLUInstance@h
	# ori r3, r3, DSLUInstance@l
	# mr r4, r17
	# lis r5, ShittySoundValue@ha
	# lwz r5, ShittySoundValue@l(r5)
	# bl storeSoundHandleByID__26DynamicSoundLoaderUnloaderFPQ34nw4r3snd11SoundHandlei

	#Then you can hijack
	lis r3, DSLUInstance@h
	ori r3, r3, DSLUInstance@l
	lis r4, ShittySoundValue@ha
	lwz r4, ShittySoundValue@l(r4)
	bl hijackSFX__26DynamicSoundLoaderUnloaderFi

	mr r5, r3

	mr r3, r16
	mr r4, r17
	mr r6, r19
	mr r7, r20

noHax:
	b PlaySoundWithFunctionB4_2

removeHax:
	li r5, 1999
	b doHax


.global loadFileAtIndex__FPUiUiPUi #r3 = file ptr; r4 = file size; r5 = ptr to write on
loadFileAtIndex__FPUiUiPUi:
	stwu r1, -0x10(r1)
	mflr r0
	stw r0, 0x14(r1)
	stw r31, 0xC(r1)

	li r6, 0
	b startLoop

continueLoop: #r6 = i; r7 = byte to write
	lwzx r7, r3, r6
	stwx r7, r5, r6

	addi r6, r6, 4

startLoop:
	cmpw r6, r4
	blt continueLoop

	
return2:
	lwz r0, 0x14(r1)
	lwz r31, 0xC(r1)
	mtlr r0
	addi r1, r1, 16
	blr
	

.extern SoundRelatedClass

.global NewSFXHax
NewSFXHax:
	lis r7, SoundRelatedClass@h
	ori r7, r7, SoundRelatedClass@l
	lwz r7, 0(r7)
	lwz r7, 0x61C(r7)
	lwz r7, -0x20(r7)
	lwz r7, 0(r7)
	lwz r7, 0(r7)

	lis r8, 0x78
	ori r8, r8, 0xEBE0
	#addi r7, r7, 0x78EBE0
	add r7, r7, r8

	cmpw r4, r7

	bne retNewSFXHax

	#no "stand-alone" currentPtr
	lis r7, DSLUInstance@h				
	ori r7, r7, DSLUInstance@l
	lwz r4, 4(r7)

retNewSFXHax:
	lwz r0, 0x10(r4)
	b doneWithNewSFXHax


.global LoadAllSFXs
LoadAllSFXs:
	stwu r1, -0x20(r1)
	mflr r0
	stw r0, 0x24(r1)
	stw r31, 0x1C(r1)
	stw r30, 0x18(r1)
	stw r29, 0x14(r1)
	stw r28, 0x10(r1)

	bl loadAllSFXs__Fv

	lwz r0, 0x24(r1)
	lwz r31, 0x1C(r1)
	lwz r30, 0x18(r1)
	lwz r29, 0x14(r1)
	lwz r28, 0x10(r1)
	mtlr r0
	addi r1, r1, 0x20

	mr r3, r29
	blr
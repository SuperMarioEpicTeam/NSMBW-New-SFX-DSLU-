#include <asm_setup.S>

.text
.align 4

# .extern update__26DynamicSoundLoaderUnloaderFv
# .extern DSLUInstance
# .extern GlobalTickCount
# .extern DoneWithUpdateDSLUSysChecks						#0x800e5098

# .global UpdateDSLUSysChecks
# UpdateDSLUSysChecks:
	# lis		r3, DSLUInstance@h
	# ori		r3, r3, DSLUInstance@h
	# bl		update__26DynamicSoundLoaderUnloaderFv

	# lis		r3, GlobalTickCount@h
	# ori		r3, r3, GlobalTickCount@l
	# b		DoneWithUpdateDSLUSysChecks


# .extern DoneWithThisGameTriggersMeALot				#80276110

# .global ThisGameTriggersMeALot
# ThisGameTriggersMeALot:
	# cmpwi	r3, 0
	# beq		GoYourself

	# lwz		r3, 4(r3)								#og instruction
	# b		DoneWithThisGameTriggersMeALot
# GoYourself:
	# li		r3, 0									#return false
	# blr

# .extern ContinueFromThisGameTriggersMeALot2			#8027611c
# .extern ReturnThisGameTriggersMeALot2				#8027621c

# .global ThisGameTriggersMeALot2
# ThisGameTriggersMeALot2:
	# cmpwi	r3, 0
	# beq		GoYourself2

	# lwz		r6, 0x28(r3)
	# b		ContinueFromThisGameTriggersMeALot2
# GoYourself2:
	# li		r3, 0
	# b		ReturnThisGameTriggersMeALot2



.global freeFileAtIndex__FPUiUi			#Passi passi... j'en ai marre
freeFileAtIndex__FPUiUi:
	stwu	r1, -0x10(r1)
	mflr	r0
	stw		r0, 0x14(r1)
	stw		r31, 0xC(r1)

	li		r6, 0
	b		startLoop2

continueLoop2:
	li		r0, 0
	stwx	r0, r3, r6

	addi	r6, r6, 4
startLoop2:
	cmpw	r6, r4
	blt		continueLoop2

return3:
	lwz r0, 0x14(r1)
	lwz r31, 0xC(r1)
	mtlr r0
	addi r1, r1, 16
	blr
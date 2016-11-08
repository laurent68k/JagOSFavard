
START:			MOVE.W	#$1879,$F00000
				MOVE.W	#$2700,SR
				MOVE	SR,$250.W
				MOVE.L	#$E00000,$24C.W
				
				CLR.L	$26E.W
				CLR.W	$F00058
				MOVE.L	#$FF,$F0002A
				
				BRA.S	L1
				
				MOVE.L	#$FF0000,$F0002A
				MOVE.W	(A7)+,$250.W
				MOVE.L	(A7)+,$24C.W
				CMPI.B	#4,$26E.W
				
				BNE.S		L1
				
				ADDQ.L	#2,$24C.W

L1:				MOVEM.L	D0-D7/A0-A7,$208.W
				MOVE.L	USP,A0
				MOVE.L	A0,$248.W

				LEA		$F02110,A0
				MOVEQ		#0,D0
				MOVE.L	4(A0),$25A.W
				MOVE.L	D0,4(A0)
				MOVE.L	(A0),D1
				ADDQ.L	#2,D1
				MOVE.L	D1,$256.W
				
				LEA		$F1A110,A0
				MOVE.L	4(A0),$262.W
				MOVE.L	D0,4(A0)
				MOVE.L	(A0),D1
				ADDQ.L	#2,D1
				MOVE.L	D1,$25E.W
				
				MOVE.L	$100.W,$252.W
				MOVE.L	$F00400,$26A.W
				
				LEA		$F00000,A0
				MOVE.W	#$35CC,D1
				MOVE.W	$F14002,D0
				BTST		#5,D0
				BNE.S		L2
				
				MOVE.W	#$35DD,D1

L2:			MOVE.W	D1,2(A0)
				MOVE.W	#8,$F10014
				
				BTST		#4,D0
				BEQ.S		L3
				
				MOVE.W	#$B5,$F10012
				MOVE.W	#$34C,$2E(A0)
				MOVE.W	#$6B1,$30(A0)
				MOVE.W	#$7D,$32(A0)
				MOVE.W	#$6CD,$34(A0)
				MOVE.W	#$28B,$36(A0)
				MOVE.W	#$A6,$38(A0)
				MOVE.W	#$A6,$3A(A0)
				MOVE.W	#$6A0,$3C(A0)
				MOVE.W	#$20B,$3E(A0)
				MOVE.W	#$1F4,$40(A0)
				MOVE.W	#$18,$42(A0)
				MOVE.W	#$205,$44(A0)
				MOVE.W	#$2E,$46(A0)
				MOVE.W	#$1F0,$48(A0)
				MOVE.W	#$1FF,$4A(A0)
				MOVE.W	#6,$4C(A0)
				MOVE.W	#$310,$54(A0)
		
				BRA.S		L4

L3:			MOVE.W	#$E2,$F10012
				MOVE.W	#$352,$2E(A0)
				MOVE.W	#$6AF,$30(A0)
				MOVE.W	#$9E,$32(A0)
				MOVE.W	#$6D5,$34(A0)
				MOVE.W	#$259,$36(A0)
				MOVE.W	#$A6,$38(A0)
				MOVE.W	#$A6,$3A(A0)
				MOVE.W	#$6A0,$3C(A0)
				MOVE.W	#$271,$3E(A0)
				MOVE.W	#$258,$40(A0)
				MOVE.W	#$22,$42(A0)
				MOVE.W	#$26A,$44(A0)
				MOVE.W	#$2E,$46(A0)
				MOVE.W	#$1AD,$48(A0)
				MOVE.W	#$265,$4A(A0)
				MOVE.W	#6,$4C(A0)
				MOVE.W	#$313,$54(A0)

L4:			MOVE.L	#$70007,D0
				MOVE.L	D0,$F1A10C
				MOVE.L	D0,$F0210C
				MOVE.W	#$2700,SR
				LEA		$1FB000,A5
				LEA		$1C5E(A5),A7
				CLR.W		$F00028
				
				BSR		SR_1
				BSR		SR_2
				BSR		SR_3

				BNE		SR_4

				BSR		SR_5
				BSR		SR_6
				BSR		SR_7
				
				MOVE.W	#$1F01,$F000E0
				CLR.L		1954(A5)
				MOVE.W	#$4C1,$F00028
				BSR		SR_9
				
				CLR.L		$1958(A5)
				LEA		$75177(PC),A0
				BSR		SR_10
				
				MOVE.L	#3,$1958(A5)
				LEA		$75207(PC),A0
				
				BSR		SR_12
				BSR		SR_13
				BSR		SR_14
				BSR		SR_15
				
				CLR.L		$1958(A5)
				BSR		$73E00
				
				MOVE.L	$1954(A5),D1
				BTST		#$1C,D1
				BNE.S		$728B8
				BSR		$7404E
				BEQ.S		$72884

				MOVE.L	#7,$1958(A5)
				BSR		$72AB6
				CLR.L		$1958(A5)
				MOVE.L	#7,$1958(A5)
				LEA		$752C7(PC),A0
				BSR		$734FE
				PEA		$72860(PC)
				MOVE.L	$1954(A5),D0
				BTST		#$1D,D0
				BNE		$741FE
				BTST		#$19,D0
				BNE.S		$728BE
				BTST		#4,D0
				BNE		$734B4
				RTS
				
				RESET
				BRA		START
				MOVE.W	#$1861,D0
				OR.B		$800400,D0
				MOVE.L	#$3D0DEAD,$F03000
				LEA		$250(A5),A2
				MOVE.L	$72900(PC),(A2)
				MOVE.L	$72904(PC),4(A2)
				MOVE.W	#$8100,$F14000
				LEA		$F00000,A0
				LEA		$800404,A1
				MOVE.W	#$FFFF,$4E(A0)
				ORI.W		#$700,SR
				JMP		(A2)
				MOVE.W	D0,(A0)
				MOVEA.L	(A1),A1
				JMP		(A1)
				NOP
				MOVE.W	#$2700,SR
				CLR.W		$F000E0
				MOVE.W	#$FFFF,$F00004E
				LEA		$4000.W,A0
				MOVE.W	#$1000,D0
				CLR.L		-(A0)
				DBF		D0,$72922
				JMP		$E00008


;
;	Subroutines identified...
;

SR_1:			LEA	$5EC2A(PC),A0
				LEA	$1E0.W,A1
				MOVE.L	(A0)+,(A1)+
				MOVE.L	(A0)+,(A1)+
				MOVE.L	(A0)+,(A1)+
				MOVE.L	(A0)+,(A1)+
				MOVE.L	(A0)+,(A1)+
				MOVE.L	(A0)+,(A1)+
				LEA	8.W,A0
				MOVEQ	#$75,D0
				LEA	$20001E0,A1
LOOPSR_1:	MOVE.L	A1,(A0)+
				ADDA.L	#$1000000,A1
				DBF	D0,LOOPSR_1
				RTS

SR_2:			LEA	$5F076(PC),A0
				LEA	$195C(A5),A1
				MOVEQ	#$32,D0
LOOP1SR_2:	MOVE.W	(A0)+,(A1)+
				DBF	D0,LOOP1SR_2
				LEA	$5F13C(PC),A0
				LEA	$1A3E(A5),A1
				MOVEQ	#$47,D0
LOOP2SR_2:	MOVE.W	(A0)+,(A1)+
				DBF	D0,LOOP2SR_2
				MOVEQ	#$3D,D0
				LEA	$5F228(PC),A0
				LEA	$19C2(A5),A1
LOOP3SR_2:	MOVE.W	(A0)+,(A1)+
				DBF	D0,LOOP3SR_2
				RTS

SR_3:			LEA	$1FFFF0,A0
				MOVEA.L	(A0)+,A1
				MOVE.L	(A0)+,D1
				BEQ.S	FINSR_3
			
				CMPI.L	#$22071970,(A0)
				BNE.S	FINSR_3
			
				MOVE.L	A1,D0
				AND.W	#3,D0
				BNE.S	FINSR_3
				MOVE.L	D1,D0
				LSL.L	#2,D0
				ADD.L	A1,D0
				CMP.L	#$200000,D0
				BHI.S	FINSR_3
			
				SUBQ.L	#1,D1
				MOVEQ	#0,D0
LOOPSR_3:	ADD.L	(A1)+,D0
				DBF	D1,LOOPSR_3
				SUB.L	#$10000,D1
				BPL.S	LOOPSR_3
			
				CMP.L	#$22071970,D0
				BNE.S	FINSR_3
				MOVEQ	#-1,D0
				RTS

FINSR_3:		moveq	#0,d0
				rts

SR_4:			BSR	SR_3
				BEQ.S	FINSR_4
				LEA	$1FFFF0,A0
				MOVEA.L	(A0)+,A1
				MOVEQ	#-1,D0
LOOPSR_4:	MOVE.W	D0,$F00058
				DBF	D0,LOOPSR_4
				CLR.W	$F00058
				JSR	(A1)
FINSR_4:		RTS

SR_5:			MOVE.W	#$30E,$F00054
				MOVE.W	#$681,$F0003C
				MOVE.W	#$CB,$F00038
				MOVE.W	#$CB,$F0003A
				MOVE.W	#$26,$F00046
				MOVE.W	#$206,$F00048
				RTS

SR_6:			MOVE	SR,D0
				BSR	SR_8
				LEA	$F00000,A0
				LEA	0(A5),A1
				MOVE.L	A1,D0
				SWAP	D0
				MOVE.L	D0,$20(A0)
				CLR.W	$26(A0)
				MOVE.W	#$1F01,$E0(A0)
				CLR.W	$E2(A0)
				MOVE.W	#$207,$4E(A0)
				MOVE	SR,D0
				LEA	$60200(PC),A0
				MOVE.L	A0,$100.W
				AND.W	#$F8FF,D0
				MOVE.W	D0,SR
				RTS

SR_7:			LEA	$5F3B6(PC),A0
				LEA	$50(A5),A1
				MOVE.L	(A0)+,D0
				LSR.W	#1,D0
				SUBQ.W	#1,D0
LOOPSR_7:	MOVE.W	(A0)+,(A1)+
				DBF	D0,LOOPSR_7
				RTS


SR_8:			MOVEM.L	D0-D5/A0-A2,-(A7)
				LEA	0(A5),A0
				LEA	(A0),A1
				LEA	$601E2(PC),A2
				MOVE.W	(A2)+,D1
				MOVE.W	D1,D5
				BPL.S	$600C2
				CMP.W	#$AFFE,D1
				BEQ	$60198
				LEA	$1A(A2),A2
				BRA.S	$600AE
				EXT.L	D1
				MOVE.L	(A2)+,D0
				BPL.S	$600CC
				NEG.L	D0
				ADD.L	A5,D0
				AND.W	#$FFF8,D0
				LSL.L	#8,D0
				MOVE.W	(A2)+,D2
				AND.W	#$7FF,D2
				EXT.L	D2
				MOVE.W	(A2)+,D3
				AND.W	#$7FF,D3
				LSL.W	#3,D3
				OR.W	D3,D1
				MOVEQ	#0,D3
				MOVE.W	(A2)+,D3
				AND.W	#$3FF,D3
				SWAP	D3
				LSR.L	#2,D3
				OR.L	D3,D1
				MOVE.W	(A2)+,D3
				LSL.W	#5,D3
				EXT.L	D3
				ADD.L	A1,D3
				LSR.L	#3,D3
				ROR.L	#8,D3
				MOVE.L	D3,D4
				AND.L	#$FF000000,D3
				OR.L	D3,D1
				MOVE.L	D4,D3
				AND.L	#$FFFFFF,D3
				OR.L	D3,D0
				MOVEM.L	D0-D1,(A0)
				MOVE.B	(A2)+,D0
				AND.W	#7,D0
				ROR.W	#4,D0
				OR.W	D0,D2
				MOVEQ	#0,D0
				MOVE.B	(A2)+,D0
				AND.W	#7,D0
				SWAP	D0
				LSR.L	#1,D0
				OR.L	D0,D2
				MOVEQ	#0,D0
				MOVE.W	(A2)+,D0
				AND.W	#$3FF,D0
				LSL.W	#2,D0
				SWAP	D0
				OR.L	D0,D2
				MOVEQ	#0,D0
				MOVE.W	(A2)+,D0
				AND.W	#$3FF,D0
				ROR.L	#4,D0
				MOVE.L	D0,D1
				CLR.W	D0
				OR.L	D0,D2
				EXT.L	D1
				MOVE.B	(A2)+,D0
				AND.W	#$7F,D0
				LSL.W	#6,D0
				OR.W	D0,D1
				MOVEQ	#0,D0
				MOVE.B	(A2)+,D0
				SWAP	D0
				LSR.L	#3,D0
				OR.L	D0,D1
				MOVEM.L	D1-D2,8(A0)
				MOVEM.W	(A2)+,D0-D2
				LEA	$20(A0),A0
				LEA	$20(A1),A1
				CMP.W	#1,D5
				BNE		$600AE
				MOVE.B	D0,-8(A0)
				MOVE.B	D1,-9(A0)
				MOVE.B	D2,-$A(A0)
				MOVE.B	#$12,-$B(A0)
				MOVE.B	-$C(A0),1.W
				BRA		$600AE
				CLR.L		(A0)+
				MOVEQ		#4,D0
				MOVE.L	D0,(A0)+
				MOVEM.L	(A7)+,D0-D5/A0-A2
				RTS

SR_9:			LEA		$250(A5),A0
				MOVE.L	#$F0FF,$F00400
				MOVE.W	#$5A0,D0
				MOVEQ		#0,D1
LOOPSR_9:	MOVE.L	D1,(A0)+
				DBF		D0,LOOPSR_9
				CLR.L		$1958(A5)
				RTS


SR_10:		MOVEM.L	D0/A0,-(A7)
LOOPSR_10:	MOVE.B	(A0)+,D0
				BEQ.S		FINSR_10
				BSR		SR_11
				BRA.S		LOOPSR_10
FINSR_10:	MOVEM.L	(A7)+,D0/A0
				RTS


SR_11:		MOVEM.L	D0/A0-A1,-(A7)
				AND.W	#$FF,D0
				ADD.W	D0,D0
				ADD.W	D0,D0
				ADD.W	D0,D0
				LEA	$65236(PC,D0.W),A0
				LEA	$250(A5),A1
				MOVE.W	$195A(A5),D0
				MULU.W	#$180,D0
				ADD.W	$1958(A5),D0
				ADDA.W	D0,A1
				ADDQ.W	#1,$1958(A5)
				CMPI.W	#$30,$1958(A5)
				BNE.S	FINSR_11
				CLR.W	$1958(A5)
				ADDQ.W	#1,$195A(A5)
				CMPI.W	#$F,$195A(A5)
				BNE.S	FINSR_11
				CLR.W	$195A(A5)

FINSR_11:	MOVE.B	(A0)+,(A1)
				MOVE.B	(A0)+,$30(A1)
				MOVE.B	(A0)+,$60(A1)
				MOVE.B	(A0)+,$90(A1)
				MOVE.B	(A0)+,$C0(A1)
				MOVE.B	(A0)+,$F0(A1)
				MOVE.B	(A0)+,$120(A1)
				MOVE.B	(A0)+,$150(A1)
				MOVEM.L	(A7)+,D0/A0-A1
				RTS

SR_12:		MOVEM.L	D0/A0,-(A7)
LOOPSR_12:	MOVE.B	(A0)+,D0
				BEQ.S	FINSR_12
				BSR	SR_11
				BRA.S	LOOPSR_12
FINSR_12:	MOVEM.L	(A7)+,D0/A0
				RTS

SR_13:		MOVE.L	#9,$1958(A5)
				LEA	$5EBB8(PC),A0
				LEA	$208.W,A1
				MOVEQ	#3,D2
				BSR	$5F6E8
				MOVEQ	#3,D1
				MOVE.L	(A1)+,D0
				BSR	$5F718
				DBF	D1,$5EB30
				CLR.W	$1958(A5)
				ADDQ.W	#1,$195A(A5)
				ADDQ.L	#7,A0
				DBF	D2,$5EB2A
				BSR	$5F6E8
				MOVE.L	$248.W,D0
				BSR	$5F718
				ADDQ.L	#7,A0
				BSR	$5F6E8
				ADDQ.L	#6,A0
				MOVE.W	$250.W,D0
				BSR	$5F72A
				ADDQ.W	#1,$1958(A5)
				BSR	$5F6E8
				ADDQ.L	#6,A0
				MOVE.L	$24C.W,D0
				BSR	$5F718
				BSR	$5F6E8
				ADDQ.L	#6,A0
				MOVE.B	$26E.W,D0
				BSR	$5F732
				CLR.W	$1958(A5)
				ADDQ.W	#1,$195A(A5)
				BSR	$5F6E8
				ADDQ.L	#7,A0
				MOVE.L	$256.W,D0
				BSR	$5F718
				MOVE.L	$25A.W,D0
				BSR	$5F718
				BSR	$5F6E8
				MOVE.L	$25E.W,D0
				BSR	$5F718
				MOVE.L	$262.W,D0
				BSR	$5F718
				RTS
			
SR_14:		MOVE.L	$F02114,D0
				BCLR	#0,D0
				MOVE.L	D0,$F02114
				MOVE.L	D0,$25A.W
				MOVEQ	#7,D0
				LEA	$F02100,A1
				LEA	$10(A1),A0
				MOVE.L	(A0),D0
				ADDQ.L	#2,D0
				MOVE.L	D0,$256.W
				MOVE.L	(A1),D0
				MOVE.L	D0,D2
				MOVEQ	#7,D1
				AND.W	D0,D1
				MOVE.W	D1,$272.W
				AND.W	#$BE0F,D0
				MOVE.L	D0,(A1)
				MOVE.L	#$1FB050,(A0)+
				MOVEQ	#1,D0
				MOVE.L	D0,(A0)
				MOVE.L	(A0),D0
				BTST	#0,D0
				BNE.S	$5EEC2
				MOVE.L	D2,(A1)
				RTS

SR_15:		MOVE.L	$F1A114,D0
				BCLR	#0,D0
				MOVE.L	D0,$F1A114
				MOVE.L	D0,$262.W
				LEA	$F1A100,A1
				LEA	$10(A1),A0
				MOVE.L	(A0),D0
				ADDQ.L	#2,D0
				MOVE.L	D0,$25E.W
				MOVE.L	(A1),D0
				MOVE.L	D0,D2
				MOVEQ	#7,D1
				AND.W	D0,D1
				MOVE.W	D1,$274.W
				AND.W	#$BE0F,D0
				MOVE.L	D0,(A1)
				MOVE.L	#$1FB132,(A0)
				MOVEQ	#1,D0
				MOVE.L	D0,4(A0)
				MOVE.L	4(A0),D0
				BTST	#0,D0
				BNE.S	$5EF70
				MOVE.L	D2,(A1)
				RTS


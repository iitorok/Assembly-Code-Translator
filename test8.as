	beq	7	7	-1
	jalr	0	7
	sw	4	1	-5
	beq	0	7	-1
	lw	3	6	32767
	sw	7	2	32767
	beq	7	7	32767
	lw	1	2	-32768
	sw	4	7	-32768
	beq	7	0	-32768
	halt
	noop

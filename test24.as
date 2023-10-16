Bear	lw	0	2	SubAdr
	add	1	2	1
Monday	jalr	7	6
	beq	1	2	Bear
	sw	2	1	neg2
	lw	6	5	Monday
neg2	.fill	-2
	.fill	SubAdr
	.fill	Bear

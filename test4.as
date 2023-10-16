start	beq	0	0	0
	lw	1	1	seven
	add	1	2	1
	add	2	1	1
	jalr	0	0
	sw	0	0	0
	noop
done	halt
	noop
seven	.fill	7
stAddr	.fill	start

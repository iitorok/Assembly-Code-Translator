start	lw	0	1	start
	beq	1	1	start
	beq	1	5	start
	jalr	1	7
	beq	1	6	1
	noop
	noop
	noop
	halt
done	halt
neg2	.fill	-2
stAddr	.fill	start

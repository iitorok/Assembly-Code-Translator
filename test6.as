	jalr	0	1
start2	beq	1	1	start
	beq	1	5	start2
	jalr	1	7
	sw	7	7	7
	lw	3	4	done
	halt
start	halt
	beq	3	3	-2
done	halt
neg2	.fill	-2
stAddr	.fill	start

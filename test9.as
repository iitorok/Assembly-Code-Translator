	lw	0	1	ArrayS
	lw	0	2	Count
	lw	0	3	Fcall
	jalr	3	7
	halt
Find	lw	0	6	NegOne	// r6=-1
	add	0	2	3	// r3=Count
Top	add	2	6	2	// Decrement Count
	add	1	2	4	// 4 is address
	lw	4	5	0	// ld array element
	beq	5	0	skip	// is  element 0?
	add	3	6	3	// if not sub 1
skip	beq	0	2	Done	// if Count=0 we are done
	beq	0	0	Top
Done	jalr	7	5
Zero	.fill	0
NegOne	.fill	-1
Fcall	.fill	Find
ArrayS	.fill	Array
Count	.fill	4
Array	.fill	0
	.fill	1
	.fill	1111
	.fill	0

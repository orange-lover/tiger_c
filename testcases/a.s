.data
.align 2
L34:
.word 1
.ascii " "
.align 2
L33:
.word 1
.ascii "\n"
.align 2
L18:
.word 1
.ascii "0"
.align 2
L8:
.word 1
.ascii "\n"
.align 2
L7:
.word 1
.ascii " "
.align 2
L1:
.word 1
.ascii "9"
.align 2
L0:
.word 1
.ascii "0"
.text
.align 2
.globl tigerMain
tigerMain:
sw $fp,-44($sp)
move $fp,$sp
addi $sp,$sp,-56

L39: 
sw $ra, -8($fp)
sw $s0, -12($fp)
sw $s1, -16($fp)
sw $s2, -20($fp)
sw $s3, -24($fp)
sw $s4, -28($fp)
sw $s5, -32($fp)
sw $s6, -36($fp)
sw $s7, -40($fp)
sw $a0, 0($fp)
addi $s7, $fp, -4
move $s7, $s7
jal tigerGetchar
nop
move $v0, $v0
sw $v0, 0($s7)
addi $a0, $fp, 0
jal readlist
nop
move $s6, $v0
addi $s7, $fp, -4
move $s7, $s7
jal tigerGetchar
nop
move $v0, $v0
sw $v0, 0($s7)
addi $a0, $fp, 0
jal readlist
nop
move $a2, $v0
move $fp, $fp
addi $a0, $fp, 0
move $a1, $s6
move $a2, $a2
jal merge
nop
move $a1, $v0
move $a0, $fp
move $a1, $a1
jal printlist
nop
move $v0, $v0
lw $s0, -12($fp)
lw $s1, -16($fp)
lw $s2, -20($fp)
lw $s3, -24($fp)
lw $s4, -28($fp)
lw $s5, -32($fp)
lw $s6, -36($fp)
lw $s7, -40($fp)
lw $ra, -8($fp)
j L38
nop

L38: 

addi $sp,$sp,56
lw $fp,-44($sp)
jr $ra




.text
.align 2
.globl printlist
printlist:
sw $fp,-40($sp)
move $fp,$sp
addi $sp,$sp,-48

L41: 
sw $ra, -4($fp)
sw $s0, -8($fp)
sw $s1, -12($fp)
sw $s2, -16($fp)
sw $s3, -20($fp)
sw $s4, -24($fp)
sw $s5, -28($fp)
sw $s6, -32($fp)
sw $s7, -36($fp)
sw $a0, 0($fp)
sw $a1, 4($fp)
lw $s7,4($fp)
beq $s7, $zero, L35
nop

L36: 
lw $s7,4($fp)
lw $a0,0($s7)
move $a0, $a0
jal printInt
nop
la $a0,L34
move $a0, $a0
jal printStr
nop
lw $a0,0($fp)
move $a0, $a0
lw $s7,4($fp)
lw $a1,4($s7)
move $a1, $a1
jal printlist
nop
move $v0, $v0

L37: 
move $v0, $v0
lw $s0, -8($fp)
lw $s1, -12($fp)
lw $s2, -16($fp)
lw $s3, -20($fp)
lw $s4, -24($fp)
lw $s5, -28($fp)
lw $s6, -32($fp)
lw $s7, -36($fp)
lw $ra, -4($fp)
j L40
nop

L35: 
la $a0,L33
move $a0, $a0
jal printStr
nop
move $v0, $v0
j L37
nop

L40: 

addi $sp,$sp,48
lw $fp,-40($sp)
jr $ra




.text
.align 2
.globl merge
merge:
sw $fp,-40($sp)
move $fp,$sp
addi $sp,$sp,-52

L43: 
sw $ra, -4($fp)
sw $s0, -8($fp)
sw $s1, -12($fp)
sw $s2, -16($fp)
sw $s3, -20($fp)
sw $s4, -24($fp)
sw $s5, -28($fp)
sw $s6, -32($fp)
sw $s7, -36($fp)
sw $a0, 0($fp)
sw $a1, 4($fp)
sw $a2, 8($fp)
lw $s7,4($fp)
beq $s7, $zero, L30
nop

L31: 
lw $s7,8($fp)
beq $s7, $zero, L27
nop

L28: 
lw $s7,4($fp)
lw $s7,0($s7)
lw $s6,8($fp)
lw $s6,0($s6)
blt $s7, $s6, L24
nop

L25: 
li $a0,8
move $a0, $a0
jal allocRecord
nop
move $s6, $v0
addi $s7, $s6, 4
move $s5, $s7
lw $a0,0($fp)
move $a0, $a0
lw $a1,4($fp)
move $a1, $a1
lw $s7,8($fp)
lw $a2,4($s7)
move $a2, $a2
jal merge
nop
move $v0, $v0
sw $v0, 0($s5)
lw $s7,8($fp)
lw $s7,0($s7)
sw $s7, 0($s6)
move $v0, $s6

L26: 
move $v0, $v0

L29: 
move $v0, $v0

L32: 
move $v0, $v0
lw $s0, -8($fp)
lw $s1, -12($fp)
lw $s2, -16($fp)
lw $s3, -20($fp)
lw $s4, -24($fp)
lw $s5, -28($fp)
lw $s6, -32($fp)
lw $s7, -36($fp)
lw $ra, -4($fp)
j L42
nop

L30: 
lw $v0, 8($fp)
j L32
nop

L27: 
lw $v0, 4($fp)
j L29
nop

L24: 
li $a0,8
move $a0, $a0
jal allocRecord
nop
move $s6, $v0
addi $s7, $s6, 4
move $s7, $s7
lw $a0,0($fp)
move $a0, $a0
lw $s5,4($fp)
lw $a1,4($s5)
move $a1, $a1
lw $a2,8($fp)
move $a2, $a2
jal merge
nop
move $v0, $v0
sw $v0, 0($s7)
lw $s7,4($fp)
lw $s7,0($s7)
sw $s7, 0($s6)
move $v0, $s6
j L26
nop

L42: 

addi $sp,$sp,52
lw $fp,-40($sp)
jr $ra




.text
.align 2
.globl readlist
readlist:
sw $fp,-40($sp)
move $fp,$sp
addi $sp,$sp,-48

L45: 
sw $ra, -4($fp)
sw $s0, -8($fp)
sw $s1, -12($fp)
sw $s2, -16($fp)
sw $s3, -20($fp)
sw $s4, -24($fp)
sw $s5, -28($fp)
sw $s6, -32($fp)
sw $s7, -36($fp)
sw $a0, 0($fp)
li $a0,4
move $a0, $a0
jal allocRecord
nop
move $v0, $v0
sw $zero, 0($v0)
move $s7, $v0
lw $a0,0($fp)
move $a0, $a0
move $a1, $s7
jal readint
nop
move $s5, $v0
lw $s7,0($s7)
bne $s7, $zero, L21
nop

L22: 
li $v0, 0

L23: 
move $v0, $v0
lw $s0, -8($fp)
lw $s1, -12($fp)
lw $s2, -16($fp)
lw $s3, -20($fp)
lw $s4, -24($fp)
lw $s5, -28($fp)
lw $s6, -32($fp)
lw $s7, -36($fp)
lw $ra, -4($fp)
j L44
nop

L21: 
li $a0,8
move $a0, $a0
jal allocRecord
nop
move $s7, $v0
addi $s6, $s7, 4
move $s6, $s6
lw $a0,0($fp)
move $a0, $a0
jal readlist
nop
move $v0, $v0
sw $v0, 0($s6)
sw $s5, 0($s7)
move $v0, $s7
j L23
nop

L44: 

addi $sp,$sp,48
lw $fp,-40($sp)
jr $ra




.text
.align 2
.globl readint
readint:
sw $fp,-40($sp)
move $fp,$sp
addi $sp,$sp,-48

L47: 
sw $ra, -4($fp)
sw $s0, -8($fp)
sw $s1, -12($fp)
sw $s2, -16($fp)
sw $s3, -20($fp)
sw $s4, -24($fp)
sw $s5, -28($fp)
sw $s6, -32($fp)
sw $s7, -36($fp)
sw $a0, 0($fp)
sw $a1, 4($fp)
li $s6, 0
addi $a0, $fp, 0
jal skipto
nop
lw $s7,4($fp)
addi $s7, $s7, 0
move $s7, $s7
addi $a0, $fp, 0
lw $s5,0($fp)
lw $a1,-4($s5)
move $a1, $a1
jal isdigit
nop
move $v0, $v0
sw $v0, 0($s7)

L19: 
addi $a0, $fp, 0
lw $s7,0($fp)
lw $a1,-4($s7)
move $a1, $a1
jal isdigit
nop
move $v0, $v0
beq $v0, $zero, L17
nop

L20: 
mul $s7, $s6, 10
move $s7, $s7
lw $s6,0($fp)
lw $a0,-4($s6)
move $a0, $a0
jal ord
nop
move $v0, $v0
add $s7, $s7, $v0
move $s7, $s7
la $a0,L18
move $a0, $a0
jal ord
nop
move $v0, $v0
sub $s7, $s7, $v0
move $s6, $s7
lw $s7,0($fp)
addi $s7, $s7, -4
move $s7, $s7
jal tigerGetchar
nop
move $v0, $v0
sw $v0, 0($s7)
j L19
nop

L17: 
move $v0, $s6
lw $s0, -8($fp)
lw $s1, -12($fp)
lw $s2, -16($fp)
lw $s3, -20($fp)
lw $s4, -24($fp)
lw $s5, -28($fp)
lw $s6, -32($fp)
lw $s7, -36($fp)
lw $ra, -4($fp)
j L46
nop

L46: 

addi $sp,$sp,48
lw $fp,-40($sp)
jr $ra




.text
.align 2
.globl skipto
skipto:
sw $fp,-40($sp)
move $fp,$sp
addi $sp,$sp,-52

L49: 
sw $ra, -4($fp)
sw $s0, -8($fp)
sw $s1, -12($fp)
sw $s2, -16($fp)
sw $s3, -20($fp)
sw $s4, -24($fp)
sw $s5, -28($fp)
sw $s6, -32($fp)
sw $s7, -36($fp)
sw $a0, 0($fp)

L15: 
lw $s7,0($fp)
lw $s7,0($s7)
lw $a0,-4($s7)
move $a0, $a0
la $a1,L7
move $a1, $a1
jal stringcmp
nop
move $v0, $v0
beq $v0, $zero, L9
nop

L10: 
li $s6, 1
lw $s7,0($fp)
lw $s7,0($s7)
lw $a0,-4($s7)
move $a0, $a0
la $a1,L8
move $a1, $a1
jal stringcmp
nop
move $v0, $v0
beq $v0, $zero, L12
nop

L13: 
li $s6, 0

L12: 
move $s7, $s6

L11: 
beq $s7, $zero, L14
nop

L16: 
lw $s7,0($fp)
lw $s7,0($s7)
addi $s7, $s7, -4
move $s7, $s7
jal tigerGetchar
nop
move $v0, $v0
sw $v0, 0($s7)
j L15
nop

L9: 
li $s7, 1
j L11
nop

L14: 
li $v0, 0
lw $s0, -8($fp)
lw $s1, -12($fp)
lw $s2, -16($fp)
lw $s3, -20($fp)
lw $s4, -24($fp)
lw $s5, -28($fp)
lw $s6, -32($fp)
lw $s7, -36($fp)
lw $ra, -4($fp)
j L48
nop

L48: 

addi $sp,$sp,52
lw $fp,-40($sp)
jr $ra




.text
.align 2
.globl isdigit
isdigit:
sw $fp,-40($sp)
move $fp,$sp
addi $sp,$sp,-48

L51: 
sw $ra, -4($fp)
sw $s0, -8($fp)
sw $s1, -12($fp)
sw $s2, -16($fp)
sw $s3, -20($fp)
sw $s4, -24($fp)
sw $s5, -28($fp)
sw $s6, -32($fp)
sw $s7, -36($fp)
sw $a0, 0($fp)
sw $a1, 4($fp)
lw $s7,0($fp)
lw $s7,0($s7)
lw $a0,-4($s7)
move $a0, $a0
jal ord
nop
move $v0, $v0
move $s7, $v0
la $a0,L0
move $a0, $a0
jal ord
nop
move $v0, $v0
bge $s7, $v0, L2
nop

L3: 
li $v0, 0

L4: 
move $v0, $v0
lw $s0, -8($fp)
lw $s1, -12($fp)
lw $s2, -16($fp)
lw $s3, -20($fp)
lw $s4, -24($fp)
lw $s5, -28($fp)
lw $s6, -32($fp)
lw $s7, -36($fp)
lw $ra, -4($fp)
j L50
nop

L2: 
li $s6, 1
lw $s7,0($fp)
lw $s7,0($s7)
lw $a0,-4($s7)
move $a0, $a0
jal ord
nop
move $v0, $v0
move $s7, $v0
la $a0,L1
move $a0, $a0
jal ord
nop
move $v0, $v0
ble $s7, $v0, L5
nop

L6: 
li $s6, 0

L5: 
move $v0, $s6
j L4
nop

L50: 

addi $sp,$sp,48
lw $fp,-40($sp)
jr $ra





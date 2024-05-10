## ---- WORKED WITH MATTHEW KUM ---- ##
.data 

orig: .space 100	# In terms of bytes (25 elements * 4 bytes each)
sorted: .space 100

str0: .asciiz "Enter the number of assignments (between 1 and 25): "
str1: .asciiz "Enter score: "
str2: .asciiz "Original scores: "
str3: .asciiz "Sorted scores (in descending order): "
str4: .asciiz "Enter the number of (lowest) scores to drop: "
str5: .asciiz "Average (rounded down) with dropped scores removed: "
space: .asciiz " "
newLine: .asciiz "\n"

.text 


# ======================================================================================================================================= #
# ======================================================================================================================================= #

# This is the main program.
# It first asks user to enter the number of assignments.
# It then asks user to input the scores, one at a time.
# It then calls selSort to perform selection sort.
# It then calls printArray twice to print out contents of the original and sorted scores.
# It then asks user to enter the number of (lowest) scores to drop.
# It then calls calcSum on the sorted array with the adjusted length (to account for dropped scores).
# It then prints out average score with the specified number of (lowest) scores dropped from the calculation.
main: 
	addi $sp, $sp -4
	sw $ra, 0($sp)
	li $v0, 4 
	la $a0, str0 
	syscall 
	li $v0, 5	# Read the number of scores from user
	syscall
	move $s0, $v0	# $s0 = numScores
	move $t0, $0
	la $s1, orig	# $s1 = orig
	la $s2, sorted	# $s2 = sorted
loop_in:
	li $v0, 4 
	la $a0, str1 
	syscall 
	sll $t1, $t0, 2
	add $t1, $t1, $s1
	li $v0, 5	# Read elements from user
	syscall
	sw $v0, 0($t1)
	addi $t0, $t0, 1
	bne $t0, $s0, loop_in
	
	move $a0, $s0
	jal selSort	# Call selSort to perform selection sort in original array
	
	li $v0, 4 
	la $a0, str2 
	syscall
	move $a0, $s1	# More efficient than la $a0, orig
	move $a1, $s0
	jal printArray	# Print original scores
	li $v0, 4 
	la $a0, str3 
	syscall 
	move $a0, $s2	# More efficient than la $a0, sorted
	jal printArray	# Print sorted scores
	
	li $v0, 4 
	la $a0, str4 
	syscall 
	li $v0, 5	# Read the number of (lowest) scores to drop
	syscall
	move $a1, $v0
	sub $a1, $s0, $a1	# numScores - drop
	move $a0, $s2
	jal calcSum	# Call calcSum to RECURSIVELY compute the sum of scores that are not dropped
	
	# Your code here to compute average and print it
	move $t1, $v0
	div $a1, $a1, 4
	
	li $v0, 4
	la $a0, str5	# Prints line before showing average with dropped scores
	syscall 
	
	li $v0, 1
	div $t1, $t1, $a1	# calcSum(sorted, numScores - drop)/(numScores - drop))
	move $a0, $t1
	syscall			# Prints out the average rounded down
	
	#div $t1, $a2, $a1
	
	lw $ra, 0($sp)
	addi $sp, $sp 4
	li $v0, 10 
	syscall
	

# ======================================================================================================================================= #
# ======================================================================================================================================= #
	
# printList takes in an array and its size as arguments. 
# It prints all the elements in one line with a newline at the end.
printArray:
	# Your implementation of printList here	
	#$a0 holds the array, and $a1 holds the length
	
	li $t0, 0 # reset $t0 to act as the loop counter
	li $t3, 0 # reset array index holder
	
	la $t4, 0($a0) # save the address of the array that was sent in
	
loopStart: 
	#print the value
	li $v0, 1
	add $t5, $t4, $t3 # set $t5 to the value of the address at $a0 plus the index (4 * index)
	lw $a0, 0($t5)
	syscall
	
	#print a space
	li $v0, 4
	la $a0, space
	syscall
	
	# end of the loop
	addi, $t3, $t3, 4
	addi, $t0, $t0, 1
	bne $t0, $a1, loopStart
		
	#print a newLine
	li $v0, 4
	la $a0, newLine
	syscall

	jr $ra
	
	
# ======================================================================================================================================= #
# ======================================================================================================================================= #
	
# selSort takes in the number of scores as argument. 
# It performs SELECTION sort in descending order and populates the sorted array
selSort:
	# Your implementation of selSort here
	# PROCESS:
	# copy over all the elements of orig into the new array.
	# then loop through and check if each element has an element that is bigger than it in the array
	# after the end of the inner loop swap the values
	move $s0, $a0
	
	# copy over the array
	li $t0, 0 # reset $t0 to act as the loop counter
	li $t3, 0 # reset array index holder
	
copyLoop:  
	# IF FACE AN ISSUE WHERE PRINTING ORIG PRINTS NOTHING. TRY DOING WHATS ON THE THIS -> move orig's value to $t4, then copy $t4's new value to $t5 so that they both have it. Then return $t4's value to orig and transfer $t5's value to sorted
	lw $t4, orig($t3)
	#move $t5, $t4
	sw $t4, orig($t3)
	sw $t4, sorted($t3)

	addi, $t3, $t3, 4
	addi, $t0, $t0, 1
	bne $t0, $s0, copyLoop

	# sort via nested loop
	li $t0, 0 # reset $t0 to act as the outer loop counter
	li $t1, 0 # set $t1 to act as the inner loop counter
	li $t3, 0 # reset $t3 to act as the outer array index holder
	li $t4, 0 # reset $t4 to act as the inner array index holder
	
	# check if is only 1 score
	addi $t0, $0, 1
	beq $a0, $t0, onlyOneElement
	li $t0, 0
	
	subi $t5, $s0, 1 # use as the outer loop limit (len - 1 in the c version)
	#$t6 will serve as maxIndex in the c version
	
outerLoop:
	move $t6, $t3 # set maxIndex.
	li $s7, 4
	addi $t1, $t0, 1 # set $t1 to $t0 + 1. AKA set the inner loop limit to the outer loop limit + 1
	mul $t4, $s7, $t1 # set $t4 to the index($t1) times 4(the number of bytes)
	
innerLoop:
	# check if sorted($t4) is greater than sorted($t6). I.e. check if the current value is greater than the value indicated by maxIndex
	# do so by checking if less than or equal
	lw $s3, sorted($t4) # sorted[j] in the c version
	lw $s4, sorted($t6) # sorted[maxIndex] in the c version
	ble $s3, $s4, isLessOrEqual # if the current value is not greater. AKA if the current value is less than or equal
	move $t6, $t4 # this will trigger if the current index is greater
	
isLessOrEqual:
	# for ending the inner loop
	addi, $t4, $t4, 4
	addi, $t1, $t1, 1
	bne $t1, $s0, innerLoop
	# inner loop ends here
	
	#swap the max element in sorted with the element pointed to by $t3
	lw, $t2, sorted($t6) # temp in c version
	lw $s5, sorted($t3) # sorted[i] in c version
	sw $s5, sorted($t6)
	sw $t2, sorted($t3)

	# for ending the outer loop
	addi, $t3, $t3, 4
	addi, $t0, $t0, 1
	bne $t0, $t5, outerLoop			

onlyOneElement:
	jr $ra
	

# ======================================================================================================================================= #
# ======================================================================================================================================= #
	
# calcSum takes in an array and its size as arguments.
# It RECURSIVELY computes and returns the sum of elements in the array.
# Note: you MUST NOT use iterative approach in this function.
calcSum:
	# Your implementation of calcSum here
	# a1 = len, a0 = *arr
	bgt $a1, $0, ifGreaterThanZero	# if the length of the array is greater than 0
	li $v0, 0
	jr $ra
	
ifGreaterThanZero:
	addi $sp, $sp, -8	# Save space for 2 words in stack
	sw $a1, 0($sp)
	sw $ra, 4($sp)
	addi $a1, $a1, -1	# len - 1
	jal calcSum
	
	lw $a1, 0($sp)
	lw $ra, 4($sp)
	
	li $t0, 4
	mul $a1, $a1, $t0	
	add $t1, $a0, $a1	# calcSum(arr, len - 1) + arr[len - 1]
	addi $t1, $t1, -4
		
	lw $t1, 0($t1)
	add $v0, $v0, $t1
	
	addi $sp, $sp, 8
	jr $ra

End:

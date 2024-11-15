.set noreorder
    li $v0,5
    sb $v0,0x66($a1)
    li $v1,3
    jr $ra
    sb $v1,0x0($a1)
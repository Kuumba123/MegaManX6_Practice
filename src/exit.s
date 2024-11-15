.set noreorder
    li $a1,1
    nop
    j 0x80033034
    sb $a1,0x26($s0)
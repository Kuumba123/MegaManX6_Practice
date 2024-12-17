.set noreorder
    li $a1,1
    nop
    j ExitBranch
    sb $a1,0x26($s0)
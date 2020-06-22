maiorIndex(int*, int):
        blez    a1,.L5
        mv      a4,a0
        li      a5,0
        li      a0,0
        li      a2,0
.L4:
        lw      a3,0(a4)
        addi    a4,a4,4
        bge     a2,a3,.L3
        mv      a0,a5
        mv      a2,a3
.L3:
        addiw   a5,a5,1
        bne     a1,a5,.L4
        ret
.L5:
        li      a0,0
        ret

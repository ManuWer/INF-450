# a1 guarda tamanho do vetor
addi a1,x0,12

# inicialização
addi a4,x0,1024
addi a6,x0,21
sd a6,0(a4)
addi a6,x0,4
sd a6,8(a4)
addi a6,x0,13
sd a6,16(a4)
addi a6,x0,7
sd a6,24(a4)
addi a6,x0,34
sd a6,32(a4)
addi a6,x0,10
sd a6,40(a4)
addi a6,x0,5
sd a6,48(a4)
addi a6,x0,28
sd a6,56(a4)
addi a6,x0,19
sd a6,64(a4)
addi a6,x0,30
sd a6,72(a4)
addi a6,x0,3
sd a6,80(a4)
addi a6,x0,11
sd a6,88(a4)

addi a4,a0,1024  # vetor começa na pos 1024
addi a5,x0,0 
addi a0,x0,0     # a0 maior inteiro
addi a2,x0,0     # a2 index do maior

loop: lw a3,0(a4)
	  addi a4,a4,8
	  bge a0,a3,cont
	  addi a0,a3,0 
	  addi a2,a5,0
cont: addiw a5,a5,1
	  bne a1,a5,loop

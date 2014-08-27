
obj/flash.o:     file format elf32-littlemips


Disassembly of section .text.FlashOperation:

00000000 <FlashOperation>:
   0:	6b01      	li	v1,1
   2:	f008 6a00 	li	v0,16384
   6:	eb6b      	neg	v1
   8:	ec4d      	or	a0,v0
   a:	675c      	move	v0,gp
   c:	f000 ca60 	sh	v1,0(v0)
  10:	b30d      	lw	v1,44 <FlashOperation+0x44>
  12:	b20e      	lw	v0,48 <FlashOperation+0x48>
  14:	b50e      	lw	a1,4c <FlashOperation+0x4c>
  16:	dd80      	sw	a0,0(a1)
  18:	da60      	sw	v1,0(v0)
  1a:	b30e      	lw	v1,50 <FlashOperation+0x50>
  1c:	da60      	sw	v1,0(v0)
  1e:	f010 6b00 	li	v1,32768
  22:	b20d      	lw	v0,54 <FlashOperation+0x54>
  24:	da60      	sw	v1,0(v0)
  26:	9d60      	lw	v1,0(a1)
  28:	f010 6a00 	li	v0,32768
  2c:	ea6c      	and	v0,v1
  2e:	2afb      	bnez	v0,26 <FlashOperation+0x26>
  30:	f008 6b00 	li	v1,16384
  34:	b209      	lw	v0,58 <FlashOperation+0x58>
  36:	da60      	sw	v1,0(v0)
  38:	b205      	lw	v0,4c <FlashOperation+0x4c>
  3a:	9a60      	lw	v1,0(v0)
  3c:	f006 6a00 	li	v0,12288
  40:	e820      	jr	ra
  42:	ea6c      	and	v0,v1
  44:	6655      	0x6655
  46:	aa99      	lhu	a0,50(v0)
	...
  50:	99aa      	lw	a1,40(s1)
  52:	5566      	slti	a1,102
	...

Disassembly of section .text.FlashErasePage:

0000005c <FlashErasePage>:
  5c:	b213      	lw	v0,a8 <FlashErasePage+0x4c>
  5e:	6b01      	li	v1,1
  60:	ec4c      	and	a0,v0
  62:	b213      	lw	v0,ac <FlashErasePage+0x50>
  64:	da80      	sw	a0,0(v0)
  66:	f008 6a04 	li	v0,16388
  6a:	eb6b      	neg	v1
  6c:	b411      	lw	a0,b0 <FlashErasePage+0x54>
  6e:	dc40      	sw	v0,0(a0)
  70:	675c      	move	v0,gp
  72:	f000 ca60 	sh	v1,0(v0)
  76:	b310      	lw	v1,b4 <FlashErasePage+0x58>
  78:	b210      	lw	v0,b8 <FlashErasePage+0x5c>
  7a:	da60      	sw	v1,0(v0)
  7c:	b310      	lw	v1,bc <FlashErasePage+0x60>
  7e:	da60      	sw	v1,0(v0)
  80:	f010 6b00 	li	v1,32768
  84:	b20f      	lw	v0,c0 <FlashErasePage+0x64>
  86:	da60      	sw	v1,0(v0)
  88:	9c60      	lw	v1,0(a0)
  8a:	f010 6a00 	li	v0,32768
  8e:	ea6c      	and	v0,v1
  90:	2afb      	bnez	v0,88 <FlashErasePage+0x2c>
  92:	f008 6b00 	li	v1,16384
  96:	b20c      	lw	v0,c4 <FlashErasePage+0x68>
  98:	da60      	sw	v1,0(v0)
  9a:	b206      	lw	v0,b0 <FlashErasePage+0x54>
  9c:	9a60      	lw	v1,0(v0)
  9e:	f006 6a00 	li	v0,12288
  a2:	e820      	jr	ra
  a4:	ea6c      	and	v0,v1
  a6:	6500      	nop
  a8:	ffff      	daddiu	a3,sp,124
  aa:	1fff 0000 	jalx	ffc0000 <delay_count.6841+0xffbfd44>
  ae:	0000      	addiu	s0,sp,0
  b0:	0000      	addiu	s0,sp,0
  b2:	0000      	addiu	s0,sp,0
  b4:	6655      	0x6655
  b6:	aa99      	lhu	a0,50(v0)
  b8:	0000      	addiu	s0,sp,0
  ba:	0000      	addiu	s0,sp,0
  bc:	99aa      	lw	a1,40(s1)
  be:	5566      	slti	a1,102
	...

Disassembly of section .text.FlashWriteWord:

00000124 <FlashWriteWord>:
 124:	b214      	lw	v0,174 <FlashWriteWord+0x50>
 126:	6b01      	li	v1,1
 128:	ec4c      	and	a0,v0
 12a:	b214      	lw	v0,178 <FlashWriteWord+0x54>
 12c:	da80      	sw	a0,0(v0)
 12e:	b214      	lw	v0,17c <FlashWriteWord+0x58>
 130:	daa0      	sw	a1,0(v0)
 132:	f008 6a01 	li	v0,16385
 136:	eb6b      	neg	v1
 138:	b412      	lw	a0,180 <FlashWriteWord+0x5c>
 13a:	dc40      	sw	v0,0(a0)
 13c:	675c      	move	v0,gp
 13e:	f000 ca60 	sh	v1,0(v0)
 142:	b311      	lw	v1,184 <FlashWriteWord+0x60>
 144:	b211      	lw	v0,188 <FlashWriteWord+0x64>
 146:	da60      	sw	v1,0(v0)
 148:	b311      	lw	v1,18c <FlashWriteWord+0x68>
 14a:	da60      	sw	v1,0(v0)
 14c:	f010 6b00 	li	v1,32768
 150:	b210      	lw	v0,190 <FlashWriteWord+0x6c>
 152:	da60      	sw	v1,0(v0)
 154:	9c60      	lw	v1,0(a0)
 156:	f010 6a00 	li	v0,32768
 15a:	ea6c      	and	v0,v1
 15c:	2afb      	bnez	v0,154 <FlashWriteWord+0x30>
 15e:	f008 6b00 	li	v1,16384
 162:	b20d      	lw	v0,194 <FlashWriteWord+0x70>
 164:	da60      	sw	v1,0(v0)
 166:	b207      	lw	v0,180 <FlashWriteWord+0x5c>
 168:	9a60      	lw	v1,0(v0)
 16a:	f006 6a00 	li	v0,12288
 16e:	e820      	jr	ra
 170:	ea6c      	and	v0,v1
 172:	6500      	nop
 174:	ffff      	daddiu	a3,sp,124
 176:	1fff 0000 	jalx	ffc0000 <delay_count.6841+0xffbfd44>
 17a:	0000      	addiu	s0,sp,0
	...
 184:	6655      	0x6655
 186:	aa99      	lhu	a0,50(v0)
 188:	0000      	addiu	s0,sp,0
 18a:	0000      	addiu	s0,sp,0
 18c:	99aa      	lw	a1,40(s1)
 18e:	5566      	slti	a1,102
	...


obj/usb_function_hid.o:     file format elf32-littlemips


Disassembly of section .text.USBCheckHIDRequest:

00000000 <USBCheckHIDRequest>:
   0:	677c      	move	v1,gp
   2:	6a1f      	li	v0,31
   4:	f000 a380 	lbu	a0,0(v1)
   8:	ea8c      	and	v0,a0
   a:	7201      	cmpi	v0,1
   c:	6001      	bteqz	10 <USBCheckHIDRequest+0x10>
   e:	e8a0      	jrc	ra
  10:	67a3      	move	a1,v1
  12:	f000 4d00 	addiu	a1,0
  16:	a544      	lbu	v0,4(a1)
  18:	ea11      	zeb	v0
  1a:	2a65      	bnez	v0,e6 <USBCheckHIDRequest+0xe6>
  1c:	a541      	lbu	v0,1(a1)
  1e:	ea11      	zeb	v0
  20:	7206      	cmpi	v0,6
  22:	601e      	bteqz	60 <USBCheckHIDRequest+0x60>
  24:	f000 a340 	lbu	v0,0(v1)
  28:	ea11      	zeb	v0
  2a:	3456      	srl	a0,v0,5
  2c:	6a03      	li	v0,3
  2e:	ea8c      	and	v0,a0
  30:	7201      	cmpi	v0,1
  32:	6159      	btnez	e6 <USBCheckHIDRequest+0xe6>
  34:	a541      	lbu	v0,1(a1)
  36:	ea11      	zeb	v0
  38:	4afe      	addiu	v0,-2
  3a:	5a0a      	sltiu	v0,10
  3c:	60e8      	bteqz	e <USBCheckHIDRequest+0xe>
  3e:	3644      	sll	a2,v0,1
  40:	0c03      	la	a0,4c <USBCheckHIDRequest+0x4c>
  42:	e4d9      	addu	a2,a0,a2
  44:	8ec0      	lh	a2,0(a2)
  46:	e4d1      	addu	a0,a2
  48:	ec80      	jrc	a0
  4a:	6500      	nop
  4c:	0079      	addiu	s0,sp,484
  4e:	0065      	addiu	s0,sp,404
  50:	ffc3      	daddiu	a2,sp,12
  52:	ffc3      	daddiu	a2,sp,12
  54:	ffc3      	daddiu	a2,sp,12
  56:	ffc3      	daddiu	a2,sp,12
  58:	ffc3      	daddiu	a2,sp,12
  5a:	ffc3      	daddiu	a2,sp,12
  5c:	0055      	addiu	s0,sp,340
  5e:	008d      	addiu	s0,sp,564
  60:	a543      	lbu	v0,3(a1)
  62:	ea11      	zeb	v0
  64:	7221      	cmpi	v0,33
  66:	600f      	bteqz	86 <USBCheckHIDRequest+0x86>
  68:	7222      	cmpi	v0,34
  6a:	61dc      	btnez	24 <USBCheckHIDRequest+0x24>
  6c:	f000 a340 	lbu	v0,0(v1)
  70:	7201      	cmpi	v0,1
  72:	61d8      	btnez	24 <USBCheckHIDRequest+0x24>
  74:	b41d      	lw	a0,e8 <USBCheckHIDRequest+0xe8>
  76:	b21e      	lw	v0,ec <USBCheckHIDRequest+0xec>
  78:	da80      	sw	a0,0(v0)
  7a:	6c1d      	li	a0,29
  7c:	ca84      	sh	a0,8(v0)
  7e:	6c40      	li	a0,64
  80:	ec8b      	neg	a0
  82:	c284      	sb	a0,4(v0)
  84:	17cf      	b	24 <USBCheckHIDRequest+0x24>
  86:	f000 a340 	lbu	v0,0(v1)
  8a:	7201      	cmpi	v0,1
  8c:	61cb      	btnez	24 <USBCheckHIDRequest+0x24>
  8e:	b419      	lw	a0,f0 <USBCheckHIDRequest+0xf0>
  90:	b217      	lw	v0,ec <USBCheckHIDRequest+0xec>
  92:	da80      	sw	a0,0(v0)
  94:	6c09      	li	a0,9
  96:	ca84      	sh	a0,8(v0)
  98:	6c40      	li	a0,64
  9a:	ec8b      	neg	a0
  9c:	c284      	sb	a0,4(v0)
  9e:	17c2      	b	24 <USBCheckHIDRequest+0x24>
  a0:	6c80      	li	a0,128
  a2:	ec8b      	neg	a0
  a4:	b212      	lw	v0,ec <USBCheckHIDRequest+0xec>
  a6:	c284      	sb	a0,4(v0)
  a8:	a543      	lbu	v0,3(a1)
  aa:	f000 c340 	sb	v0,0(v1)
  ae:	e8a0      	jrc	ra
  b0:	f000 4b00 	addiu	v1,0
  b4:	b20e      	lw	v0,ec <USBCheckHIDRequest+0xec>
  b6:	da60      	sw	v1,0(v0)
  b8:	6b01      	li	v1,1
  ba:	ca64      	sh	v1,8(v0)
  bc:	6b7f      	li	v1,127
  be:	eb6b      	neg	v1
  c0:	e820      	jr	ra
  c2:	c264      	sb	v1,4(v0)
  c4:	f000 4b00 	addiu	v1,0
  c8:	b209      	lw	v0,ec <USBCheckHIDRequest+0xec>
  ca:	da60      	sw	v1,0(v0)
  cc:	6b01      	li	v1,1
  ce:	ca64      	sh	v1,8(v0)
  d0:	6b3f      	li	v1,63
  d2:	eb6b      	neg	v1
  d4:	e820      	jr	ra
  d6:	c264      	sb	v1,4(v0)
  d8:	6c80      	li	a0,128
  da:	ec8b      	neg	a0
  dc:	b204      	lw	v0,ec <USBCheckHIDRequest+0xec>
  de:	c284      	sb	a0,4(v0)
  e0:	a542      	lbu	v0,2(a1)
  e2:	f000 c340 	sb	v0,0(v1)
  e6:	e8a0      	jrc	ra
	...
  f0:	0012      	addiu	s0,sp,72
	...

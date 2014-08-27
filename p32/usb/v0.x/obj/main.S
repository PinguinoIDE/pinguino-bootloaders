
obj/main.o:     file format elf32-littlemips


Disassembly of section .text.BlinkUSBStatus:

00000000 <BlinkUSBStatus>:
   0:	677c      	move	v1,gp
   2:	f000 ab40 	lhu	v0,0(v1)
   6:	2a1a      	bnez	v0,3c <BlinkUSBStatus+0x3c>
   8:	f52e 6a0f 	li	v0,29999
   c:	f000 cb40 	sh	v0,0(v1)
  10:	b219      	lw	v0,74 <BlinkUSBStatus+0x74>
  12:	9a40      	lw	v0,0(v0)
  14:	3446      	srl	a0,v0,1
  16:	6a01      	li	v0,1
  18:	ea8c      	and	v0,a0
  1a:	2a2a      	bnez	v0,70 <BlinkUSBStatus+0x70>
  1c:	f000 9b60 	lw	v1,0(v1)
  20:	f52e 6a0f 	li	v0,29999
  24:	2318      	beqz	v1,56 <BlinkUSBStatus+0x56>
  26:	7301      	cmpi	v1,1
  28:	6020      	bteqz	6a <BlinkUSBStatus+0x6a>
  2a:	7302      	cmpi	v1,2
  2c:	601d      	bteqz	68 <BlinkUSBStatus+0x68>
  2e:	7304      	cmpi	v1,4
  30:	601b      	bteqz	68 <BlinkUSBStatus+0x68>
  32:	7310      	cmpi	v1,16
  34:	6014      	bteqz	5e <BlinkUSBStatus+0x5e>
  36:	7320      	cmpi	v1,32
  38:	6012      	bteqz	5e <BlinkUSBStatus+0x5e>
  3a:	e8a0      	jrc	ra
  3c:	b40e      	lw	a0,70 <BlinkUSBStatus+0x70>
  3e:	4aff      	addiu	v0,-1
  40:	9c80      	lw	a0,0(a0)
  42:	ea31      	zeh	v0
  44:	3586      	srl	a1,a0,1
  46:	6c01      	li	a0,1
  48:	ecac      	and	a0,a1
  4a:	f000 cb40 	sh	v0,0(v1)
  4e:	2c07      	bnez	a0,5e <BlinkUSBStatus+0x5e>
  50:	f000 9b60 	lw	v1,0(v1)
  54:	2be8      	bnez	v1,26 <BlinkUSBStatus+0x26>
  56:	6b01      	li	v1,1
  58:	b208      	lw	v0,78 <BlinkUSBStatus+0x78>
  5a:	e820      	jr	ra
  5c:	da60      	sw	v1,0(v0)
  5e:	2a08      	bnez	v0,70 <BlinkUSBStatus+0x70>
  60:	6b01      	li	v1,1
  62:	b207      	lw	v0,7c <BlinkUSBStatus+0x7c>
  64:	e820      	jr	ra
  66:	da60      	sw	v1,0(v0)
  68:	6b01      	li	v1,1
  6a:	b206      	lw	v0,80 <BlinkUSBStatus+0x80>
  6c:	e820      	jr	ra
  6e:	da60      	sw	v1,0(v0)
  70:	e8a0      	jrc	ra
  72:	6500      	nop
	...

Disassembly of section .text.PicReset:

00000084 <PicReset>:
  84:	41606000 	di
  88:	3c03aa99 	lui	v1,0xaa99
  8c:	24636655 	addiu	v1,v1,26197
  90:	3c020000 	lui	v0,0x0
  94:	ac400000 	sw	zero,0(v0)
  98:	ac430000 	sw	v1,0(v0)
  9c:	3c035566 	lui	v1,0x5566
  a0:	346399aa 	ori	v1,v1,0x99aa
  a4:	ac430000 	sw	v1,0(v0)
  a8:	24030001 	li	v1,1
  ac:	3c020000 	lui	v0,0x0
  b0:	ac430000 	sw	v1,0(v0)
  b4:	3c020000 	lui	v0,0x0
  b8:	8c420000 	lw	v0,0(v0)
  bc:	0800000e 	j	38 <BlinkUSBStatus+0x38>
  c0:	00000000 	nop

Disassembly of section .text.BootApplication:

00000148 <BootApplication>:
 148:	64f5      	save	40,ra,s0-s1
 14a:	673c      	move	s1,gp
 14c:	f000 a140 	lbu	v0,0(s1)
 150:	2a29      	bnez	v0,1a4 <BootApplication+0x5c>
 152:	f000 9940 	lw	v0,0(s1)
 156:	2204      	beqz	v0,160 <BootApplication+0x18>
 158:	a240      	lbu	v0,0(v0)
 15a:	ea11      	zeb	v0
 15c:	325e      	srl	v0,7
 15e:	2a20      	bnez	v0,1a0 <BootApplication+0x58>
 160:	f000 9940 	lw	v0,0(s1)
 164:	2204      	beqz	v0,16e <BootApplication+0x26>
 166:	a240      	lbu	v0,0(v0)
 168:	ea11      	zeb	v0
 16a:	325e      	srl	v0,7
 16c:	2a19      	bnez	v0,1a0 <BootApplication+0x58>
 16e:	f480 b41c 	lw	a0,608 <BootApplication+0x4c0>
 172:	f480 b51c 	lw	a1,60c <BootApplication+0x4c4>
 176:	1800 0000 	jal	0 <BlinkUSBStatus>
 17a:	6e40      	li	a2,64
 17c:	6c01      	li	a0,1
 17e:	6d00      	li	a1,0
 180:	f480 b60c 	lw	a2,60c <BootApplication+0x4c4>
 184:	1800 0000 	jal	0 <BlinkUSBStatus>
 188:	6f40      	li	a3,64
 18a:	f000 d940 	sw	v0,0(s1)
 18e:	6a01      	li	v0,1
 190:	f480 b400 	lw	a0,610 <BootApplication+0x4c8>
 194:	6d00      	li	a1,0
 196:	f000 c140 	sb	v0,0(s1)
 19a:	1800 0000 	jal	0 <BlinkUSBStatus>
 19e:	6e40      	li	a2,64
 1a0:	6475      	restore	40,ra,s0-s1
 1a2:	e8a0      	jrc	ra
 1a4:	f460 b304 	lw	v1,608 <BootApplication+0x4c0>
 1a8:	a340      	lbu	v0,0(v1)
 1aa:	4afe      	addiu	v0,-2
 1ac:	5a07      	sltiu	v0,7
 1ae:	60f8      	bteqz	1a0 <BootApplication+0x58>
 1b0:	3544      	sll	a1,v0,1
 1b2:	0c03      	la	a0,1bc <BootApplication+0x74>
 1b4:	e4b5      	addu	a1,a0,a1
 1b6:	8da0      	lh	a1,0(a1)
 1b8:	e4b1      	addu	a0,a1
 1ba:	ec80      	jrc	a0
 1bc:	0027      	addiu	s0,sp,156
 1be:	ffe5      	daddiu	a3,sp,20
 1c0:	0091      	addiu	s0,sp,580
 1c2:	0105      	addiu	s1,sp,20
 1c4:	00cd      	addiu	s0,sp,820
 1c6:	01a1      	addiu	s1,sp,644
 1c8:	000f      	addiu	s0,sp,60
 1ca:	6b00      	li	v1,0
 1cc:	f440 b208 	lw	v0,614 <BootApplication+0x4cc>
 1d0:	da60      	sw	v1,0(v0)
 1d2:	f7ff 6a1f 	li	v0,65535
 1d6:	6500      	nop
 1d8:	4aff      	addiu	v0,-1
 1da:	2afd      	bnez	v0,1d6 <BootApplication+0x8e>
 1dc:	1800 0000 	jal	0 <BlinkUSBStatus>
 1e0:	6500      	nop
 1e2:	6c02      	li	a0,2
 1e4:	f420 b20c 	lw	v0,610 <BootApplication+0x4c8>
 1e8:	c280      	sb	a0,0(v0)
 1ea:	6c38      	li	a0,56
 1ec:	f000 99e0 	lw	a3,0(s1)
 1f0:	c281      	sb	a0,1(v0)
 1f2:	6c03      	li	a0,3
 1f4:	6d48      	li	a1,72
 1f6:	c282      	sb	a0,2(v0)
 1f8:	6c01      	li	a0,1
 1fa:	c2a5      	sb	a1,5(v0)
 1fc:	c283      	sb	a0,3(v0)
 1fe:	6c00      	li	a0,0
 200:	c284      	sb	a0,4(v0)
 202:	c286      	sb	a0,6(v0)
 204:	6c1d      	li	a0,29
 206:	c287      	sb	a0,7(v0)
 208:	f400 b410 	lw	a0,618 <BootApplication+0x4d0>
 20c:	e791      	addu	a0,a3,a0
 20e:	3582      	srl	a1,a0,8
 210:	c288      	sb	a0,8(v0)
 212:	f600 3482 	srl	a0,24
 216:	c2a9      	sb	a1,9(v0)
 218:	c28b      	sb	a0,11(v0)
 21a:	6c01      	li	a0,1
 21c:	35a2      	srl	a1,8
 21e:	ec8b      	neg	a0
 220:	c2aa      	sb	a1,10(v0)
 222:	c28c      	sb	a0,12(v0)
 224:	f000 9940 	lw	v0,0(s1)
 228:	2204      	beqz	v0,232 <BootApplication+0xea>
 22a:	a240      	lbu	v0,0(v0)
 22c:	ea11      	zeb	v0
 22e:	325e      	srl	v0,7
 230:	2ab7      	bnez	v0,1a0 <BootApplication+0x58>
 232:	6c01      	li	a0,1
 234:	6d01      	li	a1,1
 236:	b6f7      	lw	a2,610 <BootApplication+0x4c8>
 238:	1800 0000 	jal	0 <BlinkUSBStatus>
 23c:	6f40      	li	a3,64
 23e:	f000 d940 	sw	v0,0(s1)
 242:	6a00      	li	v0,0
 244:	f000 c140 	sb	v0,0(s1)
 248:	6475      	restore	40,ra,s0-s1
 24a:	e8a0      	jrc	ra
 24c:	f000 a140 	lbu	v0,0(s1)
 250:	2210      	beqz	v0,272 <BootApplication+0x12a>
 252:	b3f3      	lw	v1,61c <BootApplication+0x4d4>
 254:	6800      	li	s0,0
 256:	6783      	move	a0,v1
 258:	1800 0000 	jal	0 <BlinkUSBStatus>
 25c:	d305      	sw	v1,20(sp)
 25e:	1800 0000 	jal	0 <BlinkUSBStatus>
 262:	4801      	addiu	s0,1
 264:	f000 a140 	lbu	v0,0(s1)
 268:	9305      	lw	v1,20(sp)
 26a:	e842      	slt	s0,v0
 26c:	f400 4b00 	addiu	v1,1024
 270:	61f2      	btnez	256 <BootApplication+0x10e>
 272:	b4ec      	lw	a0,620 <BootApplication+0x4d8>
 274:	f008 6a01 	li	v0,16385
 278:	aca0      	lhu	a1,0(a0)
 27a:	ea4b      	neg	v0
 27c:	eaac      	and	v0,a1
 27e:	cc40      	sh	v0,0(a0)
 280:	6a00      	li	v0,0
 282:	f000 c140 	sb	v0,0(s1)
 286:	178c      	b	1a0 <BootApplication+0x58>
 288:	f000 a180 	lbu	a0,0(s1)
 28c:	b0e6      	lw	s0,624 <BootApplication+0x4dc>
 28e:	240f      	beqz	a0,2ae <BootApplication+0x166>
 290:	f000 9960 	lw	v1,0(s1)
 294:	98a0      	lw	a1,0(s0)
 296:	3488      	sll	a0,2
 298:	1800 0000 	jal	0 <BlinkUSBStatus>
 29c:	e393      	subu	a0,v1,a0
 29e:	f000 a180 	lbu	a0,0(s1)
 2a2:	4804      	addiu	s0,4
 2a4:	4cff      	addiu	a0,-1
 2a6:	ec11      	zeb	a0
 2a8:	f000 c180 	sb	a0,0(s1)
 2ac:	2cf1      	bnez	a0,290 <BootApplication+0x148>
 2ae:	6500      	nop
 2b0:	6a01      	li	v0,1
 2b2:	ea4b      	neg	v0
 2b4:	f000 d940 	sw	v0,0(s1)
 2b8:	6a00      	li	v0,0
 2ba:	f000 c140 	sb	v0,0(s1)
 2be:	1770      	b	1a0 <BootApplication+0x58>
 2c0:	f000 9980 	lw	a0,0(s1)
 2c4:	4441      	addiu	v0,a0,1
 2c6:	f180 220d 	beqz	v0,5e4 <BootApplication+0x49c>
 2ca:	b3d0      	lw	v1,608 <BootApplication+0x4c0>
 2cc:	a3a2      	lbu	a1,2(v1)
 2ce:	a341      	lbu	v0,1(v1)
 2d0:	35a0      	sll	a1,8
 2d2:	ed4d      	or	a1,v0
 2d4:	a343      	lbu	v0,3(v1)
 2d6:	a364      	lbu	v1,4(v1)
 2d8:	3240      	sll	v0,8
 2da:	3240      	sll	v0,8
 2dc:	eaad      	or	v0,a1
 2de:	f600 3360 	sll	v1,24
 2e2:	ea6d      	or	v0,v1
 2e4:	ec4a      	cmp	a0,v0
 2e6:	61ad      	btnez	242 <BootApplication+0xfa>
 2e8:	b7c8      	lw	a3,608 <BootApplication+0x4c0>
 2ea:	a7a5      	lbu	a1,5(a3)
 2ec:	36aa      	srl	a2,a1,2
 2ee:	26a9      	beqz	a2,242 <BootApplication+0xfa>
 2f0:	6b00      	li	v1,0
 2f2:	d304      	sw	v1,16(sp)
 2f4:	1028      	b	346 <BootApplication+0x1fe>
 2f6:	9704      	lw	a3,16(sp)
 2f8:	324b      	sra	v0,2
 2fa:	e749      	addu	v0,a3,v0
 2fc:	4a02      	addiu	v0,2
 2fe:	b7c3      	lw	a3,608 <BootApplication+0x4c0>
 300:	3248      	sll	v0,2
 302:	e749      	addu	v0,a3,v0
 304:	4c04      	addiu	a0,4
 306:	a201      	lbu	s0,1(v0)
 308:	a2e0      	lbu	a3,0(v0)
 30a:	3000      	sll	s0,8
 30c:	ef0d      	or	a3,s0
 30e:	a202      	lbu	s0,2(v0)
 310:	a243      	lbu	v0,3(v0)
 312:	3000      	sll	s0,8
 314:	3000      	sll	s0,8
 316:	ef0d      	or	a3,s0
 318:	f600 3240 	sll	v0,24
 31c:	eaed      	or	v0,a3
 31e:	f000 d980 	sw	a0,0(s1)
 322:	db40      	sw	v0,0(v1)
 324:	f000 a140 	lbu	v0,0(s1)
 328:	4a01      	addiu	v0,1
 32a:	ea11      	zeb	v0
 32c:	720e      	cmpi	v0,14
 32e:	f000 c140 	sb	v0,0(s1)
 332:	f140 6001 	bteqz	5b8 <BootApplication+0x470>
 336:	9004      	lw	s0,16(sp)
 338:	4801      	addiu	s0,1
 33a:	e811      	zeb	s0
 33c:	e8c3      	sltu	s0,a2
 33e:	d004      	sw	s0,16(sp)
 340:	6080      	bteqz	242 <BootApplication+0xfa>
 342:	f000 9980 	lw	a0,0(s1)
 346:	f000 a140 	lbu	v0,0(s1)
 34a:	b7b7      	lw	a3,624 <BootApplication+0x4dc>
 34c:	3248      	sll	v0,2
 34e:	e74d      	addu	v1,a3,v0
 350:	6a38      	li	v0,56
 352:	e2ab      	subu	v0,a1
 354:	5200      	slti	v0,0
 356:	60cf      	bteqz	2f6 <BootApplication+0x1ae>
 358:	4a03      	addiu	v0,3
 35a:	17cd      	b	2f6 <BootApplication+0x1ae>
 35c:	f000 9940 	lw	v0,0(s1)
 360:	2205      	beqz	v0,36c <BootApplication+0x224>
 362:	a240      	lbu	v0,0(v0)
 364:	ea11      	zeb	v0
 366:	325e      	srl	v0,7
 368:	f71f 2a1a 	bnez	v0,1a0 <BootApplication+0x58>
 36c:	b4a7      	lw	a0,608 <BootApplication+0x4c0>
 36e:	6a07      	li	v0,7
 370:	b5a8      	lw	a1,610 <BootApplication+0x4c8>
 372:	c540      	sb	v0,0(a1)
 374:	a4c2      	lbu	a2,2(a0)
 376:	a441      	lbu	v0,1(a0)
 378:	a483      	lbu	a0,3(a0)
 37a:	b7a4      	lw	a3,608 <BootApplication+0x4c0>
 37c:	3480      	sll	a0,8
 37e:	3480      	sll	a0,8
 380:	36c0      	sll	a2,8
 382:	ee4d      	or	a2,v0
 384:	6744      	move	v0,a0
 386:	a784      	lbu	a0,4(a3)
 388:	eacd      	or	v0,a2
 38a:	f600 3480 	sll	a0,24
 38e:	ec4d      	or	a0,v0
 390:	3282      	srl	v0,a0,8
 392:	c581      	sb	a0,1(a1)
 394:	c542      	sb	v0,2(a1)
 396:	3242      	srl	v0,8
 398:	c543      	sb	v0,3(a1)
 39a:	f600 3282 	srl	v0,a0,24
 39e:	c544      	sb	v0,4(a1)
 3a0:	a745      	lbu	v0,5(a3)
 3a2:	c545      	sb	v0,5(a1)
 3a4:	324a      	srl	v0,2
 3a6:	f75f 2204 	beqz	v0,232 <BootApplication+0xea>
 3aa:	b6a0      	lw	a2,628 <BootApplication+0x4e0>
 3ac:	67e4      	move	a3,a0
 3ae:	efcd      	or	a3,a2
 3b0:	7201      	cmpi	v0,1
 3b2:	9f00      	lw	s0,0(a3)
 3b4:	6f10      	li	a3,16
 3b6:	e75f      	subu	a3,v0
 3b8:	3302      	srl	v1,s0,8
 3ba:	37e8      	sll	a3,2
 3bc:	e5fd      	addu	a3,a1,a3
 3be:	c700      	sb	s0,0(a3)
 3c0:	c761      	sb	v1,1(a3)
 3c2:	f600 3002 	srl	s0,24
 3c6:	3362      	srl	v1,8
 3c8:	c703      	sb	s0,3(a3)
 3ca:	c762      	sb	v1,2(a3)
 3cc:	f73f 6011 	bteqz	232 <BootApplication+0xea>
 3d0:	44e4      	addiu	a3,a0,4
 3d2:	efcd      	or	a3,a2
 3d4:	7202      	cmpi	v0,2
 3d6:	9f00      	lw	s0,0(a3)
 3d8:	6f11      	li	a3,17
 3da:	e75f      	subu	a3,v0
 3dc:	3302      	srl	v1,s0,8
 3de:	37e8      	sll	a3,2
 3e0:	e5fd      	addu	a3,a1,a3
 3e2:	c700      	sb	s0,0(a3)
 3e4:	c761      	sb	v1,1(a3)
 3e6:	f600 3002 	srl	s0,24
 3ea:	3362      	srl	v1,8
 3ec:	c703      	sb	s0,3(a3)
 3ee:	c762      	sb	v1,2(a3)
 3f0:	f71f 601f 	bteqz	232 <BootApplication+0xea>
 3f4:	44e7      	addiu	a3,a0,7
 3f6:	4f01      	addiu	a3,1
 3f8:	efcd      	or	a3,a2
 3fa:	7203      	cmpi	v0,3
 3fc:	9f00      	lw	s0,0(a3)
 3fe:	6f12      	li	a3,18
 400:	e75f      	subu	a3,v0
 402:	3302      	srl	v1,s0,8
 404:	37e8      	sll	a3,2
 406:	e5fd      	addu	a3,a1,a3
 408:	c700      	sb	s0,0(a3)
 40a:	c761      	sb	v1,1(a3)
 40c:	f600 3002 	srl	s0,24
 410:	3362      	srl	v1,8
 412:	c703      	sb	s0,3(a3)
 414:	c762      	sb	v1,2(a3)
 416:	f71f 600c 	bteqz	232 <BootApplication+0xea>
 41a:	44e7      	addiu	a3,a0,7
 41c:	4f05      	addiu	a3,5
 41e:	efcd      	or	a3,a2
 420:	7204      	cmpi	v0,4
 422:	9f00      	lw	s0,0(a3)
 424:	6f13      	li	a3,19
 426:	e75f      	subu	a3,v0
 428:	3302      	srl	v1,s0,8
 42a:	37e8      	sll	a3,2
 42c:	e5fd      	addu	a3,a1,a3
 42e:	c700      	sb	s0,0(a3)
 430:	c761      	sb	v1,1(a3)
 432:	f600 3002 	srl	s0,24
 436:	3362      	srl	v1,8
 438:	c703      	sb	s0,3(a3)
 43a:	c762      	sb	v1,2(a3)
 43c:	f6ff 6019 	bteqz	232 <BootApplication+0xea>
 440:	44e7      	addiu	a3,a0,7
 442:	4f09      	addiu	a3,9
 444:	efcd      	or	a3,a2
 446:	7205      	cmpi	v0,5
 448:	9f00      	lw	s0,0(a3)
 44a:	6f14      	li	a3,20
 44c:	e75f      	subu	a3,v0
 44e:	3302      	srl	v1,s0,8
 450:	37e8      	sll	a3,2
 452:	e5fd      	addu	a3,a1,a3
 454:	c700      	sb	s0,0(a3)
 456:	c761      	sb	v1,1(a3)
 458:	f600 3002 	srl	s0,24
 45c:	3362      	srl	v1,8
 45e:	c703      	sb	s0,3(a3)
 460:	c762      	sb	v1,2(a3)
 462:	f6ff 6006 	bteqz	232 <BootApplication+0xea>
 466:	44e7      	addiu	a3,a0,7
 468:	4f0d      	addiu	a3,13
 46a:	efcd      	or	a3,a2
 46c:	7206      	cmpi	v0,6
 46e:	9f00      	lw	s0,0(a3)
 470:	6f15      	li	a3,21
 472:	e75f      	subu	a3,v0
 474:	3302      	srl	v1,s0,8
 476:	37e8      	sll	a3,2
 478:	e5fd      	addu	a3,a1,a3
 47a:	c700      	sb	s0,0(a3)
 47c:	c761      	sb	v1,1(a3)
 47e:	f600 3002 	srl	s0,24
 482:	3362      	srl	v1,8
 484:	c703      	sb	s0,3(a3)
 486:	c762      	sb	v1,2(a3)
 488:	f6df 6013 	bteqz	232 <BootApplication+0xea>
 48c:	44e7      	addiu	a3,a0,7
 48e:	4f11      	addiu	a3,17
 490:	efcd      	or	a3,a2
 492:	7207      	cmpi	v0,7
 494:	9f00      	lw	s0,0(a3)
 496:	6f16      	li	a3,22
 498:	e75f      	subu	a3,v0
 49a:	3302      	srl	v1,s0,8
 49c:	37e8      	sll	a3,2
 49e:	e5fd      	addu	a3,a1,a3
 4a0:	c700      	sb	s0,0(a3)
 4a2:	c761      	sb	v1,1(a3)
 4a4:	f600 3002 	srl	s0,24
 4a8:	3362      	srl	v1,8
 4aa:	c703      	sb	s0,3(a3)
 4ac:	c762      	sb	v1,2(a3)
 4ae:	f6df 6000 	bteqz	232 <BootApplication+0xea>
 4b2:	44e7      	addiu	a3,a0,7
 4b4:	4f15      	addiu	a3,21
 4b6:	efcd      	or	a3,a2
 4b8:	7208      	cmpi	v0,8
 4ba:	9f00      	lw	s0,0(a3)
 4bc:	6f17      	li	a3,23
 4be:	e75f      	subu	a3,v0
 4c0:	3302      	srl	v1,s0,8
 4c2:	37e8      	sll	a3,2
 4c4:	e5fd      	addu	a3,a1,a3
 4c6:	c700      	sb	s0,0(a3)
 4c8:	c761      	sb	v1,1(a3)
 4ca:	f600 3002 	srl	s0,24
 4ce:	3362      	srl	v1,8
 4d0:	c703      	sb	s0,3(a3)
 4d2:	c762      	sb	v1,2(a3)
 4d4:	f6bf 600d 	bteqz	232 <BootApplication+0xea>
 4d8:	44e7      	addiu	a3,a0,7
 4da:	4f19      	addiu	a3,25
 4dc:	efcd      	or	a3,a2
 4de:	7209      	cmpi	v0,9
 4e0:	9f00      	lw	s0,0(a3)
 4e2:	6f18      	li	a3,24
 4e4:	e75f      	subu	a3,v0
 4e6:	3302      	srl	v1,s0,8
 4e8:	37e8      	sll	a3,2
 4ea:	e5fd      	addu	a3,a1,a3
 4ec:	c700      	sb	s0,0(a3)
 4ee:	c761      	sb	v1,1(a3)
 4f0:	f600 3002 	srl	s0,24
 4f4:	3362      	srl	v1,8
 4f6:	c703      	sb	s0,3(a3)
 4f8:	c762      	sb	v1,2(a3)
 4fa:	f69f 601a 	bteqz	232 <BootApplication+0xea>
 4fe:	44e7      	addiu	a3,a0,7
 500:	4f1d      	addiu	a3,29
 502:	efcd      	or	a3,a2
 504:	6b19      	li	v1,25
 506:	9f00      	lw	s0,0(a3)
 508:	e34f      	subu	v1,v0
 50a:	3368      	sll	v1,2
 50c:	e575      	addu	a1,v1
 50e:	3302      	srl	v1,s0,8
 510:	c500      	sb	s0,0(a1)
 512:	c561      	sb	v1,1(a1)
 514:	f600 3002 	srl	s0,24
 518:	3362      	srl	v1,8
 51a:	720a      	cmpi	v0,10
 51c:	b73d      	lw	a3,610 <BootApplication+0x4c8>
 51e:	c562      	sb	v1,2(a1)
 520:	c503      	sb	s0,3(a1)
 522:	f69f 6006 	bteqz	232 <BootApplication+0xea>
 526:	44a7      	addiu	a1,a0,7
 528:	4d21      	addiu	a1,33
 52a:	edcd      	or	a1,a2
 52c:	720b      	cmpi	v0,11
 52e:	9d00      	lw	s0,0(a1)
 530:	6d1a      	li	a1,26
 532:	e557      	subu	a1,v0
 534:	3302      	srl	v1,s0,8
 536:	35a8      	sll	a1,2
 538:	e7b5      	addu	a1,a3,a1
 53a:	c500      	sb	s0,0(a1)
 53c:	c561      	sb	v1,1(a1)
 53e:	f600 3002 	srl	s0,24
 542:	3362      	srl	v1,8
 544:	c503      	sb	s0,3(a1)
 546:	c562      	sb	v1,2(a1)
 548:	f67f 6013 	bteqz	232 <BootApplication+0xea>
 54c:	44a7      	addiu	a1,a0,7
 54e:	4d25      	addiu	a1,37
 550:	edcd      	or	a1,a2
 552:	720c      	cmpi	v0,12
 554:	9d00      	lw	s0,0(a1)
 556:	6d1b      	li	a1,27
 558:	e557      	subu	a1,v0
 55a:	3302      	srl	v1,s0,8
 55c:	35a8      	sll	a1,2
 55e:	e7b5      	addu	a1,a3,a1
 560:	c500      	sb	s0,0(a1)
 562:	c561      	sb	v1,1(a1)
 564:	f600 3002 	srl	s0,24
 568:	3362      	srl	v1,8
 56a:	c503      	sb	s0,3(a1)
 56c:	c562      	sb	v1,2(a1)
 56e:	f67f 6000 	bteqz	232 <BootApplication+0xea>
 572:	44a7      	addiu	a1,a0,7
 574:	4d29      	addiu	a1,41
 576:	edcd      	or	a1,a2
 578:	720d      	cmpi	v0,13
 57a:	9d00      	lw	s0,0(a1)
 57c:	6d1c      	li	a1,28
 57e:	e557      	subu	a1,v0
 580:	3302      	srl	v1,s0,8
 582:	35a8      	sll	a1,2
 584:	e7b5      	addu	a1,a3,a1
 586:	c500      	sb	s0,0(a1)
 588:	c561      	sb	v1,1(a1)
 58a:	f600 3002 	srl	s0,24
 58e:	3362      	srl	v1,8
 590:	c503      	sb	s0,3(a1)
 592:	c562      	sb	v1,2(a1)
 594:	f65f 600d 	bteqz	232 <BootApplication+0xea>
 598:	4c34      	addiu	a0,52
 59a:	eccd      	or	a0,a2
 59c:	6d1d      	li	a1,29
 59e:	9c80      	lw	a0,0(a0)
 5a0:	e54b      	subu	v0,a1,v0
 5a2:	3248      	sll	v0,2
 5a4:	e75d      	addu	a3,v0
 5a6:	3282      	srl	v0,a0,8
 5a8:	c780      	sb	a0,0(a3)
 5aa:	c741      	sb	v0,1(a3)
 5ac:	f600 3482 	srl	a0,24
 5b0:	3242      	srl	v0,8
 5b2:	c783      	sb	a0,3(a3)
 5b4:	c742      	sb	v0,2(a3)
 5b6:	163d      	b	232 <BootApplication+0xea>
 5b8:	b01b      	lw	s0,624 <BootApplication+0x4dc>
 5ba:	1002      	b	5c0 <BootApplication+0x478>
 5bc:	f000 9980 	lw	a0,0(s1)
 5c0:	3248      	sll	v0,2
 5c2:	98a0      	lw	a1,0(s0)
 5c4:	1800 0000 	jal	0 <BlinkUSBStatus>
 5c8:	e453      	subu	a0,v0
 5ca:	f000 a140 	lbu	v0,0(s1)
 5ce:	4804      	addiu	s0,4
 5d0:	4aff      	addiu	v0,-1
 5d2:	ea11      	zeb	v0
 5d4:	f000 c140 	sb	v0,0(s1)
 5d8:	2af1      	bnez	v0,5bc <BootApplication+0x474>
 5da:	b20c      	lw	v0,608 <BootApplication+0x4c0>
 5dc:	a2a5      	lbu	a1,5(v0)
 5de:	6500      	nop
 5e0:	36aa      	srl	a2,a1,2
 5e2:	16a9      	b	336 <BootApplication+0x1ee>
 5e4:	b409      	lw	a0,608 <BootApplication+0x4c0>
 5e6:	b509      	lw	a1,608 <BootApplication+0x4c0>
 5e8:	a441      	lbu	v0,1(a0)
 5ea:	a482      	lbu	a0,2(a0)
 5ec:	3480      	sll	a0,8
 5ee:	ea8d      	or	v0,a0
 5f0:	a583      	lbu	a0,3(a1)
 5f2:	3480      	sll	a0,8
 5f4:	3480      	sll	a0,8
 5f6:	ea8d      	or	v0,a0
 5f8:	a584      	lbu	a0,4(a1)
 5fa:	f600 3480 	sll	a0,24
 5fe:	ec4d      	or	a0,v0
 600:	f000 d980 	sw	a0,0(s1)
 604:	1671      	b	2e8 <BootApplication+0x1a0>
 606:	6500      	nop
	...
 618:	b800      	lwu	s0,0(s0)
 61a:	e2ff      	subu	a3,v0,a3
 61c:	4800      	addiu	s0,0
 61e:	1d00 0000 	jalx	200000 <led_count.7426+0x200000>
 622:	0000      	addiu	s0,sp,0
 624:	0000      	addiu	s0,sp,0
 626:	0000      	addiu	s0,sp,0
 628:	0000      	addiu	s0,sp,0
 62a:	8000      	lb	s0,0(s0)

Disassembly of section .text.startup.main:

00000774 <main>:
 774:	6b01      	li	v1,1
 776:	b23e      	lw	v0,86c <main+0xf8>
 778:	64f4      	save	32,ra,s0-s1
 77a:	6d02      	li	a1,2
 77c:	da60      	sw	v1,0(v0)
 77e:	b23d      	lw	v0,870 <main+0xfc>
 780:	da60      	sw	v1,0(v0)
 782:	b23d      	lw	v0,874 <main+0x100>
 784:	daa0      	sw	a1,0(v0)
 786:	b63d      	lw	a2,878 <main+0x104>
 788:	b23d      	lw	v0,87c <main+0x108>
 78a:	673c      	move	s1,gp
 78c:	9a40      	lw	v0,0(v0)
 78e:	9ea0      	lw	a1,0(a2)
 790:	6a00      	li	v0,0
 792:	35a2      	srl	a1,8
 794:	35aa      	srl	a1,2
 796:	f000 c160 	sb	v1,0(s1)
 79a:	f000 c1a0 	sb	a1,0(s1)
 79e:	9ea0      	lw	a1,0(a2)
 7a0:	b638      	lw	a2,880 <main+0x10c>
 7a2:	4bfe      	addiu	v1,-2
 7a4:	e5d5      	addu	a1,a2
 7a6:	f000 d940 	sw	v0,0(s1)
 7aa:	f000 d940 	sw	v0,0(s1)
 7ae:	f000 c140 	sb	v0,0(s1)
 7b2:	f000 c140 	sb	v0,0(s1)
 7b6:	f000 d9a0 	sw	a1,0(s1)
 7ba:	f000 d960 	sw	v1,0(s1)
 7be:	1800 0000 	jal	0 <BlinkUSBStatus>
 7c2:	6500      	nop
 7c4:	1800 0000 	jal	0 <BlinkUSBStatus>
 7c8:	6500      	nop
 7ca:	f000 a940 	lhu	v0,0(s1)
 7ce:	b02e      	lw	s0,884 <main+0x110>
 7d0:	2a28      	bnez	v0,822 <main+0xae>
 7d2:	f52e 6a0f 	li	v0,29999
 7d6:	f000 c940 	sh	v0,0(s1)
 7da:	9840      	lw	v0,0(s0)
 7dc:	3346      	srl	v1,v0,1
 7de:	6a01      	li	v0,1
 7e0:	ea6c      	and	v0,v1
 7e2:	2a2c      	bnez	v0,83c <main+0xc8>
 7e4:	f52e 6a0f 	li	v0,29999
 7e8:	f000 9960 	lw	v1,0(s1)
 7ec:	232f      	beqz	v1,84c <main+0xd8>
 7ee:	7301      	cmpi	v1,1
 7f0:	6033      	bteqz	858 <main+0xe4>
 7f2:	7302      	cmpi	v1,2
 7f4:	6030      	bteqz	856 <main+0xe2>
 7f6:	7304      	cmpi	v1,4
 7f8:	602e      	bteqz	856 <main+0xe2>
 7fa:	7310      	cmpi	v1,16
 7fc:	6031      	bteqz	860 <main+0xec>
 7fe:	7320      	cmpi	v1,32
 800:	6020      	bteqz	842 <main+0xce>
 802:	5b20      	sltiu	v1,32
 804:	6126      	btnez	852 <main+0xde>
 806:	9840      	lw	v0,0(s0)
 808:	3346      	srl	v1,v0,1
 80a:	6a01      	li	v0,1
 80c:	ea6c      	and	v0,v1
 80e:	2a21      	bnez	v0,852 <main+0xde>
 810:	1800 0000 	jal	0 <BlinkUSBStatus>
 814:	6500      	nop
 816:	1800 0000 	jal	0 <BlinkUSBStatus>
 81a:	6500      	nop
 81c:	f000 a940 	lhu	v0,0(s1)
 820:	22d8      	beqz	v0,7d2 <main+0x5e>
 822:	9860      	lw	v1,0(s0)
 824:	4aff      	addiu	v0,-1
 826:	3566      	srl	a1,v1,1
 828:	ea31      	zeh	v0
 82a:	6b01      	li	v1,1
 82c:	ebac      	and	v1,a1
 82e:	f000 c940 	sh	v0,0(s1)
 832:	23da      	beqz	v1,7e8 <main+0x74>
 834:	2a03      	bnez	v0,83c <main+0xc8>
 836:	6b01      	li	v1,1
 838:	b214      	lw	v0,888 <main+0x114>
 83a:	da60      	sw	v1,0(v0)
 83c:	f000 9960 	lw	v1,0(s1)
 840:	17e0      	b	802 <main+0x8e>
 842:	2ae1      	bnez	v0,806 <main+0x92>
 844:	6b01      	li	v1,1
 846:	b211      	lw	v0,888 <main+0x114>
 848:	da60      	sw	v1,0(v0)
 84a:	17dd      	b	806 <main+0x92>
 84c:	6a01      	li	v0,1
 84e:	b309      	lw	v1,870 <main+0xfc>
 850:	db40      	sw	v0,0(v1)
 852:	6474      	restore	32,ra,s0-s1
 854:	e8a0      	jrc	ra
 856:	6b01      	li	v1,1
 858:	b20d      	lw	v0,88c <main+0x118>
 85a:	da60      	sw	v1,0(v0)
 85c:	6474      	restore	32,ra,s0-s1
 85e:	e8a0      	jrc	ra
 860:	2af8      	bnez	v0,852 <main+0xde>
 862:	6b01      	li	v1,1
 864:	b209      	lw	v0,888 <main+0x114>
 866:	da60      	sw	v1,0(v0)
 868:	17f4      	b	852 <main+0xde>
 86a:	6500      	nop
	...
 880:	0000      	addiu	s0,sp,0
 882:	1d00 0000 	jalx	200000 <led_count.7426+0x200000>
 886:	0000      	addiu	s0,sp,0
	...

Disassembly of section .text.WriteFlashSubBlock:

00001004 <WriteFlashSubBlock>:
    1004:	64f4      	save	32,ra,s0-s1
    1006:	671c      	move	s0,gp
    1008:	b10b      	lw	s1,1034 <WriteFlashSubBlock+0x30>
    100a:	f000 a080 	lbu	a0,0(s0)
    100e:	240f      	beqz	a0,102e <WriteFlashSubBlock+0x2a>
    1010:	f000 9840 	lw	v0,0(s0)
    1014:	99a0      	lw	a1,0(s1)
    1016:	3488      	sll	a0,2
    1018:	1800 0000 	jal	0 <BlinkUSBStatus>
    101c:	e293      	subu	a0,v0,a0
    101e:	f000 a080 	lbu	a0,0(s0)
    1022:	4904      	addiu	s1,4
    1024:	4cff      	addiu	a0,-1
    1026:	ec11      	zeb	a0
    1028:	f000 c080 	sb	a0,0(s0)
    102c:	2cf1      	bnez	a0,1010 <WriteFlashSubBlock+0xc>
    102e:	6500      	nop
    1030:	6474      	restore	32,ra,s0-s1
    1032:	e8a0      	jrc	ra
    1034:	0000      	addiu	s0,sp,0
	...

Disassembly of section .text.USBCBInitEP:

0000203c <USBCBInitEP>:
    203c:	64c3      	save	24,ra
    203e:	6c01      	li	a0,1
    2040:	1800 0000 	jal	0 <BlinkUSBStatus>
    2044:	6d1d      	li	a1,29
    2046:	6c01      	li	a0,1
    2048:	6d00      	li	a1,0
    204a:	b605      	lw	a2,205c <USBCBInitEP+0x20>
    204c:	1800 0000 	jal	0 <BlinkUSBStatus>
    2050:	6f40      	li	a3,64
    2052:	677c      	move	v1,gp
    2054:	6443      	restore	24,ra
    2056:	f000 db40 	sw	v0,0(v1)
    205a:	e8a0      	jrc	ra
    205c:	0000      	addiu	s0,sp,0
	...

Disassembly of section .text.USER_USB_CALLBACK_EVENT_HANDLER:

0000409c <USER_USB_CALLBACK_EVENT_HANDLER>:
    409c:	7403      	cmpi	a0,3
    409e:	64c3      	save	24,ra
    40a0:	601a      	bteqz	40d6 <USER_USB_CALLBACK_EVENT_HANDLER+0x3a>
    40a2:	7472      	cmpi	a0,114
    40a4:	6014      	bteqz	40ce <USER_USB_CALLBACK_EVENT_HANDLER+0x32>
    40a6:	7401      	cmpi	a0,1
    40a8:	6003      	bteqz	40b0 <USER_USB_CALLBACK_EVENT_HANDLER+0x14>
    40aa:	6443      	restore	24,ra
    40ac:	e820      	jr	ra
    40ae:	6a01      	li	v0,1
    40b0:	1800 0000 	jal	0 <BlinkUSBStatus>
    40b4:	6d1d      	li	a1,29
    40b6:	6c01      	li	a0,1
    40b8:	6d00      	li	a1,0
    40ba:	b60b      	lw	a2,40e4 <USER_USB_CALLBACK_EVENT_HANDLER+0x48>
    40bc:	1800 0000 	jal	0 <BlinkUSBStatus>
    40c0:	6f40      	li	a3,64
    40c2:	677c      	move	v1,gp
    40c4:	6443      	restore	24,ra
    40c6:	f000 db40 	sw	v0,0(v1)
    40ca:	e820      	jr	ra
    40cc:	6a01      	li	v0,1
    40ce:	6500      	nop
    40d0:	6443      	restore	24,ra
    40d2:	e820      	jr	ra
    40d4:	6a01      	li	v0,1
    40d6:	1800 0000 	jal	0 <BlinkUSBStatus>
    40da:	6500      	nop
    40dc:	6443      	restore	24,ra
    40de:	e820      	jr	ra
    40e0:	6a01      	li	v0,1
    40e2:	6500      	nop
    40e4:	0000      	addiu	s0,sp,0
	...

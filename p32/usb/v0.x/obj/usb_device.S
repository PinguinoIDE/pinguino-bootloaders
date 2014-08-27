
obj/usb_device.o:     file format elf32-littlemips


Disassembly of section .text.USBCtrlTrfTxService:

00000000 <USBCtrlTrfTxService>:
   0:	64b1      	save	8,s0-s1
   2:	b634      	lw	a2,d0 <USBCtrlTrfTxService+0xd0>
   4:	67fc      	move	a3,gp
   6:	aea4      	lhu	a1,8(a2)
   8:	5d40      	sltiu	a1,64
   a:	6033      	bteqz	72 <USBCtrlTrfTxService+0x72>
   c:	f000 a760 	lbu	v1,0(a3)
  10:	6745      	move	v0,a1
  12:	ea11      	zeb	v0
  14:	2b54      	bnez	v1,be <USBCtrlTrfTxService+0xbe>
  16:	6b01      	li	v1,1
  18:	f000 c760 	sb	v1,0(a3)
  1c:	6b00      	li	v1,0
  1e:	f000 9f00 	lw	s0,0(a3)
  22:	ce64      	sh	v1,8(a2)
  24:	f3e0 6b1f 	li	v1,1023
  28:	ebac      	and	v1,a1
  2a:	6783      	move	a0,v1
  2c:	ec11      	zeb	a0
  2e:	a022      	lbu	s1,2(s0)
  30:	c082      	sb	a0,2(s0)
  32:	a023      	lbu	s1,3(s0)
  34:	3462      	srl	a0,v1,8
  36:	6b04      	li	v1,4
  38:	eb6b      	neg	v1
  3a:	eb2c      	and	v1,s1
  3c:	eb8d      	or	v1,a0
  3e:	c063      	sb	v1,3(s0)
  40:	b325      	lw	v1,d4 <USBCtrlTrfTxService+0xd4>
  42:	a684      	lbu	a0,4(a2)
  44:	f000 df60 	sw	v1,0(a3)
  48:	6b01      	li	v1,1
  4a:	eb8c      	and	v1,a0
  4c:	233e      	beqz	v1,ca <USBCtrlTrfTxService+0xca>
  4e:	250f      	beqz	a1,6e <USBCtrlTrfTxService+0x6e>
  50:	b421      	lw	a0,d4 <USBCtrlTrfTxService+0xd4>
  52:	1002      	b	58 <USBCtrlTrfTxService+0x58>
  54:	f000 9f80 	lw	a0,0(a3)
  58:	4461      	addiu	v1,a0,1
  5a:	4aff      	addiu	v0,-1
  5c:	f000 df60 	sw	v1,0(a3)
  60:	9e60      	lw	v1,0(a2)
  62:	ea11      	zeb	v0
  64:	43a1      	addiu	a1,v1,1
  66:	dea0      	sw	a1,0(a2)
  68:	a360      	lbu	v1,0(v1)
  6a:	c460      	sb	v1,0(a0)
  6c:	2af3      	bnez	v0,54 <USBCtrlTrfTxService+0x54>
  6e:	6431      	restore	8,s0-s1
  70:	e8a0      	jrc	ra
  72:	f000 9f40 	lw	v0,0(a3)
  76:	4dc0      	addiu	a1,-64
  78:	cea4      	sh	a1,8(a2)
  7a:	a262      	lbu	v1,2(v0)
  7c:	6b40      	li	v1,64
  7e:	c262      	sb	v1,2(v0)
  80:	a283      	lbu	a0,3(v0)
  82:	4bbc      	addiu	v1,-68
  84:	eb8c      	and	v1,a0
  86:	c263      	sb	v1,3(v0)
  88:	b313      	lw	v1,d4 <USBCtrlTrfTxService+0xd4>
  8a:	6a01      	li	v0,1
  8c:	f000 df60 	sw	v1,0(a3)
  90:	a664      	lbu	v1,4(a2)
  92:	ea6c      	and	v0,v1
  94:	2a12      	bnez	v0,ba <USBCtrlTrfTxService+0xba>
  96:	6a40      	li	v0,64
  98:	b40f      	lw	a0,d4 <USBCtrlTrfTxService+0xd4>
  9a:	1002      	b	a0 <USBCtrlTrfTxService+0xa0>
  9c:	f000 9f80 	lw	a0,0(a3)
  a0:	4461      	addiu	v1,a0,1
  a2:	4aff      	addiu	v0,-1
  a4:	f000 df60 	sw	v1,0(a3)
  a8:	9e60      	lw	v1,0(a2)
  aa:	ea11      	zeb	v0
  ac:	43a1      	addiu	a1,v1,1
  ae:	dea0      	sw	a1,0(a2)
  b0:	a360      	lbu	v1,0(v1)
  b2:	c460      	sb	v1,0(a0)
  b4:	2af3      	bnez	v0,9c <USBCtrlTrfTxService+0x9c>
  b6:	6431      	restore	8,s0-s1
  b8:	e8a0      	jrc	ra
  ba:	6a40      	li	v0,64
  bc:	17c9      	b	50 <USBCtrlTrfTxService+0x50>
  be:	7301      	cmpi	v1,1
  c0:	61ad      	btnez	1c <USBCtrlTrfTxService+0x1c>
  c2:	6b02      	li	v1,2
  c4:	f000 c760 	sb	v1,0(a3)
  c8:	17a9      	b	1c <USBCtrlTrfTxService+0x1c>
  ca:	2de6      	bnez	a1,98 <USBCtrlTrfTxService+0x98>
  cc:	17d0      	b	6e <USBCtrlTrfTxService+0x6e>
  ce:	6500      	nop
	...

Disassembly of section .text.USBDeviceInit:

000000d8 <USBDeviceInit>:
  d8:	64b1      	save	8,s0-s1
  da:	b356      	lw	v1,230 <USBDeviceInit+0x158>
  dc:	6a09      	li	v0,9
  de:	9ba0      	lw	a1,0(v1)
  e0:	ea4b      	neg	v0
  e2:	eaac      	and	v0,a1
  e4:	6e00      	li	a2,0
  e6:	db40      	sw	v0,0(v1)
  e8:	6aff      	li	v0,255
  ea:	b353      	lw	v1,234 <USBDeviceInit+0x15c>
  ec:	b753      	lw	a3,238 <USBDeviceInit+0x160>
  ee:	db40      	sw	v0,0(v1)
  f0:	df40      	sw	v0,0(a3)
  f2:	b253      	lw	v0,23c <USBDeviceInit+0x164>
  f4:	dac0      	sw	a2,0(v0)
  f6:	652a      	move	t1,v0
  f8:	b252      	lw	v0,240 <USBDeviceInit+0x168>
  fa:	dac0      	sw	a2,0(v0)
  fc:	b252      	lw	v0,244 <USBDeviceInit+0x16c>
  fe:	dac0      	sw	a2,0(v0)
 100:	6a9f      	li	v0,159
 102:	b352      	lw	v1,248 <USBDeviceInit+0x170>
 104:	db40      	sw	v0,0(v1)
 106:	b352      	lw	v1,24c <USBDeviceInit+0x174>
 108:	db40      	sw	v0,0(v1)
 10a:	b252      	lw	v0,250 <USBDeviceInit+0x178>
 10c:	6b0f      	li	v1,15
 10e:	9aa0      	lw	a1,0(v0)
 110:	b151      	lw	s1,254 <USBDeviceInit+0x17c>
 112:	ebac      	and	v1,a1
 114:	679c      	move	a0,gp
 116:	da60      	sw	v1,0(v0)
 118:	9a60      	lw	v1,0(v0)
 11a:	da60      	sw	v1,0(v0)
 11c:	b34f      	lw	v1,258 <USBDeviceInit+0x180>
 11e:	6a01      	li	v0,1
 120:	a3a0      	lbu	a1,0(v1)
 122:	eaad      	or	v0,a1
 124:	b54e      	lw	a1,25c <USBDeviceInit+0x184>
 126:	6705      	move	s0,a1
 128:	e82c      	and	s0,s1
 12a:	c340      	sb	v0,0(v1)
 12c:	f600 3302 	srl	v1,s0,24
 130:	b24c      	lw	v0,260 <USBDeviceInit+0x188>
 132:	3002      	srl	s0,8
 134:	da60      	sw	v1,0(v0)
 136:	3302      	srl	v1,s0,8
 138:	b24b      	lw	v0,264 <USBDeviceInit+0x18c>
 13a:	da60      	sw	v1,0(v0)
 13c:	b34b      	lw	v1,268 <USBDeviceInit+0x190>
 13e:	db00      	sw	s0,0(v1)
 140:	6a00      	li	v0,0
 142:	6b00      	li	v1,0
 144:	dd40      	sw	v0,0(a1)
 146:	dd61      	sw	v1,4(a1)
 148:	dd42      	sw	v0,8(a1)
 14a:	dd63      	sw	v1,12(a1)
 14c:	dd44      	sw	v0,16(a1)
 14e:	dd65      	sw	v1,20(a1)
 150:	dd46      	sw	v0,24(a1)
 152:	dd67      	sw	v1,28(a1)
 154:	dd48      	sw	v0,32(a1)
 156:	dd69      	sw	v1,36(a1)
 158:	dd4a      	sw	v0,40(a1)
 15a:	dd6b      	sw	v1,44(a1)
 15c:	dd4c      	sw	v0,48(a1)
 15e:	dd6d      	sw	v1,52(a1)
 160:	dd4e      	sw	v0,56(a1)
 162:	b243      	lw	v0,26c <USBDeviceInit+0x194>
 164:	dd6f      	sw	v1,60(a1)
 166:	a260      	lbu	v1,0(v0)
 168:	6802      	li	s0,2
 16a:	651b      	move	t8,v1
 16c:	6770      	move	v1,s0
 16e:	6718      	move	s0,t8
 170:	eb0d      	or	v1,s0
 172:	c260      	sb	v1,0(v0)
 174:	b33f      	lw	v1,270 <USBDeviceInit+0x198>
 176:	dbc0      	sw	a2,0(v1)
 178:	a2c0      	lbu	a2,0(v0)
 17a:	6b21      	li	v1,33
 17c:	eb6b      	neg	v1
 17e:	ebcc      	and	v1,a2
 180:	c260      	sb	v1,0(v0)
 182:	a2c0      	lbu	a2,0(v0)
 184:	6b03      	li	v1,3
 186:	eb6b      	neg	v1
 188:	ebcc      	and	v1,a2
 18a:	b63b      	lw	a2,274 <USBDeviceInit+0x19c>
 18c:	c260      	sb	v1,0(v0)
 18e:	9e40      	lw	v0,0(a2)
 190:	334e      	srl	v1,v0,3
 192:	6a01      	li	v0,1
 194:	ea6c      	and	v0,v1
 196:	220f      	beqz	v0,1b6 <USBDeviceInit+0xde>
 198:	6a08      	li	v0,8
 19a:	df40      	sw	v0,0(a3)
 19c:	9e40      	lw	v0,0(a2)
 19e:	334e      	srl	v1,v0,3
 1a0:	6a01      	li	v0,1
 1a2:	ea6c      	and	v0,v1
 1a4:	2af9      	bnez	v0,198 <USBDeviceInit+0xc0>
 1a6:	6b00      	li	v1,0
 1a8:	b234      	lw	v0,278 <USBDeviceInit+0x1a0>
 1aa:	c264      	sb	v1,4(v0)
 1ac:	b234      	lw	v0,27c <USBDeviceInit+0x1a4>
 1ae:	c264      	sb	v1,4(v0)
 1b0:	6b00      	li	v1,0
 1b2:	c265      	sb	v1,5(v0)
 1b4:	c266      	sb	v1,6(v0)
 1b6:	6a01      	li	v0,1
 1b8:	6764      	move	v1,a0
 1ba:	f000 dc40 	sw	v0,0(a0)
 1be:	f000 dc40 	sw	v0,0(a0)
 1c2:	6a00      	li	v0,0
 1c4:	f000 4b00 	addiu	v1,0
 1c8:	db41      	sw	v0,4(v1)
 1ca:	6764      	move	v1,a0
 1cc:	f000 4b00 	addiu	v1,0
 1d0:	db41      	sw	v0,4(v1)
 1d2:	6764      	move	v1,a0
 1d4:	f000 4b00 	addiu	v1,0
 1d8:	c341      	sb	v0,1(v1)
 1da:	6764      	move	v1,a0
 1dc:	f000 4b00 	addiu	v1,0
 1e0:	c341      	sb	v0,1(v1)
 1e2:	b328      	lw	v1,280 <USBDeviceInit+0x1a8>
 1e4:	67c9      	move	a2,t1
 1e6:	f000 dc60 	sw	v1,0(a0)
 1ea:	6b0d      	li	v1,13
 1ec:	f000 dc40 	sw	v0,0(a0)
 1f0:	f000 dc40 	sw	v0,0(a0)
 1f4:	de60      	sw	v1,0(a2)
 1f6:	6764      	move	v1,a0
 1f8:	f000 4b00 	addiu	v1,0
 1fc:	eb2c      	and	v1,s1
 1fe:	f000 dc40 	sw	v0,0(a0)
 202:	dd61      	sw	v1,4(a1)
 204:	adc1      	lhu	a2,2(a1)
 206:	f400 6b00 	li	v1,1024
 20a:	eb6b      	neg	v1
 20c:	ee6c      	and	a2,v1
 20e:	6b40      	li	v1,64
 210:	ebcd      	or	v1,a2
 212:	f000 dc40 	sw	v0,0(a0)
 216:	cd61      	sh	v1,2(a1)
 218:	6b84      	li	v1,132
 21a:	f000 c440 	sb	v0,0(a0)
 21e:	cd60      	sh	v1,0(a1)
 220:	f000 c440 	sb	v0,0(a0)
 224:	f000 c440 	sb	v0,0(a0)
 228:	f000 dc40 	sw	v0,0(a0)
 22c:	6431      	restore	8,s0-s1
 22e:	e8a0      	jrc	ra
	...
 254:	ffff      	daddiu	a3,sp,124
 256:	1fff 0000 	jalx	ffc0000 <$L129+0xffbf0bc>
 25a:	0000      	addiu	s0,sp,0
	...
 280:	0010      	addiu	s0,sp,64
	...

Disassembly of section .text.USBDeviceTasks:

0000035c <USBDeviceTasks>:
     35c:	64f9      	save	72,ra,s0-s1
     35e:	673c      	move	s1,gp
     360:	f000 9940 	lw	v0,0(s1)
     364:	f200 2a18 	bnez	v0,798 <$L75+0x284>
     368:	f3a2 b308 	lw	v1,1710 <$L129+0x7cc>
     36c:	db40      	sw	v0,0(v1)
     36e:	f3a2 b408 	lw	a0,1714 <$L129+0x7d0>
     372:	f3a2 b308 	lw	v1,1718 <$L129+0x7d4>
     376:	dc40      	sw	v0,0(a0)
     378:	db40      	sw	v0,0(v1)
     37a:	6a9f      	li	v0,159
     37c:	f3a2 b300 	lw	v1,171c <$L129+0x7d8>
     380:	db40      	sw	v0,0(v1)
     382:	dc40      	sw	v0,0(a0)
     384:	f382 b21c 	lw	v0,1720 <$L129+0x7dc>
     388:	6b0f      	li	v1,15
     38a:	9aa0      	lw	a1,0(v0)
     38c:	f382 b018 	lw	s0,1724 <$L129+0x7e0>
     390:	ebac      	and	v1,a1
     392:	da60      	sw	v1,0(v0)
     394:	9a60      	lw	v1,0(v0)
     396:	da60      	sw	v1,0(v0)
     398:	9860      	lw	v1,0(s0)
     39a:	6a01      	li	v0,1
     39c:	ea6c      	and	v0,v1
     39e:	2a08      	bnez	v0,3b0 <$L66>
     3a0:	a060      	lbu	v1,0(s0)
     3a2:	6a01      	li	v0,1
     3a4:	ea6d      	or	v0,v1
     3a6:	c040      	sb	v0,0(s0)
     3a8:	9860      	lw	v1,0(s0)
     3aa:	6a01      	li	v0,1
     3ac:	ea6c      	and	v0,v1
     3ae:	22f8      	beqz	v0,3a0 <USBDeviceTasks+0x44>

000003b0 <$L66>:
     3b0:	6bff      	li	v1,255
     3b2:	f362 b218 	lw	v0,1728 <$L129+0x7e4>
     3b6:	da60      	sw	v1,0(v0)
     3b8:	6a00      	li	v0,0
     3ba:	6b01      	li	v1,1
     3bc:	dc40      	sw	v0,0(a0)
     3be:	f362 b210 	lw	v0,172c <$L129+0x7e8>
     3c2:	a280      	lbu	a0,0(v0)
     3c4:	eb8d      	or	v1,a0
     3c6:	c260      	sb	v1,0(v0)
     3c8:	a280      	lbu	a0,0(v0)
     3ca:	6b10      	li	v1,16
     3cc:	eb8d      	or	v1,a0
     3ce:	c260      	sb	v1,0(v0)
     3d0:	6a02      	li	v0,2
     3d2:	6b01      	li	v1,1
     3d4:	f000 d940 	sw	v0,0(s1)
     3d8:	f342 b218 	lw	v0,1730 <$L129+0x7ec>
     3dc:	9a40      	lw	v0,0(v0)
     3de:	3252      	srl	v0,4
     3e0:	ea6c      	and	v0,v1
     3e2:	ea11      	zeb	v0
     3e4:	f1e0 2a04 	bnez	v0,7b0 <$L75+0x29c>
     3e8:	f342 b20c 	lw	v0,1734 <$L129+0x7f0>
     3ec:	6c01      	li	a0,1
     3ee:	9a40      	lw	v0,0(v0)
     3f0:	3246      	srl	v0,1
     3f2:	ea8c      	and	v0,a0
     3f4:	ea11      	zeb	v0
     3f6:	f200 2a02 	bnez	v0,7fe <$L75+0x2ea>
     3fa:	f342 b300 	lw	v1,1738 <$L129+0x7f4>
     3fe:	9b40      	lw	v0,0(v1)
     400:	ea8c      	and	v0,a0
     402:	ea11      	zeb	v0
     404:	2207      	beqz	v0,414 <$L66+0x64>
     406:	f322 b208 	lw	v0,172c <$L129+0x7e8>
     40a:	9a40      	lw	v0,0(v0)
     40c:	ec4c      	and	a0,v0
     40e:	ec11      	zeb	a0
     410:	f260 2c14 	bnez	a0,8fc <$L161+0x4e>
     414:	9b40      	lw	v0,0(v1)
     416:	6c01      	li	a0,1
     418:	3252      	srl	v0,4
     41a:	ea8c      	and	v0,a0
     41c:	ea11      	zeb	v0
     41e:	2208      	beqz	v0,430 <$L66+0x80>
     420:	f302 b20c 	lw	v0,172c <$L129+0x7e8>
     424:	9a40      	lw	v0,0(v0)
     426:	3252      	srl	v0,4
     428:	ec4c      	and	a0,v0
     42a:	ec11      	zeb	a0
     42c:	f260 2c19 	bnez	a0,922 <$L161+0x74>
     430:	9b40      	lw	v0,0(v1)
     432:	6c01      	li	a0,1
     434:	324a      	srl	v0,2
     436:	ea8c      	and	v0,a0
     438:	ea11      	zeb	v0
     43a:	226c      	beqz	v0,514 <$L75>
     43c:	f2e2 b210 	lw	v0,172c <$L129+0x7e8>
     440:	9a40      	lw	v0,0(v0)
     442:	324a      	srl	v0,2
     444:	ec4c      	and	a0,v0
     446:	ec11      	zeb	a0
     448:	f320 2c02 	bnez	a0,a90 <$L95+0xa2>
     44c:	6c04      	li	a0,4
     44e:	f2c2 b21c 	lw	v0,1728 <$L129+0x7e4>
     452:	da80      	sw	a0,0(v0)
     454:	f000 a140 	lbu	v0,0(s1)
     458:	f200 2a07 	bnez	v0,86a <$L142+0x1e>
     45c:	f000 9940 	lw	v0,0(s1)
     460:	2a59      	bnez	v0,514 <$L75>
     462:	6c01      	li	a0,1
     464:	f000 d980 	sw	a0,0(s1)
     468:	f000 9940 	lw	v0,0(s1)
     46c:	2a53      	bnez	v0,514 <$L75>
     46e:	f000 a140 	lbu	v0,0(s1)
     472:	f000 d980 	sw	a0,0(s1)
     476:	7202      	cmpi	v0,2
     478:	f5e0 601c 	bteqz	1074 <$L129+0x130>
     47c:	7201      	cmpi	v0,1
     47e:	614a      	btnez	514 <$L75>
     480:	f000 9940 	lw	v0,0(s1)
     484:	6e40      	li	a2,64
     486:	6804      	li	s0,4
     488:	a2a2      	lbu	a1,2(v0)
     48a:	c2c2      	sb	a2,2(v0)
     48c:	a2a3      	lbu	a1,3(v0)
     48e:	e80b      	neg	s0
     490:	ed0c      	and	a1,s0
     492:	652e      	move	t1,a2
     494:	c2a3      	sb	a1,3(v0)
     496:	67d1      	move	a2,s1
     498:	f2a2 b504 	lw	a1,173c <$L129+0x7f8>
     49c:	f000 4e00 	addiu	a2,0
     4a0:	edcc      	and	a1,a2
     4a2:	6fff      	li	a3,255
     4a4:	67c5      	move	a2,a1
     4a6:	eeec      	and	a2,a3
     4a8:	654e      	move	t2,a2
     4aa:	a2c4      	lbu	a2,4(v0)
     4ac:	67ca      	move	a2,t2
     4ae:	c2c4      	sb	a2,4(v0)
     4b0:	36a2      	srl	a2,a1,8
     4b2:	658e      	move	t4,a2
     4b4:	eeec      	and	a2,a3
     4b6:	650e      	move	t0,a2
     4b8:	a2c5      	lbu	a2,5(v0)
     4ba:	67c8      	move	a2,t0
     4bc:	f600 35a2 	srl	a1,24
     4c0:	c2c5      	sb	a2,5(v0)
     4c2:	67cc      	move	a2,t4
     4c4:	36c2      	srl	a2,8
     4c6:	eeec      	and	a2,a3
     4c8:	a2e6      	lbu	a3,6(v0)
     4ca:	c2c6      	sb	a2,6(v0)
     4cc:	a2e7      	lbu	a3,7(v0)
     4ce:	c2a7      	sb	a1,7(v0)
     4d0:	a2e0      	lbu	a3,0(v0)
     4d2:	6f7c      	li	a3,124
     4d4:	efeb      	neg	a3
     4d6:	c2e0      	sb	a3,0(v0)
     4d8:	a2e1      	lbu	a3,1(v0)
     4da:	67f8      	move	a3,t8
     4dc:	c2e1      	sb	a3,1(v0)
     4de:	f000 9940 	lw	v0,0(s1)
     4e2:	f000 d980 	sw	a0,0(s1)
     4e6:	67ea      	move	a3,t2
     4e8:	a282      	lbu	a0,2(v0)
     4ea:	6789      	move	a0,t1
     4ec:	c282      	sb	a0,2(v0)
     4ee:	a283      	lbu	a0,3(v0)
     4f0:	ec0c      	and	a0,s0
     4f2:	6708      	move	s0,t0
     4f4:	c283      	sb	a0,3(v0)
     4f6:	a284      	lbu	a0,4(v0)
     4f8:	c2e4      	sb	a3,4(v0)
     4fa:	a285      	lbu	a0,5(v0)
     4fc:	c205      	sb	s0,5(v0)
     4fe:	a286      	lbu	a0,6(v0)
     500:	c2c6      	sb	a2,6(v0)
     502:	a287      	lbu	a0,7(v0)
     504:	c2a7      	sb	a1,7(v0)
     506:	a280      	lbu	a0,0(v0)
     508:	6c80      	li	a0,128
     50a:	ec8b      	neg	a0
     50c:	c280      	sb	a0,0(v0)
     50e:	a281      	lbu	a0,1(v0)
     510:	6798      	move	a0,t8
     512:	c281      	sb	a0,1(v0)

00000514 <$L75>:
     514:	9b40      	lw	v0,0(v1)
     516:	6c01      	li	a0,1
     518:	325e      	srl	v0,7
     51a:	ea8c      	and	v0,a0
     51c:	ea11      	zeb	v0
     51e:	2208      	beqz	v0,530 <$L75+0x1c>
     520:	f202 b20c 	lw	v0,172c <$L129+0x7e8>
     524:	9a40      	lw	v0,0(v0)
     526:	325e      	srl	v0,7
     528:	ea8c      	and	v0,a0
     52a:	ea11      	zeb	v0
     52c:	f200 2a17 	bnez	v0,95e <$L161+0xb0>
     530:	9b40      	lw	v0,0(v1)
     532:	6c01      	li	a0,1
     534:	3246      	srl	v0,1
     536:	ea8c      	and	v0,a0
     538:	ea11      	zeb	v0
     53a:	2208      	beqz	v0,54c <$L75+0x38>
     53c:	f1e2 b210 	lw	v0,172c <$L129+0x7e8>
     540:	9a40      	lw	v0,0(v0)
     542:	3246      	srl	v0,1
     544:	ec4c      	and	a0,v0
     546:	ec11      	zeb	a0
     548:	f200 2c1d 	bnez	a0,986 <$L161+0xd8>
     54c:	f000 9900 	lw	s0,0(s1)
     550:	5804      	sltiu	s0,4
     552:	f140 6114 	btnez	7fe <$L75+0x2ea>
     556:	f1c2 b218 	lw	v0,172c <$L129+0x7e8>
     55a:	9a40      	lw	v0,0(v0)
     55c:	344e      	srl	a0,v0,3
     55e:	6a01      	li	v0,1
     560:	ea8c      	and	v0,a0
     562:	f140 220c 	beqz	v0,7fe <$L75+0x2ea>
     566:	6f04      	li	a3,4
     568:	d706      	sw	a3,24(sp)
     56a:	9b40      	lw	v0,0(v1)
     56c:	6e01      	li	a2,1
     56e:	324e      	srl	v0,3
     570:	eacc      	and	v0,a2
     572:	ea11      	zeb	v0
     574:	f140 2203 	beqz	v0,7fe <$L75+0x2ea>
     578:	f1c2 b208 	lw	v0,1740 <$L129+0x7fc>
     57c:	6808      	li	s0,8
     57e:	9a40      	lw	v0,0(v0)
     580:	f1a2 b408 	lw	a0,1728 <$L129+0x7e4>
     584:	652a      	move	t1,v0
     586:	ea11      	zeb	v0
     588:	3752      	srl	a3,v0,4
     58a:	dc00      	sw	s0,0(a0)
     58c:	6c08      	li	a0,8
     58e:	ec4c      	and	a0,v0
     590:	f000 c140 	sb	v0,0(s1)
     594:	f000 c1e0 	sb	a3,0(s1)
     598:	f120 2c1a 	bnez	a0,810 <$L75+0x2fc>
     59c:	6791      	move	a0,s1
     59e:	f000 4c00 	addiu	a0,0
     5a2:	e791      	addu	a0,a3,a0
     5a4:	6801      	li	s0,1
     5a6:	a4a0      	lbu	a1,0(a0)
     5a8:	651c      	move	t8,a0
     5aa:	650d      	move	t0,a1
     5ac:	67a8      	move	a1,t0
     5ae:	edcc      	and	a1,a2
     5b0:	ed0e      	xor	a1,s0
     5b2:	6785      	move	a0,a1
     5b4:	6d02      	li	a1,2
     5b6:	eccc      	and	a0,a2
     5b8:	edab      	neg	a1
     5ba:	67c8      	move	a2,t0
     5bc:	edcc      	and	a1,a2
     5be:	ed8d      	or	a1,a0
     5c0:	6718      	move	s0,t8
     5c2:	c0a0      	sb	a1,0(s0)
     5c4:	f120 2f19 	bnez	a3,83a <$L75+0x326>
     5c8:	6c2d      	li	a0,45
     5ca:	6dff      	li	a1,255
     5cc:	f000 c180 	sb	a0,0(s1)
     5d0:	6709      	move	s0,t1
     5d2:	6c05      	li	a0,5
     5d4:	e8ac      	and	s0,a1
     5d6:	ec8b      	neg	a0
     5d8:	e88c      	and	s0,a0
     5da:	f140 2814 	bnez	s0,886 <$L142+0x3a>
     5de:	324a      	srl	v0,2
     5e0:	f162 b404 	lw	a0,1744 <$L129+0x800>
     5e4:	324c      	sll	v0,3
     5e6:	e449      	addu	v0,a0,v0
     5e8:	7208      	cmpi	v0,8
     5ea:	67d8      	move	a2,t8
     5ec:	f000 d940 	sw	v0,0(s1)
     5f0:	f000 d940 	sw	v0,0(s1)
     5f4:	f000 c1c0 	sb	a2,0(s1)
     5f8:	654c      	move	t2,a0
     5fa:	9a80      	lw	a0,0(v0)
     5fc:	308a      	srl	s0,a0,2
     5fe:	6c0f      	li	a0,15
     600:	ec0c      	and	a0,s0
     602:	740d      	cmpi	a0,13
     604:	f240 610c 	btnez	aa0 <$L95+0xb2>
     608:	9a01      	lw	s0,4(v0)
     60a:	f142 b400 	lw	a0,1748 <$L129+0x804>
     60e:	e88d      	or	s0,a0
     610:	a000      	lbu	s0,0(s0)
     612:	f000 c100 	sb	s0,0(s1)
     616:	9a01      	lw	s0,4(v0)
     618:	4801      	addiu	s0,1
     61a:	da01      	sw	s0,4(v0)
     61c:	9a01      	lw	s0,4(v0)
     61e:	e88d      	or	s0,a0
     620:	a000      	lbu	s0,0(s0)
     622:	6518      	move	t8,s0
     624:	6711      	move	s0,s1
     626:	f000 4800 	addiu	s0,0
     62a:	67d8      	move	a2,t8
     62c:	d007      	sw	s0,28(sp)
     62e:	c0c1      	sb	a2,1(s0)
     630:	9a01      	lw	s0,4(v0)
     632:	9607      	lw	a2,28(sp)
     634:	4801      	addiu	s0,1
     636:	da01      	sw	s0,4(v0)
     638:	9a01      	lw	s0,4(v0)
     63a:	e88d      	or	s0,a0
     63c:	a000      	lbu	s0,0(s0)
     63e:	c602      	sb	s0,2(a2)
     640:	9a01      	lw	s0,4(v0)
     642:	4801      	addiu	s0,1
     644:	da01      	sw	s0,4(v0)
     646:	9a01      	lw	s0,4(v0)
     648:	e88d      	or	s0,a0
     64a:	a000      	lbu	s0,0(s0)
     64c:	c603      	sb	s0,3(a2)
     64e:	9a01      	lw	s0,4(v0)
     650:	4801      	addiu	s0,1
     652:	da01      	sw	s0,4(v0)
     654:	9a01      	lw	s0,4(v0)
     656:	e88d      	or	s0,a0
     658:	a000      	lbu	s0,0(s0)
     65a:	c604      	sb	s0,4(a2)
     65c:	9a01      	lw	s0,4(v0)
     65e:	4801      	addiu	s0,1
     660:	da01      	sw	s0,4(v0)
     662:	9a01      	lw	s0,4(v0)
     664:	e88d      	or	s0,a0
     666:	a000      	lbu	s0,0(s0)
     668:	c605      	sb	s0,5(a2)
     66a:	9a01      	lw	s0,4(v0)
     66c:	4801      	addiu	s0,1
     66e:	da01      	sw	s0,4(v0)
     670:	9a01      	lw	s0,4(v0)
     672:	e88d      	or	s0,a0
     674:	a000      	lbu	s0,0(s0)
     676:	c606      	sb	s0,6(a2)
     678:	9a01      	lw	s0,4(v0)
     67a:	4801      	addiu	s0,1
     67c:	da01      	sw	s0,4(v0)
     67e:	9a01      	lw	s0,4(v0)
     680:	ec0d      	or	a0,s0
     682:	f0a2 b01c 	lw	s0,173c <$L129+0x7f8>
     686:	a480      	lbu	a0,0(a0)
     688:	c687      	sb	a0,7(a2)
     68a:	9a81      	lw	a0,4(v0)
     68c:	ee0c      	and	a2,s0
     68e:	4c01      	addiu	a0,1
     690:	f000 c1e0 	sb	a3,0(s1)
     694:	da81      	sw	a0,4(v0)
     696:	dac1      	sw	a2,4(v0)
     698:	f000 9940 	lw	v0,0(s1)
     69c:	f000 d9e0 	sw	a3,0(s1)
     6a0:	f000 c1e0 	sb	a3,0(s1)
     6a4:	f000 d9e0 	sw	a3,0(s1)
     6a8:	651a      	move	t8,v0
     6aa:	f000 d9e0 	sw	a3,0(s1)
     6ae:	a240      	lbu	v0,0(v0)
     6b0:	d608      	sw	a2,32(sp)
     6b2:	67d8      	move	a2,t8
     6b4:	6782      	move	a0,v0
     6b6:	a641      	lbu	v0,1(a2)
     6b8:	ec11      	zeb	a0
     6ba:	ea11      	zeb	v0
     6bc:	3240      	sll	v0,8
     6be:	6881      	li	s0,129
     6c0:	e80b      	neg	s0
     6c2:	ea8d      	or	v0,a0
     6c4:	ea0c      	and	v0,s0
     6c6:	6782      	move	a0,v0
     6c8:	f000 d9e0 	sw	a3,0(s1)
     6cc:	ecac      	and	a0,a1
     6ce:	a6c0      	lbu	a2,0(a2)
     6d0:	3242      	srl	v0,8
     6d2:	67d8      	move	a2,t8
     6d4:	c680      	sb	a0,0(a2)
     6d6:	a681      	lbu	a0,1(a2)
     6d8:	c641      	sb	v0,1(a2)
     6da:	f000 a140 	lbu	v0,0(s1)
     6de:	7208      	cmpi	v0,8
     6e0:	67d8      	move	a2,t8
     6e2:	f000 c1c0 	sb	a2,0(s1)
     6e6:	f000 9940 	lw	v0,0(s1)
     6ea:	651a      	move	t8,v0
     6ec:	67d8      	move	a2,t8
     6ee:	a240      	lbu	v0,0(v0)
     6f0:	a6c1      	lbu	a2,1(a2)
     6f2:	6782      	move	a0,v0
     6f4:	6746      	move	v0,a2
     6f6:	ea11      	zeb	v0
     6f8:	ec11      	zeb	a0
     6fa:	3240      	sll	v0,8
     6fc:	ea8d      	or	v0,a0
     6fe:	ea0c      	and	v0,s0
     700:	6782      	move	a0,v0
     702:	67d8      	move	a2,t8
     704:	ecac      	and	a0,a1
     706:	a6c0      	lbu	a2,0(a2)
     708:	3242      	srl	v0,8
     70a:	67d8      	move	a2,t8
     70c:	c680      	sb	a0,0(a2)
     70e:	a681      	lbu	a0,1(a2)
     710:	c641      	sb	v0,1(a2)
     712:	f000 a140 	lbu	v0,0(s1)
     716:	7208      	cmpi	v0,8
     718:	67d8      	move	a2,t8
     71a:	f000 9940 	lw	v0,0(s1)
     71e:	f000 c1c0 	sb	a2,0(s1)
     722:	a280      	lbu	a0,0(v0)
     724:	ec11      	zeb	a0
     726:	650c      	move	t0,a0
     728:	a281      	lbu	a0,1(v0)
     72a:	67c4      	move	a2,a0
     72c:	ee11      	zeb	a2
     72e:	36c0      	sll	a2,8
     730:	6788      	move	a0,t0
     732:	eccd      	or	a0,a2
     734:	e88c      	and	s0,a0
     736:	ed0c      	and	a1,s0
     738:	3002      	srl	s0,8
     73a:	a280      	lbu	a0,0(v0)
     73c:	c2a0      	sb	a1,0(v0)
     73e:	a281      	lbu	a0,1(v0)
     740:	f002 b50c 	lw	a1,174c <$L129+0x808>
     744:	c201      	sb	s0,1(v0)
     746:	6a00      	li	v0,0
     748:	c5e4      	sb	a3,4(a1)
     74a:	cde4      	sh	a3,8(a1)
     74c:	f002 b704 	lw	a3,1750 <$L129+0x80c>
     750:	c744      	sb	v0,4(a3)
     752:	c745      	sb	v0,5(a3)
     754:	c746      	sb	v0,6(a3)
     756:	f000 a140 	lbu	v0,0(s1)
     75a:	6c03      	li	a0,3
     75c:	ea11      	zeb	v0
     75e:	3256      	srl	v0,5
     760:	ea8c      	and	v0,a0
     762:	f140 2a02 	bnez	v0,9ea <$L256>
     766:	9007      	lw	s0,28(sp)
     768:	a041      	lbu	v0,1(s0)
     76a:	ea11      	zeb	v0
     76c:	5a0c      	sltiu	v0,12
     76e:	f120 601c 	bteqz	9ea <$L256>
     772:	3544      	sll	a1,v0,1
     774:	0c03      	la	a0,780 <$L75+0x26c>
     776:	e4b5      	addu	a1,a0,a1
     778:	8da0      	lh	a1,0(a1)
     77a:	e4b1      	addu	a0,a1
     77c:	ec80      	jrc	a0
     77e:	6500      	nop
     780:	082d      	la	s0,834 <$L75+0x320>
     782:	0705      	addiu	a3,sp,20
     784:	026b      	addiu	v0,sp,428
     786:	0705      	addiu	a3,sp,20
     788:	026b      	addiu	v0,sp,428
     78a:	0609      	addiu	a2,sp,36
     78c:	05cf      	addiu	a1,sp,828
     78e:	087b      	la	s0,978 <$L161+0xca>
     790:	06f5      	addiu	a2,sp,980
     792:	0671      	addiu	a2,sp,452
     794:	0645      	addiu	a2,sp,276
     796:	0623      	addiu	a2,sp,140
     798:	7201      	cmpi	v0,1
     79a:	f760 6013 	bteqz	1684 <$L129+0x740>
     79e:	f781 b214 	lw	v0,1730 <$L129+0x7ec>
     7a2:	6b01      	li	v1,1
     7a4:	9a40      	lw	v0,0(v0)
     7a6:	3252      	srl	v0,4
     7a8:	ea6c      	and	v0,v1
     7aa:	ea11      	zeb	v0
     7ac:	f61f 221c 	beqz	v0,3e8 <$L66+0x38>
     7b0:	f7a1 b004 	lw	s0,1754 <$L129+0x810>
     7b4:	9840      	lw	v0,0(s0)
     7b6:	3252      	srl	v0,4
     7b8:	eb4c      	and	v1,v0
     7ba:	eb11      	zeb	v1
     7bc:	f61f 2314 	beqz	v1,3e8 <$L66+0x38>
     7c0:	f781 b318 	lw	v1,1758 <$L129+0x814>
     7c4:	6f10      	li	a3,16
     7c6:	6a00      	li	v0,0
     7c8:	6c74      	li	a0,116
     7ca:	6d00      	li	a1,0
     7cc:	6e00      	li	a2,0
     7ce:	dbe0      	sw	a3,0(v1)
     7d0:	f000 d940 	sw	v0,0(s1)
     7d4:	d30c      	sw	v1,48(sp)
     7d6:	1800 0000 	jal	0 <USBCtrlTrfTxService>
     7da:	d70b      	sw	a3,44(sp)
     7dc:	a080      	lbu	a0,0(s0)
     7de:	6a11      	li	v0,17
     7e0:	ea4b      	neg	v0
     7e2:	ea8c      	and	v0,a0
     7e4:	970b      	lw	a3,44(sp)
     7e6:	930c      	lw	v1,48(sp)
     7e8:	c040      	sb	v0,0(s0)
     7ea:	f741 b20c 	lw	v0,1734 <$L129+0x7f0>
     7ee:	dbe0      	sw	a3,0(v1)
     7f0:	9a40      	lw	v0,0(v0)
     7f2:	6c01      	li	a0,1
     7f4:	3246      	srl	v0,1
     7f6:	ea8c      	and	v0,a0
     7f8:	ea11      	zeb	v0
     7fa:	f5ff 221e 	beqz	v0,3fa <$L66+0x4a>
     7fe:	f761 b300 	lw	v1,175c <$L129+0x818>
     802:	6a09      	li	v0,9
     804:	9b80      	lw	a0,0(v1)
     806:	ea4b      	neg	v0
     808:	ea8c      	and	v0,a0
     80a:	db40      	sw	v0,0(v1)
     80c:	6479      	restore	72,ra,s0-s1
     80e:	e8a0      	jrc	ra
     810:	6791      	move	a0,s1
     812:	f000 4c00 	addiu	a0,0
     816:	e791      	addu	a0,a3,a0
     818:	651c      	move	t8,a0
     81a:	a480      	lbu	a0,0(a0)
     81c:	6801      	li	s0,1
     81e:	650c      	move	t0,a0
     820:	67a8      	move	a1,t0
     822:	edcc      	and	a1,a2
     824:	ed0e      	xor	a1,s0
     826:	6c02      	li	a0,2
     828:	edcc      	and	a1,a2
     82a:	ec8b      	neg	a0
     82c:	67c8      	move	a2,t0
     82e:	eccc      	and	a0,a2
     830:	ecad      	or	a0,a1
     832:	6718      	move	s0,t8
     834:	c080      	sb	a0,0(s0)
     836:	f6df 2707 	beqz	a3,5c8 <$L75+0xb4>
     83a:	67b1      	move	a1,s1
     83c:	6c72      	li	a0,114
     83e:	f000 4d00 	addiu	a1,0
     842:	6e00      	li	a2,0
     844:	1800 0000 	jal	0 <USBCtrlTrfTxService>
     848:	d30c      	sw	v1,48(sp)
     84a:	930c      	lw	v1,48(sp)

0000084c <$L142>:
     84c:	9206      	lw	v0,24(sp)
     84e:	4aff      	addiu	v0,-1
     850:	ea11      	zeb	v0
     852:	d206      	sw	v0,24(sp)
     854:	f69f 2a09 	bnez	v0,56a <$L75+0x56>
     858:	f701 b304 	lw	v1,175c <$L129+0x818>
     85c:	6a09      	li	v0,9
     85e:	9b80      	lw	a0,0(v1)
     860:	ea4b      	neg	v0
     862:	ea8c      	and	v0,a0
     864:	db40      	sw	v0,0(v1)
     866:	6479      	restore	72,ra,s0-s1
     868:	e8a0      	jrc	ra
     86a:	4aff      	addiu	v0,-1
     86c:	ea11      	zeb	v0
     86e:	f000 c140 	sb	v0,0(s1)
     872:	f5ff 2213 	beqz	v0,45c <$L66+0xac>
     876:	9b40      	lw	v0,0(v1)
     878:	6c01      	li	a0,1
     87a:	325e      	srl	v0,7
     87c:	ea8c      	and	v0,a0
     87e:	ea11      	zeb	v0
     880:	f65f 2216 	beqz	v0,530 <$L75+0x1c>
     884:	164d      	b	520 <$L75+0xc>
     886:	7008      	cmpi	s0,8
     888:	61e1      	btnez	84c <$L142>
     88a:	f000 9980 	lw	a0,0(s1)
     88e:	f000 99c0 	lw	a2,0(s1)
     892:	6801      	li	s0,1
     894:	a440      	lbu	v0,0(a0)
     896:	f000 a180 	lbu	a0,0(s1)
     89a:	ea11      	zeb	v0
     89c:	7408      	cmpi	a0,8
     89e:	67b8      	move	a1,t8
     8a0:	325a      	srl	v0,6
     8a2:	7608      	cmpi	a2,8
     8a4:	e84c      	and	s0,v0
     8a6:	f000 c1a0 	sb	a1,0(s1)
     8aa:	f3c0 6014 	bteqz	1056 <$L129+0x112>

000008ae <$L161>:
     8ae:	f000 a140 	lbu	v0,0(s1)
     8b2:	6f01      	li	a3,1
     8b4:	eaee      	xor	v0,a3
     8b6:	f460 2215 	beqz	v0,11a4 <$L129+0x260>
     8ba:	f681 b718 	lw	a3,1750 <$L129+0x80c>
     8be:	a744      	lbu	v0,4(a3)
     8c0:	325e      	srl	v0,7
     8c2:	2218      	beqz	v0,8f4 <$L161+0x46>
     8c4:	a788      	lbu	a0,8(a3)
     8c6:	a747      	lbu	v0,7(a3)
     8c8:	3480      	sll	a0,8
     8ca:	67a4      	move	a1,a0
     8cc:	ed4d      	or	a1,v0
     8ce:	a749      	lbu	v0,9(a3)
     8d0:	3240      	sll	v0,8
     8d2:	3240      	sll	v0,8
     8d4:	6782      	move	a0,v0
     8d6:	a74a      	lbu	v0,10(a3)
     8d8:	ecad      	or	a0,a1
     8da:	f600 3240 	sll	v0,24
     8de:	ea8d      	or	v0,a0
     8e0:	2205      	beqz	v0,8ec <$L161+0x3e>
     8e2:	d30c      	sw	v1,48(sp)
     8e4:	ea40      	jalr	v0
     8e6:	d70b      	sw	a3,44(sp)
     8e8:	970b      	lw	a3,44(sp)
     8ea:	930c      	lw	v1,48(sp)
     8ec:	a784      	lbu	a0,4(a3)
     8ee:	6a7f      	li	v0,127
     8f0:	ea8c      	and	v0,a0
     8f2:	c744      	sb	v0,4(a3)
     8f4:	6a00      	li	v0,0
     8f6:	f000 c140 	sb	v0,0(s1)
     8fa:	17a8      	b	84c <$L142>
     8fc:	1800 0000 	jal	0 <USBCtrlTrfTxService>
     900:	d30c      	sw	v1,48(sp)
     902:	6a04      	li	v0,4
     904:	6c01      	li	a0,1
     906:	930c      	lw	v1,48(sp)
     908:	f000 d940 	sw	v0,0(s1)
     90c:	f601 b21c 	lw	v0,1728 <$L129+0x7e4>
     910:	da80      	sw	a0,0(v0)
     912:	9b40      	lw	v0,0(v1)
     914:	6c01      	li	a0,1
     916:	3252      	srl	v0,4
     918:	ea8c      	and	v0,a0
     91a:	ea11      	zeb	v0
     91c:	f59f 2208 	beqz	v0,430 <$L66+0x80>
     920:	157f      	b	420 <$L66+0x70>
     922:	f621 b414 	lw	a0,1754 <$L129+0x810>
     926:	6810      	li	s0,16
     928:	a440      	lbu	v0,0(a0)
     92a:	f601 b700 	lw	a3,1728 <$L129+0x7e4>
     92e:	ea0d      	or	v0,s0
     930:	6d00      	li	a1,0
     932:	c440      	sb	v0,0(a0)
     934:	6a01      	li	v0,1
     936:	6c75      	li	a0,117
     938:	6e00      	li	a2,0
     93a:	df00      	sw	s0,0(a3)
     93c:	f000 d940 	sw	v0,0(s1)
     940:	d30c      	sw	v1,48(sp)
     942:	1800 0000 	jal	0 <USBCtrlTrfTxService>
     946:	d70b      	sw	a3,44(sp)
     948:	970b      	lw	a3,44(sp)
     94a:	930c      	lw	v1,48(sp)
     94c:	df00      	sw	s0,0(a3)
     94e:	6c01      	li	a0,1
     950:	9b40      	lw	v0,0(v1)
     952:	324a      	srl	v0,2
     954:	ea8c      	and	v0,a0
     956:	ea11      	zeb	v0
     958:	f5df 221c 	beqz	v0,514 <$L75>
     95c:	156f      	b	43c <$L66+0x8c>
     95e:	f601 b504 	lw	a1,1760 <$L129+0x81c>
     962:	9d40      	lw	v0,0(a1)
     964:	3246      	srl	v0,1
     966:	ec4c      	and	a0,v0
     968:	ec11      	zeb	a0
     96a:	f340 2c12 	bnez	a0,1012 <$L129+0xce>
     96e:	6c80      	li	a0,128
     970:	f5a1 b218 	lw	v0,1728 <$L129+0x7e4>
     974:	da80      	sw	a0,0(v0)
     976:	9b40      	lw	v0,0(v1)
     978:	6c01      	li	a0,1
     97a:	3246      	srl	v0,1
     97c:	ea8c      	and	v0,a0
     97e:	ea11      	zeb	v0
     980:	f5ff 2204 	beqz	v0,54c <$L75+0x38>
     984:	15db      	b	53c <$L75+0x28>
     986:	f7ef 6c1f 	li	a0,32767
     98a:	6d00      	li	a1,0
     98c:	6e01      	li	a2,1
     98e:	1800 0000 	jal	0 <USBCtrlTrfTxService>
     992:	d30c      	sw	v1,48(sp)
     994:	6cff      	li	a0,255
     996:	f5c1 b210 	lw	v0,1764 <$L129+0x820>
     99a:	f000 9900 	lw	s0,0(s1)
     99e:	da80      	sw	a0,0(v0)
     9a0:	6c02      	li	a0,2
     9a2:	f581 b208 	lw	v0,1728 <$L129+0x7e4>
     9a6:	5804      	sltiu	s0,4
     9a8:	da80      	sw	a0,0(v0)
     9aa:	930c      	lw	v1,48(sp)
     9ac:	f5df 6013 	bteqz	556 <$L75+0x42>
     9b0:	1726      	b	7fe <$L75+0x2ea>
     9b2:	a540      	lbu	v0,0(a1)
     9b4:	325e      	srl	v0,7
     9b6:	2210      	beqz	v0,9d8 <$L250+0xa>
     9b8:	a644      	lbu	v0,4(a2)
     9ba:	ea11      	zeb	v0
     9bc:	325e      	srl	v0,7
     9be:	f660 2a1e 	bnez	v0,16be <$L129+0x77a>
     9c2:	a684      	lbu	a0,4(a2)
     9c4:	6a0f      	li	v0,15
     9c6:	ea8c      	and	v0,a0
     9c8:	6791      	move	a0,s1
     9ca:	f000 4c00 	addiu	a0,0

000009ce <$L250>:
     9ce:	e289      	addu	v0,a0
     9d0:	6c02      	li	a0,2
     9d2:	a2c0      	lbu	a2,0(v0)
     9d4:	eccd      	or	a0,a2
     9d6:	c280      	sb	a0,0(v0)
     9d8:	a581      	lbu	a0,1(a1)
     9da:	a540      	lbu	v0,0(a1)
     9dc:	3480      	sll	a0,8
     9de:	ec4d      	or	a0,v0
     9e0:	6a84      	li	v0,132
     9e2:	ea8d      	or	v0,a0
     9e4:	c540      	sb	v0,0(a1)
     9e6:	3242      	srl	v0,8
     9e8:	c541      	sb	v0,1(a1)

000009ea <$L256>:
     9ea:	f521 b01c 	lw	s0,1724 <$L129+0x7e0>

000009ee <$L95>:
     9ee:	6c03      	li	a0,3
     9f0:	6d00      	li	a1,0
     9f2:	6e00      	li	a2,0
     9f4:	d30c      	sw	v1,48(sp)
     9f6:	1800 0000 	jal	0 <USBCtrlTrfTxService>
     9fa:	d70b      	sw	a3,44(sp)
     9fc:	a080      	lbu	a0,0(s0)
     9fe:	6a21      	li	v0,33
     a00:	ea4b      	neg	v0
     a02:	ea8c      	and	v0,a0
     a04:	f541 b508 	lw	a1,174c <$L129+0x808>
     a08:	c040      	sb	v0,0(s0)
     a0a:	6c80      	li	a0,128
     a0c:	a544      	lbu	v0,4(a1)
     a0e:	ec8b      	neg	a0
     a10:	ea8c      	and	v0,a0
     a12:	930c      	lw	v1,48(sp)
     a14:	970b      	lw	a3,44(sp)
     a16:	2a52      	bnez	v0,abc <$L95+0xce>
     a18:	a744      	lbu	v0,4(a3)
     a1a:	ec4c      	and	a0,v0
     a1c:	f0e0 241f 	beqz	a0,c1e <$L95+0x230>
     a20:	6a02      	li	v0,2
     a22:	f000 9980 	lw	a0,0(s1)
     a26:	f000 c140 	sb	v0,0(s1)
     a2a:	2c2c      	bnez	a0,a84 <$L95+0x96>
     a2c:	f000 9940 	lw	v0,0(s1)
     a30:	f000 d980 	sw	a0,0(s1)
     a34:	f000 d980 	sw	a0,0(s1)
     a38:	a2a2      	lbu	a1,2(v0)
     a3a:	6d40      	li	a1,64
     a3c:	f501 b700 	lw	a3,173c <$L129+0x7f8>
     a40:	c2a2      	sb	a1,2(v0)
     a42:	a2c3      	lbu	a2,3(v0)
     a44:	4dbc      	addiu	a1,-68
     a46:	edcc      	and	a1,a2
     a48:	6eff      	li	a2,255
     a4a:	c2a3      	sb	a1,3(v0)
     a4c:	f501 b51c 	lw	a1,1768 <$L129+0x824>
     a50:	edec      	and	a1,a3
     a52:	67e5      	move	a3,a1
     a54:	efcc      	and	a3,a2
     a56:	a204      	lbu	s0,4(v0)
     a58:	c2e4      	sb	a3,4(v0)
     a5a:	37a2      	srl	a3,a1,8
     a5c:	6707      	move	s0,a3
     a5e:	e8cc      	and	s0,a2
     a60:	6508      	move	t0,s0
     a62:	37e2      	srl	a3,8
     a64:	a205      	lbu	s0,5(v0)
     a66:	6708      	move	s0,t0
     a68:	eeec      	and	a2,a3
     a6a:	c205      	sb	s0,5(v0)
     a6c:	f600 35a2 	srl	a1,24
     a70:	a2e6      	lbu	a3,6(v0)
     a72:	c2c6      	sb	a2,6(v0)
     a74:	a2c7      	lbu	a2,7(v0)
     a76:	c2a7      	sb	a1,7(v0)
     a78:	a2a0      	lbu	a1,0(v0)
     a7a:	6d38      	li	a1,56
     a7c:	edab      	neg	a1
     a7e:	c2a0      	sb	a1,0(v0)
     a80:	a2a1      	lbu	a1,1(v0)
     a82:	c281      	sb	a0,1(v0)
     a84:	6a00      	li	v0,0
     a86:	f000 d940 	sw	v0,0(s1)
     a8a:	f000 d940 	sw	v0,0(s1)
     a8e:	16de      	b	84c <$L142>
     a90:	6c73      	li	a0,115
     a92:	6d00      	li	a1,0
     a94:	6e01      	li	a2,1
     a96:	1800 0000 	jal	0 <USBCtrlTrfTxService>
     a9a:	d30c      	sw	v1,48(sp)
     a9c:	930c      	lw	v1,48(sp)
     a9e:	14d6      	b	44c <$L66+0x9c>
     aa0:	f000 a180 	lbu	a0,0(s1)
     aa4:	7402      	cmpi	a0,2
     aa6:	f2e0 601d 	bteqz	10a4 <$L129+0x160>
     aaa:	f000 9900 	lw	s0,0(s1)
     aae:	f000 c1e0 	sb	a3,0(s1)
     ab2:	f080 200b 	beqz	s0,bcc <$L95+0x1de>
     ab6:	f000 d9e0 	sw	a3,0(s1)
     aba:	16c8      	b	84c <$L142>
     abc:	f000 a140 	lbu	v0,0(s1)
     ac0:	ea11      	zeb	v0
     ac2:	325e      	srl	v0,7
     ac4:	f0c0 2a17 	bnez	v0,c76 <$L95+0x288>
     ac8:	6a02      	li	v0,2
     aca:	6804      	li	s0,4
     acc:	f000 c140 	sb	v0,0(s1)
     ad0:	f000 9940 	lw	v0,0(s1)
     ad4:	e80b      	neg	s0
     ad6:	9708      	lw	a3,32(sp)
     ad8:	a2a2      	lbu	a1,2(v0)
     ada:	6d40      	li	a1,64
     adc:	650d      	move	t0,a1
     ade:	c2a2      	sb	a1,2(v0)
     ae0:	a2a3      	lbu	a1,3(v0)
     ae2:	6eff      	li	a2,255
     ae4:	ed0c      	and	a1,s0
     ae6:	efcc      	and	a3,a2
     ae8:	c2a3      	sb	a1,3(v0)
     aea:	a2a4      	lbu	a1,4(v0)
     aec:	652f      	move	t1,a3
     aee:	c2e4      	sb	a3,4(v0)
     af0:	9708      	lw	a3,32(sp)
     af2:	35e2      	srl	a1,a3,8
     af4:	67e5      	move	a3,a1
     af6:	efcc      	and	a3,a2
     af8:	65cf      	move	t6,a3
     afa:	a2e5      	lbu	a3,5(v0)
     afc:	67ee      	move	a3,t6
     afe:	35a2      	srl	a1,8
     b00:	c2e5      	sb	a3,5(v0)
     b02:	9708      	lw	a3,32(sp)
     b04:	edcc      	and	a1,a2
     b06:	f600 37e2 	srl	a3,24
     b0a:	a2c6      	lbu	a2,6(v0)
     b0c:	c2a6      	sb	a1,6(v0)
     b0e:	a2c7      	lbu	a2,7(v0)
     b10:	c2e7      	sb	a3,7(v0)
     b12:	a2c0      	lbu	a2,0(v0)
     b14:	6e7c      	li	a2,124
     b16:	eecb      	neg	a2
     b18:	656e      	move	t3,a2
     b1a:	c2c0      	sb	a2,0(v0)
     b1c:	a2c1      	lbu	a2,1(v0)
     b1e:	6e00      	li	a2,0
     b20:	654f      	move	t2,a3
     b22:	c2c1      	sb	a2,1(v0)
     b24:	f000 d9c0 	sw	a2,0(s1)
     b28:	f000 d9c0 	sw	a2,0(s1)
     b2c:	f000 99c0 	lw	a2,0(s1)
     b30:	f69f 2e0c 	bnez	a2,84c <$L142>
     b34:	f000 99c0 	lw	a2,0(s1)
     b38:	f69f 2e08 	bnez	a2,84c <$L142>
     b3c:	6e01      	li	a2,1
     b3e:	65ae      	move	t5,a2
     b40:	f000 d9c0 	sw	a2,0(s1)
     b44:	f000 99c0 	lw	a2,0(s1)
     b48:	f69f 2e00 	bnez	a2,84c <$L142>
     b4c:	67ed      	move	a3,t5
     b4e:	f000 a1c0 	lbu	a2,0(s1)
     b52:	f000 d9e0 	sw	a3,0(s1)
     b56:	6f02      	li	a3,2
     b58:	efce      	xor	a3,a2
     b5a:	f4c0 2716 	beqz	a3,150a <$L129+0x5c6>
     b5e:	7601      	cmpi	a2,1
     b60:	67f8      	move	a3,t8
     b62:	f67f 2f13 	bnez	a3,84c <$L142>
     b66:	f000 99e0 	lw	a3,0(s1)
     b6a:	a7c2      	lbu	a2,2(a3)
     b6c:	67c8      	move	a2,t0
     b6e:	c7c2      	sb	a2,2(a3)
     b70:	a7c3      	lbu	a2,3(a3)
     b72:	ee0c      	and	a2,s0
     b74:	c7c3      	sb	a2,3(a3)
     b76:	a7c4      	lbu	a2,4(a3)
     b78:	67c9      	move	a2,t1
     b7a:	c7c4      	sb	a2,4(a3)
     b7c:	a7c5      	lbu	a2,5(a3)
     b7e:	67ce      	move	a2,t6
     b80:	c7c5      	sb	a2,5(a3)
     b82:	a7c6      	lbu	a2,6(a3)
     b84:	c7a6      	sb	a1,6(a3)
     b86:	a7c7      	lbu	a2,7(a3)
     b88:	67ca      	move	a2,t2
     b8a:	c7c7      	sb	a2,7(a3)
     b8c:	a7c0      	lbu	a2,0(a3)
     b8e:	67cb      	move	a2,t3
     b90:	c7c0      	sb	a2,0(a3)
     b92:	a7c1      	lbu	a2,1(a3)
     b94:	67d8      	move	a2,t8
     b96:	c7c1      	sb	a2,1(a3)
     b98:	67ed      	move	a3,t5
     b9a:	f000 d9e0 	sw	a3,0(s1)
     b9e:	a2c2      	lbu	a2,2(v0)
     ba0:	67c8      	move	a2,t0
     ba2:	67e9      	move	a3,t1
     ba4:	c2c2      	sb	a2,2(v0)
     ba6:	a2c3      	lbu	a2,3(v0)
     ba8:	ee0c      	and	a2,s0
     baa:	670e      	move	s0,t6
     bac:	c2c3      	sb	a2,3(v0)
     bae:	a2c4      	lbu	a2,4(v0)
     bb0:	c2e4      	sb	a3,4(v0)
     bb2:	a2c5      	lbu	a2,5(v0)
     bb4:	c205      	sb	s0,5(v0)
     bb6:	a2c6      	lbu	a2,6(v0)
     bb8:	c2a6      	sb	a1,6(v0)
     bba:	a2a7      	lbu	a1,7(v0)
     bbc:	67aa      	move	a1,t2
     bbe:	67d8      	move	a2,t8
     bc0:	c2a7      	sb	a1,7(v0)
     bc2:	a2a0      	lbu	a1,0(v0)
     bc4:	c280      	sb	a0,0(v0)
     bc6:	a281      	lbu	a0,1(v0)
     bc8:	c2c1      	sb	a2,1(v0)
     bca:	1640      	b	84c <$L142>
     bcc:	f000 9940 	lw	v0,0(s1)
     bd0:	a282      	lbu	a0,2(v0)
     bd2:	6c40      	li	a0,64
     bd4:	c282      	sb	a0,2(v0)
     bd6:	a2c3      	lbu	a2,3(v0)
     bd8:	4cbc      	addiu	a0,-68
     bda:	eccc      	and	a0,a2
     bdc:	67d1      	move	a2,s1
     bde:	c283      	sb	a0,3(v0)
     be0:	f341 b41c 	lw	a0,173c <$L129+0x7f8>
     be4:	f000 4e00 	addiu	a2,0
     be8:	eccc      	and	a0,a2
     bea:	67c4      	move	a2,a0
     bec:	eeac      	and	a2,a1
     bee:	a204      	lbu	s0,4(v0)
     bf0:	c2c4      	sb	a2,4(v0)
     bf2:	3682      	srl	a2,a0,8
     bf4:	6706      	move	s0,a2
     bf6:	e8ac      	and	s0,a1
     bf8:	6508      	move	t0,s0
     bfa:	36c2      	srl	a2,8
     bfc:	a205      	lbu	s0,5(v0)
     bfe:	6708      	move	s0,t0
     c00:	edcc      	and	a1,a2
     c02:	c205      	sb	s0,5(v0)
     c04:	f600 3482 	srl	a0,24
     c08:	a2c6      	lbu	a2,6(v0)
     c0a:	c2a6      	sb	a1,6(v0)
     c0c:	a2a7      	lbu	a1,7(v0)
     c0e:	c287      	sb	a0,7(v0)
     c10:	a280      	lbu	a0,0(v0)
     c12:	6c74      	li	a0,116
     c14:	ec8b      	neg	a0
     c16:	c280      	sb	a0,0(v0)
     c18:	a281      	lbu	a0,1(v0)
     c1a:	c2e1      	sb	a3,1(v0)
     c1c:	1617      	b	84c <$L142>
     c1e:	f000 9940 	lw	v0,0(s1)
     c22:	a2a2      	lbu	a1,2(v0)
     c24:	6d40      	li	a1,64
     c26:	c2a2      	sb	a1,2(v0)
     c28:	a2c3      	lbu	a2,3(v0)
     c2a:	4dbc      	addiu	a1,-68
     c2c:	edcc      	and	a1,a2
     c2e:	9608      	lw	a2,32(sp)
     c30:	c2a3      	sb	a1,3(v0)
     c32:	6dff      	li	a1,255
     c34:	a2e4      	lbu	a3,4(v0)
     c36:	eeac      	and	a2,a1
     c38:	9708      	lw	a3,32(sp)
     c3a:	c2c4      	sb	a2,4(v0)
     c3c:	36e2      	srl	a2,a3,8
     c3e:	67e6      	move	a3,a2
     c40:	efac      	and	a3,a1
     c42:	36c2      	srl	a2,8
     c44:	a205      	lbu	s0,5(v0)
     c46:	edcc      	and	a1,a2
     c48:	9008      	lw	s0,32(sp)
     c4a:	c2e5      	sb	a3,5(v0)
     c4c:	a2c6      	lbu	a2,6(v0)
     c4e:	c2a6      	sb	a1,6(v0)
     c50:	f600 3502 	srl	a1,s0,24
     c54:	a2c7      	lbu	a2,7(v0)
     c56:	c2a7      	sb	a1,7(v0)
     c58:	a2a0      	lbu	a1,0(v0)
     c5a:	6d74      	li	a1,116
     c5c:	edab      	neg	a1
     c5e:	c2a0      	sb	a1,0(v0)
     c60:	a2a1      	lbu	a1,1(v0)
     c62:	c281      	sb	a0,1(v0)
     c64:	f000 9940 	lw	v0,0(s1)
     c68:	a2a0      	lbu	a1,0(v0)
     c6a:	6d7c      	li	a1,124
     c6c:	edab      	neg	a1
     c6e:	c2a0      	sb	a1,0(v0)
     c70:	a2a1      	lbu	a1,1(v0)
     c72:	c281      	sb	a0,1(v0)
     c74:	15eb      	b	84c <$L142>
     c76:	6a01      	li	v0,1
     c78:	f000 c140 	sb	v0,0(s1)
     c7c:	f000 9940 	lw	v0,0(s1)
     c80:	f2c0 2216 	beqz	v0,1230 <$L129+0x2ec>
     c84:	6a00      	li	v0,0
     c86:	f000 d940 	sw	v0,0(s1)
     c8a:	f000 d940 	sw	v0,0(s1)
     c8e:	f000 9940 	lw	v0,0(s1)
     c92:	f5df 2a1b 	bnez	v0,84c <$L142>
     c96:	f000 9940 	lw	v0,0(s1)
     c9a:	f5df 2a17 	bnez	v0,84c <$L142>
     c9e:	6801      	li	s0,1
     ca0:	f000 d900 	sw	s0,0(s1)
     ca4:	f000 9940 	lw	v0,0(s1)
     ca8:	f5df 2a10 	bnez	v0,84c <$L142>
     cac:	f000 a1e0 	lbu	a3,0(s1)
     cb0:	6c02      	li	a0,2
     cb2:	ecee      	xor	a0,a3
     cb4:	f000 d900 	sw	s0,0(s1)
     cb8:	f3a0 240a 	beqz	a0,1410 <$L129+0x4cc>
     cbc:	6a01      	li	v0,1
     cbe:	ef4e      	xor	a3,v0
     cc0:	f5df 2f04 	bnez	a3,84c <$L142>
     cc4:	f000 9940 	lw	v0,0(s1)
     cc8:	6e04      	li	a2,4
     cca:	eecb      	neg	a2
     ccc:	a282      	lbu	a0,2(v0)
     cce:	6c40      	li	a0,64
     cd0:	650c      	move	t0,a0
     cd2:	c282      	sb	a0,2(v0)
     cd4:	a2a3      	lbu	a1,3(v0)
     cd6:	9408      	lw	a0,32(sp)
     cd8:	edcc      	and	a1,a2
     cda:	656e      	move	t3,a2
     cdc:	6eff      	li	a2,255
     cde:	eccc      	and	a0,a2
     ce0:	c2a3      	sb	a1,3(v0)
     ce2:	a2a4      	lbu	a1,4(v0)
     ce4:	652c      	move	t1,a0
     ce6:	c284      	sb	a0,4(v0)
     ce8:	9408      	lw	a0,32(sp)
     cea:	3582      	srl	a1,a0,8
     cec:	6785      	move	a0,a1
     cee:	eccc      	and	a0,a2
     cf0:	651c      	move	t8,a0
     cf2:	a285      	lbu	a0,5(v0)
     cf4:	6798      	move	a0,t8
     cf6:	35a2      	srl	a1,8
     cf8:	c285      	sb	a0,5(v0)
     cfa:	9408      	lw	a0,32(sp)
     cfc:	edcc      	and	a1,a2
     cfe:	a2c6      	lbu	a2,6(v0)
     d00:	f600 3682 	srl	a2,a0,24
     d04:	c2a6      	sb	a1,6(v0)
     d06:	a287      	lbu	a0,7(v0)
     d08:	c2c7      	sb	a2,7(v0)
     d0a:	a280      	lbu	a0,0(v0)
     d0c:	6c7c      	li	a0,124
     d0e:	ec8b      	neg	a0
     d10:	c280      	sb	a0,0(v0)
     d12:	a281      	lbu	a0,1(v0)
     d14:	c2e1      	sb	a3,1(v0)
     d16:	f000 9940 	lw	v0,0(s1)
     d1a:	f000 d900 	sw	s0,0(s1)
     d1e:	678b      	move	a0,t3
     d20:	a202      	lbu	s0,2(v0)
     d22:	6708      	move	s0,t0
     d24:	c202      	sb	s0,2(v0)
     d26:	a203      	lbu	s0,3(v0)
     d28:	ec0c      	and	a0,s0
     d2a:	6718      	move	s0,t8
     d2c:	c283      	sb	a0,3(v0)
     d2e:	a284      	lbu	a0,4(v0)
     d30:	6789      	move	a0,t1
     d32:	c284      	sb	a0,4(v0)
     d34:	a285      	lbu	a0,5(v0)
     d36:	c205      	sb	s0,5(v0)
     d38:	a286      	lbu	a0,6(v0)
     d3a:	c2a6      	sb	a1,6(v0)
     d3c:	a287      	lbu	a0,7(v0)
     d3e:	c2c7      	sb	a2,7(v0)
     d40:	a280      	lbu	a0,0(v0)
     d42:	6c80      	li	a0,128
     d44:	ec8b      	neg	a0
     d46:	c280      	sb	a0,0(v0)
     d48:	a281      	lbu	a0,1(v0)
     d4a:	c2e1      	sb	a3,1(v0)
     d4c:	157f      	b	84c <$L142>
     d4e:	f000 a1a0 	lbu	a1,0(s1)
     d52:	6880      	li	s0,128
     d54:	ed11      	zeb	a1
     d56:	ed0e      	xor	a1,s0
     d58:	f65f 2d07 	bnez	a1,9ea <$L256>
     d5c:	6c40      	li	a0,64
     d5e:	9007      	lw	s0,28(sp)
     d60:	ec8b      	neg	a0
     d62:	f1e1 b20c 	lw	v0,174c <$L129+0x808>
     d66:	c284      	sb	a0,4(v0)
     d68:	a083      	lbu	a0,3(s0)
     d6a:	ec11      	zeb	a0
     d6c:	7402      	cmpi	a0,2
     d6e:	f400 6005 	bteqz	157c <$L129+0x638>
     d72:	6e03      	li	a2,3
     d74:	ee8e      	xor	a2,a0
     d76:	f3e0 260b 	beqz	a2,1550 <$L129+0x60c>
     d7a:	7401      	cmpi	a0,1
     d7c:	f400 6018 	bteqz	15b0 <$L129+0x66c>
     d80:	c2a4      	sb	a1,4(v0)
     d82:	f1a1 b004 	lw	s0,1724 <$L129+0x7e0>
     d86:	1633      	b	9ee <$L95>
     d88:	f1c1 b604 	lw	a2,174c <$L129+0x808>
     d8c:	6a80      	li	v0,128
     d8e:	a684      	lbu	a0,4(a2)
     d90:	ea4b      	neg	v0
     d92:	ea8d      	or	v0,a0
     d94:	f181 b010 	lw	s0,1724 <$L129+0x7e0>
     d98:	c644      	sb	v0,4(a2)
     d9a:	6a08      	li	v0,8
     d9c:	f000 d940 	sw	v0,0(s1)
     da0:	1626      	b	9ee <$L95>
     da2:	f1a1 b00c 	lw	s0,174c <$L129+0x808>
     da6:	6a80      	li	v0,128
     da8:	a084      	lbu	a0,4(s0)
     daa:	ea4b      	neg	v0
     dac:	ea8d      	or	v0,a0
     dae:	67b1      	move	a1,s1
     db0:	c044      	sb	v0,4(s0)
     db2:	9007      	lw	s0,28(sp)
     db4:	f000 4d00 	addiu	a1,0
     db8:	a044      	lbu	v0,4(s0)
     dba:	a082      	lbu	a0,2(s0)
     dbc:	ea11      	zeb	v0
     dbe:	e2a9      	addu	v0,a1
     dc0:	c280      	sb	a0,0(v0)
     dc2:	1613      	b	9ea <$L256>
     dc4:	9007      	lw	s0,28(sp)
     dc6:	6791      	move	a0,s1
     dc8:	f000 4c00 	addiu	a0,0
     dcc:	a044      	lbu	v0,4(s0)
     dce:	ea11      	zeb	v0
     dd0:	e289      	addu	v0,a0
     dd2:	f161 b41c 	lw	a0,174c <$L129+0x808>
     dd6:	dc40      	sw	v0,0(a0)
     dd8:	a444      	lbu	v0,4(a0)
     dda:	6d01      	li	a1,1
     ddc:	6782      	move	a0,v0
     dde:	6a80      	li	v0,128
     de0:	ecad      	or	a0,a1
     de2:	ea4b      	neg	v0
     de4:	ea8d      	or	v0,a0
     de6:	f161 b608 	lw	a2,174c <$L129+0x808>
     dea:	c6a8      	sb	a1,8(a2)
     dec:	c644      	sb	v0,4(a2)
     dee:	15fd      	b	9ea <$L256>
     df0:	f141 b21c 	lw	v0,174c <$L129+0x808>
     df4:	6d00      	li	a1,0
     df6:	a284      	lbu	a0,4(v0)
     df8:	6a80      	li	v0,128
     dfa:	ea4b      	neg	v0
     dfc:	ea8d      	or	v0,a0
     dfe:	f141 b410 	lw	a0,174c <$L129+0x808>
     e02:	c444      	sb	v0,4(a0)
     e04:	f161 b208 	lw	v0,176c <$L129+0x828>
     e08:	6c00      	li	a0,0
     e0a:	651d      	move	t8,a1
     e0c:	daa0      	sw	a1,0(v0)
     e0e:	6d00      	li	a1,0
     e10:	67ca      	move	a2,t2
     e12:	f101 b014 	lw	s0,1724 <$L129+0x7e0>
     e16:	de80      	sw	a0,0(a2)
     e18:	dea1      	sw	a1,4(a2)
     e1a:	de82      	sw	a0,8(a2)
     e1c:	dea3      	sw	a1,12(a2)
     e1e:	de84      	sw	a0,16(a2)
     e20:	dea5      	sw	a1,20(a2)
     e22:	de86      	sw	a0,24(a2)
     e24:	dea7      	sw	a1,28(a2)
     e26:	de88      	sw	a0,32(a2)
     e28:	dea9      	sw	a1,36(a2)
     e2a:	de8a      	sw	a0,40(a2)
     e2c:	deab      	sw	a1,44(a2)
     e2e:	de8c      	sw	a0,48(a2)
     e30:	dead      	sw	a1,52(a2)
     e32:	de8e      	sw	a0,56(a2)
     e34:	deaf      	sw	a1,60(a2)
     e36:	a080      	lbu	a0,0(s0)
     e38:	6a02      	li	v0,2
     e3a:	ea8d      	or	v0,a0
     e3c:	f000 d9c0 	sw	a2,0(s1)
     e40:	c040      	sb	v0,0(s0)
     e42:	6758      	move	v0,t8
     e44:	a080      	lbu	a0,0(s0)
     e46:	f000 c140 	sb	v0,0(s1)
     e4a:	6a03      	li	v0,3
     e4c:	ea4b      	neg	v0
     e4e:	ea8c      	and	v0,a0
     e50:	9407      	lw	a0,28(sp)
     e52:	c040      	sb	v0,0(s0)
     e54:	f101 b21c 	lw	v0,1770 <$L129+0x82c>
     e58:	f000 d9c0 	sw	a2,0(s1)
     e5c:	f000 d940 	sw	v0,0(s1)
     e60:	a442      	lbu	v0,2(a0)
     e62:	ea11      	zeb	v0
     e64:	f000 c140 	sb	v0,0(s1)
     e68:	f2a0 2a1e 	bnez	v0,13e8 <$L129+0x4a4>
     e6c:	6a10      	li	v0,16
     e6e:	f000 d940 	sw	v0,0(s1)
     e72:	15bd      	b	9ee <$L95>
     e74:	6751      	move	v0,s1
     e76:	f000 4a00 	addiu	v0,0
     e7a:	f0c1 b514 	lw	a1,174c <$L129+0x808>
     e7e:	dd40      	sw	v0,0(a1)
     e80:	a544      	lbu	v0,4(a1)
     e82:	17ab      	b	dda <$L95+0x3ec>
     e84:	9007      	lw	s0,28(sp)
     e86:	a042      	lbu	v0,2(s0)
     e88:	ea11      	zeb	v0
     e8a:	7201      	cmpi	v0,1
     e8c:	f2e0 6004 	bteqz	1458 <$L129+0x514>
     e90:	9007      	lw	s0,28(sp)
     e92:	a042      	lbu	v0,2(s0)
     e94:	ea11      	zeb	v0
     e96:	f5bf 2a08 	bnez	v0,9ea <$L256>
     e9a:	f000 a180 	lbu	a0,0(s1)
     e9e:	6a1f      	li	v0,31
     ea0:	ea8c      	and	v0,a0
     ea2:	7202      	cmpi	v0,2
     ea4:	f5bf 6101 	btnez	9ea <$L256>
     ea8:	9007      	lw	s0,28(sp)
     eaa:	6a0f      	li	v0,15
     eac:	a084      	lbu	a0,4(s0)
     eae:	ec4c      	and	a0,v0
     eb0:	ec11      	zeb	a0
     eb2:	f59f 241a 	beqz	a0,9ea <$L256>
     eb6:	a084      	lbu	a0,4(s0)
     eb8:	ec4c      	and	a0,v0
     eba:	ec11      	zeb	a0
     ebc:	5402      	slti	a0,2
     ebe:	f59f 6014 	bteqz	9ea <$L256>
     ec2:	f000 9980 	lw	a0,0(s1)
     ec6:	7420      	cmpi	a0,32
     ec8:	f59f 610f 	btnez	9ea <$L256>
     ecc:	f081 b600 	lw	a2,174c <$L129+0x808>
     ed0:	6c80      	li	a0,128
     ed2:	a6a4      	lbu	a1,4(a2)
     ed4:	ec8b      	neg	a0
     ed6:	9007      	lw	s0,28(sp)
     ed8:	ecad      	or	a0,a1
     eda:	c684      	sb	a0,4(a2)
     edc:	a084      	lbu	a0,4(s0)
     ede:	ec11      	zeb	a0
     ee0:	349e      	srl	a0,7
     ee2:	f380 2c1d 	bnez	a0,1620 <$L129+0x6dc>
     ee6:	a084      	lbu	a0,4(s0)
     ee8:	67b1      	move	a1,s1
     eea:	ec4c      	and	a0,v0
     eec:	ec11      	zeb	a0
     eee:	f000 4d00 	addiu	a1,0
     ef2:	3488      	sll	a0,2
     ef4:	e4b1      	addu	a0,a1
     ef6:	a0a4      	lbu	a1,4(s0)
     ef8:	eaac      	and	v0,a1
     efa:	67b1      	move	a1,s1
     efc:	ea11      	zeb	v0
     efe:	f000 4d00 	addiu	a1,0
     f02:	e2a9      	addu	v0,a1
     f04:	a240      	lbu	v0,0(v0)
     f06:	d209      	sw	v0,36(sp)
     f08:	9c40      	lw	v0,0(a0)
     f0a:	9009      	lw	s0,36(sp)
     f0c:	d204      	sw	v0,16(sp)
     f0e:	6a01      	li	v0,1
     f10:	ea0c      	and	v0,s0
     f12:	f3e0 2a13 	bnez	v0,16fc <$L129+0x7b8>
     f16:	679d      	move	a0,sp
     f18:	6809      	li	s0,9
     f1a:	a450      	lbu	v0,16(a0)
     f1c:	e80b      	neg	s0
     f1e:	e84c      	and	s0,v0

00000f20 <$L127>:
     f20:	9407      	lw	a0,28(sp)
     f22:	67dd      	move	a2,sp
     f24:	c610      	sb	s0,16(a2)
     f26:	a444      	lbu	v0,4(a0)
     f28:	ea11      	zeb	v0
     f2a:	325e      	srl	v0,7
     f2c:	f3c0 2a14 	bnez	v0,16d8 <$L129+0x794>
     f30:	a484      	lbu	a0,4(a0)
     f32:	6a0f      	li	v0,15
     f34:	ea8c      	and	v0,a0
     f36:	9504      	lw	a1,16(sp)
     f38:	6791      	move	a0,s1
     f3a:	3248      	sll	v0,2
     f3c:	f000 4c00 	addiu	a0,0
     f40:	e289      	addu	v0,a0
     f42:	daa0      	sw	a1,0(v0)

00000f44 <$L129>:
     f44:	9607      	lw	a2,28(sp)
     f46:	a641      	lbu	v0,1(a2)
     f48:	ea11      	zeb	v0
     f4a:	7203      	cmpi	v0,3
     f4c:	f53f 6011 	bteqz	9b2 <$L161+0x104>
     f50:	7008      	cmpi	s0,8
     f52:	6798      	move	a0,t8
     f54:	675d      	move	v0,sp
     f56:	c290      	sb	a0,16(v0)
     f58:	9504      	lw	a1,16(sp)
     f5a:	a540      	lbu	v0,0(a1)
     f5c:	325e      	srl	v0,7
     f5e:	f380 2a19 	bnez	v0,1694 <$L129+0x750>
     f62:	a581      	lbu	a0,1(a1)
     f64:	a540      	lbu	v0,0(a1)
     f66:	3480      	sll	a0,8
     f68:	ec4d      	or	a0,v0
     f6a:	6a40      	li	v0,64
     f6c:	ea8d      	or	v0,a0
     f6e:	c540      	sb	v0,0(a1)
     f70:	3242      	srl	v0,8
     f72:	c541      	sb	v0,1(a1)
     f74:	67bd      	move	a1,sp
     f76:	c510      	sb	s0,16(a1)
     f78:	9009      	lw	s0,36(sp)
     f7a:	6a02      	li	v0,2
     f7c:	ea0c      	and	v0,s0
     f7e:	9504      	lw	a1,16(sp)
     f80:	f320 2a04 	bnez	v0,15cc <$L129+0x688>
     f84:	a540      	lbu	v0,0(a1)
     f86:	6c7c      	li	a0,124
     f88:	ec8b      	neg	a0
     f8a:	ea8c      	and	v0,a0
     f8c:	7280      	cmpi	v0,128
     f8e:	f300 601d 	bteqz	15cc <$L129+0x688>
     f92:	a581      	lbu	a0,1(a1)
     f94:	a540      	lbu	v0,0(a1)
     f96:	3480      	sll	a0,8
     f98:	ec4d      	or	a0,v0
     f9a:	6ac5      	li	v0,197
     f9c:	ea4b      	neg	v0
     f9e:	ea8c      	and	v0,a0
     fa0:	f780 b004 	lw	s0,1724 <$L129+0x7e0>
     fa4:	c540      	sb	v0,0(a1)
     fa6:	3242      	srl	v0,8
     fa8:	c541      	sb	v0,1(a1)
     faa:	1521      	b	9ee <$L95>
     fac:	6a00      	li	v0,0
     fae:	f7a0 b01c 	lw	s0,1768 <$L129+0x824>
     fb2:	c040      	sb	v0,0(s0)
     fb4:	c041      	sb	v0,1(s0)
     fb6:	f000 a180 	lbu	a0,0(s1)
     fba:	6a1f      	li	v0,31
     fbc:	ea8c      	and	v0,a0
     fbe:	6c01      	li	a0,1
     fc0:	ec4e      	xor	a0,v0
     fc2:	6528      	move	t1,s0
     fc4:	f280 2411 	beqz	a0,14ea <$L129+0x5a6>
     fc8:	6c02      	li	a0,2
     fca:	ec4e      	xor	a0,v0
     fcc:	f240 2419 	beqz	a0,1482 <$L129+0x53e>
     fd0:	f260 2217 	beqz	v0,14c2 <$L129+0x57e>
     fd4:	f760 b418 	lw	a0,174c <$L129+0x808>
     fd8:	6a80      	li	v0,128
     fda:	a4a4      	lbu	a1,4(a0)
     fdc:	ea4b      	neg	v0
     fde:	eaac      	and	v0,a1
     fe0:	ea11      	zeb	v0
     fe2:	f51f 2202 	beqz	v0,9ea <$L256>
     fe6:	6a01      	li	v0,1
     fe8:	eaad      	or	v0,a1
     fea:	67c9      	move	a2,t1
     fec:	c444      	sb	v0,4(a0)
     fee:	6a02      	li	v0,2
     ff0:	dcc0      	sw	a2,0(a0)
     ff2:	c448      	sb	v0,8(a0)
     ff4:	f720 b010 	lw	s0,1724 <$L129+0x7e0>
     ff8:	14fa      	b	9ee <$L95>
     ffa:	6c02      	li	a0,2
     ffc:	6d00      	li	a1,0
     ffe:	6e00      	li	a2,0
    1000:	d30c      	sw	v1,48(sp)
    1002:	f720 b004 	lw	s0,1724 <$L129+0x7e0>
    1006:	1800 0000 	jal	0 <USBCtrlTrfTxService>
    100a:	d70b      	sw	a3,44(sp)
    100c:	970b      	lw	a3,44(sp)
    100e:	930c      	lw	v1,48(sp)
    1010:	14ee      	b	9ee <$L95>
    1012:	f000 99c0 	lw	a2,0(s1)
    1016:	a640      	lbu	v0,0(a2)
    1018:	a681      	lbu	a0,1(a2)
    101a:	ea11      	zeb	v0
    101c:	ec11      	zeb	a0
    101e:	3480      	sll	a0,8
    1020:	ea8d      	or	v0,a0
    1022:	7280      	cmpi	v0,128
    1024:	6006      	bteqz	1032 <$L129+0xee>
    1026:	a580      	lbu	a0,0(a1)
    1028:	6a03      	li	v0,3
    102a:	ea4b      	neg	v0
    102c:	ea8c      	and	v0,a0
    102e:	c540      	sb	v0,0(a1)
    1030:	149e      	b	96e <$L161+0xc0>
    1032:	f000 9980 	lw	a0,0(s1)
    1036:	a440      	lbu	v0,0(a0)
    1038:	a481      	lbu	a0,1(a0)
    103a:	ea11      	zeb	v0
    103c:	ec11      	zeb	a0
    103e:	3480      	sll	a0,8
    1040:	ea8d      	or	v0,a0
    1042:	7284      	cmpi	v0,132
    1044:	61f0      	btnez	1026 <$L129+0xe2>
    1046:	a640      	lbu	v0,0(a2)
    1048:	6a74      	li	v0,116
    104a:	ea4b      	neg	v0
    104c:	67f8      	move	a3,t8
    104e:	c640      	sb	v0,0(a2)
    1050:	a641      	lbu	v0,1(a2)
    1052:	c6e1      	sb	a3,1(a2)
    1054:	17e8      	b	1026 <$L129+0xe2>
    1056:	6751      	move	v0,s1
    1058:	f000 4a00 	addiu	v0,0
    105c:	f700 b418 	lw	a0,1774 <$L129+0x830>
    1060:	a242      	lbu	v0,2(v0)
    1062:	ea11      	zeb	v0
    1064:	dc40      	sw	v0,0(a0)
    1066:	9c40      	lw	v0,0(a0)
    1068:	f0c0 2218 	beqz	v0,121c <$L129+0x2d8>
    106c:	6a10      	li	v0,16
    106e:	f000 d940 	sw	v0,0(s1)
    1072:	141d      	b	8ae <$L161>
    1074:	f000 9940 	lw	v0,0(s1)
    1078:	a282      	lbu	a0,2(v0)
    107a:	6798      	move	a0,t8
    107c:	c282      	sb	a0,2(v0)
    107e:	a2a3      	lbu	a1,3(v0)
    1080:	6c04      	li	a0,4
    1082:	ec8b      	neg	a0
    1084:	ecac      	and	a0,a1
    1086:	67b8      	move	a1,t8
    1088:	c283      	sb	a0,3(v0)
    108a:	a280      	lbu	a0,0(v0)
    108c:	6c38      	li	a0,56
    108e:	ec8b      	neg	a0
    1090:	c280      	sb	a0,0(v0)
    1092:	a281      	lbu	a0,1(v0)
    1094:	c2a1      	sb	a1,1(v0)
    1096:	6522      	move	at,v0
    1098:	b202      	lw	v0,10a0 <$L129+0x15c>
    109a:	ea00      	jr	v0
    109c:	6741      	move	v0,at
    109e:	6500      	nop
    10a0:	0000      	addiu	s0,sp,0
    10a2:	0000      	addiu	s0,sp,0
    10a4:	9a40      	lw	v0,0(v0)
    10a6:	f6a0 b70c 	lw	a3,1750 <$L129+0x80c>
    10aa:	3258      	sll	v0,6
    10ac:	f580 3242 	srl	v0,22
    10b0:	a786      	lbu	a0,6(a3)
    10b2:	ea11      	zeb	v0
    10b4:	a7a5      	lbu	a1,5(a3)
    10b6:	67c2      	move	a2,v0
    10b8:	3480      	sll	a0,8
    10ba:	ee31      	zeh	a2
    10bc:	ecad      	or	a0,a1
    10be:	ecc3      	sltu	a0,a2
    10c0:	6003      	bteqz	10c8 <$L129+0x184>
    10c2:	6744      	move	v0,a0
    10c4:	ea11      	zeb	v0
    10c6:	67c2      	move	a2,v0
    10c8:	e4d3      	subu	a0,a2
    10ca:	ec31      	zeh	a0
    10cc:	c785      	sb	a0,5(a3)
    10ce:	3482      	srl	a0,8
    10d0:	c786      	sb	a0,6(a3)
    10d2:	2227      	beqz	v0,1122 <$L129+0x1de>
    10d4:	4aff      	addiu	v0,-1
    10d6:	ea11      	zeb	v0
    10d8:	4a01      	addiu	v0,1
    10da:	650a      	move	t0,v0
    10dc:	f680 b20c 	lw	v0,1768 <$L129+0x824>
    10e0:	6800      	li	s0,0
    10e2:	652a      	move	t1,v0
    10e4:	a742      	lbu	v0,2(a3)
    10e6:	a781      	lbu	a0,1(a3)
    10e8:	3240      	sll	v0,8
    10ea:	a7c0      	lbu	a2,0(a3)
    10ec:	3480      	sll	a0,8
    10ee:	3240      	sll	v0,8
    10f0:	67a4      	move	a1,a0
    10f2:	6782      	move	a0,v0
    10f4:	a743      	lbu	v0,3(a3)
    10f6:	edcd      	or	a1,a2
    10f8:	ecad      	or	a0,a1
    10fa:	f600 3240 	sll	v0,24
    10fe:	ea8d      	or	v0,a0
    1100:	4281      	addiu	a0,v0,1
    1102:	3582      	srl	a1,a0,8
    1104:	c780      	sb	a0,0(a3)
    1106:	c7a1      	sb	a1,1(a3)
    1108:	35a2      	srl	a1,8
    110a:	f600 3482 	srl	a0,24
    110e:	c7a2      	sb	a1,2(a3)
    1110:	67a9      	move	a1,t1
    1112:	c783      	sb	a0,3(a3)
    1114:	e511      	addu	a0,a1,s0
    1116:	67c8      	move	a2,t0
    1118:	a480      	lbu	a0,0(a0)
    111a:	4801      	addiu	s0,1
    111c:	e8ca      	cmp	s0,a2
    111e:	c280      	sb	a0,0(v0)
    1120:	61e1      	btnez	10e4 <$L129+0x1a0>
    1122:	a746      	lbu	v0,6(a3)
    1124:	a785      	lbu	a0,5(a3)
    1126:	3240      	sll	v0,8
    1128:	f620 b008 	lw	s0,1750 <$L129+0x80c>
    112c:	ea8d      	or	v0,a0
    112e:	6518      	move	t8,s0
    1130:	650a      	move	t0,v0
    1132:	f0a0 2218 	beqz	v0,12a6 <$L129+0x362>
    1136:	f000 9940 	lw	v0,0(s1)
    113a:	f620 b710 	lw	a3,1768 <$L129+0x824>
    113e:	a282      	lbu	a0,2(v0)
    1140:	6c40      	li	a0,64
    1142:	c282      	sb	a0,2(v0)
    1144:	a2a3      	lbu	a1,3(v0)
    1146:	4cbc      	addiu	a0,-68
    1148:	ecac      	and	a0,a1
    114a:	6dff      	li	a1,255
    114c:	c283      	sb	a0,3(v0)
    114e:	f5e0 b410 	lw	a0,173c <$L129+0x7f8>
    1152:	ecec      	and	a0,a3
    1154:	67c4      	move	a2,a0
    1156:	eeac      	and	a2,a1
    1158:	a2e4      	lbu	a3,4(v0)
    115a:	c2c4      	sb	a2,4(v0)
    115c:	3682      	srl	a2,a0,8
    115e:	67e6      	move	a3,a2
    1160:	efac      	and	a3,a1
    1162:	36c2      	srl	a2,8
    1164:	edcc      	and	a1,a2
    1166:	a205      	lbu	s0,5(v0)
    1168:	f600 3482 	srl	a0,24
    116c:	c2e5      	sb	a3,5(v0)
    116e:	f000 9900 	lw	s0,0(s1)
    1172:	a2c6      	lbu	a2,6(v0)
    1174:	c2a6      	sb	a1,6(v0)
    1176:	a2a7      	lbu	a1,7(v0)
    1178:	c287      	sb	a0,7(v0)
    117a:	a080      	lbu	a0,0(s0)
    117c:	6d01      	li	a1,1
    117e:	ec11      	zeb	a0
    1180:	349a      	srl	a0,6
    1182:	ecac      	and	a0,a1
    1184:	f140 240d 	beqz	a0,1422 <$L129+0x4de>
    1188:	a280      	lbu	a0,0(v0)
    118a:	6c78      	li	a0,120
    118c:	ec8b      	neg	a0
    118e:	c280      	sb	a0,0(v0)
    1190:	a281      	lbu	a0,1(v0)
    1192:	6c00      	li	a0,0
    1194:	c281      	sb	a0,1(v0)
    1196:	6522      	move	at,v0
    1198:	b202      	lw	v0,11a0 <$L129+0x25c>
    119a:	ea00      	jr	v0
    119c:	6741      	move	v0,at
    119e:	6500      	nop
    11a0:	0000      	addiu	s0,sp,0
    11a2:	0000      	addiu	s0,sp,0
    11a4:	f5c0 b504 	lw	a1,1768 <$L129+0x824>
    11a8:	652d      	move	t1,a1
    11aa:	f580 b514 	lw	a1,173c <$L129+0x7f8>
    11ae:	67c9      	move	a2,t1
    11b0:	edcc      	and	a1,a2
    11b2:	f000 9980 	lw	a0,0(s1)
    11b6:	6eff      	li	a2,255
    11b8:	67e5      	move	a3,a1
    11ba:	efcc      	and	a3,a2
    11bc:	650f      	move	t0,a3
    11be:	a4e4      	lbu	a3,4(a0)
    11c0:	67e8      	move	a3,t0
    11c2:	c4e4      	sb	a3,4(a0)
    11c4:	37a2      	srl	a3,a1,8
    11c6:	652f      	move	t1,a3
    11c8:	efcc      	and	a3,a2
    11ca:	651f      	move	t8,a3
    11cc:	a4e5      	lbu	a3,5(a0)
    11ce:	67f8      	move	a3,t8
    11d0:	f600 35a2 	srl	a1,24
    11d4:	c4e5      	sb	a3,5(a0)
    11d6:	67e9      	move	a3,t1
    11d8:	37e2      	srl	a3,8
    11da:	eeec      	and	a2,a3
    11dc:	a4e6      	lbu	a3,6(a0)
    11de:	c4c6      	sb	a2,6(a0)
    11e0:	a4c7      	lbu	a2,7(a0)
    11e2:	c4a7      	sb	a1,7(a0)
    11e4:	d20a      	sw	v0,40(sp)
    11e6:	1800 0000 	jal	0 <USBCtrlTrfTxService>
    11ea:	d30c      	sw	v1,48(sp)
    11ec:	f000 a180 	lbu	a0,0(s1)
    11f0:	920a      	lw	v0,40(sp)
    11f2:	7402      	cmpi	a0,2
    11f4:	930c      	lw	v1,48(sp)
    11f6:	f180 601b 	bteqz	1530 <$L129+0x5ec>
    11fa:	f100 281f 	bnez	s0,143c <$L129+0x4f8>
    11fe:	f000 9940 	lw	v0,0(s1)
    1202:	a280      	lbu	a0,0(v0)
    1204:	6c38      	li	a0,56
    1206:	ec8b      	neg	a0
    1208:	c280      	sb	a0,0(v0)
    120a:	a281      	lbu	a0,1(v0)
    120c:	c201      	sb	s0,1(v0)
    120e:	6522      	move	at,v0
    1210:	b202      	lw	v0,1218 <$L129+0x2d4>
    1212:	ea00      	jr	v0
    1214:	6741      	move	v0,at
    1216:	6500      	nop
    1218:	0000      	addiu	s0,sp,0
    121a:	0000      	addiu	s0,sp,0
    121c:	6a04      	li	v0,4
    121e:	f000 d940 	sw	v0,0(s1)
    1222:	6522      	move	at,v0
    1224:	b202      	lw	v0,122c <$L129+0x2e8>
    1226:	ea00      	jr	v0
    1228:	6741      	move	v0,at
    122a:	6500      	nop
    122c:	0000      	addiu	s0,sp,0
    122e:	0000      	addiu	s0,sp,0
    1230:	9707      	lw	a3,28(sp)
    1232:	f000 d940 	sw	v0,0(s1)
    1236:	f000 d940 	sw	v0,0(s1)
    123a:	a746      	lbu	v0,6(a3)
    123c:	a787      	lbu	a0,7(a3)
    123e:	ea11      	zeb	v0
    1240:	ec11      	zeb	a0
    1242:	3480      	sll	a0,8
    1244:	ea8d      	or	v0,a0
    1246:	ad84      	lhu	a0,8(a1)
    1248:	ea83      	sltu	v0,a0
    124a:	6007      	bteqz	125a <$L129+0x316>
    124c:	a746      	lbu	v0,6(a3)
    124e:	a787      	lbu	a0,7(a3)
    1250:	ea11      	zeb	v0
    1252:	ec11      	zeb	a0
    1254:	3480      	sll	a0,8
    1256:	ea8d      	or	v0,a0
    1258:	cd44      	sh	v0,8(a1)
    125a:	f500 b010 	lw	s0,1768 <$L129+0x824>
    125e:	1800 0000 	jal	0 <USBCtrlTrfTxService>
    1262:	d30c      	sw	v1,48(sp)
    1264:	f4c0 b718 	lw	a3,173c <$L129+0x7f8>
    1268:	6750      	move	v0,s0
    126a:	eaec      	and	v0,a3
    126c:	f000 9980 	lw	a0,0(s1)
    1270:	6dff      	li	a1,255
    1272:	67c2      	move	a2,v0
    1274:	eeac      	and	a2,a1
    1276:	a4e4      	lbu	a3,4(a0)
    1278:	c4c4      	sb	a2,4(a0)
    127a:	3642      	srl	a2,v0,8
    127c:	67e6      	move	a3,a2
    127e:	efac      	and	a3,a1
    1280:	36c2      	srl	a2,8
    1282:	edcc      	and	a1,a2
    1284:	a405      	lbu	s0,5(a0)
    1286:	f600 3242 	srl	v0,24
    128a:	c4e5      	sb	a3,5(a0)
    128c:	a4c6      	lbu	a2,6(a0)
    128e:	c4a6      	sb	a1,6(a0)
    1290:	a4a7      	lbu	a1,7(a0)
    1292:	c447      	sb	v0,7(a0)
    1294:	a440      	lbu	v0,0(a0)
    1296:	6a38      	li	v0,56
    1298:	ea4b      	neg	v0
    129a:	c440      	sb	v0,0(a0)
    129c:	a441      	lbu	v0,1(a0)
    129e:	6a00      	li	v0,0
    12a0:	c441      	sb	v0,1(a0)
    12a2:	930c      	lw	v1,48(sp)
    12a4:	14ef      	b	c84 <$L95+0x296>
    12a6:	f000 99c0 	lw	a2,0(s1)
    12aa:	6711      	move	s0,s1
    12ac:	f000 4800 	addiu	s0,0
    12b0:	a642      	lbu	v0,2(a2)
    12b2:	6a40      	li	v0,64
    12b4:	c642      	sb	v0,2(a2)
    12b6:	a683      	lbu	a0,3(a2)
    12b8:	4abc      	addiu	v0,-68
    12ba:	ea8c      	and	v0,a0
    12bc:	6cff      	li	a0,255
    12be:	c643      	sb	v0,3(a2)
    12c0:	f460 b21c 	lw	v0,173c <$L129+0x7f8>
    12c4:	a6a4      	lbu	a1,4(a2)
    12c6:	e84c      	and	s0,v0
    12c8:	6750      	move	v0,s0
    12ca:	ea8c      	and	v0,a0
    12cc:	c644      	sb	v0,4(a2)
    12ce:	3202      	srl	v0,s0,8
    12d0:	a6a5      	lbu	a1,5(a2)
    12d2:	d208      	sw	v0,32(sp)
    12d4:	ea8c      	and	v0,a0
    12d6:	c645      	sb	v0,5(a2)
    12d8:	9208      	lw	v0,32(sp)
    12da:	a6a6      	lbu	a1,6(a2)
    12dc:	3242      	srl	v0,8
    12de:	ec4c      	and	a0,v0
    12e0:	d207      	sw	v0,28(sp)
    12e2:	c686      	sb	a0,6(a2)
    12e4:	f600 3402 	srl	a0,s0,24
    12e8:	a6a7      	lbu	a1,7(a2)
    12ea:	c687      	sb	a0,7(a2)
    12ec:	a6a0      	lbu	a1,0(a2)
    12ee:	6d7c      	li	a1,124
    12f0:	edab      	neg	a1
    12f2:	6758      	move	v0,t8
    12f4:	c6a0      	sb	a1,0(a2)
    12f6:	a6a1      	lbu	a1,1(a2)
    12f8:	67a8      	move	a1,t0
    12fa:	c6a1      	sb	a1,1(a2)
    12fc:	a2a8      	lbu	a1,8(v0)
    12fe:	a2c7      	lbu	a2,7(v0)
    1300:	32a0      	sll	v0,a1,8
    1302:	ee4d      	or	a2,v0
    1304:	6758      	move	v0,t8
    1306:	67b8      	move	a1,t8
    1308:	a249      	lbu	v0,9(v0)
    130a:	a5aa      	lbu	a1,10(a1)
    130c:	3240      	sll	v0,8
    130e:	3240      	sll	v0,8
    1310:	ee4d      	or	a2,v0
    1312:	f600 35a0 	sll	a1,24
    1316:	eead      	or	a2,a1
    1318:	2607      	beqz	a2,1328 <$L129+0x3e4>
    131a:	d30c      	sw	v1,48(sp)
    131c:	d40a      	sw	a0,40(sp)
    131e:	ee40      	jalr	a2
    1320:	d70b      	sw	a3,44(sp)
    1322:	970b      	lw	a3,44(sp)
    1324:	940a      	lw	a0,40(sp)
    1326:	930c      	lw	v1,48(sp)
    1328:	a7a4      	lbu	a1,4(a3)
    132a:	6e7f      	li	a2,127
    132c:	eeac      	and	a2,a1
    132e:	c7c4      	sb	a2,4(a3)
    1330:	f000 99c0 	lw	a2,0(s1)
    1334:	f29f 2e0a 	bnez	a2,84c <$L142>
    1338:	f000 99c0 	lw	a2,0(s1)
    133c:	f29f 2e06 	bnez	a2,84c <$L142>
    1340:	6e01      	li	a2,1
    1342:	652e      	move	t1,a2
    1344:	f000 d9c0 	sw	a2,0(s1)
    1348:	f000 99c0 	lw	a2,0(s1)
    134c:	f27f 2e1e 	bnez	a2,84c <$L142>
    1350:	67e9      	move	a3,t1
    1352:	6e02      	li	a2,2
    1354:	f000 d9e0 	sw	a3,0(s1)
    1358:	f000 a1e0 	lbu	a3,0(s1)
    135c:	eeee      	xor	a2,a3
    135e:	f160 261b 	beqz	a2,1658 <$L129+0x714>
    1362:	6a01      	li	v0,1
    1364:	ef4e      	xor	a3,v0
    1366:	f27f 2f11 	bnez	a3,84c <$L142>
    136a:	f000 99c0 	lw	a2,0(s1)
    136e:	6a04      	li	v0,4
    1370:	ea4b      	neg	v0
    1372:	a6a2      	lbu	a1,2(a2)
    1374:	6d40      	li	a1,64
    1376:	654d      	move	t2,a1
    1378:	c6a2      	sb	a1,2(a2)
    137a:	a6a3      	lbu	a1,3(a2)
    137c:	650a      	move	t0,v0
    137e:	651d      	move	t8,a1
    1380:	67b8      	move	a1,t8
    1382:	ed4c      	and	a1,v0
    1384:	c6a3      	sb	a1,3(a2)
    1386:	6dff      	li	a1,255
    1388:	e8ac      	and	s0,a1
    138a:	6568      	move	t3,s0
    138c:	a604      	lbu	s0,4(a2)
    138e:	670b      	move	s0,t3
    1390:	c604      	sb	s0,4(a2)
    1392:	a645      	lbu	v0,5(a2)
    1394:	9207      	lw	v0,28(sp)
    1396:	9008      	lw	s0,32(sp)
    1398:	eaac      	and	v0,a1
    139a:	e8ac      	and	s0,a1
    139c:	658a      	move	t4,v0
    139e:	67ac      	move	a1,t4
    13a0:	c605      	sb	s0,5(a2)
    13a2:	a646      	lbu	v0,6(a2)
    13a4:	c6a6      	sb	a1,6(a2)
    13a6:	a647      	lbu	v0,7(a2)
    13a8:	c687      	sb	a0,7(a2)
    13aa:	a640      	lbu	v0,0(a2)
    13ac:	6a7c      	li	v0,124
    13ae:	ea4b      	neg	v0
    13b0:	67aa      	move	a1,t2
    13b2:	c640      	sb	v0,0(a2)
    13b4:	a641      	lbu	v0,1(a2)
    13b6:	c6e1      	sb	a3,1(a2)
    13b8:	f000 9940 	lw	v0,0(s1)
    13bc:	67c9      	move	a2,t1
    13be:	f000 d9c0 	sw	a2,0(s1)
    13c2:	a2c2      	lbu	a2,2(v0)
    13c4:	c2a2      	sb	a1,2(v0)
    13c6:	a2c3      	lbu	a2,3(v0)
    13c8:	651e      	move	t8,a2
    13ca:	67b8      	move	a1,t8
    13cc:	67c8      	move	a2,t0
    13ce:	edcc      	and	a1,a2
    13d0:	c2a3      	sb	a1,3(v0)
    13d2:	67ab      	move	a1,t3
    13d4:	a2c4      	lbu	a2,4(v0)
    13d6:	c2a4      	sb	a1,4(v0)
    13d8:	a2c5      	lbu	a2,5(v0)
    13da:	c205      	sb	s0,5(v0)
    13dc:	a2c6      	lbu	a2,6(v0)
    13de:	67cc      	move	a2,t4
    13e0:	c2c6      	sb	a2,6(v0)
    13e2:	a2a7      	lbu	a1,7(v0)
    13e4:	c287      	sb	a0,7(v0)
    13e6:	14ac      	b	d40 <$L95+0x352>
    13e8:	67b1      	move	a1,s1
    13ea:	6c01      	li	a0,1
    13ec:	f000 4d00 	addiu	a1,0
    13f0:	6e01      	li	a2,1
    13f2:	d30c      	sw	v1,48(sp)
    13f4:	1800 0000 	jal	0 <USBCtrlTrfTxService>
    13f8:	d70b      	sw	a3,44(sp)
    13fa:	6a20      	li	v0,32
    13fc:	930c      	lw	v1,48(sp)
    13fe:	f000 d940 	sw	v0,0(s1)
    1402:	970b      	lw	a3,44(sp)
    1404:	6522      	move	at,v0
    1406:	b202      	lw	v0,140c <$L129+0x4c8>
    1408:	ea00      	jr	v0
    140a:	6741      	move	v0,at
    140c:	0000      	addiu	s0,sp,0
    140e:	0000      	addiu	s0,sp,0
    1410:	f000 9940 	lw	v0,0(s1)
    1414:	a2a2      	lbu	a1,2(v0)
    1416:	c282      	sb	a0,2(v0)
    1418:	a2c3      	lbu	a2,3(v0)
    141a:	6d04      	li	a1,4
    141c:	edab      	neg	a1
    141e:	edcc      	and	a1,a2
    1420:	c2a3      	sb	a1,3(v0)
    1422:	a2a0      	lbu	a1,0(v0)
    1424:	6d38      	li	a1,56
    1426:	edab      	neg	a1
    1428:	c2a0      	sb	a1,0(v0)
    142a:	a2a1      	lbu	a1,1(v0)
    142c:	c281      	sb	a0,1(v0)
    142e:	6522      	move	at,v0
    1430:	b202      	lw	v0,1438 <$L129+0x4f4>
    1432:	ea00      	jr	v0
    1434:	6741      	move	v0,at
    1436:	6500      	nop
    1438:	0000      	addiu	s0,sp,0
    143a:	0000      	addiu	s0,sp,0
    143c:	f000 9980 	lw	a0,0(s1)
    1440:	a4a0      	lbu	a1,0(a0)
    1442:	6d78      	li	a1,120
    1444:	edab      	neg	a1
    1446:	c4a0      	sb	a1,0(a0)
    1448:	a4a1      	lbu	a1,1(a0)
    144a:	c441      	sb	v0,1(a0)
    144c:	6522      	move	at,v0
    144e:	b202      	lw	v0,1454 <$L129+0x510>
    1450:	ea00      	jr	v0
    1452:	6741      	move	v0,at
    1454:	0000      	addiu	s0,sp,0
    1456:	0000      	addiu	s0,sp,0
    1458:	f000 a180 	lbu	a0,0(s1)
    145c:	6a1f      	li	v0,31
    145e:	ea8c      	and	v0,a0
    1460:	f51f 2a16 	bnez	v0,e90 <$L95+0x4a2>
    1464:	b4ba      	lw	a0,174c <$L129+0x808>
    1466:	a4a4      	lbu	a1,4(a0)
    1468:	6c80      	li	a0,128
    146a:	ec8b      	neg	a0
    146c:	ecad      	or	a0,a1
    146e:	b5b8      	lw	a1,174c <$L129+0x808>
    1470:	c584      	sb	a0,4(a1)
    1472:	a081      	lbu	a0,1(s0)
    1474:	ec11      	zeb	a0
    1476:	7403      	cmpi	a0,3
    1478:	f0a0 6004 	bteqz	15c4 <$L129+0x680>
    147c:	f000 d940 	sw	v0,0(s1)
    1480:	1507      	b	e90 <$L95+0x4a2>
    1482:	b0b3      	lw	s0,174c <$L129+0x808>
    1484:	6a80      	li	v0,128
    1486:	a084      	lbu	a0,4(s0)
    1488:	ea4b      	neg	v0
    148a:	ea8d      	or	v0,a0
    148c:	c044      	sb	v0,4(s0)
    148e:	9007      	lw	s0,28(sp)
    1490:	a044      	lbu	v0,4(s0)
    1492:	a084      	lbu	a0,4(s0)
    1494:	ea11      	zeb	v0
    1496:	325e      	srl	v0,7
    1498:	2a2f      	bnez	v0,14f8 <$L129+0x5b4>
    149a:	6a0f      	li	v0,15
    149c:	ea8c      	and	v0,a0
    149e:	6791      	move	a0,s1
    14a0:	f000 4c00 	addiu	a0,0
    14a4:	3248      	sll	v0,2
    14a6:	e289      	addu	v0,a0
    14a8:	9a80      	lw	a0,0(v0)
    14aa:	a440      	lbu	v0,0(a0)
    14ac:	a481      	lbu	a0,1(a0)
    14ae:	3480      	sll	a0,8
    14b0:	ec4d      	or	a0,v0
    14b2:	6a04      	li	v0,4
    14b4:	ea8c      	and	v0,a0
    14b6:	f59f 220d 	beqz	v0,fd4 <$L129+0x90>
    14ba:	6a01      	li	v0,1
    14bc:	6789      	move	a0,t1
    14be:	c440      	sb	v0,0(a0)
    14c0:	1589      	b	fd4 <$L129+0x90>
    14c2:	b2a3      	lw	v0,174c <$L129+0x808>
    14c4:	6d01      	li	a1,1
    14c6:	a284      	lbu	a0,4(v0)
    14c8:	6a80      	li	v0,128
    14ca:	ea4b      	neg	v0
    14cc:	ea8d      	or	v0,a0
    14ce:	b4a0      	lw	a0,174c <$L129+0x808>
    14d0:	c444      	sb	v0,4(a0)
    14d2:	f000 9940 	lw	v0,0(s1)
    14d6:	eaae      	xor	v0,a1
    14d8:	f57f 2a1c 	bnez	v0,fd4 <$L129+0x90>
    14dc:	a040      	lbu	v0,0(s0)
    14de:	6782      	move	a0,v0
    14e0:	ec11      	zeb	a0
    14e2:	6a02      	li	v0,2
    14e4:	ea8d      	or	v0,a0
    14e6:	c040      	sb	v0,0(s0)
    14e8:	1575      	b	fd4 <$L129+0x90>
    14ea:	b699      	lw	a2,174c <$L129+0x808>
    14ec:	6a80      	li	v0,128
    14ee:	a684      	lbu	a0,4(a2)
    14f0:	ea4b      	neg	v0
    14f2:	ea8d      	or	v0,a0
    14f4:	c644      	sb	v0,4(a2)
    14f6:	156e      	b	fd4 <$L129+0x90>
    14f8:	6a0f      	li	v0,15
    14fa:	ea8c      	and	v0,a0
    14fc:	6791      	move	a0,s1
    14fe:	f000 4c00 	addiu	a0,0
    1502:	3248      	sll	v0,2
    1504:	e289      	addu	v0,a0
    1506:	9a80      	lw	a0,0(v0)
    1508:	17d0      	b	14aa <$L129+0x566>
    150a:	f000 9940 	lw	v0,0(s1)
    150e:	a282      	lbu	a0,2(v0)
    1510:	c2e2      	sb	a3,2(v0)
    1512:	a2c3      	lbu	a2,3(v0)
    1514:	ee0c      	and	a2,s0
    1516:	c2c3      	sb	a2,3(v0)
    1518:	a280      	lbu	a0,0(v0)
    151a:	6c38      	li	a0,56
    151c:	ec8b      	neg	a0
    151e:	c280      	sb	a0,0(v0)
    1520:	a281      	lbu	a0,1(v0)
    1522:	c2e1      	sb	a3,1(v0)
    1524:	6522      	move	at,v0
    1526:	b202      	lw	v0,152c <$L129+0x5e8>
    1528:	ea00      	jr	v0
    152a:	6741      	move	v0,at
    152c:	0000      	addiu	s0,sp,0
    152e:	0000      	addiu	s0,sp,0
    1530:	f000 9940 	lw	v0,0(s1)
    1534:	6718      	move	s0,t8
    1536:	a280      	lbu	a0,0(v0)
    1538:	6c7c      	li	a0,124
    153a:	ec8b      	neg	a0
    153c:	c280      	sb	a0,0(v0)
    153e:	a281      	lbu	a0,1(v0)
    1540:	c201      	sb	s0,1(v0)
    1542:	6522      	move	at,v0
    1544:	b202      	lw	v0,154c <$L129+0x608>
    1546:	ea00      	jr	v0
    1548:	6741      	move	v0,at
    154a:	6500      	nop
    154c:	0000      	addiu	s0,sp,0
    154e:	0000      	addiu	s0,sp,0
    1550:	9007      	lw	s0,28(sp)
    1552:	a082      	lbu	a0,2(s0)
    1554:	ec11      	zeb	a0
    1556:	5c03      	sltiu	a0,3
    1558:	6023      	bteqz	15a0 <$L129+0x65c>
    155a:	a082      	lbu	a0,2(s0)
    155c:	b587      	lw	a1,1778 <$L129+0x834>
    155e:	ec11      	zeb	a0
    1560:	3488      	sll	a0,2
    1562:	e4b1      	addu	a0,a1
    1564:	b070      	lw	s0,1724 <$L129+0x7e0>
    1566:	9c80      	lw	a0,0(a0)
    1568:	da80      	sw	a0,0(v0)
    156a:	a480      	lbu	a0,0(a0)
    156c:	ca84      	sh	a0,8(v0)
    156e:	6522      	move	at,v0
    1570:	b202      	lw	v0,1578 <$L129+0x634>
    1572:	ea00      	jr	v0
    1574:	6741      	move	v0,at
    1576:	6500      	nop
    1578:	0000      	addiu	s0,sp,0
    157a:	0000      	addiu	s0,sp,0
    157c:	a082      	lbu	a0,2(s0)
    157e:	b580      	lw	a1,177c <$L129+0x838>
    1580:	ec11      	zeb	a0
    1582:	3488      	sll	a0,2
    1584:	e4b1      	addu	a0,a1
    1586:	b068      	lw	s0,1724 <$L129+0x7e0>
    1588:	9c80      	lw	a0,0(a0)
    158a:	da80      	sw	a0,0(v0)
    158c:	a4a2      	lbu	a1,2(a0)
    158e:	c2a8      	sb	a1,8(v0)
    1590:	a483      	lbu	a0,3(a0)
    1592:	c289      	sb	a0,9(v0)
    1594:	6522      	move	at,v0
    1596:	b202      	lw	v0,159c <$L129+0x658>
    1598:	ea00      	jr	v0
    159a:	6741      	move	v0,at
    159c:	0000      	addiu	s0,sp,0
    159e:	0000      	addiu	s0,sp,0
    15a0:	c2c4      	sb	a2,4(v0)
    15a2:	b061      	lw	s0,1724 <$L129+0x7e0>
    15a4:	6522      	move	at,v0
    15a6:	b202      	lw	v0,15ac <$L129+0x668>
    15a8:	ea00      	jr	v0
    15aa:	6741      	move	v0,at
    15ac:	0000      	addiu	s0,sp,0
    15ae:	0000      	addiu	s0,sp,0
    15b0:	b474      	lw	a0,1780 <$L129+0x83c>
    15b2:	da80      	sw	a0,0(v0)
    15b4:	6c12      	li	a0,18
    15b6:	ca84      	sh	a0,8(v0)
    15b8:	6522      	move	at,v0
    15ba:	b202      	lw	v0,15c0 <$L129+0x67c>
    15bc:	ea00      	jr	v0
    15be:	6741      	move	v0,at
    15c0:	0000      	addiu	s0,sp,0
    15c2:	0000      	addiu	s0,sp,0
    15c4:	6a01      	li	v0,1
    15c6:	f000 d940 	sw	v0,0(s1)
    15ca:	1462      	b	e90 <$L95+0x4a2>
    15cc:	9007      	lw	s0,28(sp)
    15ce:	a044      	lbu	v0,4(s0)
    15d0:	ea11      	zeb	v0
    15d2:	325e      	srl	v0,7
    15d4:	2a39      	bnez	v0,1648 <$L129+0x704>
    15d6:	a084      	lbu	a0,4(s0)
    15d8:	6a0f      	li	v0,15
    15da:	ea8c      	and	v0,a0
    15dc:	6791      	move	a0,s1
    15de:	f000 4c00 	addiu	a0,0
    15e2:	e289      	addu	v0,a0
    15e4:	6c03      	li	a0,3
    15e6:	a2c0      	lbu	a2,0(v0)
    15e8:	ec8b      	neg	a0
    15ea:	eccc      	and	a0,a2
    15ec:	6e04      	li	a2,4
    15ee:	c280      	sb	a0,0(v0)
    15f0:	a581      	lbu	a0,1(a1)
    15f2:	a540      	lbu	v0,0(a1)
    15f4:	3480      	sll	a0,8
    15f6:	ec4d      	or	a0,v0
    15f8:	6ac5      	li	v0,197
    15fa:	ea4b      	neg	v0
    15fc:	ea8c      	and	v0,a0
    15fe:	6c05      	li	a0,5
    1600:	c540      	sb	v0,0(a1)
    1602:	3242      	srl	v0,8
    1604:	b048      	lw	s0,1724 <$L129+0x7e0>
    1606:	c541      	sb	v0,1(a1)
    1608:	d30c      	sw	v1,48(sp)
    160a:	1800 0000 	jal	0 <USBCtrlTrfTxService>
    160e:	d70b      	sw	a3,44(sp)
    1610:	930c      	lw	v1,48(sp)
    1612:	970b      	lw	a3,44(sp)
    1614:	6522      	move	at,v0
    1616:	b202      	lw	v0,161c <$L129+0x6d8>
    1618:	ea00      	jr	v0
    161a:	6741      	move	v0,at
    161c:	0000      	addiu	s0,sp,0
    161e:	0000      	addiu	s0,sp,0
    1620:	9007      	lw	s0,28(sp)
    1622:	67b1      	move	a1,s1
    1624:	f000 4d00 	addiu	a1,0
    1628:	a084      	lbu	a0,4(s0)
    162a:	ec4c      	and	a0,v0
    162c:	ec11      	zeb	a0
    162e:	3488      	sll	a0,2
    1630:	e4b1      	addu	a0,a1
    1632:	a0a4      	lbu	a1,4(s0)
    1634:	eaac      	and	v0,a1
    1636:	67b1      	move	a1,s1
    1638:	ea11      	zeb	v0
    163a:	f000 4d00 	addiu	a1,0
    163e:	e2a9      	addu	v0,a1
    1640:	a240      	lbu	v0,0(v0)
    1642:	d209      	sw	v0,36(sp)
    1644:	9c40      	lw	v0,0(a0)
    1646:	1461      	b	f0a <$L95+0x51c>
    1648:	9007      	lw	s0,28(sp)
    164a:	6a0f      	li	v0,15
    164c:	a084      	lbu	a0,4(s0)
    164e:	ea8c      	and	v0,a0
    1650:	6791      	move	a0,s1
    1652:	f000 4c00 	addiu	a0,0
    1656:	17c5      	b	15e2 <$L129+0x69e>
    1658:	f000 9940 	lw	v0,0(s1)
    165c:	a282      	lbu	a0,2(v0)
    165e:	c2c2      	sb	a2,2(v0)
    1660:	a2a3      	lbu	a1,3(v0)
    1662:	6c04      	li	a0,4
    1664:	ec8b      	neg	a0
    1666:	ecac      	and	a0,a1
    1668:	c283      	sb	a0,3(v0)
    166a:	a280      	lbu	a0,0(v0)
    166c:	6c38      	li	a0,56
    166e:	ec8b      	neg	a0
    1670:	c280      	sb	a0,0(v0)
    1672:	a281      	lbu	a0,1(v0)
    1674:	c2c1      	sb	a2,1(v0)
    1676:	6522      	move	at,v0
    1678:	b202      	lw	v0,1680 <$L129+0x73c>
    167a:	ea00      	jr	v0
    167c:	6741      	move	v0,at
    167e:	6500      	nop
    1680:	0000      	addiu	s0,sp,0
    1682:	0000      	addiu	s0,sp,0
    1684:	b424      	lw	a0,1714 <$L129+0x7d0>
    1686:	6522      	move	at,v0
    1688:	b202      	lw	v0,1690 <$L129+0x74c>
    168a:	ea00      	jr	v0
    168c:	6741      	move	v0,at
    168e:	6500      	nop
    1690:	0000      	addiu	s0,sp,0
    1692:	0000      	addiu	s0,sp,0
    1694:	a581      	lbu	a0,1(a1)
    1696:	a540      	lbu	v0,0(a1)
    1698:	3480      	sll	a0,8
    169a:	ec4d      	or	a0,v0
    169c:	6a81      	li	v0,129
    169e:	ea4b      	neg	v0
    16a0:	ec4c      	and	a0,v0
    16a2:	6a40      	li	v0,64
    16a4:	ea8d      	or	v0,a0
    16a6:	6e04      	li	a2,4
    16a8:	c540      	sb	v0,0(a1)
    16aa:	3242      	srl	v0,8
    16ac:	6c05      	li	a0,5
    16ae:	c541      	sb	v0,1(a1)
    16b0:	d30c      	sw	v1,48(sp)
    16b2:	1800 0000 	jal	0 <USBCtrlTrfTxService>
    16b6:	d70b      	sw	a3,44(sp)
    16b8:	930c      	lw	v1,48(sp)
    16ba:	970b      	lw	a3,44(sp)
    16bc:	145b      	b	f74 <$L129+0x30>
    16be:	9007      	lw	s0,28(sp)
    16c0:	6a0f      	li	v0,15
    16c2:	a084      	lbu	a0,4(s0)
    16c4:	ea8c      	and	v0,a0
    16c6:	6791      	move	a0,s1
    16c8:	f000 4c00 	addiu	a0,0
    16cc:	6522      	move	at,v0
    16ce:	b202      	lw	v0,16d4 <$L129+0x790>
    16d0:	ea00      	jr	v0
    16d2:	6741      	move	v0,at
    16d4:	0000      	addiu	s0,sp,0
    16d6:	0000      	addiu	s0,sp,0
    16d8:	9507      	lw	a1,28(sp)
    16da:	6a0f      	li	v0,15
    16dc:	a584      	lbu	a0,4(a1)
    16de:	9504      	lw	a1,16(sp)
    16e0:	ea8c      	and	v0,a0
    16e2:	6791      	move	a0,s1
    16e4:	3248      	sll	v0,2
    16e6:	f000 4c00 	addiu	a0,0
    16ea:	e289      	addu	v0,a0
    16ec:	daa0      	sw	a1,0(v0)
    16ee:	6522      	move	at,v0
    16f0:	b202      	lw	v0,16f8 <$L129+0x7b4>
    16f2:	ea00      	jr	v0
    16f4:	6741      	move	v0,at
    16f6:	6500      	nop
    16f8:	0000      	addiu	s0,sp,0
    16fa:	0000      	addiu	s0,sp,0
    16fc:	67bd      	move	a1,sp
    16fe:	6808      	li	s0,8
    1700:	a550      	lbu	v0,16(a1)
    1702:	e84d      	or	s0,v0
    1704:	6522      	move	at,v0
    1706:	b202      	lw	v0,170c <$L129+0x7c8>
    1708:	ea00      	jr	v0
    170a:	6741      	move	v0,at
	...
    173c:	ffff      	daddiu	a3,sp,124
    173e:	1fff 0000 	jalx	ffc0000 <$L129+0xffbf0bc>
    1742:	0000      	addiu	s0,sp,0
    1744:	0000      	addiu	s0,sp,0
    1746:	0000      	addiu	s0,sp,0
    1748:	0000      	addiu	s0,sp,0
    174a:	a000      	lbu	s0,0(s0)
	...
    1770:	0010      	addiu	s0,sp,64
	...

Disassembly of section .text.USBCtrlEPAllowStatusStage:

00001ae0 <USBCtrlEPAllowStatusStage>:
    1ae0:	675c      	move	v0,gp
    1ae2:	f000 9a60 	lw	v1,0(v0)
    1ae6:	2b0e      	bnez	v1,1b04 <USBCtrlEPAllowStatusStage+0x24>
    1ae8:	6c01      	li	a0,1
    1aea:	f000 da80 	sw	a0,0(v0)
    1aee:	f000 9a60 	lw	v1,0(v0)
    1af2:	2b08      	bnez	v1,1b04 <USBCtrlEPAllowStatusStage+0x24>
    1af4:	f000 a260 	lbu	v1,0(v0)
    1af8:	f000 da80 	sw	a0,0(v0)
    1afc:	7302      	cmpi	v1,2
    1afe:	604d      	bteqz	1b9a <USBCtrlEPAllowStatusStage+0xba>
    1b00:	7301      	cmpi	v1,1
    1b02:	6001      	bteqz	1b06 <USBCtrlEPAllowStatusStage+0x26>
    1b04:	e8a0      	jrc	ra
    1b06:	f000 9a60 	lw	v1,0(v0)
    1b0a:	64b1      	save	8,s0-s1
    1b0c:	6e40      	li	a2,64
    1b0e:	a3a2      	lbu	a1,2(v1)
    1b10:	c3c2      	sb	a2,2(v1)
    1b12:	a3a3      	lbu	a1,3(v1)
    1b14:	6804      	li	s0,4
    1b16:	e80b      	neg	s0
    1b18:	ed0c      	and	a1,s0
    1b1a:	652e      	move	t1,a2
    1b1c:	c3a3      	sb	a1,3(v1)
    1b1e:	67c2      	move	a2,v0
    1b20:	b528      	lw	a1,1bc0 <USBCtrlEPAllowStatusStage+0xe0>
    1b22:	f000 4e00 	addiu	a2,0
    1b26:	edcc      	and	a1,a2
    1b28:	6fff      	li	a3,255
    1b2a:	67c5      	move	a2,a1
    1b2c:	eeec      	and	a2,a3
    1b2e:	650e      	move	t0,a2
    1b30:	a3c4      	lbu	a2,4(v1)
    1b32:	67c8      	move	a2,t0
    1b34:	c3c4      	sb	a2,4(v1)
    1b36:	36a2      	srl	a2,a1,8
    1b38:	656e      	move	t3,a2
    1b3a:	6726      	move	s1,a2
    1b3c:	a3c5      	lbu	a2,5(v1)
    1b3e:	67cb      	move	a2,t3
    1b40:	e9ec      	and	s1,a3
    1b42:	36c2      	srl	a2,8
    1b44:	eeec      	and	a2,a3
    1b46:	c325      	sb	s1,5(v1)
    1b48:	f600 35a2 	srl	a1,24
    1b4c:	a3e6      	lbu	a3,6(v1)
    1b4e:	c3c6      	sb	a2,6(v1)
    1b50:	a3e7      	lbu	a3,7(v1)
    1b52:	c3a7      	sb	a1,7(v1)
    1b54:	a3e0      	lbu	a3,0(v1)
    1b56:	6f7c      	li	a3,124
    1b58:	efeb      	neg	a3
    1b5a:	c3e0      	sb	a3,0(v1)
    1b5c:	a3e1      	lbu	a3,1(v1)
    1b5e:	67f8      	move	a3,t8
    1b60:	c3e1      	sb	a3,1(v1)
    1b62:	f000 da80 	sw	a0,0(v0)
    1b66:	f000 9a40 	lw	v0,0(v0)
    1b6a:	67e8      	move	a3,t0
    1b6c:	a262      	lbu	v1,2(v0)
    1b6e:	6769      	move	v1,t1
    1b70:	c262      	sb	v1,2(v0)
    1b72:	a263      	lbu	v1,3(v0)
    1b74:	eb0c      	and	v1,s0
    1b76:	c263      	sb	v1,3(v0)
    1b78:	a264      	lbu	v1,4(v0)
    1b7a:	c2e4      	sb	a3,4(v0)
    1b7c:	a265      	lbu	v1,5(v0)
    1b7e:	c225      	sb	s1,5(v0)
    1b80:	a266      	lbu	v1,6(v0)
    1b82:	c2c6      	sb	a2,6(v0)
    1b84:	a267      	lbu	v1,7(v0)
    1b86:	c2a7      	sb	a1,7(v0)
    1b88:	a260      	lbu	v1,0(v0)
    1b8a:	6b80      	li	v1,128
    1b8c:	eb6b      	neg	v1
    1b8e:	c260      	sb	v1,0(v0)
    1b90:	a261      	lbu	v1,1(v0)
    1b92:	6778      	move	v1,t8
    1b94:	c261      	sb	v1,1(v0)
    1b96:	6431      	restore	8,s0-s1
    1b98:	e8a0      	jrc	ra
    1b9a:	f000 9a40 	lw	v0,0(v0)
    1b9e:	67b8      	move	a1,t8
    1ba0:	a262      	lbu	v1,2(v0)
    1ba2:	6778      	move	v1,t8
    1ba4:	c262      	sb	v1,2(v0)
    1ba6:	a283      	lbu	a0,3(v0)
    1ba8:	6b04      	li	v1,4
    1baa:	eb6b      	neg	v1
    1bac:	eb8c      	and	v1,a0
    1bae:	c263      	sb	v1,3(v0)
    1bb0:	a260      	lbu	v1,0(v0)
    1bb2:	6b38      	li	v1,56
    1bb4:	eb6b      	neg	v1
    1bb6:	c260      	sb	v1,0(v0)
    1bb8:	a261      	lbu	v1,1(v0)
    1bba:	e820      	jr	ra
    1bbc:	c2a1      	sb	a1,1(v0)
    1bbe:	6500      	nop
    1bc0:	ffff      	daddiu	a3,sp,124
    1bc2:	1fff      	jalx	ffc0000 <$L129+0xffbf0bc>

Disassembly of section .text.USBCtrlEPAllowDataStage:

000036a4 <USBCtrlEPAllowDataStage>:
    36a4:	6a00      	li	v0,0
    36a6:	64e3      	save	24,ra,s0
    36a8:	671c      	move	s0,gp
    36aa:	f000 d840 	sw	v0,0(s0)
    36ae:	f000 d840 	sw	v0,0(s0)
    36b2:	f000 a040 	lbu	v0,0(s0)
    36b6:	7202      	cmpi	v0,2
    36b8:	603a      	bteqz	372e <USBCtrlEPAllowDataStage+0x8a>
    36ba:	6790      	move	a0,s0
    36bc:	f000 4c00 	addiu	a0,0
    36c0:	b52f      	lw	a1,377c <USBCtrlEPAllowDataStage+0xd8>
    36c2:	a446      	lbu	v0,6(a0)
    36c4:	6762      	move	v1,v0
    36c6:	a447      	lbu	v0,7(a0)
    36c8:	eb11      	zeb	v1
    36ca:	ea11      	zeb	v0
    36cc:	3240      	sll	v0,8
    36ce:	ea6d      	or	v0,v1
    36d0:	ad64      	lhu	v1,8(a1)
    36d2:	ea63      	sltu	v0,v1
    36d4:	6123      	btnez	371c <USBCtrlEPAllowDataStage+0x78>
    36d6:	1800 0000 	jal	0 <USBCtrlTrfTxService>
    36da:	6500      	nop
    36dc:	b329      	lw	v1,3780 <USBCtrlEPAllowDataStage+0xdc>
    36de:	b42a      	lw	a0,3784 <USBCtrlEPAllowDataStage+0xe0>
    36e0:	eb8c      	and	v1,a0
    36e2:	f000 9840 	lw	v0,0(s0)
    36e6:	6cff      	li	a0,255
    36e8:	67a3      	move	a1,v1
    36ea:	ed8c      	and	a1,a0
    36ec:	a2c4      	lbu	a2,4(v0)
    36ee:	c2a4      	sb	a1,4(v0)
    36f0:	3562      	srl	a1,v1,8
    36f2:	67c5      	move	a2,a1
    36f4:	ee8c      	and	a2,a0
    36f6:	35a2      	srl	a1,8
    36f8:	ecac      	and	a0,a1
    36fa:	a2e5      	lbu	a3,5(v0)
    36fc:	f600 3362 	srl	v1,24
    3700:	c2c5      	sb	a2,5(v0)
    3702:	a2a6      	lbu	a1,6(v0)
    3704:	c286      	sb	a0,6(v0)
    3706:	a287      	lbu	a0,7(v0)
    3708:	c267      	sb	v1,7(v0)
    370a:	a260      	lbu	v1,0(v0)
    370c:	6b38      	li	v1,56
    370e:	eb6b      	neg	v1
    3710:	c260      	sb	v1,0(v0)
    3712:	a261      	lbu	v1,1(v0)
    3714:	6b00      	li	v1,0
    3716:	c261      	sb	v1,1(v0)
    3718:	6463      	restore	24,ra,s0
    371a:	e8a0      	jrc	ra
    371c:	a446      	lbu	v0,6(a0)
    371e:	6762      	move	v1,v0
    3720:	a447      	lbu	v0,7(a0)
    3722:	eb11      	zeb	v1
    3724:	ea11      	zeb	v0
    3726:	3240      	sll	v0,8
    3728:	ea6d      	or	v0,v1
    372a:	cd44      	sh	v0,8(a1)
    372c:	17d4      	b	36d6 <USBCtrlEPAllowDataStage+0x32>
    372e:	f000 9840 	lw	v0,0(s0)
    3732:	a262      	lbu	v1,2(v0)
    3734:	6b40      	li	v1,64
    3736:	c262      	sb	v1,2(v0)
    3738:	a283      	lbu	a0,3(v0)
    373a:	4bbc      	addiu	v1,-68
    373c:	eb8c      	and	v1,a0
    373e:	b412      	lw	a0,3784 <USBCtrlEPAllowDataStage+0xe0>
    3740:	c263      	sb	v1,3(v0)
    3742:	b310      	lw	v1,3780 <USBCtrlEPAllowDataStage+0xdc>
    3744:	a2c4      	lbu	a2,4(v0)
    3746:	eb8c      	and	v1,a0
    3748:	67a3      	move	a1,v1
    374a:	6cff      	li	a0,255
    374c:	ed8c      	and	a1,a0
    374e:	c2a4      	sb	a1,4(v0)
    3750:	3562      	srl	a1,v1,8
    3752:	67c5      	move	a2,a1
    3754:	ee8c      	and	a2,a0
    3756:	35a2      	srl	a1,8
    3758:	ecac      	and	a0,a1
    375a:	a2e5      	lbu	a3,5(v0)
    375c:	f600 3362 	srl	v1,24
    3760:	c2c5      	sb	a2,5(v0)
    3762:	a2a6      	lbu	a1,6(v0)
    3764:	c286      	sb	a0,6(v0)
    3766:	a287      	lbu	a0,7(v0)
    3768:	c267      	sb	v1,7(v0)
    376a:	a260      	lbu	v1,0(v0)
    376c:	6b38      	li	v1,56
    376e:	eb6b      	neg	v1
    3770:	c260      	sb	v1,0(v0)
    3772:	a261      	lbu	v1,1(v0)
    3774:	6778      	move	v1,t8
    3776:	c261      	sb	v1,1(v0)
    3778:	6463      	restore	24,ra,s0
    377a:	e8a0      	jrc	ra
    377c:	0000      	addiu	s0,sp,0
    377e:	0000      	addiu	s0,sp,0
    3780:	ffff      	daddiu	a3,sp,124
    3782:	1fff 0000 	jalx	ffc0000 <$L129+0xffbf0bc>
    3786:	0000      	addiu	s0,sp,0

Disassembly of section .text.USBEnableEndpoint:

00006e2c <USBEnableEndpoint>:
    6e2c:	ed11      	zeb	a1
    6e2e:	6a08      	li	v0,8
    6e30:	eaac      	and	v0,a1
    6e32:	67dc      	move	a2,gp
    6e34:	ec11      	zeb	a0
    6e36:	2216      	beqz	v0,6e64 <USBEnableEndpoint+0x38>
    6e38:	b31a      	lw	v1,6ea0 <USBEnableEndpoint+0x74>
    6e3a:	3294      	sll	v0,a0,5
    6e3c:	e269      	addu	v0,v1
    6e3e:	6b7f      	li	v1,127
    6e40:	a2e0      	lbu	a3,0(v0)
    6e42:	ebec      	and	v1,a3
    6e44:	67e6      	move	a3,a2
    6e46:	c260      	sb	v1,0(v0)
    6e48:	f000 4f00 	addiu	a3,0
    6e4c:	3388      	sll	v1,a0,2
    6e4e:	e3ed      	addu	v1,a3
    6e50:	db40      	sw	v0,0(v1)
    6e52:	a2e0      	lbu	a3,0(v0)
    6e54:	6b41      	li	v1,65
    6e56:	eb6b      	neg	v1
    6e58:	ebec      	and	v1,a3
    6e5a:	c260      	sb	v1,0(v0)
    6e5c:	a2e8      	lbu	a3,8(v0)
    6e5e:	6b40      	li	v1,64
    6e60:	ebed      	or	v1,a3
    6e62:	c268      	sb	v1,8(v0)
    6e64:	6a04      	li	v0,4
    6e66:	eaac      	and	v0,a1
    6e68:	2215      	beqz	v0,6e94 <USBEnableEndpoint+0x68>
    6e6a:	b30f      	lw	v1,6ea4 <USBEnableEndpoint+0x78>
    6e6c:	3294      	sll	v0,a0,5
    6e6e:	e269      	addu	v0,v1
    6e70:	6b7f      	li	v1,127
    6e72:	a2e0      	lbu	a3,0(v0)
    6e74:	f000 4e00 	addiu	a2,0
    6e78:	ebec      	and	v1,a3
    6e7a:	c260      	sb	v1,0(v0)
    6e7c:	3388      	sll	v1,a0,2
    6e7e:	e3cd      	addu	v1,a2
    6e80:	db40      	sw	v0,0(v1)
    6e82:	a2c0      	lbu	a2,0(v0)
    6e84:	6b41      	li	v1,65
    6e86:	eb6b      	neg	v1
    6e88:	ebcc      	and	v1,a2
    6e8a:	c260      	sb	v1,0(v0)
    6e8c:	a2c8      	lbu	a2,8(v0)
    6e8e:	6b40      	li	v1,64
    6e90:	ebcd      	or	v1,a2
    6e92:	c268      	sb	v1,8(v0)
    6e94:	3490      	sll	a0,4
    6e96:	b205      	lw	v0,6ea8 <USBEnableEndpoint+0x7c>
    6e98:	e451      	addu	a0,v0
    6e9a:	e820      	jr	ra
    6e9c:	c4a0      	sb	a1,0(a0)
    6e9e:	6500      	nop
    6ea0:	0000      	addiu	s0,sp,0
    6ea2:	0000      	addiu	s0,sp,0
    6ea4:	0010      	addiu	s0,sp,64
    6ea6:	0000      	addiu	s0,sp,0
    6ea8:	0000      	addiu	s0,sp,0
	...

Disassembly of section .text.USBStallEndpoint:

0000dcd8 <USBStallEndpoint>:
    dcd8:	ec11      	zeb	a0
    dcda:	67dc      	move	a2,gp
    dcdc:	ed11      	zeb	a1
    dcde:	2410      	beqz	a0,dd00 <USBStallEndpoint+0x28>
    dce0:	3484      	sll	a0,1
    dce2:	e4b5      	addu	a1,a0,a1
    dce4:	b220      	lw	v0,dd64 <USBStallEndpoint+0x8c>
    dce6:	34b0      	sll	a0,a1,4
    dce8:	e291      	addu	a0,v0,a0
    dcea:	6b84      	li	v1,132
    dcec:	acc0      	lhu	a2,0(a0)
    dcee:	35b0      	sll	a1,4
    dcf0:	ee6d      	or	a2,v1
    dcf2:	4d08      	addiu	a1,8
    dcf4:	e2b5      	addu	a1,v0,a1
    dcf6:	ccc0      	sh	a2,0(a0)
    dcf8:	ad40      	lhu	v0,0(a1)
    dcfa:	eb4d      	or	v1,v0
    dcfc:	e820      	jr	ra
    dcfe:	cd60      	sh	v1,0(a1)
    dd00:	f000 9e40 	lw	v0,0(a2)
    dd04:	64b1      	save	8,s0-s1
    dd06:	a262      	lbu	v1,2(v0)
    dd08:	6b40      	li	v1,64
    dd0a:	c262      	sb	v1,2(v0)
    dd0c:	a2a3      	lbu	a1,3(v0)
    dd0e:	4bbc      	addiu	v1,-68
    dd10:	ebac      	and	v1,a1
    dd12:	67a6      	move	a1,a2
    dd14:	c263      	sb	v1,3(v0)
    dd16:	b315      	lw	v1,dd68 <USBStallEndpoint+0x90>
    dd18:	f000 4d00 	addiu	a1,0
    dd1c:	ebac      	and	v1,a1
    dd1e:	67e3      	move	a3,v1
    dd20:	6dff      	li	a1,255
    dd22:	efac      	and	a3,a1
    dd24:	a204      	lbu	s0,4(v0)
    dd26:	c2e4      	sb	a3,4(v0)
    dd28:	3762      	srl	a3,v1,8
    dd2a:	6707      	move	s0,a3
    dd2c:	e8ac      	and	s0,a1
    dd2e:	37e2      	srl	a3,8
    dd30:	edec      	and	a1,a3
    dd32:	a225      	lbu	s1,5(v0)
    dd34:	f600 3362 	srl	v1,24
    dd38:	c205      	sb	s0,5(v0)
    dd3a:	a2e6      	lbu	a3,6(v0)
    dd3c:	c2a6      	sb	a1,6(v0)
    dd3e:	a2a7      	lbu	a1,7(v0)
    dd40:	c267      	sb	v1,7(v0)
    dd42:	a260      	lbu	v1,0(v0)
    dd44:	6b74      	li	v1,116
    dd46:	eb6b      	neg	v1
    dd48:	c260      	sb	v1,0(v0)
    dd4a:	a261      	lbu	v1,1(v0)
    dd4c:	c281      	sb	a0,1(v0)
    dd4e:	f000 9e40 	lw	v0,0(a2)
    dd52:	a260      	lbu	v1,0(v0)
    dd54:	6b7c      	li	v1,124
    dd56:	eb6b      	neg	v1
    dd58:	c260      	sb	v1,0(v0)
    dd5a:	a261      	lbu	v1,1(v0)
    dd5c:	c281      	sb	a0,1(v0)
    dd5e:	6431      	restore	8,s0-s1
    dd60:	e8a0      	jrc	ra
    dd62:	6500      	nop
    dd64:	0000      	addiu	s0,sp,0
    dd66:	0000      	addiu	s0,sp,0
    dd68:	ffff      	daddiu	a3,sp,124
    dd6a:	1fff      	jalx	ffc0000 <$L129+0xffbf0bc>

Disassembly of section .text.USBTransferOnePacket:

0001ba44 <USBTransferOnePacket>:
   1ba44:	64b1      	save	8,s0-s1
   1ba46:	ec11      	zeb	a0
   1ba48:	671c      	move	s0,gp
   1ba4a:	ef11      	zeb	a3
   1ba4c:	ed11      	zeb	a1
   1ba4e:	652f      	move	t1,a3
   1ba50:	3488      	sll	a0,2
   1ba52:	6750      	move	v0,s0
   1ba54:	254c      	beqz	a1,1baee <USBTransferOnePacket+0xaa>
   1ba56:	f000 4a00 	addiu	v0,0
   1ba5a:	e289      	addu	v0,a0
   1ba5c:	9a40      	lw	v0,0(v0)
   1ba5e:	2255      	beqz	v0,1bb0a <USBTransferOnePacket+0xc6>
   1ba60:	b32c      	lw	v1,1bb10 <USBTransferOnePacket+0xcc>
   1ba62:	a224      	lbu	s1,4(v0)
   1ba64:	ee6c      	and	a2,v1
   1ba66:	67e6      	move	a3,a2
   1ba68:	6bff      	li	v1,255
   1ba6a:	ef6c      	and	a3,v1
   1ba6c:	31c2      	srl	s1,a2,8
   1ba6e:	c2e4      	sb	a3,4(v0)
   1ba70:	67f1      	move	a3,s1
   1ba72:	ef6c      	and	a3,v1
   1ba74:	651f      	move	t8,a3
   1ba76:	a2e5      	lbu	a3,5(v0)
   1ba78:	67f8      	move	a3,t8
   1ba7a:	3122      	srl	s1,8
   1ba7c:	c2e5      	sb	a3,5(v0)
   1ba7e:	e96c      	and	s1,v1
   1ba80:	a2e6      	lbu	a3,6(v0)
   1ba82:	f600 36c2 	srl	a2,24
   1ba86:	67e9      	move	a3,t1
   1ba88:	c226      	sb	s1,6(v0)
   1ba8a:	ef6c      	and	a3,v1
   1ba8c:	a227      	lbu	s1,7(v0)
   1ba8e:	c2c7      	sb	a2,7(v0)
   1ba90:	a2c2      	lbu	a2,2(v0)
   1ba92:	c2e2      	sb	a3,2(v0)
   1ba94:	a2c3      	lbu	a2,3(v0)
   1ba96:	6f04      	li	a3,4
   1ba98:	efeb      	neg	a3
   1ba9a:	eeec      	and	a2,a3
   1ba9c:	c2c3      	sb	a2,3(v0)
   1ba9e:	a2c0      	lbu	a2,0(v0)
   1baa0:	67e6      	move	a3,a2
   1baa2:	a2c1      	lbu	a2,1(v0)
   1baa4:	ef11      	zeb	a3
   1baa6:	ee11      	zeb	a2
   1baa8:	36c0      	sll	a2,8
   1baaa:	eeed      	or	a2,a3
   1baac:	6f40      	li	a3,64
   1baae:	eeec      	and	a2,a3
   1bab0:	ee6c      	and	a2,v1
   1bab2:	a2e0      	lbu	a3,0(v0)
   1bab4:	c2c0      	sb	a2,0(v0)
   1bab6:	a2c1      	lbu	a2,1(v0)
   1bab8:	6e00      	li	a2,0
   1baba:	c2c1      	sb	a2,1(v0)
   1babc:	a2c0      	lbu	a2,0(v0)
   1babe:	67e6      	move	a3,a2
   1bac0:	a2c1      	lbu	a2,1(v0)
   1bac2:	ef11      	zeb	a3
   1bac4:	ee11      	zeb	a2
   1bac6:	36c0      	sll	a2,8
   1bac8:	eeed      	or	a2,a3
   1baca:	6f88      	li	a3,136
   1bacc:	eeed      	or	a2,a3
   1bace:	ebcc      	and	v1,a2
   1bad0:	36c2      	srl	a2,8
   1bad2:	a2e0      	lbu	a3,0(v0)
   1bad4:	c260      	sb	v1,0(v0)
   1bad6:	a261      	lbu	v1,1(v0)
   1bad8:	c2c1      	sb	a2,1(v0)
   1bada:	2d0e      	bnez	a1,1baf8 <USBTransferOnePacket+0xb4>
   1badc:	f000 4800 	addiu	s0,0
   1bae0:	e091      	addu	a0,s0,a0
   1bae2:	a460      	lbu	v1,0(a0)
   1bae4:	7308      	cmpi	v1,8
   1bae6:	6778      	move	v1,t8
   1bae8:	c460      	sb	v1,0(a0)
   1baea:	6431      	restore	8,s0-s1
   1baec:	e8a0      	jrc	ra
   1baee:	f000 4a00 	addiu	v0,0
   1baf2:	e289      	addu	v0,a0
   1baf4:	9a40      	lw	v0,0(v0)
   1baf6:	17b3      	b	1ba5e <USBTransferOnePacket+0x1a>
   1baf8:	f000 4800 	addiu	s0,0
   1bafc:	e091      	addu	a0,s0,a0
   1bafe:	a460      	lbu	v1,0(a0)
   1bb00:	7308      	cmpi	v1,8
   1bb02:	6738      	move	s1,t8
   1bb04:	c420      	sb	s1,0(a0)
   1bb06:	6431      	restore	8,s0-s1
   1bb08:	e8a0      	jrc	ra
   1bb0a:	6a00      	li	v0,0
   1bb0c:	17ee      	b	1baea <USBTransferOnePacket+0xa6>
   1bb0e:	6500      	nop
   1bb10:	ffff      	daddiu	a3,sp,124
   1bb12:	1fff      	jalx	ffc0000 <$L129+0xffbf0bc>

Disassembly of section .text.USBCancelIO:

00037558 <USBCancelIO>:
   37558:	b250      	lw	v0,37698 <USBCancelIO+0x140>
   3755a:	ec11      	zeb	a0
   3755c:	9a60      	lw	v1,0(v0)
   3755e:	6a01      	li	v0,1
   37560:	3376      	srl	v1,5
   37562:	eb4c      	and	v1,v0
   37564:	eb4e      	xor	v1,v0
   37566:	2301      	beqz	v1,3756a <USBCancelIO+0x12>
   37568:	e8a0      	jrc	ra
   3756a:	64b1      	save	8,s0-s1
   3756c:	673c      	move	s1,gp
   3756e:	3488      	sll	a0,2
   37570:	f000 4900 	addiu	s1,0
   37574:	e425      	addu	s1,a0,s1
   37576:	68ff      	li	s0,255
   37578:	9940      	lw	v0,0(s1)
   3757a:	a280      	lbu	a0,0(v0)
   3757c:	a2a1      	lbu	a1,1(v0)
   3757e:	a2a2      	lbu	a1,2(v0)
   37580:	a2a3      	lbu	a1,3(v0)
   37582:	a2a4      	lbu	a1,4(v0)
   37584:	a2a5      	lbu	a1,5(v0)
   37586:	a2a6      	lbu	a1,6(v0)
   37588:	a2a7      	lbu	a1,7(v0)
   3758a:	6d40      	li	a1,64
   3758c:	ecac      	and	a0,a1
   3758e:	67b0      	move	a1,s0
   37590:	ed8c      	and	a1,a0
   37592:	a2c0      	lbu	a2,0(v0)
   37594:	c2a0      	sb	a1,0(v0)
   37596:	3582      	srl	a1,a0,8
   37598:	ed0c      	and	a1,s0
   3759a:	a2c1      	lbu	a2,1(v0)
   3759c:	c2a1      	sb	a1,1(v0)
   3759e:	a2a2      	lbu	a1,2(v0)
   375a0:	6d00      	li	a1,0
   375a2:	f600 3482 	srl	a0,24
   375a6:	c2a2      	sb	a1,2(v0)
   375a8:	a2a3      	lbu	a1,3(v0)
   375aa:	c283      	sb	a0,3(v0)
   375ac:	a284      	lbu	a0,4(v0)
   375ae:	c264      	sb	v1,4(v0)
   375b0:	a285      	lbu	a0,5(v0)
   375b2:	c265      	sb	v1,5(v0)
   375b4:	a286      	lbu	a0,6(v0)
   375b6:	c266      	sb	v1,6(v0)
   375b8:	a287      	lbu	a0,7(v0)
   375ba:	c267      	sb	v1,7(v0)
   375bc:	a2c0      	lbu	a2,0(v0)
   375be:	a2a1      	lbu	a1,1(v0)
   375c0:	a262      	lbu	v1,2(v0)
   375c2:	ed11      	zeb	a1
   375c4:	eb11      	zeb	v1
   375c6:	3360      	sll	v1,8
   375c8:	67e6      	move	a3,a2
   375ca:	35a0      	sll	a1,8
   375cc:	3360      	sll	v1,8
   375ce:	ef11      	zeb	a3
   375d0:	67c5      	move	a2,a1
   375d2:	67a3      	move	a1,v1
   375d4:	a263      	lbu	v1,3(v0)
   375d6:	eeed      	or	a2,a3
   375d8:	edcd      	or	a1,a2
   375da:	a2e4      	lbu	a3,4(v0)
   375dc:	f600 3360 	sll	v1,24
   375e0:	a2c5      	lbu	a2,5(v0)
   375e2:	ebad      	or	v1,a1
   375e4:	a2a6      	lbu	a1,6(v0)
   375e6:	ee11      	zeb	a2
   375e8:	6787      	move	a0,a3
   375ea:	36c0      	sll	a2,8
   375ec:	ed11      	zeb	a1
   375ee:	ec11      	zeb	a0
   375f0:	67e6      	move	a3,a2
   375f2:	35a0      	sll	a1,8
   375f4:	ef8d      	or	a3,a0
   375f6:	35a0      	sll	a1,8
   375f8:	6c40      	li	a0,64
   375fa:	eb8e      	xor	v1,a0
   375fc:	67c5      	move	a2,a1
   375fe:	6790      	move	a0,s0
   37600:	a2a7      	lbu	a1,7(v0)
   37602:	ec6c      	and	a0,v1
   37604:	eeed      	or	a2,a3
   37606:	f600 35a0 	sll	a1,24
   3760a:	edcd      	or	a1,a2
   3760c:	a2c0      	lbu	a2,0(v0)
   3760e:	c280      	sb	a0,0(v0)
   37610:	3462      	srl	a0,v1,8
   37612:	67c4      	move	a2,a0
   37614:	ee0c      	and	a2,s0
   37616:	3482      	srl	a0,8
   37618:	ec0c      	and	a0,s0
   3761a:	a2e1      	lbu	a3,1(v0)
   3761c:	f600 3362 	srl	v1,24
   37620:	c2c1      	sb	a2,1(v0)
   37622:	a2c2      	lbu	a2,2(v0)
   37624:	c282      	sb	a0,2(v0)
   37626:	a283      	lbu	a0,3(v0)
   37628:	c263      	sb	v1,3(v0)
   3762a:	6770      	move	v1,s0
   3762c:	ebac      	and	v1,a1
   3762e:	a284      	lbu	a0,4(v0)
   37630:	c264      	sb	v1,4(v0)
   37632:	33a2      	srl	v1,a1,8
   37634:	6783      	move	a0,v1
   37636:	ec0c      	and	a0,s0
   37638:	3362      	srl	v1,8
   3763a:	eb0c      	and	v1,s0
   3763c:	a2c5      	lbu	a2,5(v0)
   3763e:	f600 35a2 	srl	a1,24
   37642:	c285      	sb	a0,5(v0)
   37644:	a286      	lbu	a0,6(v0)
   37646:	c266      	sb	v1,6(v0)
   37648:	a267      	lbu	v1,7(v0)
   3764a:	c2a7      	sb	a1,7(v0)
   3764c:	a160      	lbu	v1,0(s1)
   3764e:	7308      	cmpi	v1,8
   37650:	6778      	move	v1,t8
   37652:	c160      	sb	v1,0(s1)
   37654:	a260      	lbu	v1,0(v0)
   37656:	6783      	move	a0,v1
   37658:	a261      	lbu	v1,1(v0)
   3765a:	ec11      	zeb	a0
   3765c:	eb11      	zeb	v1
   3765e:	3360      	sll	v1,8
   37660:	eb8d      	or	v1,a0
   37662:	6c40      	li	a0,64
   37664:	eb8c      	and	v1,a0
   37666:	eb0c      	and	v1,s0
   37668:	a280      	lbu	a0,0(v0)
   3766a:	c260      	sb	v1,0(v0)
   3766c:	a261      	lbu	v1,1(v0)
   3766e:	6b00      	li	v1,0
   37670:	c261      	sb	v1,1(v0)
   37672:	a280      	lbu	a0,0(v0)
   37674:	6b40      	li	v1,64
   37676:	67a4      	move	a1,a0
   37678:	a281      	lbu	a0,1(v0)
   3767a:	ed11      	zeb	a1
   3767c:	ec11      	zeb	a0
   3767e:	3480      	sll	a0,8
   37680:	ecad      	or	a0,a1
   37682:	ec6e      	xor	a0,v1
   37684:	6764      	move	v1,a0
   37686:	eb0c      	and	v1,s0
   37688:	3482      	srl	a0,8
   3768a:	a2a0      	lbu	a1,0(v0)
   3768c:	c260      	sb	v1,0(v0)
   3768e:	a261      	lbu	v1,1(v0)
   37690:	c281      	sb	a0,1(v0)
   37692:	6431      	restore	8,s0-s1
   37694:	e8a0      	jrc	ra
   37696:	6500      	nop
   37698:	0000      	addiu	s0,sp,0
	...

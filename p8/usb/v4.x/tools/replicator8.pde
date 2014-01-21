// This is a tool to program the pinguino bootloader in a new chip
// with your Pinguino, the self replicating machine !!
// Jean-Pierre MANDON 2010
// Fixed bug 02/07/2011 PGM pin is no longer used

#define PGC 1		// connected to the PGC pin of the blank chip
#define PGD 2		// connected to the PGD pin of the blank chip
#define VPP 3		// connected to the VPP pin of the blank chip
#define VCC 6		// connected to the power on transistor
#define redled	 5

uchar bulkerase[48] ={0x00,0x3C,0x0E,0x00,0xF8,0x6E,0x00,0x00,0x0E,0x00,0xF7,0x6E,0x00,0x05,0x0E,0x00,
                      0xF6,0x6E,0x0C,0x3F,0x3F,0x00,0x3C,0x0E,0x00,0xF8,0x6E,0x00,0x00,0x0E,0x00,0xF7,
		              0x6E,0x00,0x04,0x0E,0x00,0xF6,0x6E,0x0C,0x8F,0x8F,0x00,0x00,0x00,0x00,0x00,0x00};
			      
uchar startwrite[24]={0x00,0xA6,0x8E,0x00,0xA6,0x9C,0x00,0x00,0x0E,0x00,0xF8,0x6E,0x00,0x00,0x0E,0x00,
					  0xF7,0x6E,0x00,0x00,0x0E,0x00,0xF6,0x6E};
					  
uchar startwrid[24] ={0x00,0xA6,0x8E,0x00,0xA6,0x8C,0x00,0x30,0x0E,0x00,0xF8,0x6E,0x00,0x00,0x0E,0x00,
					  0xF7,0x6E,0x00,0x00,0x0E,0x00,0xF6,0x6E};
uchar checkmem[32];
uint address=0;

#define pic_on()  digitalWrite(VCC,LOW)
#define pic_off() digitalWrite(VCC,HIGH)
					  
// begin programming mode
// power is on ( VCC and Programing voltage )

void start_pgm()
{
	digitalWrite(VPP,HIGH);
	delayMicroseconds(4);
}

// end programming mode
// power is on ( VCC and Programing voltage )

void stop_pgm()
{
	digitalWrite(PGD,LOW);
	digitalWrite(PGC,LOW);
	digitalWrite(VPP,LOW);
}

// send a command to the chip
// SPI soft

void send_command(uchar command,uchar lowbyte,uchar highbyte)
{
	unsigned char i;
	for (i=0;i<4;i++)
		{
		digitalWrite(PGC,HIGH);
		if ((command&1)==1) digitalWrite(PGD,HIGH);
		else digitalWrite(PGD,LOW);
		digitalWrite(PGC,LOW);
		command=command>>1;
		}
	for (i=0;i<8;i++)
		{
		digitalWrite(PGC,HIGH);
		if ((lowbyte&1)==1) digitalWrite(PGD,HIGH);
		else digitalWrite(PGD,LOW);
		digitalWrite(PGC,LOW);
		lowbyte=lowbyte>>1;
		}
	for (i=0;i<8;i++)
		{
		digitalWrite(PGC,HIGH);
		if ((highbyte&1)==1) digitalWrite(PGD,HIGH);
		else digitalWrite(PGD,LOW);
		digitalWrite(PGC,LOW);
		highbyte=highbyte>>1;
		}
digitalWrite(PGC,LOW);
digitalWrite(PGD,LOW);
}

// special end of command write
// DS39622K Page 20 figure 3.5 (Flash Microcontroller Programming Specification)

void end_writecmd()
{
unsigned char i;
digitalWrite(PGD,LOW);
for (i=0;i<3;i++)
	{
	digitalWrite(PGC,HIGH);
	digitalWrite(PGC,LOW);
	}
digitalWrite(PGC,HIGH);
delay(6);
digitalWrite(PGC,LOW);
delayMicroseconds(500);
for (i=0;i<8;i++)
	{
	digitalWrite(PGC,HIGH);
	digitalWrite(PGC,LOW);
	}
for (i=0;i<8;i++)
	{
	digitalWrite(PGC,HIGH);
	digitalWrite(PGC,LOW);
	}
}	

// Read Flash memory
// Pinguino examples Flash folder

uint ReadFlash(uint address)
{
uchar high8,low8;
TBLPTRU=0;
TBLPTRH=address>>8;
TBLPTRL=address;
__asm tblrd*+ __endasm;
low8=TABLAT;
__asm tblrd*+ __endasm;
high8=TABLAT;
return((high8<<8)+low8);
}

//-----------------------------------------------------------
// begining of the main loop
//-----------------------------------------------------------

void setup()
{
pinMode(PGC,OUTPUT);
digitalWrite(PGC,LOW);
pinMode(PGD,OUTPUT);
digitalWrite(PGD,LOW);
pinMode(VPP,OUTPUT);
digitalWrite(VPP,LOW);
pinMode(redled,OUTPUT);
digitalWrite(redled,LOW);
pinMode(VCC,OUTPUT);
digitalWrite(VCC,HIGH);
}

void loop()
{
int i;

// erase chip

pic_on();
delay(20);
start_pgm();
digitalWrite(redled,HIGH);
delay(10);
for (i=0;i<48;i+=3) send_command(bulkerase[i],bulkerase[i+1],bulkerase[i+2]);
delay(100);
stop_pgm();
delay(20);
pic_off();
delay(1000);
	
// read bootloader and write it

pic_on();
delay(1000);
start_pgm();
delay(10);
for (i=0;i<24;i+=3) send_command(startwrite[i],startwrite[i+1],startwrite[i+2]);
while (address<0x1FDF)
	{
	for (i=0;i<32;i++) checkmem[i]=ReadFlash(address+i);
	for (i=0;i<30;i+=2) send_command(0b00001101,checkmem[i],checkmem[i+1]);
	send_command(0b00001110,checkmem[30],checkmem[31]);
	end_writecmd();
	address+=32;
	digitalWrite(redled,digitalRead(redled)^1);
	delay(4);
	}
digitalWrite(redled,HIGH);	
for (i=0;i<32;i++) checkmem[i]=ReadFlash(address+i);
for (i=0;i<48;i+=3) send_command(startwrite[i],startwrite[i+1],startwrite[i+2]);
for (i=0;i<30;i+=2) send_command(0b00001101,checkmem[i],checkmem[i+1]);
send_command(0b00001111,checkmem[30],checkmem[31]);
end_writecmd();

// ---------- end copying bootloader

// programing configuration bits

for (i=0;i<24;i+=3) send_command(startwrid[i],startwrid[i+1],startwrid[i+2]);
send_command(0b00001111,0x24,0x24);
end_writecmd();
send_command(0,0x01,0x0E);
send_command(0,0xF6,0x6E);
send_command(0b00001111,0x0E,0x0E);
end_writecmd();
send_command(0,0x02,0x0E);
send_command(0,0xF6,0x6E);
send_command(0b00001111,0x3F,0x3F);
end_writecmd();
send_command(0,0x03,0x0E);
send_command(0,0xF6,0x6E);
send_command(0b00001111,0x1E,0x1E);
end_writecmd();
send_command(0,0x05,0x0E);
send_command(0,0xF6,0x6E);
send_command(0b00001111,0x81,0x81);
end_writecmd();
send_command(0,0x06,0x0E);
send_command(0,0xF6,0x6E);
send_command(0b00001111,0x81,0x81);
end_writecmd();
send_command(0,0x08,0x0E);
send_command(0,0xF6,0x6E);
send_command(0b00001111,0x0F,0x0F);
end_writecmd();
send_command(0,0x09,0x0E);
send_command(0,0xF6,0x6E);
send_command(0b00001111,0xC0,0xC0);
end_writecmd();
send_command(0,0x0A,0x0E);
send_command(0,0xF6,0x6E);
send_command(0b00001111,0x0F,0x0F);
end_writecmd();
send_command(0,0x0B,0x0E);
send_command(0,0xF6,0x6E);
send_command(0b00001111,0xA0,0xA0);
end_writecmd();
send_command(0,0x0C,0x0E);
send_command(0,0xF6,0x6E);
send_command(0b00001111,0x0F,0x0F);
end_writecmd();
send_command(0,0x0D,0x0E);
send_command(0,0xF6,0x6E);
send_command(0b00001111,0x40,0x40);
end_writecmd();

// ---------- end configuration bits

stop_pgm();
pic_off();

digitalWrite(redled,LOW);

while(1);
}



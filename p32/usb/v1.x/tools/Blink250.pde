/*
void mydelay(u32 i)
{
    u32 j;
    while (i--)
    {
        j=0x4000;
        while (j--)
        {
            nop();
        }
    }
}
*/

void setup()
{                
    pinMode(USERLED, OUTPUT);
}

void loop()
{
    /*
    toggle(USERLED);
    delay(500);
    */
    digitalWrite(USERLED, ON);
    delay(100);
    digitalWrite(USERLED, OFF);
    delay(900);
}

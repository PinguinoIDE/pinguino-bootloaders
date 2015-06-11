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
    u32 timeEnd = millis() + 1000; // 1000 ms = 1 sec
    while (millis() < timeEnd);
    toggle(USERLED);
    /*
    digitalWrite(USERLED, ON);
    delay(100);
    digitalWrite(USERLED, OFF);
    delay(900);
    */
}

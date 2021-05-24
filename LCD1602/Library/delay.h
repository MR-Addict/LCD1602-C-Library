void delay_us(unsigned int us_count)
{
   while (us_count != 0)
   {
      us_count--;
   }
}

void delay_ms(unsigned int ms_count)
{
   while (ms_count != 0)
   {
      delay_us(112);
      ms_count--;
   }
}

void delay_sec(unsigned char sec_count)
{

   while (sec_count != 0)
   {
      delay_ms(1000);
      sec_count--;
   }
}
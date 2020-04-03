#include <main.h>
int8 tick = 0;
int8 milis = 0;
int8 second = 0;
int8 minute = 0;
int8 hour = 0;


// SECOND
int8 secOnes = 0;
int8 secTens = 0;

// MINUTE
int8 minOnes = 0;
int8 minTens = 0;

// HOUR
int8 hrOnes = 0;
int8 hrTens = 0;

// Control Variables
int1 isA = 1;
int1 isB = 1;
int1 isC = 1;
int1 isD = 1;
int1 isE = 1;

// SETTINGS
int1 d4 = 0;
int1 d5 = 0;
int1 d6 = 0;
int1 d7 = 0;
int8 selection = 0;
//
int1 isPaused = 0;

void renderClock()
{
   if(isA == 1 || isE == 1)
    {
        isA = 0;
        isE = 0;
        hrOnes = hour % 10;
        hrTens = (hour - hrOnes) / 10;
        output_a(hrOnes);
        output_e(hrTens);
    }
    if(isB == 1)
    {
       isB = 0;
       minOnes = minute % 10;
       minTens = (minute - minOnes) / 10;
       output_b((minTens << 4) + minOnes);
    }
    if(isC == 1)
    {
      isC = 0;
      secOnes = second % 10;
      secTens = (second - secOnes) / 10;
      output_c((secTens << 4) + secOnes);
    }
    if(isD == 1)
    {
      isD = 0;
      output_bit(pin_D3, (milis >> 3) & 0x00000001);
      output_bit(pin_D2, (milis >> 2) & 0x00000001);
      output_bit(pin_D1, (milis >> 1) & 0x00000001);
      output_bit(pin_D0, milis & 0x00000001);
    }
}

#INT_TIMER1
void TIMER_isr(void)
{
    if(isPaused == 0)
    {
        tick++;
      if(tick >= 6)
      {
         tick = 0;
         isD = 1;
         milis++;
         if(milis >= 10)
         {
            milis = 0;
            isC = 1;
            second++;
            if(second >= 60)
            {
               second = 0;
               isB = 1;
               minute++;
               if(minute >= 60)
               {
                  minute = 0;
                  isA = 1;
                  hour++;
                  if(hour >= 24)
                  {
                     hour = 0;
                     isE = 1;
                  }
               }
            }
         }
      }
      renderClock();
   }   
}
void main()
{
   setup_timer_1(T1_INTERNAL | T1_DIV_BY_1);
   set_timer1(77);
   clear_interrupt(INT_TIMER1);
   enable_interrupts(INT_TIMER1);
   enable_interrupts(GLOBAL);
   set_tris_d(255);
   
   while(TRUE)
   {
      // GET ALL SETTINGS
      d4 = input(pin_D4);
      d5 = input(pin_D5);
      d6 = input(pin_D6);
      d7 = input(pin_D7);
      //
      if(d7)
      {
         // PLAY / PAUSE
         isPaused++; // 1 bit value its toggling between 1 and 0
         delay_ms(200);
         selection = 0;
         while(input(pin_D4));
      }
      
      if(isPaused == 1)
      {
         // SETUP MODE
         if(d6)
         {
            // Digit Selector
            selection++;
            if(selection > 3)
            {
               selection = 0;
            }
            delay_ms(200);
            while(input(pin_D6));
         }
         if(d5)
         {
            // Increase Selected Digit
            switch(selection)
            {
               case 0:
               {
                  // HOUR
                  hour++;
                  if(hour >= 24)
                  {
                     hour = 0;
                  }
                  isE = 1;
                  isA = 1;
                  break;
               }
               case 1:
               {
                  // MINUTE
                  minute++;
                  if(minute >= 60)
                  {
                     minute = 0;
                  }
                  isB = 1;
                  break;
               }
               case 2:
               {
                  // SECOND
                  second++;
                  if(second >= 60)
                  {
                     second = 0;
                  }
                  isC = 1;
                  break;
               }
               case 3:
               {
                  // MILISECOND
                  milis++;
                  if(milis >= 10)
                  {
                     milis = 0;
                  }
                  isD = 1;
                  break;
               }
            }
            renderClock();
            delay_ms(200);
            while(input(pin_D5));
         }
         if(d4)
         {
            // Reduce Selected Digit
            switch(selection)
            {
               case 0:
               {
                  // HOUR
                  if(hour == 0)
                  {
                     hour = 23;
                  }
                  else
                  {
                     hour--;
                  }
                  isE = 1;
                  isA = 1;
                  break;
               }
               case 1:
               {
                  // MINUTE
                  if(minute == 0)
                  {
                     minute = 59;
                  }
                  else
                  {
                     minute--;
                  }
                  isB = 1;
                  break;
               }
               case 2:
               {
                  // SECOND
                  if(second == 0)
                  {
                     second = 59;
                  }
                  else
                  {
                     second--;
                  }
                  isC = 1;
                  break;
               }
               case 3:
               {
                  // MILISECOND
                  if(milis == 0)
                  {
                     milis = 9;
                  }
                  else
                  {
                     milis--;
                  }
                  isD = 1;
                  break;
               }
            }
            renderClock();
            delay_ms(200);
            while(input(pin_D4));
         }
      }
      delay_ms(50);
   }
}

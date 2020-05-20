#include <main.h>
int8 tick = 0;

// CURRENT TIME
int8 milis = 0;
int8 second = 0;
int8 minute = 0;
int8 hour = 0;

// ALARM
int8 alarmMin = 30;
int8 alarmHour = 12;

// VARIABLES
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
int1 isHourChanged = 1;
int1 isMinChanged = 1;
int1 isSecChanged = 1;
int1 isMilChanged = 1;

// SETTINGS
int1 d4 = 0; // button 4
int1 d5 = 0; // button 3
int1 d6 = 0; // button 2
int1 d7 = 0; // button 1

//
int8 selection = 0;
int8 mode = 0;

void setActive(int1 state)
{
	output_bit(pin_C7, state);
}

void printDisplay(int8 id, int8 value)
{
	// id = 0 Hour Tens
	// id = 1 Hour Ones
	// id = 2 Min Tens
	// id = 3 Min Ones
	// id = 4 Sec Tens
	// id = 5 Sec Ones
	// id = 6 Utility
	
	// Tüm bir bloğu değiştirmek yerine bitwise olarak yazmayı tercih ettim
	// böylece bir block içerisinde kolaylıkla hem output hem de input işlemi
	// yapabiliyorum.Örn C için 0,1,2,3,4,5,6 output  C7 ise input
	// eğer output_c olarak tanımlasaydım. tüm 8 biti de output yapacaktı.
	
	switch(id)
	{
		case 0:
		{
			// HOUR TENS
			if(value >= 0 && value <= 2)
			{
				output_bit(pin_E1, (value >> 1) & 1);
				output_bit(pin_E0, (value & 1));
			}
			break;
		}
		case 1:
		{
			// HOUR ONES
			if(value >= 0 && value <= 9)
			{
				output_bit(pin_A3, (value >> 3) & 1);
				output_bit(pin_A2, (value >> 2) & 1);
				output_bit(pin_A1, (value >> 1) & 1);
				output_bit(pin_A0, (value & 1));
			} 
			break;
		}
		case 2:
		{
			// MINUTE TENS
			if(value >= 0 && value <= 5)
			{
				output_bit(pin_B6, (value >> 2) & 1);
				output_bit(pin_B5, (value >> 1) & 1);
				output_bit(pin_B4, (value & 1));
			}
			break;
		}
		case 3:
		{
			// MINUTE ONES
			if(value >= 0 && value <= 9)
			{
				output_bit(pin_B3, (value >> 3) & 1);
				output_bit(pin_B2, (value >> 2) & 1);
				output_bit(pin_B1, (value >> 1) & 1);
				output_bit(pin_B0, (value & 1));
			}
			break;
		}
		case 4:
		{
			// SECOND TENS
			if(value >= 0 && value <= 5)
			{
				output_bit(pin_C6, (value >> 2) & 1);
				output_bit(pin_C5, (value >> 1) & 1);
				output_bit(pin_C4, (value & 1));
			} 
			break;
		}
		case 5:
		{
			// SECOND ONES
			if(value >= 0 && value <= 9)
			{
				output_bit(pin_C3, (value >> 3) & 1);
				output_bit(pin_C2, (value >> 2) & 1);
				output_bit(pin_C1, (value >> 1) & 1);
				output_bit(pin_C0, (value & 1));
			}
			break;
		}
		case 6:
		{
			// UTILITY
			if(value >= 0 && value <= 15)
			{
				output_bit(pin_D3, (value >> 3) & 1);
				output_bit(pin_D2, (value >> 2) & 1);
				output_bit(pin_D1, (value >> 1) & 1);
				output_bit(pin_D0, (value & 1));
			}
			break;
		}
		default:
		{
			// Do nothing!
			break;
		}
	}
}

void printHourDigits(int8 value)
{
	// Saat değerini birler ve onlar basamağına
	// ayırıp ayrı ayrı yazdırıyor
	if(value >= 0 && value <= 23)
	{
		hrOnes = value % 10;
		hrTens = (value - hrOnes) / 10;
		printDisplay(0, hrTens);
		printDisplay(1, hrOnes);
	}
}

void printMinuteDigits(int8 value)
{
	if(value >= 0 && value <= 59)
	{
		minOnes = value % 10;
		minTens = (value - minOnes) / 10;
		printDisplay(2, minTens);
		printDisplay(3, minOnes);
	}
}

void printSecondDigits(int8 value)
{
	if(value >= 0 && value <= 59)
	{
		secOnes = value % 10;
		secTens = (value - secOnes) / 10;
		printDisplay(4, secTens);
		printDisplay(5, secOnes);
	}
}

void printClock()
{
	if(isMilChanged == 1)
    {
		isMilChanged = 0;
		printDisplay(6, milis);
    }
    if(isSecChanged == 1)
    {
		isSecChanged = 0;
		printSecondDigits(second);
    }
    if(isMinChanged == 1)
    {
		isMinChanged = 0;
		printMinuteDigits(minute);
    }
    if(isHourChanged == 1)
    {
		isHourChanged = 0;
		printHourDigits(hour);
    }
}
void printAlarm()
{
	printHourDigits(alarmHour);
	printMinuteDigits(alarmMin);
	printSecondDigits(0);
}

void checkAlarm()
{
	if(alarmHour - hour == 0 && alarmMin - minute == 0)
	{
		setActive(TRUE);
	}
	else
	{
		setActive(FALSE);
	}
}	

#INT_TIMER1
void TIMER_isr(void)
{
    if(mode == 0)
    {
		tick++;
		if(tick >= 6)
		{
			tick = 0;
			isMilChanged = 1;
			milis++;
			if(milis >= 10)
			{
				milis = 0;
				isSecChanged = 1;
				second++;
				if(second >= 60)
				{
					second = 0;
					isMinChanged = 1;
					minute++;
					if(minute >= 60)
					{
						minute = 0;
						isHourChanged = 1;
						hour++;
						if(hour >= 24)
						{
							hour = 0;
						}
					}
				}
			}
		}
		printClock();
	}   
}

void main()
{   
	// SETTINGS
	setup_timer_1(T1_INTERNAL | T1_DIV_BY_1);
	set_timer1(77);
	clear_interrupt(INT_TIMER1);
	enable_interrupts(INT_TIMER1);
	enable_interrupts(GLOBAL);
	setActive(FALSE);

	while(TRUE)
	{
		// GET ALL SETTINGS
		d4 = input(pin_D4); //
		d5 = input(pin_D5);
		d6 = input(pin_D6);
		d7 = input(pin_D7);
		//
		if(d7)
		{
			// CLOCK / SETTING / ALARM
			mode++;
			if(mode > 2)
			{
				mode = 0;
				isHourChanged = 1;
				isMinChanged = 1;
				isSecChanged = 1;
				isMilChanged = 1;
				printClock();
			}
			//
			switch(mode)
			{
				case 1:
				{
					// CHANGE CLOCK
					printDisplay(6, 12); // 12 = 0xC = CLOCK
					break;
				}
				case 2:
				{
					// CHANGE ALARM
					printDisplay(6, 10); //10 = 0xA = ALARM
					printAlarm();
					break;
				}
				default:
				{
					break;
				}
			}
			//
			selection = 0;
			setActive(FALSE);
			delay_ms(100);
			while(input(pin_D7));
		}
		if(d6)
		{
			// SELECT DIGIT
			if(mode != 0)
			{
				selection++;
				if(mode == 1)
				{
					if(selection >= 3)
					{
						selection = 0;
					}
				}
				else if(mode == 2)
				{
					if(selection >= 2)
					{
						selection = 0;
					}
				}
			}
			delay_ms(100);
			while(input(pin_D6));
		}
		if(d5)
		{
			// Increase Selected Digit
			if(mode == 1)
			{
				// CLOCK
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
						isHourChanged = 1;
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
						isMinChanged = 1;
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
						isSecChanged = 1;
						break;
					}					
				}
				printClock();
			}
			else if(mode == 2)
			{
				// ALARM
				switch(selection)
				{
					case 0:
					{
						// HOUR
						alarmHour++;
						if(alarmHour >= 24)
						{
							alarmHour = 0;
						}
						break;
					}
					case 1:
					{
						// MINUTE
						alarmMin++;
						if(alarmMin >= 60)
						{
							alarmMin = 0;
						}
						break;
					}
				}
				printAlarm();
			}
			delay_ms(100);
			while(input(pin_D5));
		}
		if(d4)
		{
			// Reduce Selected Digit
			if(mode == 1)
			{
				// CLOCK
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
						isHourChanged = 1;
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
						isMinChanged = 1;
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
						isSecChanged = 1;
						break;
					}
				}
				printClock();
			}
			else if(mode == 2)
			{
				// ALARM
				switch(selection)
				{
					case 0:
					{
						// HOUR
						if(alarmHour == 0)
						{
							alarmHour = 23;
						}
						else
						{
							alarmHour--;
						}
						break;
					}
					case 1:
					{
						// MINUTE
						if(alarmMin == 0)
						{
							alarmMin = 59;
						}
						else
						{
							alarmMin--;
						}
						break;
					}
				}
				printAlarm();
			}
			delay_ms(100);
			while(input(pin_D4));
		}
		checkAlarm();
		delay_ms(50);
	}
}
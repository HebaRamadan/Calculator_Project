/*
 * main.c
 *
 *  Created on: Dec 3, 2020
 *      Author: Heba Ramadan
 */
#include "../Lib/Std_types.h"
#include "../Lib/Bit_math.h"
#undef F_CPU
#define F_CPU 8000000
#include <util/delay.h>
#include "Keypad_int.h"
#include "Lcd_int.h"
#include "Dio_int.h"

u8 arr[16];
f32 oprand[16];
f32 Result =0;
u8 Result_Flag = 0;
u8 Delete_Flag = 0;
u8 Syntax_Flag = 0;
u8 Arrow_Flag  = 0;
u8 Zero_Flag   = 0;
f32 num = 0;

u8 R[8]={0b00000, 0b01100, 0b01110, 0b01111, 0b01110, 0b01100, 0b00000, 0b00000};
u8 L[8]={0b00000, 0b00110, 0b01110, 0b11110, 0b01110, 0b00110, 0b00000, 0b00000};

int main(void)
{
	Lcd_vidInit();
	Keypad_vidInit();

	for(u8 i=0; i<16; i++)
	{
		arr[i] = 0xff;
	}
	for(u8 i=0; i<16; i++)
	{
		oprand[i] = 0xff;
	}

	u8 Counter_array = 0;
	u8 Counter_Left  = 0;
	u8 Counter_Right = 0;
	u8 Counter_Del   = 0;
	u8 counter = 0;

	while(1)
	{
		u8 Key = Keypad_u8GetKey();

		if(Key != KEYPAD_u8NO_KEY)
		{
			if( (Key >= 0) && (Key <= 9) )
			{
				if(Delete_Flag == 1)
				{
					Lcd_vidWriteNumber(Key);
					arr[Counter_array - Counter_Left + Counter_Right] = Key;
					Lcd_vidSendCommand(0x10);

					Delete_Flag  = 0;
					Counter_Del  = 0;
					_delay_ms(200);
				}
				else
				{
					Lcd_vidWriteNumber(Key);
					arr[Counter_array] = Key;
					Counter_array++;
				}

			}
			else if((Key =='+')||(Key =='-')||(Key =='*')||(Key =='/')||(Key =='^'))
			{
				Lcd_vidWriteChar(Key);

				arr[Counter_array] = Key;
				Counter_array++;
			}
			else if(Key == 'R')
			{
				if(Arrow_Flag == 1)
				{
					Lcd_vidSendCommand(0x01);
					u8 i =0;
					while(arr[i] != 0xff)
					{
						if(arr[i] >= 0 && arr[i] <= 9)
						{
							Lcd_vidWriteNumber(arr[i]);
						}
						else if((arr[i] =='+')||(arr[i] =='-')||(arr[i] =='*')||(arr[i] =='/') ||(arr[i] =='^'))
						{
							Lcd_vidWriteChar(arr[i]);
						}
						i++;
					}
					Arrow_Flag  = 0;
				}
				else
				{
					Lcd_vidSendCommand(0x14);
					Counter_Right++;
				}
			}
			else if(Key == 'L')
			{
				if(Arrow_Flag == 1)
				{
					Lcd_vidSendCommand(0x01);
					u8 i =0;
					while(arr[i] != 0xff)
					{
						if(arr[i] >= 0 && arr[i] <= 9)
						{
							Lcd_vidWriteNumber(arr[i]);
						}
						else if((arr[i] =='+')||(arr[i] =='-')||(arr[i] =='*')||(arr[i] =='/')||(arr[i] =='^'))
						{
							Lcd_vidWriteChar(arr[i]);
						}
						i++;
					}
					Arrow_Flag  = 0;
				}
				else
				{
					Lcd_vidSendCommand(0x10);
					Counter_Left++;
				}

			}
			else if(Key == 'D')
			{
				Lcd_vidWriteChar(' ');
				Delete_Flag = 1;
				Lcd_vidSendCommand(0x10);
				Counter_Del++;
				if(Counter_Del == 2)
				{
					arr[Counter_array - Counter_Left + Counter_Right] = ' ';
					Counter_Del = 0;
				}


			}
			else if(Key == 'C')
			{
				Lcd_vidSendCommand(0x01);

				for(u8 i=0; i<16; i++)
				{
					arr[i] = 0xff;
				}
				for(u8 i=0; i<16; i++)
				{
					oprand[i] = 0xff;
				}
				Counter_Left  = 0;
				Counter_Right = 0;
				Counter_array = 0;
				Result_Flag   = 0;
				Zero_Flag     = 0;


			}
			else if(Key == '=')
			{
				Lcd_vidWriteChar(Key);
				Result_Flag   = 1;
				Counter_Left  = 0;
				Counter_Right = 0;
			}

			_delay_ms(300);
		}

		if(Result_Flag == 1)
		{
			u8 i = 0;
			while(arr[i] != 0xff)
			{
				if((arr[0] =='+')||(arr[0] =='-')||(arr[0] =='*')||(arr[0]=='/')||(arr[0]=='^'))
				{
					Syntax_Flag = 1;
					Lcd_vidStorePattern(R , 0);
					Lcd_vidStorePattern(L , 1);
					Lcd_vidSendCommand(0x01);
					Lcd_vidSendString("Syntax Error");
					_delay_ms(500);
					Lcd_vidSendCommand(0x01);
					break;
				}
				else if(arr[i+1] == 0xff)
				{
					if((arr[i] =='+')||(arr[i] =='-')||(arr[i] =='*')||(arr[i]=='/')||(arr[i]=='^'))
					{
						Syntax_Flag = 1;
						Lcd_vidStorePattern(R , 0);
						Lcd_vidStorePattern(L , 1);
						Lcd_vidSendCommand(0x01);
						Lcd_vidSendString("Syntax Error");
						_delay_ms(500);

						Lcd_vidSendCommand(0x01);
						break;
					}
				}
				else if((arr[i] =='+')||(arr[i] =='-')||(arr[i] =='*')||(arr[i]=='/')||(arr[i]=='^'))
				{
					if((arr[i+1] =='+')||(arr[i+1] =='-')||(arr[i+1] =='*')||(arr[i+1]=='/')||(arr[i+1]=='^'))
					{
						Syntax_Flag = 1;
						Lcd_vidStorePattern(R , 0);
						Lcd_vidStorePattern(L , 1);
						Lcd_vidSendCommand(0x01);
						Lcd_vidSendString("Syntax Error");
						_delay_ms(500);

						Lcd_vidSendCommand(0x01);
						break;
					}
				}
				else
				{
					Syntax_Flag = 0;
				}
				i++;
			}

			if(Syntax_Flag ==1)
			{

				Lcd_vidSendString("[AC]  :Cancel");
				Lcd_vidSendCommand(0xC0);
				Lcd_vidWriteChar('[');
				Lcd_vidWriteChar(1);
				Lcd_vidWriteChar(']');
				Lcd_vidWriteChar('[');
				Lcd_vidWriteChar(0);
				Lcd_vidWriteChar(']');
				Lcd_vidSendString(":Goto");


				Result_Flag = 0;
				Arrow_Flag  = 1;
			}
			else
			{
				u8 i = 0;
				u8 j = 0;
				num  = 0;
				while(arr[i] != 0xff)
				{
					if(arr[i]>=0  && arr[i] <=9)
					{
						num = num *10;
						num = num + arr[i];
						oprand[j] = num;
					}
					else if((arr[i] =='+')||(arr[i] =='-')||(arr[i] =='*')||(arr[i] =='/')||(arr[i] =='^'))
					{
						j++;
						oprand[j] = arr[i];
						j++;
						num = 0 ;

					}
					i++;
				}

				counter = 0;
				while(oprand[counter] != 0xff)
				{
					if((oprand[counter] =='/'))
					{
						if(oprand[counter + 1] == 0)
						{
							Lcd_vidSendCommand(0x01);
							Lcd_vidSendString("UDEFINED......");
							Zero_Flag = 1;
							Result_Flag = 0;

						}
					}
					counter++;
				}

				if(Zero_Flag != 1)
				{
					counter = 0;
					while(oprand[counter] != 0xff)
					{
						f32 power = oprand[counter+1] ;
						f32 base  = 1;

						if(oprand[counter] == '^')
						{
							for(f32 i=1 ; i<=power; i++)
							{
								base = base * oprand[counter-1] ;
							}
							oprand[counter]   = ' ';
							oprand[counter-1] = ' ';
							oprand[counter+1] = base;

						}
						counter++;
					}

					counter = 0;
					u8 counter_1 = 0;
					while(oprand[counter] != 0xff)
					{
						f32 temp = oprand[counter];
						if(temp != ' ')
						{
							oprand[counter_1] = temp ;
							counter_1++;
						}
						counter++;
					}
					for(u8 i= counter_1 ; i<16; i++)
					{
						oprand[i] = 0xff;
					}

					counter = 0;
					while(oprand[counter] != 0xff)
					{

						if(oprand[counter] == '*')
						{

							oprand[counter+1] =  oprand[counter-1] * oprand[counter + 1];
							oprand[counter]   = ' ';
							oprand[counter-1] = ' ';
						}

						else if(oprand[counter] == '/')
						{
							oprand[counter+1] =  oprand[counter-1] / oprand[counter + 1];
							oprand[counter]   = ' ';
							oprand[counter-1] = ' ';
						}

						counter++;
					}
					counter   = 0;
					counter_1 = 0;
					while(oprand[counter] != 0xff)
					{
						f32 temp = oprand[counter];
						if(temp != ' ')
						{
							oprand[counter_1] = temp ;
							counter_1++;
						}
						counter++;
					}
					for(u8 i= counter_1 ; i<16; i++)
					{
						oprand[i] = 0xff;
					}
					counter = 0;
					while(oprand[counter] != 0xff)
					{

						if(oprand[counter] == '-')
						{

							oprand[counter+1] =  oprand[counter-1] - oprand[counter + 1];
							oprand[counter]   = ' ';
							oprand[counter-1] = ' ';
						}

						else if(oprand[counter] == '+')
						{
							oprand[counter+1] =  oprand[counter-1] + oprand[counter + 1];
							oprand[counter] = ' ';
							oprand[counter-1] = ' ';
						}

						counter++;
					}
					Lcd_vidSendCommand(0xc0);
					counter = 0;
					while(oprand[counter] != 0xff)
					{
						if(oprand[counter]!= ' ')
						{
							Result = oprand[counter];
						}
						counter++;
					}
					if(Result < 0)
					{
						Result = Result *(-1);
						Lcd_vidWriteChar('-');
					}
					if((Result - (u32)Result) ==0)
					{
						Lcd_vidWriteNumber(Result);
					}
					else
					{
						Lcd_vidWriteNumber(Result);
						Result = (Result - (u32)Result);
						Result = Result * 100;
						Lcd_vidWriteChar('.');
						Result = (u32)Result % 100;
						Lcd_vidWriteNumber(Result);
					}
				}
				Result_Flag = 0;
			}
		}

	}

	return 0;
}



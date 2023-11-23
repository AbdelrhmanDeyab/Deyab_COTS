#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include <util/delay.h>
#include "ErrType.h"

#include "DIO_interface.h"

#include "CLCD_interface.h"
#include "CLCD_prv.h"
#include "CLCD_cfg.h"
#if CLCD_u8BIT_MODE==FOUR_BIT
static void voidSetHalfDataPort(uint8 Copy_u8Data)
{
	DIO_u8SetPinValue(CLCD_u8DATA_PORT,CLCD_u8D4_PIN,GET_BIT(Copy_u8Data,0));
	DIO_u8SetPinValue(CLCD_u8DATA_PORT,CLCD_u8D5_PIN,GET_BIT(Copy_u8Data,1));
	DIO_u8SetPinValue(CLCD_u8DATA_PORT,CLCD_u8D6_PIN,GET_BIT(Copy_u8Data,2));
	DIO_u8SetPinValue(CLCD_u8DATA_PORT,CLCD_u8D7_PIN,GET_BIT(Copy_u8Data,3));
}
#endif

static void voidSendEnablePulse(void)
{
	/* send enable pulse*/
	DIO_u8SetPinValue(CLCD_u8CTRL_PORT, CLCD_u8E_PIN, DIO_u8PIN_HIGH);
	_delay_ms(2);

	/* return it to low*/
	DIO_u8SetPinValue(CLCD_u8CTRL_PORT, CLCD_u8E_PIN, DIO_u8PIN_LOW);

}

void CLCD_voidSendCmd(uint8 Copy_u8Cmd)
{
	/* set RS pin to low for command*/
	DIO_u8SetPinValue(CLCD_u8CTRL_PORT,CLCD_u8RS_PIN,DIO_u8PIN_LOW);
	/* set RW pin to low for writing*/
#if CLCD_u8RW_CONN_STS== DIO_CONNECTED
	DIO_u8SetPinValue(CLCD_u8CTRL_PORT,CLCD_u8RW_PIN,DIO_u8PIN_LOW);
#endif

#if CLCD_u8BIT_MODE==EIGHT_BIT
	/* send the command*/
	DIO_u8SetPortValue(CLCD_u8DATA_PORT,Copy_u8Cmd);
	voidSendEnablePulse();

#elif CLCD_u8BIT_MODE==FOUR_BIT
	/*SEND THE 4 MOST SIGNIFICATION BITS OF THE COMMAND FIRST*/
 voidSetHalfDataPort(Copy_u8Cmd>>4);
 voidSendEnablePulse();
 voidSetHalfDataPort(Copy_u8Cmd);
 voidSendEnablePulse();

#else
#error wrong CLCD_u8BIT_MODE configuration input
#endif


}

void CLCD_voidSendData(uint8 Copy_u8Data)
{
	/* set RS pin to high for data*/
		DIO_u8SetPinValue(CLCD_u8CTRL_PORT,CLCD_u8RS_PIN,DIO_u8PIN_HIGH);
	/* set RW pin to low for writing*/
#if CLCD_u8RW_CONN_STS== DIO_CONNECTED
		DIO_u8SetPinValue(CLCD_u8CTRL_PORT,CLCD_u8RW_PIN,DIO_u8PIN_LOW);
#endif

#if CLCD_u8BIT_MODE==EIGHT_BIT
	/* send the Data*/
	DIO_u8SetPortValue(CLCD_u8DATA_PORT,Copy_u8Data);
	voidSendEnablePulse();

#elif CLCD_u8BIT_MODE==FOUR_BIT
	/*SEND THE 4 MOST SIGNIFICATION BITS OF THE COMMAND FIRST*/
 voidSetHalfDataPort(Copy_u8Data>>4);
 voidSendEnablePulse();
 voidSetHalfDataPort(Copy_u8Data);
 voidSendEnablePulse();

#else
#error wrong CLCD_u8BIT_MODE configuration input
#endif



}

void CLCD_voidInit(void)
{
	/*wait for more than 30 ms after power on*/
	_delay_ms(40);
	/*function is set command : 2 lines and font size :5x7 */
#if CLCD_u8BIT_MODE== EIGHT_BIT
	CLCD_voidSendCmd(0b00111000);
#elif CLCD_u8BIT_MODE== FOUR_BIT

	voidSetHalfDataPort(0b0010);
	voidSendEnablePulse();
	voidSetHalfDataPort(0b0010);
	voidSendEnablePulse();
	voidSetHalfDataPort(0b1000);
	voidSendEnablePulse();

#endif
	/*display on off control :display on , cursor off , blink cursor off*/
	CLCD_voidSendCmd(0b00001100);
    /*clear display*/
	CLCD_voidSendCmd(1);
	
}

uint8 CLCD_u8SendString(const char* Copy_pchString)
{
	uint8 Local_u8ErrorState= OK;

	if(Copy_pchString!=NULL)
	{
		uint8 Local_u8Iterator=0;
		while(Copy_pchString[Local_u8Iterator]!='\0')
		{
			CLCD_voidSendData(Copy_pchString[Local_u8Iterator]);
			Local_u8Iterator++;

		}

	}
	else
	{
		Local_u8ErrorState= NULL_PTR_ERR;
	}
	return Local_u8ErrorState;
}

void CLCD_voidSendNumber(sint32 Copy_s32Number)
{
	char Local_chNumber[10];
	uint8 Local_u8RightDigit, Local_u8Counter1=0;
	sint8 Local_s8Counter2;

   if(Copy_s32Number==0)
   {
	   CLCD_voidSendData('0');
	   return;
   }
   else if (Copy_s32Number<0)
   {
	   Copy_s32Number*=-1;
	   CLCD_voidSendData('-');

   }
   while(Copy_s32Number!=0)
   {
   Local_u8RightDigit=(uint8)((uint32)Copy_s32Number %10); /*get the right most digit*/

   Copy_s32Number/=10; /* remove the right most digit*/

   Local_chNumber[Local_u8Counter1]= Local_u8RightDigit+ '0';/*CONVERT TO ASCII*/

   Local_u8Counter1++;

   }
   for (Local_s8Counter2=(sint8)Local_u8Counter1-1; Local_s8Counter2>=0;Local_s8Counter2-- )
   {
	   CLCD_voidSendData(Local_chNumber[(uint8)Local_s8Counter2]);
   }
}
void CLCD_voidGoToXY(uint8 Copy_u8XPos,uint8 Copy_u8YPos )
{
	uint8 Local_u8Address ;
	if(Copy_u8YPos==0u)
	{
		Local_u8Address=Copy_u8XPos;
	}
	else if (Copy_u8YPos==1u)
	{
		Local_u8Address=Copy_u8XPos+ 0x40;
	}
	//Local_u8Address= 0x40*Copy_u8YPos +Copy_u8XPos;
	/*Set bit 7 for setDDRAM address command*/
	SET_BIT(Local_u8Address,7);

	/*execute set DDRAM address command*/
	CLCD_voidSendCmd(Local_u8Address);
}

uint8 CLCD_u8SendSpecialCharacter(uint8 Copy_u8LocationNum,uint8* Copy_pu8Pattern,uint8 Copy_u8XPos,uint8 Copy_u8YPos)
{
	uint8 Local_u8ErrorState = OK;

	if(Copy_pu8Pattern !=NULL)
	{
		uint8 Local_u8Counter;

		uint8 Local_u8CGRAMAddress = Copy_u8LocationNum*8;
		/* set bit 6 for CGRAM Address command*/
		SET_BIT(Local_u8CGRAMAddress,6u);

		/* set CGRAM address*/
		CLCD_voidSendCmd(Local_u8CGRAMAddress);
		/* write the input pattern inside CGRAM*/

		for(Local_u8Counter=0u;Local_u8Counter<8u;Local_u8Counter++)
		{
			CLCD_voidSendData(Copy_pu8Pattern[Local_u8Counter]);
		}

		/*Go back to DDRAM*/
		CLCD_voidGoToXY(Copy_u8XPos,Copy_u8YPos);


		/*Display the special pattern inside CGRAM*/
		CLCD_voidSendData(Copy_u8LocationNum);


	}
	else
	{
		Local_u8ErrorState= NULL_PTR_ERR;
	}
	return Local_u8ErrorState;
}

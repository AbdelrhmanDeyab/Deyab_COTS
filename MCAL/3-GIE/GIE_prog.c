#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "GIE_reg.h"
#include "GIE_interface.h"
void GIE_voidEnableGlobal(void)
{
	__asm volatile("SEI");             /*inline assembly: set I-BIT*/
	/*SET_BIT(SREG,SREG_I);*/
}
void GIE_voidDisableGlobal(void)
{
	__asm volatile("CLI");             /*inline assembly: clear I-BIT*/
	/*CLR_BIT(SREG,SREG_I);*/
}

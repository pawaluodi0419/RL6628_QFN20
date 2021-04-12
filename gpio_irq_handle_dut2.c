/*
 * gpio_irq_handle.c
 *
 *  Created on: 2017-6-19
 *      Author: kay_yin
 */

#include "platform.h"


void XGPIO_dut2_1_Intrq_Handle(){
	u8 i;
	u8 j;
	u8 FT2Start_Cnt=0;
	u8 FT2StartHSK=0;
	u8 FT2Startsignal=0;
	u8 START01_GPIO22;
	u32 ft1_config_scan=0;
	u8 ft1_config_scan_count=0x1ff;
	u8 gpio26_output_value=0;
	/*
	 *if the ft2 start last for ? us ,set the ft2 start flag
	 */
	for(i=0;i<10;i++){
		if(XGpio_Dutx_ReadBit(XPAR_AXI_GPIO_dut2_1_BASEADDR,0,21) == 1){
			FT2Start_Cnt++;
		}
	}
	FT2StartHSK = (FT2Start_Cnt == 10);

	if(FT2StartHSK == 1)
	{
		if(dut2.g_start00_pulsh_count < 3)
		{
		dut2.g_start00_pulsh_count++;
		}
	else /*if(dut2.g_start00_pulsh_count == 7)*/
	{
		dut2.g_FT2StartSignal=1;
		dut2.g_start00_pulsh_count = 0;
	}
	}
	//xil_printf("dut2.FT2Start_Cnt = %x\r\n",FT2Start_Cnt);
	//xil_printf("dut2.FT2StartHSK = %x\r\n",FT2StartHSK);
	//xil_printf("dut2.g_start00_pulsh_count = %x\r\n",dut2.g_start00_pulsh_count);

	//xil_printf("dut2.g_FT2StartSignal = %x\r\n",dut2.g_FT2StartSignal);



	if((dut2.g_FT2StartSignal==1) && (dut2.g_uartPatternNum == 0x0f))
	{

		dut2.g_dut_start_ready=0x11;
	}

	else if((dut2.g_FT2StartSignal==1) && (dut2.g_uartPatternNum == 0x0e))
	{
		dut2.g_dut_start_ready=0x11;
	}
	else if((dut2.g_FT2StartSignal==1) && (dut2.g_uartPatternNum == 0x0d))
	{
		dut2.g_dut_start_ready=0x11;
	}
	else if((dut2.g_FT2StartSignal==1) && (dut2.g_uartPatternNum == 0x00))
	{

		if(dut2.g_result_fail != 0x00)
		{
				dut2.g_dut_start_ready=0x11; //fail time enough,wait for next test
				dut2.g_result_fail = 0x00;


		}
		else if(dut2.g_result_fail == 0x00)
		{
			dut2.g_dut_start_ready=0x01;
		}

	}
	else if((dut2.g_FT2StartSignal==1) && (dut2.g_uartPatternNum > 0) && (dut2.g_uartPatternNum <0x0e))
	{
		if(dut2.g_result_fail != 0x00)
		{

				dut2.g_dut_start_ready=0x11; //fail time enough,wait for next test
				dut2.g_result_fail = 0;

		}
		else if(dut2.g_result_fail == 0x00)
		{
			dut2.g_dut_start_ready=0x01;
		}
	}

	dut2.g_FT2StartSignal = 0;

	//clear the GPIO_dut2_1 interrupt
	XGpio_WriteReg(XPAR_AXI_GPIO_dut2_1_BASEADDR,XGPIO_ISR_OFFSET,XGPIO_IR_CH1_MASK);
	//xil_printf("dut2.g_dut_start_ready = %x\r\n\r\n",dut2.g_dut_start_ready);

	////xil_printf("dut2.g_result_fail = %x\r\n",dut2.g_result_fail);
	////xil_printf("dut2.g_result_fail_tmrcount = %x\r\n",dut2.g_result_fail_tmrcount);
	////xil_printf("dut2.g_uartPatternNum = %x\r\n",dut2.g_uartPatternNum);
	//xil_printf("dut2.g_dut_start_ready = %x\r\n\r\n",dut2.g_dut_start_ready);

}



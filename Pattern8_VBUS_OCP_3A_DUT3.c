#include "platform.h"
#include "Pattern7_VBUS_OCP.h"
#include "vbus_ocp_calibration.h"

u8 _by_Pattern8_VBUS_OCP_3A_dut3()
{
	u8 i;

	switch(dut3.g_pattern_step)
	{
	//3A vbus_ocp trimmng
	//loader setup 3.467A
	//enable 2 nmos:g_vbusocp_0p9a_nmos_control and g_vbusocp_1p5a_nmos_control
	case 0x0000:
	{
		dut3.g_vbusocp_board_en = 1;
		dut3.g_vbusocp_0p9a_nmos_control = 0;
		dut3.g_vbusocp_1p5a_nmos_control = 1;
		dut3.g_vbusocp_3a_nmos_control = 1;
		_vbus_ocp_calibration_dut3();
		//xil_printf("dut3_g_calibration_offset = %x\r\n", dut3.g_calibration_offset);
		//xil_printf("dut3.g_vbusocp_step = %x\r\n",dut3.g_vbusocp_step);
		////xil_printf("dut3.g_pattern_step = %x\r\n",dut3.g_pattern_step);
		if(dut3.g_vbusocp_process_bit != vbusocp_process_done_pass)
		{

		}
		else
		{
			dut3.g_vbusocp_writeefuse_3a = dut3.g_vbus_ocp_calibrationdata_buf[1];
			dut3.g_vbusocp_process_bit = vbusocp_process_waiting;
			dut3.g_vbusocp_step=0;
			if((dut3.g_vbusocp_writeefuse_3a>VBUSOCP_Efuse_3a_Min)&&(dut3.g_vbusocp_writeefuse_3a<VBUSOCP_Efuse_3a_Max))
			{
				dut3.g_pattern_step++;
			}
			else
			{
				dut3.g_result_fail = 0x01;
				dut3.g_result_fail_tmrcount = 0xffff;
				xil_printf("dut3.g_vbusocp_writeefuse_3a = %x VBUSOCP_Efuse_3a_Min=%x VBUSOCP_Efuse_3a_Max=%x\r\n",dut3.g_vbusocp_writeefuse_3a,VBUSOCP_Efuse_3a_Min,VBUSOCP_Efuse_3a_Max);
				xil_printf("dut3_g_vbusocp_writeefuse_3a fail,not fit spec!\r\n");
			}
			//////xil_printf("dut3.g_vbusocp_step = %x\r\n",dut3.g_vbusocp_step);

		}
	    //xil_printf("dut3.g_vbusocp_step = %x\r\n",dut3.g_vbusocp_step);
		break;

	}

	case 0x0001:
	{
		for(i=1; i<31; i++)
		{
			dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
		}
		dut3.g_pattern_timer = 0xfff;
		dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
		dut3.g_pattern_step = 0x00;
		dut3.g_dut_pattern_status_buf[2]++;
		dut3.g_uartPatternEnable = 0x00;

		//this is control vbus board,base addr is dutx
		XGpio_WriteBit(XPAR_AXI_GPIO_dut3_1_BASEADDR,0,23, 0);//VBUS_OCP_EN Control
		XGpio_WriteBit(XPAR_AXI_GPIO_dut3_1_BASEADDR,0,26, 0);//1.5A_EN Control
		XGpio_WriteBit(XPAR_AXI_GPIO_dut3_1_BASEADDR,0,25, 0);//3A_EN Control
		if((dut3.g_vbusocp_3a_nmos_control ==1) &&(dut3.g_vbusocp_3a_en_status==1))
		{
			g_vbusocp_3a_status=0;
		}

		xil_printf(">>>>>>>>>>>> dut3_pattern8_vbus_ocp_3a_test_pass! <<<<<<<<<<<<\r\n");
		xil_printf("dut3_efuse_3a=0x%x\r\n\r\n",dut3.g_vbusocp_writeefuse_3a);

		dut3.g_uartPatternNum = (dut3.g_uartPatternNum++ & 0xff);
		//dut3.g_uartPatternNum = 0x0c;
		result_output_for_v50(XPAR_AXI_GPIO_dut3_1_BASEADDR,dut3.g_uartPatternNum);
		//xil_printf("dut3.g_uartPatternNum = %x\r\n\r\n",dut3.g_uartPatternNum);
		break;
	}
	}

	if(dut3.g_result_fail == 0x01)
	{
		dut3.g_ft2_test_done = 0x00;
		pattern_safe_state_dut3();	//add safe state

		XGpio_WriteBit(XPAR_AXI_GPIO_dut3_1_BASEADDR,0,23, 0);
		XGpio_WriteBit(XPAR_AXI_GPIO_dut3_1_BASEADDR,0,26, 0);
		XGpio_WriteBit(XPAR_AXI_GPIO_dut3_1_BASEADDR,0,25, 0);

		msdelay(2);
		if((dut3.g_vbusocp_3a_nmos_control ==1) &&(dut3.g_vbusocp_3a_en_status==1))
		{
			g_vbusocp_3a_status=0;
		}
		xil_printf("dut3_pattern8_fail! fail_pattern_step = %x    g_vbusocp_step=%x\r\n\r\n",dut3.g_pattern_step,dut3.g_vbusocp_step);
	}

	return 0;
}



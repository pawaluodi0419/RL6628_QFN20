/*
 * Pattern2_RP_dut3.c
 *
 *  Created on: 2018-05-23
 *      Author: ashley_su
 */



#include "platform.h"
#include "Pattern2_ADC_VMON.h"

//dut3 Pattern 2£ºRP Calibration and test
u8 _by_Pattern1_RC_Clock_dut3()
{
	u8 i;
	u16 clock_sel;
	u16 clock_frq_cnt_min;
	u16 clock_frq_cnt_max;
	u8 Buff_dut3_XGPIO_0[8];


	switch(dut3.g_pattern_step)
	{
	//S0_FPGA_RELAY8 output high
	case 0x0000:
	{
		//gpio17: low bit, gpio20: high bit
		XGpio_WriteBit(XPAR_AXI_GPIO_dut3_1_BASEADDR,0,17, 1);
		XGpio_WriteBit(XPAR_AXI_GPIO_dut3_1_BASEADDR,0,18, 0);
		XGpio_WriteBit(XPAR_AXI_GPIO_dut3_1_BASEADDR,0,19, 0);
		XGpio_WriteBit(XPAR_AXI_GPIO_dut3_1_BASEADDR,0,20, 0);
		msdelay(5);

		XGpio_WriteBit(XPAR_AXI_GPIO_dut3_1_BASEADDR,0,15, 1);
		msdelay(5);

		dut3.g_pattern_step++;
	}

	//write 0x1162 with 0x01
	case 0x0001:
	{
		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		dut3.g_pattern_smbus_control_buf[2] = 0x62;
		dut3.g_pattern_smbus_control_buf[3] = 0x11;
		dut3.g_pattern_smbus_control_buf[4] = 0x01;
		dut3.g_pattern_smbus_control_buf[5] = 0x01;
		smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);

		if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			for(i=1; i<31; i++)
			{
				dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
			}
			dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut3.g_pattern_step++;
		}
		////xil_printf("dut3_rp_g_pattern_step = %x\r\n", dut3.g_pattern_step);
		//////xil_printf("dut3_pattern2_write_0x1131_with_0x80\r\n");
		break;
	}


	//write 0x1131 with 0x80
	case 0x0002:
	{
		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		dut3.g_pattern_smbus_control_buf[2] = 0x31;
		dut3.g_pattern_smbus_control_buf[3] = 0x11;
		dut3.g_pattern_smbus_control_buf[4] = 0x01;
		dut3.g_pattern_smbus_control_buf[5] = 0x80;
		smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);

		if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			for(i=1; i<31; i++)
			{
				dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
			}
			dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut3.g_pattern_step++;
		}
		////xil_printf("dut3_rp_g_pattern_step = %x\r\n", dut3.g_pattern_step);
		//////xil_printf("dut3_pattern2_write_0x1131_with_0x80\r\n");
		break;
	}

	//write 0x1131 with 0xc0
	case 0x0003:
	{
		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		dut3.g_pattern_smbus_control_buf[2] = 0x31;
		dut3.g_pattern_smbus_control_buf[3] = 0x11;
		dut3.g_pattern_smbus_control_buf[4] = 0x01;
		dut3.g_pattern_smbus_control_buf[5] = 0xc0;
		smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);

		if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			for(i=1; i<31; i++)
			{
				dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
			}
			dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut3.g_pattern_step++;
		}
		////xil_printf("dut3_rp_g_pattern_step = %x\r\n", dut3.g_pattern_step);
		//////xil_printf("dut3_pattern2_write_0x1131_with_0xc0\r\n");
		break;
	}

	case 0x0004:
	{
		   dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readmem;
		   dut3.g_pattern_smbus_control_buf[2] = 0x31;
		   dut3.g_pattern_smbus_control_buf[3] = 0x11;
		   dut3.g_pattern_smbus_control_buf[4] = 0x01;


		   smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
		   ////xil_printf("dut3.g_smbus_style =%x\r\n", dut3.g_smbus_style);
		   ////xil_printf("dut3.g_result_fail =%x\r\n", dut3.g_result_fail);
		   if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		   {
			   break;
		   }
		   else
		   {
			   msdelay(10);
			   //xil_printf("dut3.g_pattern_smbus_control_buf[10] =%x\r\n", dut3.g_pattern_smbus_control_buf[10]);
			   //xil_printf("dut3.g_pattern_smbus_control_buf[11] =%x\r\n", dut3.g_pattern_smbus_control_buf[11]);

			   clock_sel = dut3.g_pattern_smbus_control_buf[10];
			   //xil_printf("dut3_clock_sel =%x\r\n", clock_sel);

			   msdelay(10);
			   //xil_printf("dut3_clock_sel =%x\r\n", clock_sel);
			   if(clock_sel == 0xe0)
			   {
				   dut3.g_clock_reread_timer = 10;
				   dut3.g_pattern_step++;
			   }
			   else
			   {
				   if(dut3.g_clock_reread_timer > 0)
				   {
					   dut3.g_clock_reread_timer--;
				   }
				   else
				   {
					   dut3.g_result_fail = 0x01;
					   dut3.g_result_fail_tmrcount = 0xffff;
				   }
			   }


			   for(i=1; i<31; i++)
			   {
				   dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
			   }
				   dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;

				   ////xil_printf("dut3_clock_sel =%x\r\n", clock_sel);
		   }
		   //xil_printf("dut3.g_result_fail =%x\r\n", dut3.g_result_fail);
		   //xil_printf("dut3_clock_sel =%x\r\n", clock_sel);
	break;
	}

	case 0x0005:
	{
		   dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readmem;
		   dut3.g_pattern_smbus_control_buf[2] = 0x32;
		   dut3.g_pattern_smbus_control_buf[3] = 0x11;
		   dut3.g_pattern_smbus_control_buf[4] = 0x02;


		   smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
		   if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		   {
			   break;
		   }
		   else
		   {
			   //xil_printf("dut3.g_pattern_smbus_control_buf[10] =%x\r\n", dut3.g_pattern_smbus_control_buf[10]);
			   //xil_printf("dut3.g_pattern_smbus_control_buf[11] =%x\r\n", dut3.g_pattern_smbus_control_buf[11]);

			   dut3.g_clock_frq_cnt = (dut3.g_pattern_smbus_control_buf[11]<<8 | dut3.g_pattern_smbus_control_buf[10]);
			   clock_frq_cnt_min = 0x0640 *0.8;
			   clock_frq_cnt_max = 0x0640 *1.2;
			   xil_printf("dut3.g_clock_frq_cnt=%x clock_frq_cnt_min=%x clock_frq_cnt_max=%x\r\n\r\n",dut3.g_clock_frq_cnt,clock_frq_cnt_min,clock_frq_cnt_max);
			   if((dut3.g_clock_frq_cnt > clock_frq_cnt_min) &&(dut3.g_clock_frq_cnt < clock_frq_cnt_max))
			   {
				   dut3.g_efuse_data_buf[17] = dut3.g_pattern_smbus_control_buf[10];
				   dut3.g_efuse_data_buf[18] = dut3.g_pattern_smbus_control_buf[11];

				   for(i=1; i<31; i++)
					   {
						   dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
					   }
			   	   	       dut3.g_pattern_timer = 0xfff;
						   dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
					       dut3.g_pattern_step = 0x00;
					       dut3.g_dut_pattern_status_buf[2]++;
					       dut3.g_uartPatternEnable = 0x00;
					       dut3.g_uartPatternNum = (dut3.g_uartPatternNum++ & 0xff);
					       //dut3.g_uartPatternNum = (dut3.g_uartPatternNum++ & 0xff);
					       //dut3.g_uartPatternNum = (dut3.g_uartPatternNum++ & 0xff);
					       //dut3.g_uartPatternNum = (dut3.g_uartPatternNum++ & 0xff);
					       result_output_for_v50(XPAR_AXI_GPIO_dut3_1_BASEADDR,dut3.g_uartPatternNum);
					       //xil_printf("dut3.g_uartPatternNum = %x\r\n\r\n",dut3.g_uartPatternNum);
						   xil_printf("dut3_pattern1_rc_clock_pass!     read_mem:0x1132 =0x%x 0x1133 =0x%x\r\n\r\n",dut3.g_efuse_data_buf[17],dut3.g_efuse_data_buf[18]);
						   //xil_printf("dut3.g_result_fail = %x\r\n",dut3.g_result_fail);
						   ////xil_printf("dut3_read_mem:0x1132 =0x%x 0x1133 =0x%x\r\n\r\n",dut3.g_pattern_smbus_control_buf[10],dut3.g_pattern_smbus_control_buf[11]);
						   //xil_printf("dut3.g_dut_start_ready = %x\r\n\r\n",dut3.g_dut_start_ready);
						   //xil_printf("dut3.g_result_fail = %x\r\n\r\n",dut3.g_result_fail);
						   //xil_printf("dut3.g_dut_pattern_status_buf[2] = %x\r\n\r\n",dut3.g_dut_pattern_status_buf[2]);
			   }

			   else
					{
						dut3.g_result_fail = 0x01;
						dut3.g_result_fail_tmrcount = 0xffff;
						xil_printf("dut3.g_result_fail = %x\r\n",dut3.g_result_fail);
					}

		   }

	break;
	}

	}
	if(dut3.g_result_fail == 0x01)
	{
		dut3.g_ft2_test_done = 0x00;
		pattern_safe_state_dut3();	//add safe state
		xil_printf("dut3_pattern1_fail! fail_step = %x\r\n\r\n",dut3.g_pattern_step);
	}
	//xil_printf("dut3_pattern1_rcclock_step = %x\r\n\r\n",dut3.g_pattern_step);
	return 0;
}


















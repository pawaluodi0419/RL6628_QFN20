/*
 * Pattern0_Setup_dut0.c
 *
 *  Created on: 2018-05-23
 *      Author: ashley_su
 */

#include "platform.h"

u8 _by_Pattern0_Setup_dut0()
{
	u8 i;
	u8 j;
	u8 Buff_dut0_XGPIO_0[10];
	u16 adc_code;
	u8 read_icstatus_data[15];
	static u8 efuse_datavalid;
	static u8 efuse_testvalid;
	static u8 efuse_data_valid;
	static u8 efuse_ldo3v3_valid;
	static u8 efuse_rp12k_valid;
	static u8 efuse_cc_valid;
	static u8 efuse_package_valid;
	u8 efuse_rp36k_valid;
	u8 efuse_writedata_valid;
	u32 start_smbus_runtime_count;
	u8 START01_GPIO22;
	u32 ft1_config_scan=0;
	u8 ft1_config_scan_count=0x1ff;

	u8  smbus_writedata_count;
	u8  smbus_writecmd_count;
	u8  smbus_writecmd_buf[30];
	u8  smbus_get_pinstatus_result = dut0.g_smbus_status_buf[2];

	switch(dut0.g_pattern_step)
	{
	//write pattern index and power on IC
	case 0x0000:
	{
		dut0.g_uartPatternNum = 0;

		//gpio17: low bit, gpio20: high bit
		XGpio_WriteBit(XPAR_AXI_GPIO_dut0_1_BASEADDR,0,17, 0);
		XGpio_WriteBit(XPAR_AXI_GPIO_dut0_1_BASEADDR,0,18, 0);
		XGpio_WriteBit(XPAR_AXI_GPIO_dut0_1_BASEADDR,0,19, 0);
		XGpio_WriteBit(XPAR_AXI_GPIO_dut0_1_BASEADDR,0,20, 0);
		msdelay(5);

		XGpio_WriteBit(XPAR_AXI_GPIO_dut0_1_BASEADDR,0,15, 0);
		msdelay(5);

		XGpio_2_WriteBit(0,dut0_FT2_PWR_CTRL_OFFSET,0x01);	//S0-PWR_EN: 控制S0-ATX_5V是否开出S0-5V（未经过采样电阻）
		msdelay(5);
		XGpio_WriteBit(XPAR_AXI_GPIO_dut0_1_BASEADDR,0,1, 1);	//S0-PWR_EN_2: 控制S0-5V是否开出S0-5V_IN/S0-VCON_IN
		msdelay(5);

		//gpio_data = XGpio_ReadReg(XPAR_AXI_GPIO_dut0_1_BASEADDR,XGPIO_DATA_OFFSET);
		//gpio_tri = XGpio_ReadReg(XPAR_AXI_GPIO_dut0_1_BASEADDR,XGPIO_TRI_OFFSET);
		//xil_printf("gpio_data = %x\r\n", gpio_data);
		//xil_printf("gpio_tri = %x\r\n", gpio_tri);

		xil_printf("dut0 IC power on!\r\n\r\n");
		dut0.g_pattern_step++;
	}

	case 0x0001:
	{
		XGpio_WriteBit(XPAR_AXI_GPIO_dut0_1_BASEADDR,0,16, 1);	//enable power switch
		msdelay(50);

		dut0.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		dut0.g_pattern_smbus_control_buf[2] = 0xf0;
		dut0.g_pattern_smbus_control_buf[3] = 0x02;
		dut0.g_pattern_smbus_control_buf[4] = 0x01;
		dut0.g_pattern_smbus_control_buf[5] = 0xaa;

		smbus0_irq_handle(dut0.g_pattern_smbus_control_buf);
		if(dut0.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			if(dut0.g_start_smbus_retry >0)
			{
				smbus_writedata_count = dut0.g_pattern_smbus_control_buf[4];
				smbus_writecmd_count = smbus_writedata_count + 5;
				smbus_writecmd_buf[0] = dut0.g_pattern_smbus_control_buf[1];
				smbus_writecmd_buf[1] = smbus_writedata_count+3;
				smbus_writecmd_buf[2] = dut0.g_pattern_smbus_control_buf[2];
				smbus_writecmd_buf[3] = dut0.g_pattern_smbus_control_buf[3];
				smbus_writecmd_buf[4] = dut0.g_pattern_smbus_control_buf[4];

				for(i=0; i<smbus_writedata_count; i++)
				{
					smbus_writecmd_buf[i+5] = dut0.g_pattern_smbus_control_buf[i+5];
				}
				//for(i=0;i<10;i++)
				//{
					msdelay(10);
					smbus_master_write_blockwrite(SMBUS_DEV0_ADDR, smbus_writecmd_buf, smbus_writecmd_count);
					msdelay(25);
					smbus_get_pinstatus_result = smbus_master_get_pinstate_dut0(SMBUS_DEV0_ADDR, smbus_get_pinstatus_result);
					msdelay(10);

					if(smbus_get_pinstatus_result == 1)
					{
						xil_printf("dut0_start_smbus_write_pass!\r\n\r\n");

						for(i=1; i<30; i++)
						{
							dut0.g_pattern_smbus_control_buf[i] = CLEAR_;
						}

						dut0.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
						dut0.g_pattern_step++;
						return;
					}
					//xil_printf("dut0.i = %x\r\n\r\n",i);
			   //}
			   //smbus_get_pinstatus_result = smbus_master_get_pinstate_dut0(SMBUS_DEV0_ADDR, smbus_get_pinstatus_result);
			   //xil_printf("dut0.smbus_get_pinstatus_result = %x\r\n",smbus_get_pinstatus_result);
			   dut0.g_start_smbus_retry--;
			}
		}

		else
		{
			for(i=1; i<30; i++)
			{
				dut0.g_pattern_smbus_control_buf[i] = CLEAR_;
			}
			dut0.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut0.g_pattern_step++;
		}

		break;
	}

	//write register 0x02f0 data 0xaa
	case 0x0002:
	{
		dut0.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		dut0.g_pattern_smbus_control_buf[2] = 0xf0;
		dut0.g_pattern_smbus_control_buf[3] = 0x02;
		dut0.g_pattern_smbus_control_buf[4] = 0x01;
		dut0.g_pattern_smbus_control_buf[5] = 0xaa;

		smbus0_irq_handle(dut0.g_pattern_smbus_control_buf);
		if(dut0.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			for(i=1; i<30; i++)
			{
				dut0.g_pattern_smbus_control_buf[i] = CLEAR_;
			}
			dut0.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut0.g_pattern_step++;
			msdelay(50);
		}

		break;
	}


	//get ic status
	//Rh_smbusgeticstatus slave addr 10
	//01 00 00 00 00 00 DA 0B 58 65
	case 0x0003:
	{
		dut0.g_pattern_smbus_control_buf[1] = smbus_cmd_type_geticstatus;
		dut0.g_pattern_smbus_control_buf[2] = 0x00;
		dut0.g_pattern_smbus_control_buf[3] = 0x00;
		dut0.g_pattern_smbus_control_buf[4] = 0x0A;

		smbus0_irq_handle(dut0.g_pattern_smbus_control_buf);
		if(dut0.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			for(i=0;i<11;i++)
			{
				read_icstatus_data[i] = dut0.g_pattern_smbus_control_buf[i+10];
			}
			//xil_printf("dut0.read_icstatus_data[0] =0x%x\r\n", read_icstatus_data[0]);
			//xil_printf("dut0.read_icstatus_data[1] =0x%x\r\n", read_icstatus_data[1]);
			//xil_printf("dut0.read_icstatus_data[2] =0x%x\r\n", read_icstatus_data[2]);
			//xil_printf("dut0.read_icstatus_data[3] =0x%x\r\n", read_icstatus_data[3]);
			//xil_printf("dut0.read_icstatus_data[4] =0x%x\r\n", read_icstatus_data[4]);
			//xil_printf("dut0.read_icstatus_data[5] =0x%x\r\n", read_icstatus_data[5]);
			//xil_printf("dut0.read_icstatus_data[6] =0x%x\r\n", read_icstatus_data[6]);
			//xil_printf("dut0.read_icstatus_data[7] =0x%x\r\n", read_icstatus_data[7]);
			//xil_printf("dut0.read_icstatus_data[8] =0x%x\r\n", read_icstatus_data[8]);
			//xil_printf("dut0.read_icstatus_data[9] =0x%x\r\n", read_icstatus_data[9]);
			//xil_printf("dut0.read_icstatus_data[10] =0x%x\r\n", read_icstatus_data[10]);

			if((read_icstatus_data[0] == 0x01) && (read_icstatus_data[6] == 0xDA) && (read_icstatus_data[7] == 0x0B) && (read_icstatus_data[9] == 0x65) && (read_icstatus_data[8] == 0x58))
			{
				//dut0.g_pattern_step = 0x0b;
			    dut0.g_pattern_step++;
			}
			else
			{
				dut0.g_result_fail = 0x01;
				dut0.g_result_fail_tmrcount = 0xffff;
			    xil_printf("dut0 get ic inf fail!!!\r\n\r\n");
			}
			for(i=0; i<31; i++)
			{
				dut0.g_pattern_smbus_control_buf[i+1] = CLEAR_;
			}

			dut0.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			//xil_printf("dut0.g_pattern_step = %x\r\n\r\n",dut0.g_pattern_step);
			}
			//xil_printf("dut0_pattern0_get_ic_status\r\n");
		break;
	}

	//efuse_clock_enable
	case 0x0004:
	{
		   dut0.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		   dut0.g_pattern_smbus_control_buf[2] = 0x05;
		   dut0.g_pattern_smbus_control_buf[3] = 0x11;
		   dut0.g_pattern_smbus_control_buf[4] = 0x01;
		   dut0.g_pattern_smbus_control_buf[5] = 0x07;

		   smbus0_irq_handle(dut0.g_pattern_smbus_control_buf);
		   if(dut0.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		   {
			   break;
		   }
		   else
		   {
				   for(i=1; i<31; i++)
				   {
					   dut0.g_pattern_smbus_control_buf[i] = CLEAR_;
				   }
				       dut0.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
				       dut0.g_pattern_step++;
				       ////xil_printf("dut0.g_pattern_step = %x\r\n\r\n",dut0.g_pattern_step);
		   }
		   ////////////xil_printf("dut0_pattern0_efuse_clock_enable\r\n\r\n");
	break;
	}

	//Read Efuse data validbit
	case 0x0005:
	{
		dut0.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readefuse;
		dut0.g_pattern_smbus_control_buf[2] = 0x00;
		dut0.g_pattern_smbus_control_buf[3] = 0x00;
		dut0.g_pattern_smbus_control_buf[4] = 0x02;

		smbus0_irq_handle(dut0.g_pattern_smbus_control_buf);
		if(dut0.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}

		else
		{
			//("dut0.g_pattern_smbus_control_buf[10] =%x\r\n", dut0.g_pattern_smbus_control_buf[10]);
			//xil_printf("dut0.g_pattern_smbus_control_buf[11] =%x\r\n", dut0.g_pattern_smbus_control_buf[11]);
			efuse_datavalid = dut0.g_pattern_smbus_control_buf[10] & 0x01;
			efuse_testvalid = dut0.g_pattern_smbus_control_buf[11]>>0x07;

			efuse_data_valid    =  dut0.g_pattern_smbus_control_buf[10]&0x01;
			efuse_ldo3v3_valid  = (dut0.g_pattern_smbus_control_buf[10]>>1)&0x01;
			efuse_rp12k_valid   = (dut0.g_pattern_smbus_control_buf[10]>>2)&0x01;
			efuse_cc_valid      = (dut0.g_pattern_smbus_control_buf[10]>>3)&0x01;
			efuse_package_valid = (dut0.g_pattern_smbus_control_buf[10]>>4)&0x03;

			for(i=1; i<30; i++)
			{
				dut0.g_pattern_smbus_control_buf[i] = CLEAR_;
			}

			dut0.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut0.g_pattern_step++;
			//xil_printf("dut0_patternc_efuse_step = %x\r\n\r\n",dut0.g_pattern_step);
		}
		break;
	}

	//Read Efuse data validbit
	case 0x0006:
	{
		   dut0.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readefuse;
		   dut0.g_pattern_smbus_control_buf[2] = 0x1f;
		   dut0.g_pattern_smbus_control_buf[3] = 0x00;
		   dut0.g_pattern_smbus_control_buf[4] = 0x01;

		   smbus0_irq_handle(dut0.g_pattern_smbus_control_buf);
		   if(dut0.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		   {
			   break;
		   }
		   else
		   {
			   //xil_printf("dut0.g_pattern_smbus_control_buf[10] =%x\r\n", dut0.g_pattern_smbus_control_buf[10]);
			   //xil_printf("dut0.g_pattern_smbus_control_buf[11] =%x\r\n", dut0.g_pattern_smbus_control_buf[11]);
			   //efuse_rp36k_valid   =  dut0.g_pattern_smbus_control_buf[10]&0x01;
			   efuse_writedata_valid = (efuse_data_valid && efuse_ldo3v3_valid && efuse_rp12k_valid && efuse_cc_valid);

			   while((ft1_config_scan!=0x0)|(ft1_config_scan!=0xffffffff))
			   {

				   START01_GPIO22 = XGpio_Dutx_ReadBit(XPAR_AXI_GPIO_dut0_1_BASEADDR,0,22);
				   usdelay(1);
				   ft1_config_scan = ((ft1_config_scan<<1)|START01_GPIO22);
				   ft1_config_scan_count--;
				   if(ft1_config_scan_count == 0)
				   {
					   break;
				   }
			   }

			   usdelay(20);
			   xil_printf("dut0_START01_GPIO22 = %x\r\n\r\n",START01_GPIO22);
			   //xil_printf("dut0_ft1_config_scan = %x\r\n\r\n",ft1_config_scan);

			   if(efuse_package_valid==0)
			   {
				   dut0.g_retest = 0;
				   dut0.g_efuse_status = EFUSE_FT1_TEST;
			       dut0.g_dut_pattern_status_buf[2]++;
			       dut0.g_uartPatternEnable = 0x00;
			       dut0.g_uartPatternNum = (0x0c & 0xff);
			       //dut0.g_uartPatternNum = (dut0.g_uartPatternNum++ & 0xff);

			   }
			   else if((efuse_package_valid!=0) && (efuse_writedata_valid==0))
			   {
				   dut0.g_retest = 0;
				   if(ft1_config_scan == 0xffffffff)
				   {
					   ////xil_printf("START01_GPIO2222 = %x\r\n\r\n",START01_GPIO22);
					   dut0.g_efuse_status = EFUSE_FT1_TEST;
				       dut0.g_dut_pattern_status_buf[2]++;
				       dut0.g_uartPatternEnable = 0x00;
				       dut0.g_uartPatternNum = (0x0c & 0xff);
				       //dut0.g_uartPatternNum = (dut0.g_uartPatternNum++ & 0xff);
				     ////xil_printf("dut0.g_efuse_status = %x\r\n\r\n",dut0.g_efuse_status);
				   }
				   else
				   {
					   ////xil_printf("START01_GPIO2233 = %x\r\n\r\n",START01_GPIO22);
					   dut0.g_efuse_status = EFUSE_FT2_TEST;
				       dut0.g_dut_pattern_status_buf[2]++;
				       dut0.g_uartPatternEnable = 0x00;
				       dut0.g_uartPatternNum = (dut0.g_uartPatternNum++ & 0xff);
				       //dut0.g_uartPatternNum = (dut0.g_uartPatternNum++ & 0xff);
				   }

			   }
			   else if((efuse_package_valid!=0) && (efuse_writedata_valid==1))
			   {
				   dut0.g_retest = 1;
				   if(ft1_config_scan == 0xffffffff)
				   {
					   //xil_printf("d2222\r\n");
					   dut0.g_efuse_status = EFUSE_FT1_TEST;
				       dut0.g_dut_pattern_status_buf[2]++;
				       dut0.g_uartPatternEnable = 0x00;
				       dut0.g_uartPatternNum = (0x0c & 0xff);
				       //dut0.g_uartPatternNum = (dut0.g_uartPatternNum++ & 0xff);
				       xil_printf("dut0.g_efuse_status = %x\r\n",dut0.g_efuse_status);
				   }
				   else
				   {
					   dut0.g_efuse_status = EFUSE_RETEST;
				       dut0.g_dut_pattern_status_buf[2]++;
				       dut0.g_uartPatternEnable = 0x00;
				       dut0.g_uartPatternNum = (dut0.g_uartPatternNum++ & 0xff);
				       //dut0.g_uartPatternNum = (dut0.g_uartPatternNum++ & 0xff);
				   }
			   }
			   else
			   {
				   dut0.g_result_fail = 0x01;
				   dut0.g_result_fail_tmrcount = 0xffff;
			       dut0.g_dut_pattern_status_buf[2]++;
			       dut0.g_uartPatternEnable = 0x00;
			       dut0.g_uartPatternNum = (dut0.g_uartPatternNum++ & 0xff);

			   }
			   for(i=1; i<31; i++)
			   {
				   dut0.g_pattern_smbus_control_buf[i] = CLEAR_;
			   }
	   	   	       dut0.g_pattern_timer = 0xfff;
				   dut0.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			       dut0.g_pattern_step = 0x00;

			       result_output_for_v50(XPAR_AXI_GPIO_dut0_1_BASEADDR,dut0.g_uartPatternNum);
			       //xil_printf("dut0.g_efuse_status = %x\r\n",dut0.g_efuse_status);
				   xil_printf("dut0_pattern0_setup_pass!     efuse_testvalid =0x%x\r\n\r\n",dut0.g_efuse_status);
			       //xil_printf("dut0.g_uartPatternNum = %x\r\n\r\n",dut0.g_uartPatternNum);
				   //xil_printf("dut0.g_pattern_step = %x\r\n\r\n",dut0.g_pattern_step);
				   //xil_printf("dut0.g_dut_start_ready = %x\r\n\r\n",dut0.g_dut_start_ready);
				   //xil_printf("dut0.g_result_fail = %x\r\n\r\n",dut0.g_result_fail);
				   //xil_printf("dut0.g_dut_pattern_status_buf[2] = %x\r\n\r\n",dut0.g_dut_pattern_status_buf[2]);

		   }
	break;
	}
	}
	if(dut0.g_result_fail == 0x01)
	{
		dut0.g_ft2_test_done = 0x00;
		pattern_safe_state_dut0();	//add safe state
		xil_printf("dut0_pattern0_fail! fail_step = %x\r\n\r\n",dut0.g_pattern_step);
	}

	//xil_printf("dut0_pattern0_setup_step = %x\r\n\r\n",dut0.g_pattern_step);
	//xil_printf("dut0.g_result_fail = %x\r\n\r\n",dut0.g_result_fail);
	//////////xil_printf("dut0.g_uartPatternNum = %x\r\n\r\n",dut0.g_uartPatternNum);
	//////////xil_printf("dut0.g_uartPatternEnable = %x\r\n\r\n",dut0.g_uartPatternEnable);
	return 0;
	}



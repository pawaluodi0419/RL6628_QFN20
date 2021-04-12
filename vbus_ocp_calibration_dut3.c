#include "platform.h"
#include "vbus_ocp_calibration.h"

u8 _vbus_ocp_calibration_dut3()
{
	u8 i;
	u16 flt_voltage_code;
	u16 vih_code = 2.5/4.096*4095;
	u8 flt_pull_h;
	u8 vbus_en;
	u8 flt_out;
	u8 FLT_IN_Status;
	u8 dut3_vbus_ocp_3a_en;
	//u8 vbus_load_retry_time;
	//u8 vbus_calibraion_ready_retry_time;
	switch(dut3.g_vbusocp_process_bit)
	{
	    case vbusocp_process_waiting:
		{
			dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writephy;
			dut3.g_pattern_smbus_control_buf[2] = 0x02;
			dut3.g_pattern_smbus_control_buf[3] = 0x01;
			dut3.g_pattern_smbus_control_buf[4] = 0x01;
			dut3.g_pattern_smbus_control_buf[5] = 0x00;
			dut3.g_pattern_smbus_control_buf[6] = 0x80;  //qfn12/20 is bit15=1

			smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
			if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
			{
				break;
			}
			else
			{
				dut3.g_calibrate_signbit = 0;
				dut3.g_calibration_offset = 0;

				for(i=1; i<60; i++)
				{
					dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
				}

				dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
				dut3.g_vbusocp_process_bit = vbusocp_process_doing;
				dut3.g_vbusocp_step++;
			}
		   	//xil_printf("dut3.g_vbusocp_step = %x\r\n",dut3.g_vbusocp_step);
			break;
		}

	    case vbusocp_process_doing:
	    {
	    	switch(dut3.g_vbusocp_step)
	    	{
	    	//enable SY6287对应RSET电路
	    	//PHY reg REG_PSW_ISET_CFG0 (0x102) bit[15] (AIF:EN_RSET_SY6287) 置1’b1
	    	//PHY reg REG_PSW_ISET_CFG0(0x102) bit[14] (AIF:EN_RSET_SY6861) 置1’b0
	    	case 0x0000:
	    	{
	    		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writephy;
	    		dut3.g_pattern_smbus_control_buf[2] = 0x02;
	    		dut3.g_pattern_smbus_control_buf[3] = 0x01;
	    		dut3.g_pattern_smbus_control_buf[4] = 0x01;
	    		dut3.g_pattern_smbus_control_buf[5] = 0x00;
	    		dut3.g_pattern_smbus_control_buf[6] = 0x80;  //qfn12/20 is bit15=1

	    		smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
	    		if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
	    		{
	    			break;
	    		}
	    		else
	    		{
	    			dut3.g_calibrate_signbit = 0;
	    			dut3.g_calibration_offset = 0;
	    			for(i=1; i<60; i++)
	    			{
	    				dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
	    			}

	    			dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
	    			dut3.g_vbusocp_process_bit = vbusocp_process_doing;
	    			dut3.g_vbusocp_step++;
	    		}
	    		//xil_printf("dut3.g_vbusocp_step = %x\r\n",dut3.g_vbusocp_step);
	    		break;
	    	}

	    	//将PHY reg REG_PSW_ISET_CFG1(0x103) bit[15:8] (AIF:RSET_SY6287[7:0])写成8‘b0
	    	case 0x0001:
	    	{
	    		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writephy;
	    		dut3.g_pattern_smbus_control_buf[2] = 0x03;
	    		dut3.g_pattern_smbus_control_buf[3] = 0x01;
	    		dut3.g_pattern_smbus_control_buf[4] = 0x01;
	    		dut3.g_pattern_smbus_control_buf[5] = 0x0;
	    		dut3.g_pattern_smbus_control_buf[6] = 0x0;

	    		smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
	    		if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
	    		{
	    			break;
	    		}
	    		else
	    		{
	    			for(i=1; i<60; i++)
	    			{
	    				dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
	    			}

	    			dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;

	    			dut3.g_vbusocp_trimming_low  =0;
	    			dut3.g_vbusocp_trimming_mid  =0x1f;
	    			dut3.g_vbusocp_trimming_high =0x3f;
	    			dut3.g_vbusocp_pre_flt_status = 0;
	    			dut3.g_vbusocp_retry_time=5;
	    			dut3.g_vbusocp_process_bit = vbusocp_process_doing;
	    			dut3.g_vbusocp_step++;
	    		}
	    		//xil_printf("dut3.g_vbusocp_step = %x\r\n",dut3.g_vbusocp_step);
	    		break;
	    	}

	    	//set flt_in=gpio3,enable flt_out debounce
	    	//write mem 0x104a with data 0xc3
	    	case 0x0002:
	    	{
	    		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
	    		dut3.g_pattern_smbus_control_buf[2] = 0x4a;
	    		dut3.g_pattern_smbus_control_buf[3] = 0x10;
	    		dut3.g_pattern_smbus_control_buf[4] = 0x01;
	    		dut3.g_pattern_smbus_control_buf[5] = 0xc3;

	    		smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
	    		if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
	    		{
	    			break;
	    		}
	    		else
	    		{
	    			for(i=1; i<60; i++)
	    			{
	    				dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
	    			}
	    			dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
	    			dut3.g_vbusocp_process_bit = vbusocp_process_doing;
	    			dut3.g_vbusocp_step++;
	    		}
	    		//xil_printf("dut3.g_vbusocp_step = %x\r\n",dut3.g_vbusocp_step);
	    		break;
	    	}

	    	//set flt_out has 6ms debounce
	    	//write mem 0x104b with data 0x3c
	    	case 0x0003:
	    	{
	    		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
	    		dut3.g_pattern_smbus_control_buf[2] = 0x4b;
	    		dut3.g_pattern_smbus_control_buf[3] = 0x10;
	    		dut3.g_pattern_smbus_control_buf[4] = 0x01;
	    		dut3.g_pattern_smbus_control_buf[5] = 0x3c;

	    		smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
	    		if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
	    		{
	    			break;
	    		}
	    		else
	    		{
	    			for(i=1; i<60; i++)
	    			{
	    				dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
	    			}
	    			dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
	    			dut3.g_vbusocp_process_bit = vbusocp_process_doing;
	    			dut3.g_vbusocp_step++;
	    		}
	    		//xil_printf("dut3.g_vbusocp_step = %x\r\n",dut3.g_vbusocp_step);
	    		break;
	    	}

	    	//将#FLT_IN内建pull up打开
	    	//QFN20：GPIO3（#FLT_IN） pull up ，先读出确认当前是否是no pull，no drive
	    	case 0x0004:
	    	{
	    		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
	    		dut3.g_pattern_smbus_control_buf[2] = 0x13;
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
	    			for(i=1; i<60; i++)
	    			{
	    				dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
	    			}
	    			dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
	    			dut3.g_vbusocp_process_bit = vbusocp_process_doing;
	    			dut3.g_vbusocp_step++;
	    			//dut3.g_vbusocp_step++;
	    		}
	    		//xil_printf("dut3.g_vbusocp_step = %x\r\n",dut3.g_vbusocp_step);
	    		break;
	    	}

	    	//confirm flt_in whether has been pull high
	    	case 0x0005:
	    	{
	    		msdelay(1);
	    		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readmem;
				dut3.g_pattern_smbus_control_buf[2] = 0x13;
				dut3.g_pattern_smbus_control_buf[3] = 0x11;
				dut3.g_pattern_smbus_control_buf[4] = 0x01;

				smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
				if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
				{
					break;
				}
				else
				{
					flt_pull_h=((dut3.g_pattern_smbus_control_buf[10]>>6&3)==2);
					//xil_printf("dut3_flt_pull_h = %x,register= 0x%x\r\n\r\n",dut3.g_pattern_smbus_control_buf[10],0x1113);

					if(flt_pull_h==1)
					{
						dut3.g_vbusocp_step = 0x07;
					}
					else
					{
						dut3.g_result_fail = 0x01;
						dut3.g_result_fail_tmrcount = 0xffff;
					}
					for(i=1; i<60; i++)
					{
						dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
					}
					dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
					dut3.g_vbusocp_process_bit = vbusocp_process_doing;
				}
				//xil_printf("dut3.g_result_fail = %x\r\n",dut3.g_result_fail);
				//xil_printf("dut3.g_vbusocp_step = %x\r\n",dut3.g_vbusocp_step);
				break;
	    	}
/*
	    	//先将PHY reg REG_PSW_ISET_CFG1(0x103) bit[15:8]value值写0x00（OCP阈值最大档位）
	    	case 0x0006:
	    	{
	    		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writephy;
	    		dut3.g_pattern_smbus_control_buf[2] = 0x03;
	    		dut3.g_pattern_smbus_control_buf[3] = 0x01;
	    		dut3.g_pattern_smbus_control_buf[4] = 0x01;
	    		dut3.g_pattern_smbus_control_buf[5] = 0x0;
	    		dut3.g_pattern_smbus_control_buf[6] = 0x0;

	    		smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
	    		if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
	    		{
	    			break;
	    		}
	    		else
	    		{
	    			for(i=1; i<60; i++)
	    			{
	    				dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
	    			}
	    			dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
	    			dut3.g_vbusocp_process_bit = vbusocp_process_doing;
	    			dut3.g_vbusocp_step++;
	    			//dut3.g_vbusocp_step++;
				}
	    		//xil_printf("dut3.g_vbusocp_step = %x\r\n",dut3.g_vbusocp_step);
	    		break;
	    	}
*/
	    	//VBUS_OUT上负载电路关闭，不抽电
	    	case 0x0007:
	    	{
	    		//this is control vbus board,base addr is dutx
	    		XGpio_WriteBit(XPAR_AXI_GPIO_dut3_1_BASEADDR,0,23, 0);
	    		XGpio_WriteBit(XPAR_AXI_GPIO_dut3_1_BASEADDR,0,26, 0);
	    		XGpio_WriteBit(XPAR_AXI_GPIO_dut3_1_BASEADDR,0,25, 0);

	    		msdelay(10);
	    		dut3.g_vbusocp_process_bit = vbusocp_process_doing;
	    		dut3.g_vbusocp_step++;
	    	}

	    	//先将PSW_EN drive low，关闭VBUS电压， delay时间15ms；
	    	case 0x0008:
	    	{
				dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
				dut3.g_pattern_smbus_control_buf[2] = 0x10;  //qfn20 0x1110
				dut3.g_pattern_smbus_control_buf[3] = 0x11;
				dut3.g_pattern_smbus_control_buf[4] = 0x01;
				dut3.g_pattern_smbus_control_buf[5] = 0x11;

				smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
				if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
				{
					break;
				}
				else
				{
					for(i=1; i<60; i++)
					{
						dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
					}
					dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
					dut3.g_vbusocp_process_bit = vbusocp_process_doing;
					msdelay(100);
					dut3.g_vbusocp_step++;
				}
				//xil_printf("dut3.g_vbusocp_step = %x\r\n",dut3.g_vbusocp_step);
				break;
	    	}

	    	//confirm power_en has been pull high
	    	case 0x0009:
	    	{
	    		msdelay(1);
	    		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readmem;
	    		dut3.g_pattern_smbus_control_buf[2] = 0x10;
	    		dut3.g_pattern_smbus_control_buf[3] = 0x11;
	    		dut3.g_pattern_smbus_control_buf[4] = 0x01;

	    		smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
	    		if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
	    		{
	    			break;
	    		}
	    		else
	    		{
	    			vbus_en=((dut3.g_pattern_smbus_control_buf[10]&0x11)==0x11);
	    			//xil_printf("dut3_vbus_power_en_register_data = 0x%x,register_slave= 0x%x\r\n",dut3.g_pattern_smbus_control_buf[10],0x1110);

	    			if(vbus_en==1)
	    			{
	    				dut3.g_vbusocp_step++;
	    			}
	    			else
	    			{
	    				dut3.g_result_fail = 0x01;
	    				dut3.g_result_fail_tmrcount = 0xffff;
	    			}
	    			for(i=1; i<60; i++)
	    			{
	    				dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
	    			}
	    			dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
	    			dut3.g_vbusocp_process_bit = vbusocp_process_doing;
	    		}
	    		//xil_printf("dut3.g_result_fail = %x\r\n",dut3.g_result_fail);
	    		//xil_printf("dut3.g_vbusocp_step = %x\r\n",dut3.g_vbusocp_step);
	    		break;
	    	}

	    	//通过SMBus指令控制PSW_EN drive high，开启VBUS电压，检测VBUS电压
	    	//QFN12：GPIO8（PSW_EN） 改为输出high，先读出确认当前是否是no pull，no drive
	    	//将reg SYS_GPIO_CTL(0x1110)  bit0 写入1，bit1写入1
	    	case 0x000a:
	    	{
	    		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
	    		dut3.g_pattern_smbus_control_buf[2] = 0x10;  //qfn12 0x1110
	    		dut3.g_pattern_smbus_control_buf[3] = 0x11;
	    		dut3.g_pattern_smbus_control_buf[4] = 0x01;
	    		dut3.g_pattern_smbus_control_buf[5] = 0x13;

	    		smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
	    		if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
	    		{
	    			break;
	    		}
	    		else
	    		{
	    			for(i=1; i<60; i++)
	    			{
	    				dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
	    			}
	    			dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
	    			dut3.g_vbusocp_process_bit = vbusocp_process_doing;
	    			msdelay(15);
	    			dut3.g_vbusocp_step++;
	    			dut3.vbus_load_retry_time=4;
	    			dut3.vbus_calibraion_ready_retry_time=4;
	    		}
	    		//xil_printf("dut3.g_vbusocp_step = %x\r\n",dut3.g_vbusocp_step);
	    		break;
	    	}

	    	//confirm power_en has been pull high
	    	case 0x000b:
	    	{
	    		msdelay(1);
	    		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readmem;
	    		dut3.g_pattern_smbus_control_buf[2] = 0x10;
	    		dut3.g_pattern_smbus_control_buf[3] = 0x11;
	    		dut3.g_pattern_smbus_control_buf[4] = 0x01;

	    		smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
	    		if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
	    		{
	    			break;
	    		}
	    		else
	    		{
	    			vbus_en=((dut3.g_pattern_smbus_control_buf[10]&0x13)==0x13);
	    			//xil_printf("dut3_vbus_power_en_register_data = 0x%x,register_slave= 0x%x\r\n",dut3.g_pattern_smbus_control_buf[10],0x1110);

	    			if(vbus_en==1)
	    			{
	    				dut3.g_vbusocp_step++;
	    			}
	    			else
	    			{
	    				dut3.g_result_fail = 0x01;
	    				dut3.g_result_fail_tmrcount = 0xffff;
	    			}
	    			for(i=1; i<60; i++)
	    			{
	    				dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
	    			}
	    			dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
	    			dut3.g_vbusocp_process_bit = vbusocp_process_doing;
	    		}
	    		//xil_printf("dut3.g_result_fail = %x\r\n",dut3.g_result_fail);
	    		//xil_printf("dut3.g_vbusocp_step = %x\r\n",dut3.g_vbusocp_step);
	    		break;
	    	}

	    	//open vbus_in power
	    	//S0_FPGA_RELAY10 output high
	    	case 0x000c:
	    	{
	    		if(dut3.g_vbusocp_3a_nmos_control ==1)
	    		{
	    			switch(g_vbusocp_3a_status)
	    			{
	    			case 0x00:
	    			{
	    				g_vbusocp_3a_status = 0x13;
	    				dut3.g_vbusocp_3a_en_status =1;
	    				break;
	    			}
	    			case 0x10:
	    			{
	    				g_vbusocp_3a_status = 0x13;
	    				dut3.g_vbusocp_3a_en_status =1;
	    				break;
	    			}
	    			}
	    		}
	    		//xil_printf("1dut3.g_vbusocp_3a_nmos_control = %x\r\n",dut3.g_vbusocp_3a_nmos_control);
	    		//xil_printf("1dut3.g_vbusocp_3a_en_status = %x\r\n",dut3.g_vbusocp_3a_en_status);
	    		//xil_printf("1g_vbusocp_3a_status = %x\r\n",g_vbusocp_3a_status);
	    		//xil_printf("dut3.g_vbusocp_board_en = %x\r\n",dut3.g_vbusocp_board_en);
	    		//xil_printf("dut3.g_vbusocp_1p5a_nmos_control = %x\r\n",dut3.g_vbusocp_1p5a_nmos_control);
	    		//xil_printf("dut3.g_vbusocp_3a_nmos_control = %x\r\n",dut3.g_vbusocp_3a_nmos_control);
	    		//xil_printf("dut3.g_vbusocp_0p9a_nmos_control = %x\r\n\r\n",dut3.g_vbusocp_0p9a_nmos_control);
	    		if(dut3.g_dut_pattern_status_buf[7] == 0x00)
	    		{
	    			if((g_vbusocp_3a_status==0) &&(dut3.g_vbusocp_3a_nmos_control==0))
	    			{
	    				xil_printf("-------------------------dut3_0p9a/1p5a_test-------------------------\r\n");

	    				//this is control vbus board,base addr is dutx
	    				XGpio_WriteBit(XPAR_AXI_GPIO_dut3_1_BASEADDR,0,23, dut3.g_vbusocp_board_en);
	    				XGpio_WriteBit(XPAR_AXI_GPIO_dut3_1_BASEADDR,0,26, dut3.g_vbusocp_1p5a_nmos_control);
	    				XGpio_WriteBit(XPAR_AXI_GPIO_dut3_1_BASEADDR,0,25, dut3.g_vbusocp_3a_nmos_control);
	    				msdelay(50);
	    				dut3.g_dut_pattern_status_buf[7] = 0x01;
	    				dut3.g_vbusocp_readadc_ready = 0x01;  //ready to read adc vbus_out pin
	    				//dut3.vbus_load_retry_time=6;
	    				//dut3.vbus_calibraion_ready_retry_time=6;
	    				//xil_printf("dut3.g_vbusocp_readadc_ready = %x\r\n", dut3.g_vbusocp_readadc_ready);
	    			}

	    			else if((dut3.g_vbusocp_3a_nmos_control==1)&&(dut3.g_vbusocp_3a_en_status==1))
	    			{
	    				xil_printf("-------------------------dut3_3a_test-------------------------\r\n");

	    				//this is control vbus board,base addr is dutx
	    				XGpio_WriteBit(XPAR_AXI_GPIO_dut3_1_BASEADDR,0,23, dut3.g_vbusocp_board_en);
	    				XGpio_WriteBit(XPAR_AXI_GPIO_dut3_1_BASEADDR,0,26, dut3.g_vbusocp_1p5a_nmos_control);
	    				usdelay(200);
	    				XGpio_WriteBit(XPAR_AXI_GPIO_dut3_1_BASEADDR,0,25, dut3.g_vbusocp_3a_nmos_control);
	    				msdelay(50);
	    				dut3.g_dut_pattern_status_buf[7] = 0x01;
	    				dut3.g_vbusocp_readadc_ready = 0x01;  //ready to read adc vbus_out
	    			}

	    			else
	    			{
	    				//xil_printf("dut3_return~~\r\n");
	    				return;
	    			}
	    		}

	    		else if(dut3.g_dut_pattern_status_buf[7] == 0x01)
	    		{
	    			if(dut3.g_relay_control_timer == 0 )
	    			{
	    				//msdelay(50);
	    				dut3.g_dut_pattern_status_buf[7] = 0x00;
	    				dut3.g_vbusocp_step++;
	    			}
	    			//dut3.g_result_fail = 0x01;
	    		}

	    		else
	    		{
	    			dut3.g_result_fail = 0x01;
	    			dut3.g_result_fail_tmrcount = 0xffff;
	    		}
	    		break;
	    	}

	    	//when retry read flt_out pin
	    	case 0x000d:
	    	{
	    		msdelay(1);
	    		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readmem;
	    		dut3.g_pattern_smbus_control_buf[2] = 0x4a;
	    		dut3.g_pattern_smbus_control_buf[3] = 0x10;
	    		dut3.g_pattern_smbus_control_buf[4] = 0x01;

	    		smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
	    		if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
	    		{
	    			break;
	    		}
	    		else
	    		{
	    			flt_out=((dut3.g_pattern_smbus_control_buf[10]>>6)&1);
	    			//xil_printf("dut3_flt_out = 0x%x,0x104a_register_data= 0x%x\r\n",flt_out,dut3.g_pattern_smbus_control_buf[10]);

	    			dut3.g_vbusocp_step++;

	    			for(i=1; i<60; i++)
	    			{
	    				dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
	    			}
	    			dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
	    			dut3.g_vbusocp_process_bit = vbusocp_process_doing;
	    		}
	    		//xil_printf("dut3.g_result_fail = %x\r\n",dut3.g_result_fail);
	    		//xil_printf("dut3.g_vbusocp_step = %x\r\n",dut3.g_vbusocp_step);
	    		break;
	    	}

	    	case 0x000e:
	    	{
	    		//dut3.g_result_fail = 0x01;
	    		msdelay(1);
	    		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readmem;
	    		dut3.g_pattern_smbus_control_buf[2] = 0x13;
	    		dut3.g_pattern_smbus_control_buf[3] = 0x11;
	    		dut3.g_pattern_smbus_control_buf[4] = 0x01;
	    		//dut3.g_result_fail = 0x01;
	    		//xil_printf("dut3.g_pattern_smbus_control_buf[5] = %x\r\n",dut3.g_pattern_smbus_control_buf[5]);
	    		smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
	    		if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
	    		{
	    			break;
	    		}
	    		else
	    		{
	    			//xil_printf("dut3_register_0x1113_data = 0x%x\r\n", dut3.g_pattern_smbus_control_buf[10]);
	    			FLT_IN_Status= (dut3.g_pattern_smbus_control_buf[10]>>2)&1;
	    			//xil_printf("dut3_g_pattern_smbus_control_buf[10] = %x\r\n", dut3.g_pattern_smbus_control_buf[10]);
	    			xil_printf("dut3_vbus_rset_firstload_FLT_IN_Status = %x\r\n\r\n", FLT_IN_Status);
	    			//when FLT_IN_Status =1,vbus ocp not be occur

	    			if(FLT_IN_Status == 1)//no ovp
	    			{
	    				dut3.g_vbusocp_step++;
	    			}

	    			if(FLT_IN_Status == 0)//ocp occur
	    			{
	    				xil_printf("dut3_vbus_rset_firstload_retry_time = %x\r\n\r\n", 4-dut3.vbus_load_retry_time);
	    				if(dut3.vbus_load_retry_time>0)
	    				{
	    					dut3.g_vbusocp_step--;
	    					dut3.g_vbusocp_step--;
	    					dut3.vbus_load_retry_time--;
	    				}
	    				else
				   	   {
	    					dut3.g_result_fail = 0x01;
	    					dut3.g_result_fail_tmrcount = 0xffff;
				   	   }
	    			}

	    			for(i=1; i<60; i++)
	    			{
	    				dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
	    			}
	    			dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
	    			dut3.g_vbusocp_process_bit = vbusocp_process_doing;
	    		}
	    		break;
	    	}

	    	//将PHY reg REG_PSW_ISET_CFG1(0x103) bit[15:8] (AIF:RSET_SY6287[7:0])写成8‘b1f
	    	case 0x000f:
	    	{
	    		msdelay(1);
	    		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writephy;
	    		dut3.g_pattern_smbus_control_buf[2] = 0x03;
	    		dut3.g_pattern_smbus_control_buf[3] = 0x01;
	    		dut3.g_pattern_smbus_control_buf[4] = 0x01;
	    		dut3.g_pattern_smbus_control_buf[5] = 0x1f;
	    		dut3.g_pattern_smbus_control_buf[6] = 0x1f;

	    		smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
	    		if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
	    		{
	    			break;
	    		}
	    		else
	    		{
	    			//dut3.g_result_fail = 0x01;
	    			for(i=1; i<60; i++)
	    			{
	    				dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
	    			}
	    			dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;

	    			dut3.g_vbusocp_trimming_low  =0;
	    			dut3.g_vbusocp_trimming_mid  =0x1f;
	    			dut3.g_vbusocp_trimming_high =0x3f;
	    			dut3.g_vbusocp_pre_flt_status = 0;
	    			dut3.g_vbusocp_retry_time=5;
	    			dut3.g_vbusocp_process_bit = vbusocp_process_doing;
	    			dut3.g_vbusocp_step++;
	    			msdelay(20);
	    		}
	    		//xil_printf("dut3.g_vbusocp_step = %x\r\n",dut3.g_vbusocp_step);
	    		break;
	    	}

	    	//when ocp rset trimming read flt_out pin
	    	case 0x0010:
	    	{
	    		msdelay(1);
	    		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readmem;
	    		dut3.g_pattern_smbus_control_buf[2] = 0x4a;
	    		dut3.g_pattern_smbus_control_buf[3] = 0x10;
	    		dut3.g_pattern_smbus_control_buf[4] = 0x01;

	    		smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
	    		if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
	    		{
	    			break;
	    		}
	    		else
	    		{
	    			flt_out=((dut3.g_pattern_smbus_control_buf[10]>>6)&1);
	    			//xil_printf("dut3_flt_out = 0x%x,0x104a_register_data= 0x%x\r\n",flt_out,dut3.g_pattern_smbus_control_buf[10]);
	    			dut3.g_vbusocp_step++;

	    			for(i=1; i<60; i++)
	    			{
	    				dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
	    			}
	    			dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
	    			dut3.g_vbusocp_process_bit = vbusocp_process_doing;

	    		}
	    		//xil_printf("dut3.g_result_fail = %x\r\n",dut3.g_result_fail);
	    		//xil_printf("dut3.g_vbusocp_step = %x\r\n",dut3.g_vbusocp_step);
	    		break;
	    	}

	    	case 0x0011:
	    	{
	    		msdelay(1);
	    		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readmem;
	    		dut3.g_pattern_smbus_control_buf[2] = 0x13;
	    		dut3.g_pattern_smbus_control_buf[3] = 0x11;
	    		dut3.g_pattern_smbus_control_buf[4] = 0x01;

	    		smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
	    		if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
	    		{
	    			break;
	    		}
	    		else
	    		{
	    			//xil_printf("dut3_register_0x1113_data = 0x%x\r\n", dut3.g_pattern_smbus_control_buf[10]);
	    			FLT_IN_Status= (dut3.g_pattern_smbus_control_buf[10]>>2)&1;

					//when FLT_IN_Status =1, vbus ocp not be occur
					if(FLT_IN_Status == 1)
					{
						if(dut3.g_vbusocp_1p5a_nmos_control==1)		//for 3A/1.5A Trimming
						{
							if(dut3.g_vbusocp_trimming_mid  ==0x1f)
							{
								if(dut3.vbus_calibraion_ready_retry_time>0)
								{
									xil_printf("dut3_vbus_calibraion_ready_retry_time=0x%x\r\n\r\n",dut3.vbus_calibraion_ready_retry_time);
									dut3.g_vbusocp_step--;
									dut3.g_vbusocp_step--;
									dut3.g_vbusocp_step--;
									dut3.vbus_calibraion_ready_retry_time--;
								}
								else
								{
									dut3.g_result_fail = 0x01;
									dut3.g_result_fail_tmrcount = 0xffff;
								}
							}
							else
							{
								if(dut3.g_vbusocp_trimming_low < dut3.g_vbusocp_trimming_high-1)
								{
									dut3.g_vbusocp_trimming_low = dut3.g_vbusocp_trimming_mid;
									dut3.g_vbusocp_trimming_mid = (dut3.g_vbusocp_trimming_low + dut3.g_vbusocp_trimming_high)/2;
									dut3.g_vbusocp_pre_flt_status = 1;
									dut3.g_calibration_offset = dut3.g_vbusocp_trimming_mid;
									dut3.g_vbusocp_step++;
									//xil_printf("dut3_g_calibration_offset = %x\r\n", dut3.g_calibration_offset);
								}
								else
								{
									if(dut3.g_vbusocp_pre_flt_status == 0)
									{
										dut3.g_calibration_offset=dut3.g_vbusocp_trimming_mid;
										dut3.g_vbusocp_step = 0x001a;
									}
									else
									{
										if(dut3.g_vbusocp_retry_time>0)
										{
											//dut3.g_calibration_offset++;
											dut3.g_calibration_offset++;
											dut3.g_vbusocp_retry_time--;
											dut3.g_vbusocp_step++;
										}
										else
										{
											dut3.g_result_fail = 0x01;
											dut3.g_result_fail_tmrcount = 0xffff;
											xil_printf("dut3_pattern7_calibrate_havn't suitable data! now_status:no ocp,pre_status:no ocp!\r\n");
											xil_printf("dut3.g_vbusocp_pre_flt_status = %x dut3_now_flt_status = %x\r\n", dut3.g_vbusocp_pre_flt_status,FLT_IN_Status);
											xil_printf("dut3.g_calibration_offset = %x\r\n", dut3.g_calibration_offset);
											//xil_printf("dut3_pattern7_0x0103_register havn't suitable data!    dut3.g_calibration_offset = %x\r\n", dut3.g_calibration_offset);
											//xil_printf("dut3_ocp_step = %x\r\n", dut3.g_vbusocp_step);
											//xil_printf("dut3_pattern7_step = %x\r\n", dut3.g_pattern_step);
										}
									}
								}
							}
						}
						else	//for 0.9A Trimming
						{
							if(dut3.g_vbusocp_trimming_low < dut3.g_vbusocp_trimming_high-1)
							{
								dut3.g_vbusocp_trimming_low = dut3.g_vbusocp_trimming_mid;
								dut3.g_vbusocp_trimming_mid = (dut3.g_vbusocp_trimming_low + dut3.g_vbusocp_trimming_high)/2;
								dut3.g_vbusocp_pre_flt_status = 1;
								dut3.g_calibration_offset = dut3.g_vbusocp_trimming_mid;
								dut3.g_vbusocp_step++;
								//xil_printf("dut3_g_calibration_offset = %x\r\n", dut3.g_calibration_offset);
							}
							else
							{
								if(dut3.g_vbusocp_pre_flt_status == 0)
								{
									dut3.g_calibration_offset=dut3.g_vbusocp_trimming_mid;
									dut3.g_vbusocp_step = 0x001a;
								}
								else
								{
									if(dut3.g_vbusocp_retry_time>0)
									{
										//dut3.g_calibration_offset++;
										dut3.g_calibration_offset++;
										dut3.g_vbusocp_retry_time--;
										dut3.g_vbusocp_step++;
									}
									else
									{
										dut3.g_result_fail = 0x01;
										dut3.g_result_fail_tmrcount = 0xffff;
										xil_printf("dut3_pattern7_calibrate_havn't suitable data! now_status:no ocp,pre_status:no ocp!\r\n");
										xil_printf("dut3.g_vbusocp_pre_flt_status = %x dut3_now_flt_status = %x\r\n", dut3.g_vbusocp_pre_flt_status,FLT_IN_Status);
										xil_printf("dut3.g_calibration_offset = %x\r\n", dut3.g_calibration_offset);
										//xil_printf("dut3_pattern7_step = %x\r\n", dut3.g_pattern_step);
									}
								}
							}
						}
					}

					//when FLT_IN_Status =0, vbus ocp occur
					if(FLT_IN_Status == 0)
					{
						if(dut3.g_vbusocp_trimming_low < dut3.g_vbusocp_trimming_high-1)
						{
							dut3.g_vbusocp_trimming_high = dut3.g_vbusocp_trimming_mid;
							dut3.g_vbusocp_trimming_mid = (dut3.g_vbusocp_trimming_low + dut3.g_vbusocp_trimming_high)/2;
							dut3.g_vbusocp_pre_flt_status = 0;
							dut3.g_calibration_offset = dut3.g_vbusocp_trimming_mid;
							dut3.g_vbusocp_step++;
						}
						else
						{
							if(dut3.g_vbusocp_pre_flt_status == 1)
							{
								dut3.g_calibration_offset=dut3.g_vbusocp_trimming_mid;
								dut3.g_vbusocp_step = 0x001a;
							}
							else
							{
								if(dut3.g_vbusocp_retry_time>0)
								{
									//dut3.g_calibration_offset--;
									dut3.g_calibration_offset--;
									dut3.g_vbusocp_retry_time--;
									dut3.g_vbusocp_step++;
								}
								else
								{
									dut3.g_result_fail = 0x01;
									dut3.g_result_fail_tmrcount = 0xffff;
									xil_printf("dut3_pattern7_calibrate_havn't suitable data! now_status:do ocp,pre_status:do ocp!\r\n");
									xil_printf("dut3.g_vbusocp_pre_flt_status = %x dut3_now_flt_status = %x\r\n", dut3.g_vbusocp_pre_flt_status,FLT_IN_Status);
									xil_printf("dut3.g_calibration_offset = %x\r\n", dut3.g_calibration_offset);
								}
							}
						}
					}

					for(i=1; i<60; i++)
					{
						dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
					}
					dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
					dut3.g_vbusocp_process_bit = vbusocp_process_doing;
					xil_printf("dut3_FLT_IN_Status = %x\r\n", FLT_IN_Status);
					//xil_printf("dut3_vbus_calibraion_ready_retry_time = %x\r\n", dut3.vbus_calibraion_ready_retry_time);
					//xil_printf("dut3_vbusocp_firsttrimming_retry_time = %x\r\n", (4-dut3.vbus_calibraion_ready_retry_time));
					//xil_printf("dut3_nextime_vbusocp_trimming_low = %x\r\n", dut3.g_vbusocp_trimming_low);
					//xil_printf("dut3_nextime_vbusocp_trimming_high = %x\r\n", dut3.g_vbusocp_trimming_high);
					//xil_printf("dut3_nextime_vbusocp_trimming_mid = %x\r\n", dut3.g_vbusocp_trimming_mid);
					xil_printf("dut3_nextime_calibration_offset = %x\r\n\r\n",dut3.g_calibration_offset);
	    		}
	    		//xil_printf("dut3_g_calibration_offset = %x\r\n", dut3.g_calibration_offset);
	    		//xil_printf("dut3_pattern7_step = %x\r\n", dut3.g_vbusocp_step);
	    		break;
	    	}

	    	//先将PHY reg REG_PSW_ISET_CFG1(0x103) bit[15:8]value值写0x00（OCP阈值最大档位）
	    	case 0x0012:
	    	{
	    		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writephy;
	    		dut3.g_pattern_smbus_control_buf[2] = 0x03;
	    		dut3.g_pattern_smbus_control_buf[3] = 0x01;
				dut3.g_pattern_smbus_control_buf[4] = 0x01;
				dut3.g_pattern_smbus_control_buf[5] = 0x0;
				dut3.g_pattern_smbus_control_buf[6] = 0x0;

				smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
				if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
				{
					break;
				}
				else
				{
					for(i=1; i<60; i++)
					{
						dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
					}
					dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
					dut3.g_vbusocp_process_bit = vbusocp_process_doing;
					dut3.g_vbusocp_step++;
					//dut3.g_vbusocp_step++;
				}
				//xil_printf("dut3.g_vbusocp_step = %x\r\n",dut3.g_vbusocp_step);
				break;
	    	}

	    	//VBUS_OUT上负载电路关闭，不抽电
	    	case 0x0013:
	    	{
	    		//this is control vbus board,base addr is dutx
	    		XGpio_WriteBit(XPAR_AXI_GPIO_dut3_1_BASEADDR,0,23, 0);
	    		XGpio_WriteBit(XPAR_AXI_GPIO_dut3_1_BASEADDR,0,26, 0);
	    		XGpio_WriteBit(XPAR_AXI_GPIO_dut3_1_BASEADDR,0,25, 0);
	    		msdelay(10);
	    		dut3.g_vbusocp_process_bit = vbusocp_process_doing;
	    		dut3.g_vbusocp_step++;
	    	}

	    	//先将PSW_EN drive low，关闭VBUS电压， delay时间15ms；
	    	case 0x0014:
	    	{
	    		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
	    		dut3.g_pattern_smbus_control_buf[2] = 0x10;  //qfn20 0x1110
	    		dut3.g_pattern_smbus_control_buf[3] = 0x11;
	    		dut3.g_pattern_smbus_control_buf[4] = 0x01;
	    		dut3.g_pattern_smbus_control_buf[5] = 0x11;

	    		smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
	    		if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
	    		{
	    			break;
	    		}
	    		else
	    		{
	    			for(i=1; i<60; i++)
	    			{
	    				dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
	    			}
	    			dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
	    			dut3.g_vbusocp_process_bit = vbusocp_process_doing;
	    			msdelay(100);
	    			dut3.g_vbusocp_step++;
	    		}
	    		//xil_printf("dut3.g_vbusocp_step = %x\r\n",dut3.g_vbusocp_step);
	    		break;
	    	}

	    	//confirm power_en has been pull high
	    	case 0x0015:
	    	{
	    		msdelay(1);
	    		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readmem;
	    		dut3.g_pattern_smbus_control_buf[2] = 0x10;
	    		dut3.g_pattern_smbus_control_buf[3] = 0x11;
	    		dut3.g_pattern_smbus_control_buf[4] = 0x01;

	    		smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
	    		if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
	    		{
	    			break;
	    		}
	    		else
	    		{
	    			vbus_en=((dut3.g_pattern_smbus_control_buf[10]&0x11)==0x11);
	    			//xil_printf("dut3_vbus_power_en_register_data = 0x%x,register_slave= 0x%x\r\n",dut3.g_pattern_smbus_control_buf[10],0x1110);

	    			if(vbus_en==1)
	    			{
	    				dut3.g_vbusocp_step++;
	    			}
	    			else
	    			{
	    				dut3.g_result_fail = 0x01;
	    				dut3.g_result_fail_tmrcount = 0xffff;
	    			}
	    			for(i=1; i<60; i++)
	    			{
	    				dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
	    			}
	    			dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
	    			dut3.g_vbusocp_process_bit = vbusocp_process_doing;
				}
	    		//xil_printf("dut3.g_result_fail = %x\r\n",dut3.g_result_fail);
	    		//xil_printf("dut3.g_vbusocp_step = %x\r\n",dut3.g_vbusocp_step);
	    		break;
	    	}

	    	//通过SMBus指令控制PSW_EN drive high，开启VBUS电压，检测VBUS电压
	    	//QFN12：GPIO8（PSW_EN） 改为输出high，先读出确认当前是否是no pull，no drive
	    	//将reg SYS_GPIO_CTL(0x1110)  bit0 写入1，bit1写入1
	    	case 0x0016:
	    	{
	    		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
	    		dut3.g_pattern_smbus_control_buf[2] = 0x10;  //qfn20 0x1110
	    		dut3.g_pattern_smbus_control_buf[3] = 0x11;
	    		dut3.g_pattern_smbus_control_buf[4] = 0x01;
	    		dut3.g_pattern_smbus_control_buf[5] = 0x13;

	    		smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
	    		if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
	    		{
	    			break;
	    		}
	    		else
	    		{
	    			for(i=1; i<60; i++)
	    			{
	    				dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
	    			}
	    			dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
	    			dut3.g_vbusocp_process_bit = vbusocp_process_doing;
	    			dut3.g_vbusocp_step++;
	    			msdelay(15);
	    			dut3.vbus_load_retry_time=4;
	    			dut3.vbus_calibraion_ready_retry_time=4;
	    		}
	    		//xil_printf("dut3.g_vbusocp_step = %x\r\n",dut3.g_vbusocp_step);
	    		break;
	    	}

	    	//confirm power_en has been pull high
	    	case 0x0017:
	    	{
	    		msdelay(1);
	    		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readmem;
	    		dut3.g_pattern_smbus_control_buf[2] = 0x10;
	    		dut3.g_pattern_smbus_control_buf[3] = 0x11;
	    		dut3.g_pattern_smbus_control_buf[4] = 0x01;

	    		smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
	    		if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
	    		{
	    			break;
	    		}
	    		else
	    		{
	    			vbus_en=((dut3.g_pattern_smbus_control_buf[10]&0x13)==0x13);
	    			//xil_printf("dut3_vbus_power_en_register_data = 0x%x,register_slave= 0x%x\r\n",dut3.g_pattern_smbus_control_buf[10],0x1110);

					if(vbus_en==1)
					{
						dut3.g_vbusocp_step++;
					}
					else
					{
						dut3.g_result_fail = 0x01;
						dut3.g_result_fail_tmrcount = 0xffff;
					}
					for(i=1; i<60; i++)
					{
						dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
					}
					dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
					dut3.g_vbusocp_process_bit = vbusocp_process_doing;
	    		}
	    		//xil_printf("dut3.g_result_fail = %x\r\n",dut3.g_result_fail);
	    		//xil_printf("dut3.g_vbusocp_step = %x\r\n",dut3.g_vbusocp_step);
	    		break;
	    	}

	    	//open vbus_in power
	    	//S0_FPGA_RELAY10 output high
	    	case 0x0018:
	    	{
	    		if(dut3.g_vbusocp_3a_nmos_control ==1)
	    		{
	    			switch(g_vbusocp_3a_status)
	    			{
	    			case 0x00:
	    			{
	    				g_vbusocp_3a_status = 0x13;
	    				dut3.g_vbusocp_3a_en_status =1;
	    				break;
	    			}
					case 0x10:
					{
						g_vbusocp_3a_status = 0x13;
						dut3.g_vbusocp_3a_en_status =1;
						break;
					}
	    			}
	    		}
	    		//xil_printf("1dut3.g_vbusocp_3a_nmos_control = %x\r\n",dut3.g_vbusocp_3a_nmos_control);
	    		//xil_printf("1dut3.g_vbusocp_3a_en_status = %x\r\n",dut3.g_vbusocp_3a_en_status);
	    		//xil_printf("1g_vbusocp_3a_status = %x\r\n",g_vbusocp_3a_status);
	    		//xil_printf("dut3.g_vbusocp_board_en = %x\r\n",dut3.g_vbusocp_board_en);
	    		//xil_printf("dut3.g_vbusocp_1p5a_nmos_control = %x\r\n",dut3.g_vbusocp_1p5a_nmos_control);
	    		//xil_printf("dut3.g_vbusocp_3a_nmos_control = %x\r\n",dut3.g_vbusocp_3a_nmos_control);
	    		//xil_printf("dut3.g_vbusocp_0p9a_nmos_control = %x\r\n\r\n",dut3.g_vbusocp_0p9a_nmos_control);
	    		if(dut3.g_dut_pattern_status_buf[7] == 0x00)
	    		{
	    			if((g_vbusocp_3a_status==0) &&(dut3.g_vbusocp_3a_nmos_control==0))
	    			{
	    				//xil_printf("dut3_0p9a/1p5a_test~~\r\n");

	    				//this is control vbus board,base addr is dutx
						XGpio_WriteBit(XPAR_AXI_GPIO_dut3_1_BASEADDR,0,23, dut3.g_vbusocp_board_en);
						XGpio_WriteBit(XPAR_AXI_GPIO_dut3_1_BASEADDR,0,26, dut3.g_vbusocp_1p5a_nmos_control);
						XGpio_WriteBit(XPAR_AXI_GPIO_dut3_1_BASEADDR,0,25, dut3.g_vbusocp_3a_nmos_control);
						msdelay(50);
						dut3.g_dut_pattern_status_buf[7] = 0x01;
						dut3.g_vbusocp_readadc_ready = 0x01;  //ready to read adc vbus_out pin
						//dut3.vbus_load_retry_time=6;
						//dut3.vbus_calibraion_ready_retry_time=6;
						//xil_printf("dut3.g_vbusocp_readadc_ready = %x\r\n", dut3.g_vbusocp_readadc_ready);
					}
	    			else if((dut3.g_vbusocp_3a_nmos_control==1)&&(dut3.g_vbusocp_3a_en_status==1))
	    			{
	    				//this is control vbus board,base addr is dutx
	    				XGpio_WriteBit(XPAR_AXI_GPIO_dut3_1_BASEADDR,0,23, dut3.g_vbusocp_board_en);
	    				XGpio_WriteBit(XPAR_AXI_GPIO_dut3_1_BASEADDR,0,26, dut3.g_vbusocp_1p5a_nmos_control);
	    				usdelay(200);
	    				XGpio_WriteBit(XPAR_AXI_GPIO_dut3_1_BASEADDR,0,25, dut3.g_vbusocp_3a_nmos_control);
	    				msdelay(50);
	    				dut3.g_dut_pattern_status_buf[7] = 0x01;
	    				dut3.g_vbusocp_readadc_ready = 0x01;
	    			}
	    			else
	    			{
	    				//xil_printf("dut3_return~~\r\n");
	    				return;
	    			}
	    		}
	    		else if(dut3.g_dut_pattern_status_buf[7] == 0x01)
	    		{
	    			if(dut3.g_relay_control_timer == 0 )
	    			{
	    				//msdelay(50);
	    				dut3.g_dut_pattern_status_buf[7] = 0x00;
	    				dut3.g_vbusocp_step++;
	    			}
	    		}
	    		else
	    		{
	    			dut3.g_result_fail = 0x01;
	    			dut3.g_result_fail_tmrcount = 0xffff;
	    		}
	    		break;
	    	}

	    	case 0x0019:
	    	{
	    		//msdelay(100);
	    		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writephy;
	    		dut3.g_pattern_smbus_control_buf[2] = 0x03;
	    		dut3.g_pattern_smbus_control_buf[3] = 0x01;
	    		dut3.g_pattern_smbus_control_buf[4] = 0x01;
	    		dut3.g_pattern_smbus_control_buf[5] = dut3.g_calibration_offset;
	    		dut3.g_pattern_smbus_control_buf[6] = dut3.g_calibration_offset;

	    		smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
	    		if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
	    		{
	    			break;
	    		}
				else
				{
					for(i=1; i<60; i++)
					{
						dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
					}
					dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
					dut3.g_vbusocp_step = 0x0010;
					msdelay(15); //ocp writemem debounce time
					//dut3.g_result_fail = 0x01;
				}
	    		break;
	    	}
/*
	    	case 0x001a:
	    	{
	    		//msdelay(100);
				dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writephy;
				dut3.g_pattern_smbus_control_buf[2] = 0x03;
				dut3.g_pattern_smbus_control_buf[3] = 0x01;
				dut3.g_pattern_smbus_control_buf[4] = 0x01;
				dut3.g_pattern_smbus_control_buf[5] = dut3.g_calibration_offset;
				dut3.g_pattern_smbus_control_buf[6] = dut3.g_calibration_offset;

				smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
				if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
				{
					break;
				}
				else
				{
					for(i=1; i<60; i++)
					{
						dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
					}
					dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
					dut3.g_vbusocp_step++;
					msdelay(15); //ocp writemem debounce time
					//dut3.g_result_fail = 0x01;
				}
				//xil_printf("dut3_pattern7_step = %x\r\n", dut3.g_vbusocp_step);
				break;
	    	}
*/
	    	//read 0x0103 data and save to efuse buf
	    	case 0x001a:
	    	{
	    		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readphy;
	    		dut3.g_pattern_smbus_control_buf[2] = 0x03;
	    		dut3.g_pattern_smbus_control_buf[3] = 0x01;
	    		dut3.g_pattern_smbus_control_buf[4] = 0x01;

	    		smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
				if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
				{
					break;
				}
				else
				{
					dut3.g_vbus_ocp_calibrationdata_buf[0] = dut3.g_pattern_smbus_control_buf[10];
					dut3.g_vbus_ocp_calibrationdata_buf[1] = dut3.g_pattern_smbus_control_buf[11];
					//xil_printf("dut3.g_vbus_ocp_calibrationdata_buf[1] = %x\r\n",dut3.g_vbus_ocp_calibrationdata_buf[1]);

					dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
					dut3.g_vbusocp_step++;
				}
				//xil_printf("dut3.g_vbusocp_step = %x\r\n",dut3.g_vbusocp_step);
				break;
	    	}

	    	//close vbus_in power
	    	//S0_FPGA_RELAY10 output low
	    	case 0x001b:
	    	{
				//this is control vbus board,base addr is dutx
				XGpio_WriteBit(XPAR_AXI_GPIO_dut3_1_BASEADDR,0,23, 0);
				XGpio_WriteBit(XPAR_AXI_GPIO_dut3_1_BASEADDR,0,26, 0);
				XGpio_WriteBit(XPAR_AXI_GPIO_dut3_1_BASEADDR,0,25, 0);
				msdelay(2);

				dut3.g_dut_pattern_status_buf[7] = 0x01;
				dut3.g_vbusocp_process_bit = vbusocp_process_done_pass;
				dut3.g_vbusocp_readadc_ready = 0x00;
				//xil_printf("dut3.g_vbusocp_3a_nmos_control = %x\r\n",dut3.g_vbusocp_3a_nmos_control);
				//xil_printf("dut3.g_vbusocp_3a_en_status = %x\r\n",dut3.g_vbusocp_3a_en_status);
				if((dut3.g_vbusocp_3a_nmos_control ==1) &&(dut3.g_vbusocp_3a_en_status==1))
				{
					g_vbusocp_3a_status=0;
					dut3.g_vbusocp_3a_en_status=0;
				}

				if(dut3.g_relay_control_timer == 0)
				{
					msdelay(1);
					dut3.g_dut_pattern_status_buf[7] = 0x00;
					dut3.g_vbusocp_step=0;
				}
				break;
	    	}
	    	break;
	    	}
	    }
	}

	if(dut3.g_result_fail == 0x01)
	{
		XGpio_WriteBit(XPAR_AXI_GPIO_dut3_1_BASEADDR,0,23, 0);
		XGpio_WriteBit(XPAR_AXI_GPIO_dut3_1_BASEADDR,0,26, 0);
		XGpio_WriteBit(XPAR_AXI_GPIO_dut3_1_BASEADDR,0,25, 0);
		msdelay(2);

		if((dut3.g_vbusocp_3a_nmos_control ==1) &&(dut3.g_vbusocp_3a_en_status==1))
		{
			g_vbusocp_3a_status=0;
			dut3.g_vbusocp_3a_en_status=0;
		}
	}
	//xil_printf("dut3.g_result_fail = %x\r\n",dut3.g_result_fail);
	//xil_printf("dut3.g_vbusocp_3a_nmos_control = %x\r\n",dut3.g_vbusocp_3a_nmos_control);
	//xil_printf("dut3.g_vbusocp_3a_en_status = %x\r\n",dut3.g_vbusocp_3a_en_status);
	//xil_printf("g_vbusocp_3a_status = %x\r\n",g_vbusocp_3a_status);
	//xil_printf("dut3.g_vbusocp_step = %x\r\n\r\n",dut3.g_vbusocp_step);
	//xil_printf("dut3_vbus_calibraion_ready_retry_time=0x%x\r\n",dut3.vbus_calibraion_ready_retry_time);

	return 0;
}



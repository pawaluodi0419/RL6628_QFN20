#include "platform.h"
#include "adc_vmon_calibration.h"

u8 _vmon_calibration_dut0()
{
	u8 i;
	switch(dut0.g_vmoncompare_process_bit)
	{
	case vmon_process_waiting:
	{
		dut0.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		dut0.g_pattern_smbus_control_buf[2] = 0x47;
		dut0.g_pattern_smbus_control_buf[3] = 0x10;
		dut0.g_pattern_smbus_control_buf[4] = 0x01;
		dut0.g_pattern_smbus_control_buf[5] = ((dut0.g_vmon_calib_level &0x07) |(dut0.g_vmon_temp_voltage[2]&0xF8));

		//xil_printf("dut0.g_pattern_smbus_control_buf[5] = %x\r\n",dut0.g_pattern_smbus_control_buf[5]);
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
			dut0.g_vmoncompare_process_bit = vmon_process_doing;
			dut0.g_vmon_compare_step++;
		}

		break;
	}

	case vmon_process_doing:
	{
		switch(dut0.g_vmon_compare_step)
		{
		case 0x0000:
		{
			dut0.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		    dut0.g_pattern_smbus_control_buf[2] = 0x47;
		    dut0.g_pattern_smbus_control_buf[3] = 0x10;
		    dut0.g_pattern_smbus_control_buf[4] = 0x01;
		    dut0.g_pattern_smbus_control_buf[5] = ((dut0.g_vmon_calib_level &0x07) |(dut0.g_vmon_temp_voltage[2]&0xF8));

		    //xil_printf("dut0.g_pattern_smbus_control_buf[5] = %x\r\n",dut0.g_pattern_smbus_control_buf[5]);
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
		    	dut0.g_vmoncompare_process_bit = vmon_process_doing;
		    	dut0.g_vmon_compare_step++;
		    }

		    break;
		}

		case 0x0001:
		{
			XGpio_WriteBit(XPAR_AXI_GPIO_dut0_1_BASEADDR,0,5, 1);	//S0-VMON_TRIM_EN
			msdelay(10);

	    	dut0.g_vmon_compare_step++;
	    	dut0.g_vmoncompare_process_bit = vmon_process_doing;

			break;
		}

		//write REG_VBUS_COMP_CFG4 reg 0x1049 bit[4:0] (AIF:REG_VTH2_TUNE) with 0x0f
		case 0x0002:
		{
			dut0.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		    dut0.g_pattern_smbus_control_buf[2] = 0x49;
		    dut0.g_pattern_smbus_control_buf[3] = 0x10;
		    dut0.g_pattern_smbus_control_buf[4] = 0x01;
		    dut0.g_pattern_smbus_control_buf[5] = 0x0f;

		    smbus0_irq_handle(dut0.g_pattern_smbus_control_buf);
		    if(dut0.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		    {
		    	break;
		    }
		    else
		    {
		    	dut0.g_vmon_calibration_data_buf[0] = 0x0f;
		    	for(i=1; i<31; i++)
		    	{
		    		dut0.g_pattern_smbus_control_buf[i] = CLEAR_;
		    	}
		    	dut0.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
		    	dut0.g_vmoncompare_process_bit = vmon_process_doing;
		    	dut0.g_vmon_compare_step++;
		    }
		    break;
		}

		//read REG_VBUS_COMP_DBC2 reg 0x1045 bit5 and set calibration register signbit
		case 0x0003:
		{
			dut0.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readmem;
			dut0.g_pattern_smbus_control_buf[2] = 0x45;
			dut0.g_pattern_smbus_control_buf[3] = 0x10;
			dut0.g_pattern_smbus_control_buf[4] = 0x01;

			smbus0_irq_handle(dut0.g_pattern_smbus_control_buf);
			if(dut0.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
			{
				break;
			}
			else
			{

				//xil_printf("dut0.g_pattern_smbus_control_buf[10] =%x\r\n", dut0.g_pattern_smbus_control_buf[10]);
				if(((dut0.g_pattern_smbus_control_buf[10] >>5) & 0x01) == 1)
				{
					dut0.g_calibrate_signbit = 0;
					dut0.g_calibration_offset = 1;
					dut0.g_vmon_compare_step++;
				}
				else
				{
					dut0.g_calibrate_signbit = 1;
					dut0.g_calibration_offset = 1;
					dut0.g_vmon_compare_step++;
				}

				for(i=0; i<31; i++)
				{
					dut0.g_pattern_smbus_control_buf[i+1] = CLEAR_;
				}
				dut0.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
				dut0.g_vmoncompare_process_bit = vmon_process_doing;

				//xil_printf("dut0.g_calibrate_signbit =%x\r\n", dut0.g_calibrate_signbit);
				//xil_printf("dut0.g_calibration_offset =%x\r\n", dut0.g_calibration_offset);
			}
			break;
		}

		//write to calibration mem
		case 0x0004:
		{
			dut0.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
			dut0.g_pattern_smbus_control_buf[2] = 0x49;
			dut0.g_pattern_smbus_control_buf[3] = 0x10;
			dut0.g_pattern_smbus_control_buf[4] = 0x01;

			if(dut0.g_calibrate_signbit == 0)
			{
				if((dut0.g_calibration_offset >=0) && (dut0.g_calibration_offset <= 0x10))	//ÓÒÒÆ
				{
					dut0.g_pattern_smbus_control_buf[5] = (dut0.g_vmon_calibration_data_buf[0]+dut0.g_calibration_offset);
				}
				else
				{
					dut0.g_result_fail = 0x01;
					dut0.g_result_fail_tmrcount = 0xffff;
				}
			}
			else if(dut0.g_calibrate_signbit == 1)
			{
				if((dut0.g_calibration_offset >=0) && (dut0.g_calibration_offset <= 0x0f))	//×óÒÆ
				{
					dut0.g_pattern_smbus_control_buf[5] = (dut0.g_vmon_calibration_data_buf[0]-dut0.g_calibration_offset);
				}
				else
				{
					dut0.g_result_fail = 0x01;
					dut0.g_result_fail_tmrcount = 0xffff;
				}
			}
			else
			{
				dut0.g_result_fail = 0x01;
				dut0.g_result_fail_tmrcount = 0xffff;
			}

			smbus0_irq_handle(dut0.g_pattern_smbus_control_buf);
			if(dut0.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
			{

			}
			else
			{
				for(i=1; i<31; i++)
				{
					dut0.g_pattern_smbus_control_buf[i] = CLEAR_;
				}
				dut0.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
				dut0.g_vmoncompare_process_bit = vmon_process_doing;
				dut0.g_vmon_compare_step++;
			}
			break;
		}

		//read 0x1045 data and calibration
		case 0x0005:
		{
			dut0.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readmem;
			dut0.g_pattern_smbus_control_buf[2] = 0x45;
			dut0.g_pattern_smbus_control_buf[3] = 0x10;
			dut0.g_pattern_smbus_control_buf[4] = 0x01;

			smbus0_irq_handle(dut0.g_pattern_smbus_control_buf);

			if(dut0.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
			{
				break;
			}
			else
			{
				if(((dut0.g_pattern_smbus_control_buf[10] >> 5) & 0x01) == 0)
				{
					if(dut0.g_calibrate_signbit == 1)
					{
						if((dut0.g_calibration_offset >=0) && (dut0.g_calibration_offset < 0x0f))
						{
							dut0.g_calibration_offset++;
							dut0.g_vmon_compare_step--;
						}
						else
						{
							dut0.g_result_fail = 0x01;
							dut0.g_result_fail_tmrcount = 0xffff;
						}
					}

					else if(dut0.g_calibrate_signbit == 0)
					{
						if((dut0.g_calibration_offset >=0) && (dut0.g_calibration_offset <= 0x10))
						{
							dut0.g_vmon_compare_step++;
						}
						else
						{
							dut0.g_result_fail = 0x01;
							dut0.g_result_fail_tmrcount = 0xffff;
						}
					}
				}

				else if(((dut0.g_pattern_smbus_control_buf[10] >> 5) & 0x01) == 1)
				{
					if(dut0.g_calibrate_signbit == 0)
					{
						if((dut0.g_calibration_offset >=0) && (dut0.g_calibration_offset < 0x10))
						{
							dut0.g_calibration_offset++;
							dut0.g_vmon_compare_step--;
						}
						else
						{
							dut0.g_result_fail = 0x01;
							dut0.g_result_fail_tmrcount = 0xffff;
						}
					}

					else if(dut0.g_calibrate_signbit == 1)
					{
						if((dut0.g_calibration_offset >=0) && (dut0.g_calibration_offset <= 0x0f))
						{
							dut0.g_vmon_compare_step++;
						}
						else
						{
							dut0.g_result_fail = 0x01;
							dut0.g_result_fail_tmrcount = 0xffff;
						}
					}
				}

				for(i=1; i<31; i++)
				{
					dut0.g_pattern_smbus_control_buf[i] = CLEAR_;
				}
				dut0.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			}

			break;
		}

		//read 0x1049 data and save to efuse buf
		case 0x0006:
		{
			dut0.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readmem;
			dut0.g_pattern_smbus_control_buf[2] = 0x49;
			dut0.g_pattern_smbus_control_buf[3] = 0x10;
			dut0.g_pattern_smbus_control_buf[4] = 0x01;

			smbus0_irq_handle(dut0.g_pattern_smbus_control_buf);
			if(dut0.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
			{
				break;
			}
			else
			{
				dut0.g_vmon_calibration_data_buf[2] = dut0.g_pattern_smbus_control_buf[10];
				dut0.g_vmon_calibration_data_buf[3] = dut0.g_pattern_smbus_control_buf[11];

				dut0.g_vmoncompare_process_bit = vmon_process_done_pass;
				dut0.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
				dut0.g_vmon_compare_step = 0;
			}

			break;
		}
		break;
		}
	}
	}

	return 0;
}



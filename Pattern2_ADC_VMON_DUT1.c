#include "platform.h"
#include "Pattern2_ADC_VMON.h"
#include "adc_vmon_calibration.h"

u8 _by_Pattern2_ADCcalibration_dut1()
{
	u8 i;
	u16 adc_code;

	u8 Buff_dut1_XGPIO_0[8];

	switch(dut1.g_pattern_step)
	{
	//read 0x1045 data
	case 0x0000:
	{
		dut1.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readmem;
		dut1.g_pattern_smbus_control_buf[2] = 0x45;
		dut1.g_pattern_smbus_control_buf[3] = 0x10;
		dut1.g_pattern_smbus_control_buf[4] = 0x01;

		smbus1_irq_handle(dut1.g_pattern_smbus_control_buf);
		if(dut1.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			dut1.g_vmon_temp_voltage[0] = dut1.g_pattern_smbus_control_buf[10];		//低8位
			dut1.g_vmon_temp_voltage[1] = dut1.g_pattern_smbus_control_buf[11];		//高8位
			//xil_printf("dut1.g_vmon_temp_voltage[0] =%x\r\n", dut1.g_vmon_temp_voltage[0]);
			//xil_printf("dut1.g_vmon_temp_voltage[1] =%x\r\n", dut1.g_vmon_temp_voltage[1]);

			for(i=0; i<31; i++)
			{
				dut1.g_pattern_smbus_control_buf[i+1] = CLEAR_;
			}
			dut1.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut1.g_pattern_step++;
		}
		break;
	}

	//REG_VBUS_COMP_DBC2 reg 0x1045 bit7(AIF:cfg_vbus_comp_en)write1‘b1
	case 0x0001:
	{
		dut1.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		dut1.g_pattern_smbus_control_buf[2] = 0x45;
		dut1.g_pattern_smbus_control_buf[3] = 0x10;
		dut1.g_pattern_smbus_control_buf[4] = 0x01;
		dut1.g_pattern_smbus_control_buf[5] = (dut1.g_vmon_temp_voltage[0] |(1<<7));

		//xil_printf("dut1.g_pattern_smbus_control_buf[5] = %x\r\n",dut1.g_pattern_smbus_control_buf[5]);

		smbus1_irq_handle(dut1.g_pattern_smbus_control_buf);
		if(dut1.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			for(i=1; i<31; i++)
			{
				dut1.g_pattern_smbus_control_buf[i] = CLEAR_;
			}
			dut1.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut1.g_pattern_step++;
		}
		break;
	}

	//read 0x1047 data
	case 0x0002:
	{
		dut1.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readmem;
		dut1.g_pattern_smbus_control_buf[2] = 0x47;
		dut1.g_pattern_smbus_control_buf[3] = 0x10;
		dut1.g_pattern_smbus_control_buf[4] = 0x01;

		smbus1_irq_handle(dut1.g_pattern_smbus_control_buf);
		if(dut1.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			dut1.g_vmon_temp_voltage[2] = dut1.g_pattern_smbus_control_buf[10];
			//xil_printf("dut1.g_vmon_temp_voltage[2] =%x\r\n", dut1.g_vmon_temp_voltage[2]);

			for(i=0; i<31; i++)
			{
				dut1.g_pattern_smbus_control_buf[i+1] = CLEAR_;
			}
			dut1.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut1.g_pattern_step++;
		}
		break;
	}

	/****************************************
	*VMON_calibrator Calibration start!
	*1.enable VBUS低压版比较器， 将
	*REG_VBUS_COMP_DBC2 reg 0x1045 bit7(AIF:cfg_vbus_comp_en)置1‘b1;
	*REG_VBUS_COMP_CFG2 reg 0x1047 bit2(AIF:REG_POW2)置1'b1;
	*2.REG_VBUS_COMP_CFG2 reg 0x1047 bit[1:0] (AIF:REG_VMON_SEL)写2'b00, 将分压电阻切到0.6V档位
	*3.VMON_APAD外灌0.6V 电压
	*4.改变REG_VBUS_COMP_CFG4 reg 0x1049 bit[4:0] (AIF:REG_VTH2_TUNE)，让比较器输出翻转
	*5.通过REG_VBUS_COMP_DBC2 reg 0x1045 bit5确认比较器输出翻转;
	*6.将REG_VBUS_COMP_CFG4 reg 0x1049 bit[4:0] (AIF:REG_VTH2_TUNE )烧入规划的efuse:VBUS 0.6V Cal[5:0]
	*7.efuse:VBUS 1V Cal[4:0]，VBUS 2.5V Cal[4:0]，VBUS 3V Cal[4:0]写入和VBUS 0.6V Cal[4:0]同样的值；
	******************************************/

	case 0x0003:
	{
		dut1.g_vmon_calib_level = vmon_calib_600mv;
		//dut1.g_vmon_calib_voltage = 600;
		_vmon_calibration_dut1();

		if(dut1.g_vmoncompare_process_bit != vmon_process_done_pass)
		{

		}
		else
		{
			dut1.g_vmon_writeefuse_600mv[0] = dut1.g_vmon_calibration_data_buf[2];
			dut1.g_vmon_writeefuse_600mv[1] = dut1.g_vmon_calibration_data_buf[3];
			//xil_printf("dut1.g_vmon_writeefuse_600mv[0] = %x\r\n",dut1.g_vmon_writeefuse_600mv[0]);
			//xil_printf("dut1.g_vmon_writeefuse_600mv[1] = %x\r\n",dut1.g_vmon_writeefuse_600mv[1]);

			if((dut1.g_vmon_writeefuse_600mv[0]>VMON_Efuse_Min)&&(dut1.g_vmon_writeefuse_600mv[0]<VMON_Efuse_Max))
			{
				for(i=0;i<4;i++)
				{
					dut1.g_vmon_calibration_data_buf[i] = 0;
				}
				dut1.g_vmoncompare_process_bit = vmon_process_waiting;
				dut1.g_vmon_compare_step = 0;
				dut1.g_pattern_step++;
			}
			else
			{
				dut1.g_result_fail = 0x01;
				xil_printf("dut1.g_vmon_writeefuse_600mv[0] =%x VMON_Efuse_Min =%x VMON_Efuse_Max =%x\r\n",dut1.g_vmon_writeefuse_600mv[0],VMON_Efuse_Min,VMON_Efuse_Max);
				xil_printf("dut1.g_vmon_writeefuse fail, not fit spec!\r\n");
			}
		}
		break;
	}

	case 0x0004:
	{
		XGpio_WriteBit(XPAR_AXI_GPIO_dut1_1_BASEADDR,0,5, 0);	//S0-VMON_TRIM_EN
		msdelay(10);

		for(i=1; i<31; i++)
		{
			dut1.g_pattern_smbus_control_buf[i] = CLEAR_;
		}
		dut1.g_pattern_timer = 0xfff;
		dut1.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
		dut1.g_pattern_step = 0x00;
		dut1.g_dut_pattern_status_buf[2]++;
		dut1.g_uartPatternEnable = 0x00;
		dut1.g_uartPatternNum = (dut1.g_uartPatternNum++ & 0xff);
		result_output_for_v50(XPAR_AXI_GPIO_dut1_1_BASEADDR,dut1.g_uartPatternNum);
		xil_printf("dut1_pattern2_adc_vmon_calibration_pass! vmon_calibration_data= 0x%x\r\n\r\n",dut1.g_vmon_writeefuse_600mv[0]);

		break;
	}
	}

	if(dut1.g_result_fail == 0x01)
	{
		dut1.g_ft2_test_done = 0x00;
		pattern_safe_state_dut1();	//add safe state
		xil_printf("dut1_pattern2_fail! fail_step = %x\r\n\r\n",dut1.g_pattern_step);
	}
	return 0;
}



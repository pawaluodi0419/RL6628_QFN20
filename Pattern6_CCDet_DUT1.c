#include "platform.h"
#include "Pattern6_CCDet.h"
#include "ccdet_calibration_dut.h"

//dut1 Pattern 6£ºLDO3V3 Calibration and test
u8 _by_Pattern6_CCDet_dut1()
{
	u8 i;

	u8 Buff_dut1_XGPIO_0[8];

	switch(dut1.g_ccdet_step)
	{
	case 0x0000:
	{
		//gpio17: low bit, gpio20: high bit
		XGpio_WriteBit(XPAR_AXI_GPIO_dut1_1_BASEADDR,0,17, 0);
		XGpio_WriteBit(XPAR_AXI_GPIO_dut1_1_BASEADDR,0,18, 1);
		XGpio_WriteBit(XPAR_AXI_GPIO_dut1_1_BASEADDR,0,19, 1);
		XGpio_WriteBit(XPAR_AXI_GPIO_dut1_1_BASEADDR,0,20, 0);
		msdelay(5);

		//enable ADG1604
		XGpio_WriteBit(XPAR_AXI_GPIO_dut1_1_BASEADDR,0,13, 1);	//S0-CC1_TEST_EN
		XGpio_WriteBit(XPAR_AXI_GPIO_dut1_1_BASEADDR,0,14, 1);	//S0-CC2_TEST_EN
		msdelay(5);
		//select ADG1604 S1 channel
		XGpio_WriteBit(XPAR_AXI_GPIO_dut1_1_BASEADDR,0,9, 0);	//S0-CC1_TEST_SEL0
		XGpio_WriteBit(XPAR_AXI_GPIO_dut1_1_BASEADDR,0,10, 1);	//S0-CC1_TEST_SEL1

		XGpio_WriteBit(XPAR_AXI_GPIO_dut1_1_BASEADDR,0,24, 0);	//S0-CC2_TEST_SEL0
		XGpio_WriteBit(XPAR_AXI_GPIO_dut1_1_BASEADDR,0,27, 1);	//S0-CC2_TEST_SEL1
		msdelay(5);

		dut1.g_ccdet_step++;
	}

	//cc1 0.2v
	case 0x0001:
	{
		dut1.g_dac_data_buf[0] = 0x01;
		dut1.g_dac_data_buf[1] = 0x90;
		dut1.g_dac_data_buf[2] = 0x01;
		dut1.g_dac_data_buf[3] = 0x68;
		dut1.g_dac_data_buf[4] = 0x01;
		dut1.g_dac_data_buf[5] = 0xb8;
		dut1.g_dac_data_buf[6] = 0x01;

		dut1.g_ccdet_phy_calibration_addr_buf[0] = 0x0f;
		dut1.g_ccdet_phy_calibration_addr_buf[1] = 0x01;
		dut1.g_ccdet_voltage_sel = 0x55;
		dut1.g_ccdet_calibration_data_buf[0]     = dut1.g_ccdet_calibration_data_buf[4];
		dut1.g_ccdet_calibration_data_buf[1]     = dut1.g_ccdet_calibration_data_buf[5];
		dut1.g_ccdet_phy_calibration_bit         = 3;
		dut1.g_ccdet_ana_result_bit              = 2;

		_ccdet_calibration_dut1();
		if(dut1.g_ccdet_process_bit != ccdet_process_done_pass)
		{

		}
		else
		{
			dut1.g_ccdet_calibration_data_buf[4] = dut1.g_ccdet_calibration_data_buf[2];
			dut1.g_ccdet_calibration_data_buf[5] = dut1.g_ccdet_calibration_data_buf[3];
			for(i=0;i<4;i++)
			{
				dut1.g_ccdet_calibration_data_buf[i] = 0;
			}
			dut1.g_ccdet_process_bit = ccdet_process_waiting;
			dut1.g_ccdet_retest_signbit = 0;
			dut1.g_ccdet_step++;
			dut1.g_pattern_step = 0;
			//////xil_printf("dut1.g_ccdet_step = %x\r\n",dut1.g_ccdet_step);
		}
			////////xil_printf("dut1_pattern6_CC1_0.2V_Calibration\r\n");
		    break;
	}

	//cc2 0.2v
	case 0x0002:
	{
		dut1.g_dac_data_buf[0] = 0x03;
		dut1.g_dac_data_buf[1] = 0x90;
		dut1.g_dac_data_buf[2] = 0x01;
		dut1.g_dac_data_buf[3] = 0x68;
		dut1.g_dac_data_buf[4] = 0x01;
		dut1.g_dac_data_buf[5] = 0xb8;
		dut1.g_dac_data_buf[6] = 0x01;

		dut1.g_ccdet_phy_calibration_addr_buf[0] = 0x0f;
		dut1.g_ccdet_phy_calibration_addr_buf[1] = 0x01;
		dut1.g_ccdet_voltage_sel = 0x55;
		dut1.g_ccdet_calibration_data_buf[0]     = dut1.g_ccdet_calibration_data_buf[4];
		dut1.g_ccdet_calibration_data_buf[1]     = dut1.g_ccdet_calibration_data_buf[5];
		dut1.g_ccdet_phy_calibration_bit         = 2;
		dut1.g_ccdet_ana_result_bit              = 5;

		_ccdet_calibration_dut1();
		if(dut1.g_ccdet_process_bit != ccdet_process_done_pass)
		{

		}
		else
		{
			dut1.g_ccdet_calibration_data_buf[4] = dut1.g_ccdet_calibration_data_buf[2];
			dut1.g_ccdet_calibration_data_buf[5] = dut1.g_ccdet_calibration_data_buf[3];
			for(i=0;i<4;i++)
			{
				dut1.g_ccdet_calibration_data_buf[i] = 0;
			}
			dut1.g_ccdet_process_bit = ccdet_process_waiting;
			dut1.g_ccdet_retest_signbit = 0;
			dut1.g_ccdet_step++;
			dut1.g_pattern_step = 0;
			//////xil_printf("dut1.g_ccdet_step = %x\r\n",dut1.g_ccdet_step);
		}
			////////xil_printf("dut1_pattern6_CC2_0.2V_Calibration\r\n");
		    break;
	}

	//cc1 0.4v
	case 0x0003:
	{
		dut1.g_dac_data_buf[0] = 0x01;
		dut1.g_dac_data_buf[1] = 0x20;
		dut1.g_dac_data_buf[2] = 0x03;
		dut1.g_dac_data_buf[3] = 0xe4;
		dut1.g_dac_data_buf[4] = 0x02;
		dut1.g_dac_data_buf[5] = 0x5c;
		dut1.g_dac_data_buf[6] = 0x03;

		dut1.g_ccdet_phy_calibration_addr_buf[0] = 0x0f;
		dut1.g_ccdet_phy_calibration_addr_buf[1] = 0x01;
		dut1.g_ccdet_voltage_sel = 0xaa;
		dut1.g_ccdet_calibration_data_buf[0]     = dut1.g_ccdet_calibration_data_buf[4];
		dut1.g_ccdet_calibration_data_buf[1]     = dut1.g_ccdet_calibration_data_buf[5];
		dut1.g_ccdet_phy_calibration_bit         = 1;
		dut1.g_ccdet_ana_result_bit              = 3;

		_ccdet_calibration_dut1();
		if(dut1.g_ccdet_process_bit != ccdet_process_done_pass)
		{

		}
		else
		{
			dut1.g_ccdet_calibration_data_buf[4] = dut1.g_ccdet_calibration_data_buf[2];
			dut1.g_ccdet_calibration_data_buf[5] = dut1.g_ccdet_calibration_data_buf[3];
			for(i=0;i<4;i++)
			{
				dut1.g_ccdet_calibration_data_buf[i] = 0;
			}
			dut1.g_ccdet_process_bit = ccdet_process_waiting;
			dut1.g_ccdet_retest_signbit = 0;

			dut1.g_ccdet_step++;
			dut1.g_pattern_step = 0;
			//////xil_printf("dut1.g_ccdet_step = %x\r\n",dut1.g_ccdet_step);
		}
			////////xil_printf("dut1_pattern6_CC1_0.4V_Calibration\r\n");
		    break;
	}

	//cc2 0.4v
	case 0x0004:
	{
		dut1.g_dac_data_buf[0] = 0x03;
		dut1.g_dac_data_buf[1] = 0x20;
		dut1.g_dac_data_buf[2] = 0x03;
		dut1.g_dac_data_buf[3] = 0xe4;
		dut1.g_dac_data_buf[4] = 0x02;
		dut1.g_dac_data_buf[5] = 0x5c;
		dut1.g_dac_data_buf[6] = 0x03;

		dut1.g_ccdet_phy_calibration_addr_buf[0] = 0x0f;
		dut1.g_ccdet_phy_calibration_addr_buf[1] = 0x01;
		dut1.g_ccdet_voltage_sel = 0xaa;
		dut1.g_ccdet_calibration_data_buf[0]     = dut1.g_ccdet_calibration_data_buf[4];
		dut1.g_ccdet_calibration_data_buf[1]     = dut1.g_ccdet_calibration_data_buf[5];
		dut1.g_ccdet_phy_calibration_bit         = 0;
		dut1.g_ccdet_ana_result_bit              = 6;

		_ccdet_calibration_dut1();
		if(dut1.g_ccdet_process_bit != ccdet_process_done_pass)
		{

		}
		else
		{
			dut1.g_ccdet_calibration_data_buf[4] = dut1.g_ccdet_calibration_data_buf[2];
			dut1.g_ccdet_calibration_data_buf[5] = dut1.g_ccdet_calibration_data_buf[3];
			for(i=0;i<4;i++)
			{
				dut1.g_ccdet_calibration_data_buf[i] = 0;
			}
			dut1.g_ccdet_process_bit = ccdet_process_waiting;
			dut1.g_ccdet_retest_signbit = 0;
			dut1.g_ccdet_step++;
			dut1.g_pattern_step = 0;
			//////xil_printf("dut1.g_ccdet_step = %x\r\n",dut1.g_ccdet_step);
		}
			////////xil_printf("dut1_pattern6_CC2_0.4V_Calibration\r\n");
		    break;
	}

	//cc1 0.8v
	case 0x0005:
	{
		dut1.g_dac_data_buf[0] = 0x01;
		dut1.g_dac_data_buf[1] = 0x40;
		dut1.g_dac_data_buf[2] = 0x06;
		dut1.g_dac_data_buf[3] = 0x04;
		dut1.g_dac_data_buf[4] = 0x06;
		dut1.g_dac_data_buf[5] = 0x08;
		dut1.g_dac_data_buf[6] = 0x07;

		dut1.g_ccdet_phy_calibration_addr_buf[0] = 0x10;
		dut1.g_ccdet_phy_calibration_addr_buf[1] = 0x01;
		dut1.g_ccdet_voltage_sel = 0xff;
		dut1.g_ccdet_calibration_data_buf[0]     = dut1.g_ccdet_calibration_data_buf[6];
		dut1.g_ccdet_calibration_data_buf[1]     = dut1.g_ccdet_calibration_data_buf[7];
		dut1.g_ccdet_phy_calibration_bit         = 1;
		dut1.g_ccdet_ana_result_bit              = 3;

		_ccdet_calibration_dut1();
		if(dut1.g_ccdet_process_bit != ccdet_process_done_pass)
		{

		}
		else
		{
			dut1.g_ccdet_calibration_data_buf[6] = dut1.g_ccdet_calibration_data_buf[2];
			dut1.g_ccdet_calibration_data_buf[7] = dut1.g_ccdet_calibration_data_buf[3];
			////xil_printf("dut1.g_ccdet_calibration_data_buf[2] = %x\r\n",dut1.g_ccdet_calibration_data_buf[2]);
			////xil_printf("dut1.g_ccdet_calibration_data_buf[3] = %x\r\n",dut1.g_ccdet_calibration_data_buf[3]);
			////xil_printf("dut1.g_ccdet_calibration_data_buf[6] = %x\r\n",dut1.g_ccdet_calibration_data_buf[6]);
			////xil_printf("dut1.g_ccdet_calibration_data_buf[7] = %x\r\n",dut1.g_ccdet_calibration_data_buf[7]);
			for(i=0;i<4;i++)
			{
				dut1.g_ccdet_calibration_data_buf[i] = 0;
			}
			dut1.g_ccdet_process_bit = ccdet_process_waiting;
			dut1.g_ccdet_retest_signbit = 0;
			dut1.g_ccdet_step++;
			dut1.g_pattern_step = 0;
			//////xil_printf("dut1.g_ccdet_step = %x\r\n",dut1.g_ccdet_step);
			////xil_printf("dut1_pattern6_CC1_0.8V_Calibration\r\n");
			////xil_printf("dut1.g_ccdet_calibration_data_buf[6] = %x\r\n",dut1.g_ccdet_calibration_data_buf[6]);
			////xil_printf("dut1.g_ccdet_calibration_data_buf[7] = %x\r\n",dut1.g_ccdet_calibration_data_buf[7]);
		}
			////////xil_printf("dut1_pattern6_CC1_0.8V_Calibration\r\n");
		    break;
	}

	//cc2 0.8v
	case 0x0006:
	{
		dut1.g_dac_data_buf[0] = 0x03;
		dut1.g_dac_data_buf[1] = 0x40;
		dut1.g_dac_data_buf[2] = 0x06;
		dut1.g_dac_data_buf[3] = 0x04;
		dut1.g_dac_data_buf[4] = 0x06;
		dut1.g_dac_data_buf[5] = 0x08;
		dut1.g_dac_data_buf[6] = 0x07;

		dut1.g_ccdet_phy_calibration_addr_buf[0] = 0x10;
		dut1.g_ccdet_phy_calibration_addr_buf[1] = 0x01;
		dut1.g_ccdet_voltage_sel = 0xff;
		dut1.g_ccdet_calibration_data_buf[0]     = dut1.g_ccdet_calibration_data_buf[6];
		dut1.g_ccdet_calibration_data_buf[1]     = dut1.g_ccdet_calibration_data_buf[7];
		dut1.g_ccdet_phy_calibration_bit         = 0;
		dut1.g_ccdet_ana_result_bit              = 6;

		_ccdet_calibration_dut1();
		if(dut1.g_ccdet_process_bit != ccdet_process_done_pass)
		{

		}
		else
		{
			dut1.g_ccdet_calibration_data_buf[6] = dut1.g_ccdet_calibration_data_buf[2];
			dut1.g_ccdet_calibration_data_buf[7] = dut1.g_ccdet_calibration_data_buf[3];
			////xil_printf("dut1.g_ccdet_calibration_data_buf[2] = %x\r\n",dut1.g_ccdet_calibration_data_buf[2]);
			////xil_printf("dut1.g_ccdet_calibration_data_buf[3] = %x\r\n",dut1.g_ccdet_calibration_data_buf[3]);
			////xil_printf("dut1.g_ccdet_calibration_data_buf[6] = %x\r\n",dut1.g_ccdet_calibration_data_buf[6]);
			////xil_printf("dut1.g_ccdet_calibration_data_buf[7] = %x\r\n",dut1.g_ccdet_calibration_data_buf[7]);
			for(i=0;i<4;i++)
			{
				dut1.g_ccdet_calibration_data_buf[i] = 0;
			}
			dut1.g_ccdet_process_bit = ccdet_process_waiting;
			dut1.g_ccdet_retest_signbit = 0;
			dut1.g_ccdet_step++;
			dut1.g_pattern_step = 0;
			//////xil_printf("dut1.g_ccdet_step = %x\r\n",dut1.g_ccdet_step);
			////xil_printf("dut1_pattern6_CC2_0.8V_Calibration\r\n");
			////xil_printf("dut1.g_ccdet_calibration_data_buf[6] = %x\r\n",dut1.g_ccdet_calibration_data_buf[6]);
			////xil_printf("dut1.g_ccdet_calibration_data_buf[7] = %x\r\n\r\n",dut1.g_ccdet_calibration_data_buf[7]);
		}
			////////xil_printf("dut1_pattern6_CC2_0.8V_Calibration\r\n");
		    break;
	}

	//cc1 0.66v
	case 0x0007:
	{
		dut1.g_dac_data_buf[0] = 0x01;
		dut1.g_dac_data_buf[1] = 0x28;
		dut1.g_dac_data_buf[2] = 0x05;
		dut1.g_dac_data_buf[3] = 0x00;
		dut1.g_dac_data_buf[4] = 0x05;
		dut1.g_dac_data_buf[5] = 0x50;
		dut1.g_dac_data_buf[6] = 0x05;

		dut1.g_ccdet_phy_calibration_addr_buf[0] = 0x10;
		dut1.g_ccdet_phy_calibration_addr_buf[1] = 0x01;
		dut1.g_ccdet_voltage_sel = 0x00;
		dut1.g_ccdet_calibration_data_buf[0]     = dut1.g_ccdet_calibration_data_buf[6];
		dut1.g_ccdet_calibration_data_buf[1]     = dut1.g_ccdet_calibration_data_buf[7];
		dut1.g_ccdet_phy_calibration_bit         = 3;
		dut1.g_ccdet_ana_result_bit              = 3;
		////xil_printf("dut1.g_ccdet_calibration_data_buf[0] = %x\r\n",dut1.g_ccdet_calibration_data_buf[0]);
		////xil_printf("dut1.g_ccdet_calibration_data_buf[1] = %x\r\n",dut1.g_ccdet_calibration_data_buf[1]);
		////xil_printf("dut1.g_ccdet_calibration_data_buf[6] = %x\r\n",dut1.g_ccdet_calibration_data_buf[6]);
		////xil_printf("dut1.g_ccdet_calibration_data_buf[7] = %x\r\n\r\n",dut1.g_ccdet_calibration_data_buf[7]);
		////xil_printf("dut1.g_calibration_offset =%x\r\n", dut1.g_calibration_offset);
		////xil_printf("dut1.g_ccdet_retest_signbit =%x\r\n", dut1.g_ccdet_retest_signbit);
		_ccdet_calibration_dut1();
		////xil_printf("dut1.g_ccdet_step = %x\r\n",dut1.g_ccdet_step);
		//xil_printf("dut1.g_pattern_step = %x\r\n",dut1.g_pattern_step);
		if(dut1.g_ccdet_process_bit != ccdet_process_done_pass)
		{

		}
		else
		{
			dut1.g_ccdet_calibration_data_buf[6] = dut1.g_ccdet_calibration_data_buf[2];
			dut1.g_ccdet_calibration_data_buf[7] = dut1.g_ccdet_calibration_data_buf[3];

			//xil_printf("dut1.g_ccdet_calibration_data_buf[2] = %x\r\n",dut1.g_ccdet_calibration_data_buf[2]);
			//xil_printf("dut1.g_ccdet_calibration_data_buf[3] = %x\r\n",dut1.g_ccdet_calibration_data_buf[3]);
			//xil_printf("dut1.g_ccdet_calibration_data_buf[6] = %x\r\n",dut1.g_ccdet_calibration_data_buf[6]);
			//xil_printf("dut1.g_ccdet_calibration_data_buf[7] = %x\r\n\r\n",dut1.g_ccdet_calibration_data_buf[7]);
			for(i=0;i<4;i++)
			{
				dut1.g_ccdet_calibration_data_buf[i] = 0;
			}
			dut1.g_ccdet_process_bit = ccdet_process_waiting;
			dut1.g_ccdet_retest_signbit = 0;
			dut1.g_ccdet_step++;
			dut1.g_pattern_step = 0;
			//////xil_printf("dut1.g_ccdet_step = %x\r\n",dut1.g_ccdet_step);
			////xil_printf("dut1_pattern6_CC1_0.66V_Calibration\r\n");
			////xil_printf("dut1.g_ccdet_calibration_data_buf[6] = %x\r\n",dut1.g_ccdet_calibration_data_buf[6]);

		}
			////////xil_printf("dut1_pattern6_CC1_0.66V_Calibration\r\n");
		    break;
	}

	//cc2 0.66v
	case 0x0008:
	{
		dut1.g_dac_data_buf[0] = 0x03;
		dut1.g_dac_data_buf[1] = 0x28;
		dut1.g_dac_data_buf[2] = 0x05;
		dut1.g_dac_data_buf[3] = 0x00;
		dut1.g_dac_data_buf[4] = 0x05;
		dut1.g_dac_data_buf[5] = 0x50;
		dut1.g_dac_data_buf[6] = 0x05;

		dut1.g_ccdet_phy_calibration_addr_buf[0] = 0x10;
		dut1.g_ccdet_phy_calibration_addr_buf[1] = 0x01;
		dut1.g_ccdet_voltage_sel = 0x00;
		dut1.g_ccdet_calibration_data_buf[0]     = dut1.g_ccdet_calibration_data_buf[6];
		dut1.g_ccdet_calibration_data_buf[1]     = dut1.g_ccdet_calibration_data_buf[7];
		dut1.g_ccdet_phy_calibration_bit         = 2;
		dut1.g_ccdet_ana_result_bit              = 6;

		_ccdet_calibration_dut1();
		if(dut1.g_ccdet_process_bit != ccdet_process_done_pass)
		{

		}
		else
		{
			dut1.g_ccdet_calibration_data_buf[6] = dut1.g_ccdet_calibration_data_buf[2];
			dut1.g_ccdet_calibration_data_buf[7] = dut1.g_ccdet_calibration_data_buf[3];
			////xil_printf("dut1.g_ccdet_calibration_data_buf[2] = %x\r\n",dut1.g_ccdet_calibration_data_buf[2]);
			////xil_printf("dut1.g_ccdet_calibration_data_buf[3] = %x\r\n",dut1.g_ccdet_calibration_data_buf[3]);
			////xil_printf("dut1.g_ccdet_calibration_data_buf[6] = %x\r\n",dut1.g_ccdet_calibration_data_buf[6]);
			////xil_printf("dut1.g_ccdet_calibration_data_buf[7] = %x\r\n",dut1.g_ccdet_calibration_data_buf[7]);
			for(i=0;i<4;i++)
			{
				dut1.g_ccdet_calibration_data_buf[i] = 0;
			}
			dut1.g_ccdet_process_bit = ccdet_process_waiting;
			dut1.g_ccdet_retest_signbit = 0;
			dut1.g_ccdet_step++;
			dut1.g_pattern_step = 0;
			//////xil_printf("dut1.g_ccdet_step = %x\r\n",dut1.g_ccdet_step);
			////xil_printf("dut1_pattern6_CC2_0.66V_Calibration\r\n");
			////xil_printf("dut1.g_ccdet_calibration_data_buf[6] = %x\r\n",dut1.g_ccdet_calibration_data_buf[6]);
			////xil_printf("dut1.g_ccdet_calibration_data_buf[7] = %x\r\n",dut1.g_ccdet_calibration_data_buf[7]);
		}
			////////xil_printf("dut1_pattern6_CC2_0.66V_Calibration\r\n");
		    break;
	}

	//cc1 1.23v
	case 0x0009:
	{
		dut1.g_dac_data_buf[0] = 0x01;
		dut1.g_dac_data_buf[1] = 0x9c;
		dut1.g_dac_data_buf[2] = 0x09;
		dut1.g_dac_data_buf[3] = 0x60;
		dut1.g_dac_data_buf[4] = 0x09;
		dut1.g_dac_data_buf[5] = 0xd8;
		dut1.g_dac_data_buf[6] = 0x09;

		dut1.g_ccdet_phy_calibration_addr_buf[0] = 0x11;
		dut1.g_ccdet_phy_calibration_addr_buf[1] = 0x01;
		dut1.g_ccdet_voltage_sel = 0x00;
		dut1.g_ccdet_calibration_data_buf[0]     = dut1.g_ccdet_calibration_data_buf[8];
		dut1.g_ccdet_calibration_data_buf[1]     = dut1.g_ccdet_calibration_data_buf[9];
		dut1.g_ccdet_phy_calibration_bit         = 3;
		dut1.g_ccdet_ana_result_bit              = 4;
		////////xil_printf("dut1_pattern6_CC1_1.23V_Calibration_before\r\n");
		_ccdet_calibration_dut1();
		if(dut1.g_ccdet_process_bit != ccdet_process_done_pass)
		{

		}
		else
		{
			dut1.g_ccdet_calibration_data_buf[8] = dut1.g_ccdet_calibration_data_buf[2];
			dut1.g_ccdet_calibration_data_buf[9] = dut1.g_ccdet_calibration_data_buf[3];
			for(i=0;i<4;i++)
			{
				dut1.g_ccdet_calibration_data_buf[i] = 0;
			}
			dut1.g_ccdet_process_bit = ccdet_process_waiting;
			dut1.g_ccdet_retest_signbit = 0;
			dut1.g_ccdet_step++;
			dut1.g_pattern_step = 0;
			//////xil_printf("dut1.g_ccdet_step = %x\r\n",dut1.g_ccdet_step);
		}
			////////xil_printf("dut1_pattern6_CC1_1.23V_Calibration\r\n");
		    break;
	}

	//cc2 1.23v
	case 0x000a:
	{
		dut1.g_dac_data_buf[0] = 0x03;
		dut1.g_dac_data_buf[1] = 0x9c;
		dut1.g_dac_data_buf[2] = 0x09;
		dut1.g_dac_data_buf[3] = 0x60;
		dut1.g_dac_data_buf[4] = 0x09;
		dut1.g_dac_data_buf[5] = 0xd8;
		dut1.g_dac_data_buf[6] = 0x09;

		dut1.g_ccdet_phy_calibration_addr_buf[0] = 0x11;
		dut1.g_ccdet_phy_calibration_addr_buf[1] = 0x01;
		dut1.g_ccdet_voltage_sel = 0x00;
		dut1.g_ccdet_calibration_data_buf[0]     = dut1.g_ccdet_calibration_data_buf[8];
		dut1.g_ccdet_calibration_data_buf[1]     = dut1.g_ccdet_calibration_data_buf[9];
		dut1.g_ccdet_phy_calibration_bit         = 2;
		dut1.g_ccdet_ana_result_bit              = 7;

		_ccdet_calibration_dut1();
		if(dut1.g_ccdet_process_bit != ccdet_process_done_pass)
		{

		}
		else
		{
			dut1.g_ccdet_calibration_data_buf[8] = dut1.g_ccdet_calibration_data_buf[2];
			dut1.g_ccdet_calibration_data_buf[9] = dut1.g_ccdet_calibration_data_buf[3];
			for(i=0;i<4;i++)
			{
				dut1.g_ccdet_calibration_data_buf[i] = 0;
			}
			dut1.g_ccdet_process_bit = ccdet_process_waiting;
			dut1.g_ccdet_retest_signbit = 0;
			dut1.g_ccdet_step++;
			dut1.g_pattern_step = 0;
			//////xil_printf("dut1.g_ccdet_step = %x\r\n",dut1.g_ccdet_step);
		}
			////////xil_printf("dut1_pattern6_CC2_1.23V_Calibration\r\n");
		    break;
	}

	//cc1 1.6v
	case 0x000b:
	{
		dut1.g_dac_data_buf[0] = 0x01;
		dut1.g_dac_data_buf[1] = 0x80;
		dut1.g_dac_data_buf[2] = 0x0c;
		dut1.g_dac_data_buf[3] = 0xf4;
		dut1.g_dac_data_buf[4] = 0x0b;
		dut1.g_dac_data_buf[5] = 0xbc;
		dut1.g_dac_data_buf[6] = 0x0c;

		dut1.g_ccdet_phy_calibration_addr_buf[0] = 0x11;
		dut1.g_ccdet_phy_calibration_addr_buf[1] = 0x01;
		dut1.g_ccdet_voltage_sel = 0x55;
		dut1.g_ccdet_calibration_data_buf[0]     = dut1.g_ccdet_calibration_data_buf[8];
		dut1.g_ccdet_calibration_data_buf[1]     = dut1.g_ccdet_calibration_data_buf[9];
		dut1.g_ccdet_phy_calibration_bit         = 1;
		dut1.g_ccdet_ana_result_bit              = 4;

		_ccdet_calibration_dut1();
		if(dut1.g_ccdet_process_bit != ccdet_process_done_pass)
		{
			break;
		}
		else
		{
			dut1.g_ccdet_calibration_data_buf[8] = dut1.g_ccdet_calibration_data_buf[2];
			dut1.g_ccdet_calibration_data_buf[9] = dut1.g_ccdet_calibration_data_buf[3];
			for(i=0;i<4;i++)
			{
				dut1.g_ccdet_calibration_data_buf[i] = 0;
			}
			dut1.g_ccdet_process_bit = ccdet_process_waiting;
			dut1.g_ccdet_retest_signbit = 0;
			dut1.g_ccdet_step++;
			dut1.g_pattern_step = 0;
			//////xil_printf("dut1.g_ccdet_step = %x\r\n",dut1.g_ccdet_step);
		}
			////////xil_printf("dut1_pattern6_CC1_1.6V_Calibration\r\n");
		    break;
	}

	//cc2 1.6v
	case 0x000c:
	{
		dut1.g_dac_data_buf[0] = 0x03;
		dut1.g_dac_data_buf[1] = 0x80;
		dut1.g_dac_data_buf[2] = 0x0c;
		dut1.g_dac_data_buf[3] = 0xf4;
		dut1.g_dac_data_buf[4] = 0x0b;
		dut1.g_dac_data_buf[5] = 0xbc;
		dut1.g_dac_data_buf[6] = 0x0c;

		dut1.g_ccdet_phy_calibration_addr_buf[0] = 0x11;
		dut1.g_ccdet_phy_calibration_addr_buf[1] = 0x01;
		dut1.g_ccdet_voltage_sel = 0x55;
		dut1.g_ccdet_calibration_data_buf[0]     = dut1.g_ccdet_calibration_data_buf[8];
		dut1.g_ccdet_calibration_data_buf[1]     = dut1.g_ccdet_calibration_data_buf[9];
		dut1.g_ccdet_phy_calibration_bit         = 0;
		dut1.g_ccdet_ana_result_bit              = 7;

		_ccdet_calibration_dut1();
		if(dut1.g_ccdet_process_bit != ccdet_process_done_pass)
		{

		}
		else
		{
			dut1.g_ccdet_calibration_data_buf[8] = dut1.g_ccdet_calibration_data_buf[2];
			dut1.g_ccdet_calibration_data_buf[9] = dut1.g_ccdet_calibration_data_buf[3];
			for(i=0;i<4;i++)
			{
				dut1.g_ccdet_calibration_data_buf[i] = 0;
			}
			dut1.g_ccdet_process_bit = ccdet_process_waiting;
			dut1.g_ccdet_retest_signbit = 0;
			dut1.g_ccdet_step++;
			dut1.g_pattern_step = 0;
			//////xil_printf("dut1.g_ccdet_step = %x\r\n",dut1.g_ccdet_step);
		}
			////////xil_printf("dut1_pattern6_CC2_1.6V_Calibration\r\n");
		    break;
	}

	//cc1 2.6v
	case 0x000d:
	{
		dut1.g_dac_data_buf[0] = 0x05;
		dut1.g_dac_data_buf[1] = 0x28;
		dut1.g_dac_data_buf[2] = 0x0a;
		dut1.g_dac_data_buf[3] = 0xa6;
		dut1.g_dac_data_buf[4] = 0x09;
		dut1.g_dac_data_buf[5] = 0xaa;
		dut1.g_dac_data_buf[6] = 0x0a;

		dut1.g_ccdet_phy_calibration_addr_buf[0] = 0x12;
		dut1.g_ccdet_phy_calibration_addr_buf[1] = 0x01;
		dut1.g_ccdet_voltage_sel = 0xff;
		dut1.g_ccdet_calibration_data_buf[0]     = dut1.g_ccdet_calibration_data_buf[10];
		dut1.g_ccdet_calibration_data_buf[1]     = dut1.g_ccdet_calibration_data_buf[11];
		dut1.g_ccdet_phy_calibration_bit         = 1;
		dut1.g_ccdet_ana_result_bit              = 4;

		_ccdet_calibration_dut1();
		if(dut1.g_ccdet_process_bit != ccdet_process_done_pass)
		{

		}
		else
		{
			dut1.g_ccdet_calibration_data_buf[10] = dut1.g_ccdet_calibration_data_buf[2];
			dut1.g_ccdet_calibration_data_buf[11] = dut1.g_ccdet_calibration_data_buf[3];
			for(i=0;i<4;i++)
			{
				dut1.g_ccdet_calibration_data_buf[i] = 0;
			}
			dut1.g_ccdet_process_bit = ccdet_process_waiting;
			dut1.g_ccdet_retest_signbit = 0;
			dut1.g_ccdet_step++;
			dut1.g_pattern_step = 0;
			//////xil_printf("dut1.g_ccdet_step = %x\r\n",dut1.g_ccdet_step);
		}
			////////xil_printf("dut1_pattern6_CC1_2.6V_Calibration\r\n");
		    break;
	}

	//cc2 2.6v
	case 0x000e:
	{
		dut1.g_dac_data_buf[0] = 0x07;
		dut1.g_dac_data_buf[1] = 0x28;
		dut1.g_dac_data_buf[2] = 0x0a;
		dut1.g_dac_data_buf[3] = 0xa6;
		dut1.g_dac_data_buf[4] = 0x09;
		dut1.g_dac_data_buf[5] = 0xaa;
		dut1.g_dac_data_buf[6] = 0x0a;

		dut1.g_ccdet_phy_calibration_addr_buf[0] = 0x12;
		dut1.g_ccdet_phy_calibration_addr_buf[1] = 0x01;
		dut1.g_ccdet_voltage_sel = 0xff;
		dut1.g_ccdet_calibration_data_buf[0]     = dut1.g_ccdet_calibration_data_buf[10];
		dut1.g_ccdet_calibration_data_buf[1]     = dut1.g_ccdet_calibration_data_buf[11];
		dut1.g_ccdet_phy_calibration_bit         = 0;
		dut1.g_ccdet_ana_result_bit              = 7;
		////////xil_printf("\r\ndut1_pattern6_CC2_2.6V_Calibration_before01\r\n");
		////////xil_printf("dut1.g_pattern_step = %x\r\n",dut1.g_pattern_step);
		_ccdet_calibration_dut1();

		if(dut1.g_ccdet_process_bit != ccdet_process_done_pass)
		{

		}
		else
		{
			dut1.g_ccdet_calibration_data_buf[10] = dut1.g_ccdet_calibration_data_buf[2];
			dut1.g_ccdet_calibration_data_buf[11] = dut1.g_ccdet_calibration_data_buf[3];

			dut1.g_dac_data_buf[0] = 0;
			dut1.g_dac_data_buf[1] = 0;
			dut1.g_dac_data_buf[2] = 0;
			dut1.g_dac_data_buf[3] = 0;
			dut1.g_dac_data_buf[4] = 0;
			dut1.g_dac_data_buf[5] = 0;
			dut1.g_dac_data_buf[6] = 0;
			for(i=0;i<4;i++)
			{
				dut1.g_ccdet_calibration_data_buf[i] = 0;
			}
			dut1.g_ccdet_process_bit = ccdet_process_waiting;
			dut1.g_ccdet_retest_signbit = 0;
			dut1.g_ccdet_step = 0;
			dut1.g_pattern_step = 0;
			dut1.g_dut_pattern_status_buf[2] = 0x0f;
			/*dut1.g_dut_pattern_status_buf[2] = 0;
   	   	    dut1.g_retest = 0;
   	   		for(i=0;i<20;i++)
   	   		{
   	   		dut1.g_ccdet_calibration_data_buf[i] = 0x88;
   	   		}
   	   		for(i=0;i<8;i++)
   	   		{
   	   		dut1.g_dut_pattern_status_buf[i] = 0;
   	   		}
 		   dut1.g_smbus_status_buf[0] = 0x00;
 		   dut1.g_smbus_status_buf[1] = 0xff;
 		   dut1.g_smbus_style = 0x00;
 		   dut1.g_smbus_timer = 0xff;
 		   dut1.g_relay_control_timer = 0xff;
 		   dut1.g_smbus_getstatus_timer = 0x02;
 		   XGpio_2_WriteBit(0,DUT0_FT2_PWR_CTRL_OFFSET,0x01);
 		   Gpio_0_WriteBit(0,FT2_PWR_DSCHG_OFFSET,!(0x00));
		   XGpio_2_WriteBit(0,DUT0_FT2_PWR_CTRL_OFFSET,0x01);*/
	   	       dut1.g_pattern_timer = 0xfff;
		       dut1.g_uartPatternEnable = 0x00;
		       //dut1.g_uartPatternNum = 0x0c;

		       dut1.g_uartPatternNum = (dut1.g_uartPatternNum++ & 0xff);
			    result_output_for_v50(XPAR_AXI_GPIO_dut1_1_BASEADDR,dut1.g_uartPatternNum);
			    ////xil_printf("dut1.g_uartPatternNum = %x\r\n\r\n",dut1.g_uartPatternNum);


      		XGpio_0_WriteBit(0,LED_D6_OFFSET,g_ledD6Tg);
      		g_ledD6Tg=~g_ledD6Tg;
      		////////xil_printf("dut1.g_ccdet_step = %x\r\n",dut1.g_ccdet_step);
      		////////xil_printf("dut1.g_pattern_step = %x\r\n",dut1.g_pattern_step);
      		////xil_printf("dut1.g_ccdet_calibration_data_buf[4] =%x\r\n", dut1.g_ccdet_calibration_data_buf[4]);
      		////xil_printf("dut1.g_ccdet_calibration_data_buf[5] =%x\r\n", dut1.g_ccdet_calibration_data_buf[5]);
      		////xil_printf("dut1.g_ccdet_calibration_data_buf[6] =%x\r\n", dut1.g_ccdet_calibration_data_buf[6]);
      		////xil_printf("dut1.g_ccdet_calibration_data_buf[7] =%x\r\n", dut1.g_ccdet_calibration_data_buf[7]);
      		////xil_printf("dut1.g_ccdet_calibration_data_buf[8] =%x\r\n", dut1.g_ccdet_calibration_data_buf[8]);
      		////xil_printf("dut1.g_ccdet_calibration_data_buf[9] =%x\r\n", dut1.g_ccdet_calibration_data_buf[9]);
      		////xil_printf("dut1.g_ccdet_calibration_data_buf[10] =%x\r\n", dut1.g_ccdet_calibration_data_buf[10]);
      		////xil_printf("dut1.g_ccdet_calibration_data_buf[11] =%x\r\n\r\n", dut1.g_ccdet_calibration_data_buf[11]);
      		//xil_printf("dut1_pattern6_CC2_2.6V_Calibration\r\n\r\n\r\n");
      		xil_printf("dut1_pattern6_CC_Calibration_pass!\r\n\r\n");


				XGpio_0_WriteBit(0,LED_D6_OFFSET,g_ledD6Tg);
				g_ledD6Tg=~g_ledD6Tg;
		}
		    break;
	}
	}
	if(dut1.g_result_fail == 0x01)
	{
		dut1.g_ft2_test_done = 0x00;
		pattern_safe_state_dut1();	//add safe state
		xil_printf("dut1_pattern6_fail! fail_step = %x\r\n\r\n",dut1.g_pattern_step);
	}

	//////xil_printf("dut1.g_pattern_step = %x\r\n",dut1.g_pattern_step);
	////xil_printf("dut1_pattern6_ccdet_step = %x\r\n\r\n",dut1.g_ccdet_step);
	   return 0;
}


















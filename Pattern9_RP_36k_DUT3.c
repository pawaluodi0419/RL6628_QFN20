#include "platform.h"
#include "Pattern9_RP_36k.h"

//dut3 Pattern 8£ºRP Calibration and test
u8 _by_Pattern9_RP_36k_dut3()
{
	u8 i;
	u16 voltage_ldo3v3;
	u16 rp36k_ref;
	u16 rp36k_min;
	u16 rp36k_max;

	u16 cc1_rp_voltage;
	u16 cc2_rp_voltage;
	u16 cc_rp_voltage;

	u8 cc1_rp36k_offset;
	u8 Buff_dut3_XGPIO_0[8];

	switch(dut3.g_pattern_step)
	{
	case 0x0000:
	{
		//gpio17: low bit, gpio20: high bit
		XGpio_WriteBit(XPAR_AXI_GPIO_dut3_1_BASEADDR,0,17, 1);
		XGpio_WriteBit(XPAR_AXI_GPIO_dut3_1_BASEADDR,0,18, 1);
		XGpio_WriteBit(XPAR_AXI_GPIO_dut3_1_BASEADDR,0,19, 0);
		XGpio_WriteBit(XPAR_AXI_GPIO_dut3_1_BASEADDR,0,20, 0);
		msdelay(5);

		//enable ADG1604
		XGpio_WriteBit(XPAR_AXI_GPIO_dut3_1_BASEADDR,0,13, 1);	//for CC1
		XGpio_WriteBit(XPAR_AXI_GPIO_dut3_1_BASEADDR,0,14, 1);	//for CC2
		msdelay(5);
		//select ADG1604 S1 channel
		XGpio_WriteBit(XPAR_AXI_GPIO_dut3_1_BASEADDR,0,9, 0);	//for CC1
		XGpio_WriteBit(XPAR_AXI_GPIO_dut3_1_BASEADDR,0,10, 0);

		XGpio_WriteBit(XPAR_AXI_GPIO_dut3_1_BASEADDR,0,24, 0);	//for CC2
		XGpio_WriteBit(XPAR_AXI_GPIO_dut3_1_BASEADDR,0,27, 0);
		msdelay(5);

		dut3.g_pattern_step = 0x100;
	}


	//smbus_write_mem 0xC1AE 1 0x00
	//disconnect CC from BMC PHY//
	case 0x0100:
	{
		   dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		   dut3.g_pattern_smbus_control_buf[2] = 0xAE;
		   dut3.g_pattern_smbus_control_buf[3] = 0xC1;
		   dut3.g_pattern_smbus_control_buf[4] = 0x01;
		   dut3.g_pattern_smbus_control_buf[5] = 0x00;

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
		   //////////////xil_printf("\r\n dut3_pattern8_0xC1AE_be_writen\r\n");
		   //////xil_printf("dut3_pattern8_step = %x\r\n", dut3.g_pattern_step);
		   break;
	}

	//smbus_write_mem 0xC108 1 0x88
	case 0x0101:
	{
		   dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		   dut3.g_pattern_smbus_control_buf[2] = 0x08;
		   dut3.g_pattern_smbus_control_buf[3] = 0xC1;
		   dut3.g_pattern_smbus_control_buf[4] = 0x01;
		   dut3.g_pattern_smbus_control_buf[5] = 0x88;

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
		   //////////////xil_printf("dut3_pattern8_0xC108_be_writen\r\n");
		   //////xil_printf("dut3_pattern8_step = %x\r\n", dut3.g_pattern_step);
		   break;
	}

	//smbus_write_mem 0x1022/1023 1 0x26
	case 0x0102:
	{
		   dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		   dut3.g_pattern_smbus_control_buf[2] = 0x22;
		   dut3.g_pattern_smbus_control_buf[3] = 0x10;
		   dut3.g_pattern_smbus_control_buf[4] = 0x02;
		   dut3.g_pattern_smbus_control_buf[5] = 0x26;
		   dut3.g_pattern_smbus_control_buf[6] = 0x26;

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
		   //////xil_printf("dut3_pattern8_step = %x\r\n", dut3.g_pattern_step);
		   //////////////xil_printf("dut3_pattern8_0x1022/1023_be_writen\r\n\r\n");
		   break;
	}

	//smbus_write_mem 0x1117 data:0x1b
	case 0x0103:
	{
		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		dut3.g_pattern_smbus_control_buf[2] = 0x17;
		dut3.g_pattern_smbus_control_buf[3] = 0x11;
		dut3.g_pattern_smbus_control_buf[4] = 0x01;
		dut3.g_pattern_smbus_control_buf[5] = 0x1b;
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
			dut3.g_pattern_step = 0x0200;
		}
		////xil_printf("dut3_rp_g_pattern_step = %x\r\n", dut3.g_pattern_step);
		//////xil_printf("dut3_pattern8_write_0x1131_with_0xc0\r\n");
		break;
	}
	   /****************************************
	    *RP_36K Calibration start!
	    *
	    *
	    *
	    *AD7994:
	    * D7-4:0001   CH0   CC1
	    * D7-4:0010   CH1   CC2
	    * D7-4:0100   CH2   3V3/2
	    * D7-4:1000   CH3   1V2
	    ******************************************/



	//Read LDO3V3 Voltage from AD7994 CH3
	case 0x0200:
	{
		msdelay(100);
		//_byAD7994ReadCvsData_dut3(0x05, 5);
		_byAD7994ReadCvsData_dut3(AD7994_ADC1_SLAVE_ADDR,0x05, 5);
    	voltage_ldo3v3 = dut3.g_as7994Cvsdata * 2;     //6628 ldo3v3 NO output pin
    	//voltage_ldo3v3 = 0x0a8e *2;       //6628 define Ldo3v3 data,just for Rp test
    	//xil_printf("dut3_voltage_ldo3v3 = %x\r\n", voltage_ldo3v3);
    	rp36k_ref  = (51/(360*1.000+51))*voltage_ldo3v3;
    	rp36k_min  = (51/(360*1.085+51))*voltage_ldo3v3;
    	rp36k_max  = (51/(360*0.915+51))*voltage_ldo3v3;

    	dut3.g_dut_pattern_status_buf[9] = (voltage_ldo3v3 & 0xFF);
    	dut3.g_dut_pattern_status_buf[10] = (voltage_ldo3v3 & 0xFF00)>>8;

    	dut3.g_dut_pattern_status_buf[11] = (rp36k_ref & 0xFF);
    	dut3.g_dut_pattern_status_buf[12] = (rp36k_ref & 0xFF00)>>8;
    	dut3.g_dut_pattern_status_buf[13] = (rp36k_min & 0xFF);
    	dut3.g_dut_pattern_status_buf[14] = (rp36k_min & 0xFF00)>>8;
    	dut3.g_dut_pattern_status_buf[15] = (rp36k_max & 0xFF);
    	dut3.g_dut_pattern_status_buf[16] = (rp36k_max & 0xFF00)>>8;

    	dut3.g_pattern_step++;
		//////////////xil_printf("dut3_pattern8_read_LDO3V3\r\n\r\n");
    	//////xil_printf("dut3_pattern8_step = %x\r\n", dut3.g_pattern_step);
		break;
	}

	//write to phy 0x10d default data
	case 0x0201:
	{
    	dut3.g_dut_pattern_status_buf[30] = 0x00;
    	dut3.g_dut_pattern_status_buf[31] = 0x00;

    	dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writephy;
    	dut3.g_pattern_smbus_control_buf[2] = 0x0d;
    	dut3.g_pattern_smbus_control_buf[3] = 0x01;
    	dut3.g_pattern_smbus_control_buf[4] = 0x01;
    	dut3.g_pattern_smbus_control_buf[5] = 0xef;
    	dut3.g_pattern_smbus_control_buf[6] = 0x01;

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
    	//////xil_printf("dut3_pattern8_step = %x\r\n", dut3.g_pattern_step);
    	//////////////xil_printf("dut3_pattern8_write_0x106_default_data\r\n");
		break;
	}

	//read cc1 voltage
	case 0x0202:
	{
		_byAD7994ReadCvsData_dut3(AD7994_ADC1_SLAVE_ADDR,0x01, 5);

    	dut3.g_dut_pattern_status_buf[26] = dut3.g_as7994Cvsdata & 0xFF;
    	dut3.g_dut_pattern_status_buf[27] = (dut3.g_as7994Cvsdata & 0xFF00)>>8;

    	cc1_rp_voltage = (u16)dut3.g_as7994Cvsdata;
    	cc_rp_voltage = (u16)dut3.g_as7994Cvsdata;
    	rp36k_ref = (u16)(dut3.g_dut_pattern_status_buf[12]<<8 | dut3.g_dut_pattern_status_buf[11]);
    	rp36k_min = (u16)(dut3.g_dut_pattern_status_buf[14]<<8 | dut3.g_dut_pattern_status_buf[13]);
    	rp36k_max = (u16)(dut3.g_dut_pattern_status_buf[16]<<8 | dut3.g_dut_pattern_status_buf[15]);

    	//////////////xil_printf("dut3.g_as7994Cvsdata = %d\r\n", dut3.g_as7994Cvsdata);
    	//////////////xil_printf("cc1_rp36k_voltage = %d\r\n", cc1_rp_voltage);
    	//////////////xil_printf("rp36k_ref = %d\r\n",rp36k_ref );
    	//////////////xil_printf("rp36k_min = %d\r\n",rp36k_min );
    	//////////////xil_printf("rp36k_max = %d\r\n",rp36k_max );
    	//////////////xil_printf("dut3.g_dut_pattern_status_buf[30] =%d\r\n", dut3.g_dut_pattern_status_buf[30]);
    	//////////////xil_printf("dut3.g_dut_pattern_status_buf[31] =%d\r\n", dut3.g_dut_pattern_status_buf[31]);

    	if((dut3.g_dut_pattern_status_buf[31]  >= 0) &&(dut3.g_dut_pattern_status_buf[31] <= 0x0F))
    	{
    		if(cc1_rp_voltage > rp36k_ref)
    		{
    			dut3.g_dut_pattern_status_buf[30] = 0x00;
    			dut3.g_dut_pattern_status_buf[31]++;
    		}
    		else if(cc1_rp_voltage < rp36k_ref)
    		{
    			dut3.g_dut_pattern_status_buf[30] = 0x01;
    			dut3.g_dut_pattern_status_buf[31]++;
    		}
       	}

    	dut3.g_pattern_step++;
    	//////xil_printf("dut3_pattern8_step = %x\r\n", dut3.g_pattern_step);
    	//////////////xil_printf("dut3_pattern8_read_cc1_rp36k_default_voltage\r\n");
    	break;
	}

	case 0x0203:
	{
		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writephy;
		dut3.g_pattern_smbus_control_buf[2] = 0x0d;
		dut3.g_pattern_smbus_control_buf[3] = 0x01;
		dut3.g_pattern_smbus_control_buf[4] = 0x01;
		if(dut3.g_dut_pattern_status_buf[30] == 0)
		{
			dut3.g_pattern_smbus_control_buf[5] = ((0x0f+dut3.g_dut_pattern_status_buf[31])<<5 |(0x0f+dut3.g_dut_pattern_status_buf[31])) & 0xff;
			dut3.g_pattern_smbus_control_buf[6] = (((0x0f+dut3.g_dut_pattern_status_buf[31])<<5 |0x0f)>>8)&0x03;
		}
		else if(dut3.g_dut_pattern_status_buf[30] == 1)
		{
			dut3.g_pattern_smbus_control_buf[5] = ((0x0f-dut3.g_dut_pattern_status_buf[31])<<5 |(0x0f-dut3.g_dut_pattern_status_buf[31])) & 0xff;
			dut3.g_pattern_smbus_control_buf[6] = (((0x0f-dut3.g_dut_pattern_status_buf[31])<<5 |0x0f)>>8)&0x03;
		   }
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
		   //////xil_printf("dut3_pattern8_step = %x\r\n", dut3.g_pattern_step);
		//////////////xil_printf("dut3_pattern8_write_cc1_rp36k_0x106\r\n");
		break;
	}

	//read cc1 voltage
	case 0x0204:
	{
		_byAD7994ReadCvsData_dut3(AD7994_ADC1_SLAVE_ADDR,0x01, 5);

    	dut3.g_dut_pattern_status_buf[26] = dut3.g_as7994Cvsdata & 0xFF;
    	dut3.g_dut_pattern_status_buf[27] = (dut3.g_as7994Cvsdata & 0xFF00)>>8;


    	cc1_rp_voltage = (u16)dut3.g_as7994Cvsdata;
    	cc_rp_voltage = (u16)dut3.g_as7994Cvsdata;
    	rp36k_ref = (u16)(dut3.g_dut_pattern_status_buf[12]<<8 | dut3.g_dut_pattern_status_buf[11]);
    	rp36k_min = (u16)(dut3.g_dut_pattern_status_buf[14]<<8 | dut3.g_dut_pattern_status_buf[13]);
    	rp36k_max = (u16)(dut3.g_dut_pattern_status_buf[16]<<8 | dut3.g_dut_pattern_status_buf[15]);

    	////xil_printf("dut3.g_as7994Cvsdata = %d\r\n", dut3.g_as7994Cvsdata);
		//xil_printf("cc1_rp36k_voltage = %d\r\n", cc1_rp_voltage);
	    //xil_printf("rp36k_ref = %d\r\n",rp36k_ref );
	    //xil_printf("rp36k_min = %d\r\n",rp36k_min );
	    //xil_printf("rp36k_max = %d\r\n",rp36k_max );

	    //rp36k calibration
    	if((dut3.g_dut_pattern_status_buf[31] >= 0) && (dut3.g_dut_pattern_status_buf[31]<= 0x0F))
    	{
    		if(cc1_rp_voltage > rp36k_ref)
    		{

    			if(dut3.g_dut_pattern_status_buf[30] == 0x00){

    			dut3.g_dut_pattern_status_buf[31]++;
				dut3.g_pattern_smbus_control_buf[5] = ((0x0f+dut3.g_dut_pattern_status_buf[31])<<5 |(0x0f+dut3.g_dut_pattern_status_buf[31])) & 0xff;
				dut3.g_pattern_smbus_control_buf[6] = (((0x0f+dut3.g_dut_pattern_status_buf[31])<<5 |0x0f)>>8)&0x03;

				dut3.g_pattern_step++;
    			}
    			else if(dut3.g_dut_pattern_status_buf[30] == 0x01)
    			{
    				if((cc1_rp_voltage > rp36k_min) && (cc1_rp_voltage < rp36k_max)){
    					dut3.g_pattern_step++;
    					dut3.g_pattern_step++;
    			}
			   		   else
			   		   {
			   			   dut3.g_result_fail = 0x01;
			   			dut3.g_result_fail_tmrcount = 0xffff;
			   		   }
			    break;
    			}
    			}

    		else if(cc1_rp_voltage < rp36k_ref)
    		{

    			if(dut3.g_dut_pattern_status_buf[30] == 0x01)
    			{

    			dut3.g_dut_pattern_status_buf[31]++;
				dut3.g_pattern_smbus_control_buf[5] = ((0x0f-dut3.g_dut_pattern_status_buf[31])<<5 |(0x0f-dut3.g_dut_pattern_status_buf[31])) & 0xff;
				dut3.g_pattern_smbus_control_buf[6] = (((0x0f-dut3.g_dut_pattern_status_buf[31])<<5 |0x0f)>>8)&0x03;
				dut3.g_pattern_step++;
    			}
    			else if(dut3.g_dut_pattern_status_buf[30] == 0x00){
    				if((cc1_rp_voltage > rp36k_min) && (cc1_rp_voltage < rp36k_max)){
    					dut3.g_pattern_step++;
    					dut3.g_pattern_step++;
    			}
			   		   else{
			   			   dut3.g_result_fail = 0x01;
			   			dut3.g_result_fail_tmrcount = 0xffff;
			   		   }

    				//////////////xil_printf("dut3_pattern8_read_cc1_rp36k_calibration_voltage\r\n");
			    break;
    			}

    			}
    		}

    		//calibration has done,output register data

    	else{
    		dut3.g_result_fail = 0x01;
    		dut3.g_result_fail_tmrcount = 0xffff;
    		break;
    	}



    	////////////xil_printf("dut3_rp_g_pattern_step = %x\r\n", dut3.g_pattern_step);
	break;
	}

	case 0x0205:
	{
		   dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writephy;
		   dut3.g_pattern_smbus_control_buf[2] = 0x0d;
		   dut3.g_pattern_smbus_control_buf[3] = 0x01;
		   dut3.g_pattern_smbus_control_buf[4] = 0x01;
		   if(dut3.g_dut_pattern_status_buf[30] == 0){

				dut3.g_pattern_smbus_control_buf[5] = ((0x0f+dut3.g_dut_pattern_status_buf[31])<<5 |(0x0f+dut3.g_dut_pattern_status_buf[31])) & 0xff;
				dut3.g_pattern_smbus_control_buf[6] = (((0x0f+dut3.g_dut_pattern_status_buf[31])<<5 |0x0f)>>8)&0x03;
		   }
		   else if(dut3.g_dut_pattern_status_buf[30] == 1){
				dut3.g_pattern_smbus_control_buf[5] = ((0x0f-dut3.g_dut_pattern_status_buf[31])<<5 |(0x0f-dut3.g_dut_pattern_status_buf[31])) & 0xff;
				dut3.g_pattern_smbus_control_buf[6] = (((0x0f-dut3.g_dut_pattern_status_buf[31])<<5 |0x0f)>>8)&0x03;
		   }

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

				       dut3.g_pattern_step--;
		   }
		   ////////////xil_printf("dut3_rp_g_pattern_step = %x\r\n", dut3.g_pattern_step);
		   //////////////xil_printf("dut3_pattern8_write_cc1_rp36k_calibration_0x106\r\n");
	break;
	}


	//read 0x10d data and save to efuse buf
	case 0x0206:
	{
		   dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readphy;
		   dut3.g_pattern_smbus_control_buf[2] = 0x0d;
		   dut3.g_pattern_smbus_control_buf[3] = 0x01;
		   dut3.g_pattern_smbus_control_buf[4] = 0x01;
		   //dut3.g_pattern_smbus_control_buf[5] = 0x00;
		   //dut3.g_pattern_smbus_control_buf[6] = 0x00;

		   smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
		   if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		   {
			   break;
		   }
		   else
		   {
		       cc1_rp36k_offset = (((dut3.g_pattern_smbus_control_buf[11] << 8) | (dut3.g_pattern_smbus_control_buf[10]))>>5)&0x1f;
		       if(cc1_rp36k_offset >= 0x0f)
		       {
		    	   dut3.g_dut_pattern_status_buf[30] = 0;
		    	   dut3.g_dut_pattern_status_buf[31] = cc1_rp36k_offset - 0x0f;
		    	   dut3.g_efuse_data_buf[7] = 1;
		    	   dut3.g_efuse_data_buf[8] = dut3.g_dut_pattern_status_buf[30];
		    	   dut3.g_efuse_data_buf[9] = dut3.g_dut_pattern_status_buf[31];
		       }
		       else if(cc1_rp36k_offset < 0x0f)
		       {
		    	   dut3.g_dut_pattern_status_buf[30] = 1;
		    	   dut3.g_dut_pattern_status_buf[31] = 0x0f- cc1_rp36k_offset;
		    	   dut3.g_efuse_data_buf[7] = 1;
		    	   dut3.g_efuse_data_buf[8] = dut3.g_dut_pattern_status_buf[30];
		    	   dut3.g_efuse_data_buf[9] = dut3.g_dut_pattern_status_buf[31];
		       }

				       dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;

				       dut3.g_pattern_step = 0x0300;
					//xil_printf("dut3_cc1_rp36k_offset =%x\r\n", cc1_rp36k_offset);
		   }
		   ////////////xil_printf("dut3_rp_g_pattern_step = %x\r\n", dut3.g_pattern_step);
	       //////////////xil_printf("cc1_rp36k_offset =%x\r\n", cc1_rp36k_offset);
		   //////////////xil_printf("dut3.g_pattern_smbus_control_buf[10] =%x\r\n", dut3.g_pattern_smbus_control_buf[10]);
		   //////////////xil_printf("dut3.g_pattern_smbus_control_buf[11] =%x\r\n", dut3.g_pattern_smbus_control_buf[11]);
		   //////////////xil_printf("dut3.g_dut_pattern_status_buf[30] =%x\r\n", dut3.g_dut_pattern_status_buf[30]);
		   //////////////xil_printf("dut3.g_dut_pattern_status_buf[31] =%x\r\n", dut3.g_dut_pattern_status_buf[31]);
		   //////////////xil_printf("cc1_rp36k_voltage =%d\r\n", cc1_rp_voltage);
		   //////////xil_printf("dut3.g_efuse_data_buf[8] =%x\r\n", dut3.g_efuse_data_buf[8]);
		   //////////xil_printf("dut3.g_efuse_data_buf[9] =%x\r\n", dut3.g_efuse_data_buf[9]);
		   //////////////xil_printf("dut3_pattern8_read_cc1_rp36k_calibration_0x106\r\n");
	break;
	}

	//CC2 RP36K test
	case 0x0300:
	{
		   dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writephy;
		   dut3.g_pattern_smbus_control_buf[2] = 0x0d;
		   dut3.g_pattern_smbus_control_buf[3] = 0x01;
		   dut3.g_pattern_smbus_control_buf[4] = 0x01;
		   if(dut3.g_dut_pattern_status_buf[30] == 0){

				dut3.g_pattern_smbus_control_buf[5] = ((0x0f+dut3.g_dut_pattern_status_buf[31])<<5 |(0x0f+dut3.g_dut_pattern_status_buf[31])) & 0xff;
				dut3.g_pattern_smbus_control_buf[6] = (((0x0f+dut3.g_dut_pattern_status_buf[31])<<5 |0x0f)>>8)&0x03;

		   }
		   else if(dut3.g_dut_pattern_status_buf[30] == 1){
				dut3.g_pattern_smbus_control_buf[5] = ((0x0f-dut3.g_dut_pattern_status_buf[31])<<5 |(0x0f-dut3.g_dut_pattern_status_buf[31])) & 0xff;
				dut3.g_pattern_smbus_control_buf[6] = (((0x0f-dut3.g_dut_pattern_status_buf[31])<<5 |0x0f)>>8)&0x03;
		   }
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
		   ////////////xil_printf("dut3_rp_g_pattern_step = %x\r\n", dut3.g_pattern_step);
		   //////////////xil_printf("dut3_pattern8_write_cc2_rp36k_calibration_0x106\r\n");
	break;
	}

	//read cc2 voltage
	case 0x0301:
	{
		_byAD7994ReadCvsData_dut3(AD7994_ADC1_SLAVE_ADDR,0x03, 5);

    	dut3.g_dut_pattern_status_buf[26] = dut3.g_as7994Cvsdata & 0xFF;
    	dut3.g_dut_pattern_status_buf[27] = (dut3.g_as7994Cvsdata & 0xFF00)>>8;

    	cc2_rp_voltage = (u16)dut3.g_as7994Cvsdata;
    	cc_rp_voltage = (u16)dut3.g_as7994Cvsdata;
    	rp36k_ref = (u16)(dut3.g_dut_pattern_status_buf[12]<<8 | dut3.g_dut_pattern_status_buf[11]);
    	rp36k_min = (u16)(dut3.g_dut_pattern_status_buf[14]<<8 | dut3.g_dut_pattern_status_buf[13]);
    	rp36k_max = (u16)(dut3.g_dut_pattern_status_buf[16]<<8 | dut3.g_dut_pattern_status_buf[15]);

    	   //////////////xil_printf("dut3.g_as7994Cvsdata = %d\r\n", dut3.g_as7994Cvsdata);
		   //////////////xil_printf("cc2_rp36k_voltage = %d\r\n", cc2_rp_voltage);
	       //////////////xil_printf("rp36k_ref = %d\r\n",rp36k_ref );
	       //////////////xil_printf("rp36k_min = %d\r\n",rp36k_min );
	       //////////////xil_printf("rp36k_max = %d\r\n",rp36k_max );

			if((cc2_rp_voltage > rp36k_min) && (cc2_rp_voltage < rp36k_max))
			{
				dut3.g_pattern_timer = 0xfff;
				dut3.g_pattern_step = 0;
				dut3.g_dut_pattern_status_buf[2]++;
				dut3.g_uartPatternEnable = 0x00;
				dut3.g_uartPatternNum = (dut3.g_uartPatternNum++ & 0xff);
				//dut3.g_uartPatternNum = (dut3.g_uartPatternNum++ & 0xff);
				result_output_for_v50(XPAR_AXI_GPIO_dut3_1_BASEADDR,dut3.g_uartPatternNum);
				//xil_printf("dut3.g_uartPatternNum = %x\r\n\r\n",dut3.g_uartPatternNum);
				xil_printf("dut3_pattern9_rp_36k_test_pass!    rp36k_signbit= 0x%x  rp36k_offset=0x%x\r\n\r\n",dut3.g_efuse_data_buf[8],dut3.g_efuse_data_buf[9]);
				//xil_printf("dut3.g_result_fail = %x\r\n",dut3.g_result_fail);
			}

			else
			{
				dut3.g_result_fail = 0x01;
	   			dut3.g_result_fail_tmrcount = 0xffff;
			}
			////////////xil_printf("dut3_rp_g_pattern_step = %x\r\n", dut3.g_pattern_step);
			//////////////xil_printf("dut3_pattern8_read_cc2_rp36k_voltage\r\n");
			break;
	}
	}

	if(dut3.g_result_fail == 0x01)
	{
		dut3.g_ft2_test_done = 0x00;
		pattern_safe_state_dut3();	//add safe state
		xil_printf("dut3_pattern9_fail! fail_step = %x\r\n",dut3.g_pattern_step);
	    xil_printf("rp36k_min = %d rp36k_max = %d\r\n",rp36k_min, rp36k_max);
	    xil_printf("dut3_cc_rp_voltage = %d\r\n\r\n",cc_rp_voltage);
	}
	//xil_printf("dut3_pattern8_rp_step = 0x%x\r\n", dut3.g_pattern_step);
	return 0;
}



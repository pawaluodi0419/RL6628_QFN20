#include "platform.h"
#include "PatternA_RP_4p7k.h"

//dut2 Pattern 9£ºRP Calibration and test
u8 _by_Pattern10_RP_4p7k_dut2()
{
	u8 i;
	u16 voltage_ldo3v3;
	u16 rp4p7k_ref;
	u16 rp4p7k_min;
	u16 rp4p7k_max;

	u16 cc1_rp_voltage;
	u16 cc2_rp_voltage;
	u16 cc_rp_voltage;

	u8 cc1_rp4p7k_offset;
	u8 Buff_dut2_XGPIO_0[8];

	switch(dut2.g_pattern_step)
	{
	//smbus_write_mem 0x1022/1023 1 0x16
	case 0x0000:
	{
		   dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		   dut2.g_pattern_smbus_control_buf[2] = 0x22;
		   dut2.g_pattern_smbus_control_buf[3] = 0x10;
		   dut2.g_pattern_smbus_control_buf[4] = 0x02;
		   dut2.g_pattern_smbus_control_buf[5] = 0x16;
		   dut2.g_pattern_smbus_control_buf[6] = 0x16;

		   smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);

		   if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		   {
			   break;
		   }
		   else
		   {
				   for(i=1; i<31; i++)
				   {
					   dut2.g_pattern_smbus_control_buf[i] = CLEAR_;
				   }
				       dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
				       dut2.g_pattern_step = 0x200;
		   }
		   //////xil_printf("dut2_pattern9_step = %x\r\n", dut2.g_pattern_step);
		   //////////////xil_printf("dut2_pattern9_0x1022/1023_be_writen\r\n\r\n");
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
		//_byAD7994ReadCvsData_dut2(0x05, 5);
		_byAD7994ReadCvsData_dut2(AD7994_ADC1_SLAVE_ADDR,0x05, 5);
    	voltage_ldo3v3 = dut2.g_as7994Cvsdata * 2;     //6628 ldo3v3 NO output pin
    	//voltage_ldo3v3 = 0x0a8e *2;       //6628 define Ldo3v3 data,just for Rp test
    	//xil_printf("dut2_voltage_ldo3v3 = %x\r\n", voltage_ldo3v3);
    	rp4p7k_ref  = (51/(47*1.000+51))*voltage_ldo3v3;
    	rp4p7k_min  = (51/(47*1.05+51))*voltage_ldo3v3;
    	rp4p7k_max  = (51/(47*0.95+51))*voltage_ldo3v3;

    	dut2.g_dut_pattern_status_buf[9] = (voltage_ldo3v3 & 0xFF);
    	dut2.g_dut_pattern_status_buf[10] = (voltage_ldo3v3 & 0xFF00)>>8;

    	dut2.g_dut_pattern_status_buf[11] = (rp4p7k_ref & 0xFF);
    	dut2.g_dut_pattern_status_buf[12] = (rp4p7k_ref & 0xFF00)>>8;
    	dut2.g_dut_pattern_status_buf[13] = (rp4p7k_min & 0xFF);
    	dut2.g_dut_pattern_status_buf[14] = (rp4p7k_min & 0xFF00)>>8;
    	dut2.g_dut_pattern_status_buf[15] = (rp4p7k_max & 0xFF);
    	dut2.g_dut_pattern_status_buf[16] = (rp4p7k_max & 0xFF00)>>8;

    	dut2.g_pattern_step++;
		//////////////xil_printf("dut2_pattern9_read_LDO3V3\r\n\r\n");
    	//////xil_printf("dut2_pattern9_step = %x\r\n", dut2.g_pattern_step);
		break;
	}

	//write to phy 0x10e default data
	case 0x0201:
	{
    	dut2.g_dut_pattern_status_buf[30] = 0x00;
    	dut2.g_dut_pattern_status_buf[31] = 0x00;

    	dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writephy;
    	dut2.g_pattern_smbus_control_buf[2] = 0x0e;
    	dut2.g_pattern_smbus_control_buf[3] = 0x01;
    	dut2.g_pattern_smbus_control_buf[4] = 0x01;
    	dut2.g_pattern_smbus_control_buf[5] = 0xef;
    	dut2.g_pattern_smbus_control_buf[6] = 0x01;

    	smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);

    	if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
    	{
    		break;
    	}
    	else
    	{
    		for(i=1; i<31; i++)
    		{
    			dut2.g_pattern_smbus_control_buf[i] = CLEAR_;
    		}

    		dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
    		dut2.g_pattern_step++;
    	}
    	//////xil_printf("dut2_pattern9_step = %x\r\n", dut2.g_pattern_step);
    	//////////////xil_printf("dut2_pattern9_write_0x106_default_data\r\n");
		break;
	}

	//read cc1 voltage
	case 0x0202:
	{
		_byAD7994ReadCvsData_dut2(AD7994_ADC1_SLAVE_ADDR,0x01, 5);

    	dut2.g_dut_pattern_status_buf[26] = dut2.g_as7994Cvsdata & 0xFF;
    	dut2.g_dut_pattern_status_buf[27] = (dut2.g_as7994Cvsdata & 0xFF00)>>8;

    	cc1_rp_voltage = (u16)dut2.g_as7994Cvsdata;
    	cc_rp_voltage = (u16)dut2.g_as7994Cvsdata;
    	rp4p7k_ref = (u16)(dut2.g_dut_pattern_status_buf[12]<<8 | dut2.g_dut_pattern_status_buf[11]);
    	rp4p7k_min = (u16)(dut2.g_dut_pattern_status_buf[14]<<8 | dut2.g_dut_pattern_status_buf[13]);
    	rp4p7k_max = (u16)(dut2.g_dut_pattern_status_buf[16]<<8 | dut2.g_dut_pattern_status_buf[15]);

    	//////////////xil_printf("dut2.g_as7994Cvsdata = %d\r\n", dut2.g_as7994Cvsdata);
    	//////////////xil_printf("cc1_rp4p7k_voltage = %d\r\n", cc1_rp_voltage);
    	//////////////xil_printf("rp4p7k_ref = %d\r\n",rp4p7k_ref );
    	//////////////xil_printf("rp4p7k_min = %d\r\n",rp4p7k_min );
    	//////////////xil_printf("rp4p7k_max = %d\r\n",rp4p7k_max );
    	//////////////xil_printf("dut2.g_dut_pattern_status_buf[30] =%d\r\n", dut2.g_dut_pattern_status_buf[30]);
    	//////////////xil_printf("dut2.g_dut_pattern_status_buf[31] =%d\r\n", dut2.g_dut_pattern_status_buf[31]);

    	if((dut2.g_dut_pattern_status_buf[31]  >= 0) &&(dut2.g_dut_pattern_status_buf[31] <= 0x0F))
    	{
    		if(cc1_rp_voltage > rp4p7k_ref)
    		{
    			dut2.g_dut_pattern_status_buf[30] = 0x00;
    			dut2.g_dut_pattern_status_buf[31]++;
    		}
    		else if(cc1_rp_voltage < rp4p7k_ref)
    		{
    			dut2.g_dut_pattern_status_buf[30] = 0x01;
    			dut2.g_dut_pattern_status_buf[31]++;
    		}
       	}

    	dut2.g_pattern_step++;
    	//////xil_printf("dut2_pattern9_step = %x\r\n", dut2.g_pattern_step);
    	//////////////xil_printf("dut2_pattern9_read_cc1_rp4p7k_default_voltage\r\n");
    	break;
	}

	case 0x0203:
	{
		dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writephy;
		dut2.g_pattern_smbus_control_buf[2] = 0x0e;
		dut2.g_pattern_smbus_control_buf[3] = 0x01;
		dut2.g_pattern_smbus_control_buf[4] = 0x01;
		if(dut2.g_dut_pattern_status_buf[30] == 0)
		{
			dut2.g_pattern_smbus_control_buf[5] = ((0x0f+dut2.g_dut_pattern_status_buf[31])<<5 |(0x0f+dut2.g_dut_pattern_status_buf[31])) & 0xff;
			dut2.g_pattern_smbus_control_buf[6] = (((0x0f+dut2.g_dut_pattern_status_buf[31])<<5 |0x0f)>>8)&0x03;
		}
		else if(dut2.g_dut_pattern_status_buf[30] == 1)
		{
			dut2.g_pattern_smbus_control_buf[5] = ((0x0f-dut2.g_dut_pattern_status_buf[31])<<5 |(0x0f-dut2.g_dut_pattern_status_buf[31])) & 0xff;
			dut2.g_pattern_smbus_control_buf[6] = (((0x0f-dut2.g_dut_pattern_status_buf[31])<<5 |0x0f)>>8)&0x03;
		   }
		smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);

		if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			for(i=1; i<31; i++)
			{
				dut2.g_pattern_smbus_control_buf[i] = CLEAR_;
			}
			dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut2.g_pattern_step++;
		}
		   //////xil_printf("dut2_pattern9_step = %x\r\n", dut2.g_pattern_step);
		//////////////xil_printf("dut2_pattern9_write_cc1_rp4p7k_0x106\r\n");
		break;
	}

	//read cc1 voltage
	case 0x0204:
	{
		_byAD7994ReadCvsData_dut2(AD7994_ADC1_SLAVE_ADDR,0x01, 5);

    	dut2.g_dut_pattern_status_buf[26] = dut2.g_as7994Cvsdata & 0xFF;
    	dut2.g_dut_pattern_status_buf[27] = (dut2.g_as7994Cvsdata & 0xFF00)>>8;


    	cc1_rp_voltage = (u16)dut2.g_as7994Cvsdata;
    	cc_rp_voltage = (u16)dut2.g_as7994Cvsdata;
    	rp4p7k_ref = (u16)(dut2.g_dut_pattern_status_buf[12]<<8 | dut2.g_dut_pattern_status_buf[11]);
    	rp4p7k_min = (u16)(dut2.g_dut_pattern_status_buf[14]<<8 | dut2.g_dut_pattern_status_buf[13]);
    	rp4p7k_max = (u16)(dut2.g_dut_pattern_status_buf[16]<<8 | dut2.g_dut_pattern_status_buf[15]);

    	////xil_printf("dut2.g_as7994Cvsdata = %d\r\n", dut2.g_as7994Cvsdata);
		//xil_printf("cc1_rp4p7k_voltage = %d\r\n", cc1_rp_voltage);
	    //xil_printf("rp4p7k_ref = %d\r\n",rp4p7k_ref );
	    //xil_printf("rp4p7k_min = %d\r\n",rp4p7k_min );
	    //xil_printf("rp4p7k_max = %d\r\n",rp4p7k_max );

	    //rp4p7k calibration
    	if((dut2.g_dut_pattern_status_buf[31] >= 0) && (dut2.g_dut_pattern_status_buf[31]<= 0x0F))
    	{
    		if(cc1_rp_voltage > rp4p7k_ref)
    		{

    			if(dut2.g_dut_pattern_status_buf[30] == 0x00){

    			dut2.g_dut_pattern_status_buf[31]++;
				dut2.g_pattern_smbus_control_buf[5] = ((0x0f+dut2.g_dut_pattern_status_buf[31])<<5 |(0x0f+dut2.g_dut_pattern_status_buf[31])) & 0xff;
				dut2.g_pattern_smbus_control_buf[6] = (((0x0f+dut2.g_dut_pattern_status_buf[31])<<5 |0x0f)>>8)&0x03;

				dut2.g_pattern_step++;
    			}
    			else if(dut2.g_dut_pattern_status_buf[30] == 0x01)
    			{
    				if((cc1_rp_voltage > rp4p7k_min) && (cc1_rp_voltage < rp4p7k_max)){
    					dut2.g_pattern_step++;
    					dut2.g_pattern_step++;
    			}
			   		   else
			   		   {
			   			   dut2.g_result_fail = 0x01;
			   			dut2.g_result_fail_tmrcount = 0xffff;
			   		   }
			    break;
    			}
    			}

    		else if(cc1_rp_voltage < rp4p7k_ref)
    		{

    			if(dut2.g_dut_pattern_status_buf[30] == 0x01)
    			{

    			dut2.g_dut_pattern_status_buf[31]++;
				dut2.g_pattern_smbus_control_buf[5] = ((0x0f-dut2.g_dut_pattern_status_buf[31])<<5 |(0x0f-dut2.g_dut_pattern_status_buf[31])) & 0xff;
				dut2.g_pattern_smbus_control_buf[6] = (((0x0f-dut2.g_dut_pattern_status_buf[31])<<5 |0x0f)>>8)&0x03;
				dut2.g_pattern_step++;
    			}
    			else if(dut2.g_dut_pattern_status_buf[30] == 0x00){
    				if((cc1_rp_voltage > rp4p7k_min) && (cc1_rp_voltage < rp4p7k_max)){
    					dut2.g_pattern_step++;
    					dut2.g_pattern_step++;
    			}
			   		   else{
			   			   dut2.g_result_fail = 0x01;
			   			dut2.g_result_fail_tmrcount = 0xffff;
			   		   }

    				//////////////xil_printf("dut2_pattern9_read_cc1_rp4p7k_calibration_voltage\r\n");
			    break;
    			}

    			}
    		}

    		//calibration has done,output register data

    	else{
    		dut2.g_result_fail = 0x01;
    		dut2.g_result_fail_tmrcount = 0xffff;
    		break;
    	}



    	////////////xil_printf("dut2_rp_g_pattern_step = %x\r\n", dut2.g_pattern_step);
	break;
	}

	case 0x0205:
	{
		   dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writephy;
		   dut2.g_pattern_smbus_control_buf[2] = 0x0e;
		   dut2.g_pattern_smbus_control_buf[3] = 0x01;
		   dut2.g_pattern_smbus_control_buf[4] = 0x01;
		   if(dut2.g_dut_pattern_status_buf[30] == 0){

				dut2.g_pattern_smbus_control_buf[5] = ((0x0f+dut2.g_dut_pattern_status_buf[31])<<5 |(0x0f+dut2.g_dut_pattern_status_buf[31])) & 0xff;
				dut2.g_pattern_smbus_control_buf[6] = (((0x0f+dut2.g_dut_pattern_status_buf[31])<<5 |0x0f)>>8)&0x03;
		   }
		   else if(dut2.g_dut_pattern_status_buf[30] == 1){
				dut2.g_pattern_smbus_control_buf[5] = ((0x0f-dut2.g_dut_pattern_status_buf[31])<<5 |(0x0f-dut2.g_dut_pattern_status_buf[31])) & 0xff;
				dut2.g_pattern_smbus_control_buf[6] = (((0x0f-dut2.g_dut_pattern_status_buf[31])<<5 |0x0f)>>8)&0x03;
		   }

		   smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);

		   if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		   {
			   break;
		   }
		   else
		   {
				   for(i=1; i<31; i++)
				   {
					   dut2.g_pattern_smbus_control_buf[i] = CLEAR_;
				   }
				       dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;

				       dut2.g_pattern_step--;
		   }
		   ////////////xil_printf("dut2_rp_g_pattern_step = %x\r\n", dut2.g_pattern_step);
		   //////////////xil_printf("dut2_pattern9_write_cc1_rp4p7k_calibration_0x106\r\n");
	break;
	}


	//read 0x10e data and save to efuse buf
	case 0x0206:
	{
		   dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readphy;
		   dut2.g_pattern_smbus_control_buf[2] = 0x0e;
		   dut2.g_pattern_smbus_control_buf[3] = 0x01;
		   dut2.g_pattern_smbus_control_buf[4] = 0x01;
		   //dut2.g_pattern_smbus_control_buf[5] = 0x00;
		   //dut2.g_pattern_smbus_control_buf[6] = 0x00;

		   smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);
		   if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		   {
			   break;
		   }
		   else
		   {
		       cc1_rp4p7k_offset = (((dut2.g_pattern_smbus_control_buf[11] << 8) | (dut2.g_pattern_smbus_control_buf[10]))>>5)&0x1f;
		       if(cc1_rp4p7k_offset >= 0x0f)
		       {
		    	   dut2.g_dut_pattern_status_buf[30] = 0;
		    	   dut2.g_dut_pattern_status_buf[31] = cc1_rp4p7k_offset - 0x0f;
		    	   dut2.g_efuse_data_buf[10] = 1;
		    	   dut2.g_efuse_data_buf[11] = dut2.g_dut_pattern_status_buf[30];
		    	   dut2.g_efuse_data_buf[12] = dut2.g_dut_pattern_status_buf[31];
		       }
		       else if(cc1_rp4p7k_offset < 0x0f)
		       {
		    	   dut2.g_dut_pattern_status_buf[30] = 1;
		    	   dut2.g_dut_pattern_status_buf[31] = 0x0f- cc1_rp4p7k_offset;
		    	   dut2.g_efuse_data_buf[10] = 1;
		    	   dut2.g_efuse_data_buf[11] = dut2.g_dut_pattern_status_buf[30];
		    	   dut2.g_efuse_data_buf[12] = dut2.g_dut_pattern_status_buf[31];
		       }

				       dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;

				       dut2.g_pattern_step = 0x0300;
					//xil_printf("dut2_cc1_rp4p7k_offset =%x\r\n", cc1_rp4p7k_offset);
		   }
		   ////////////xil_printf("dut2_rp_g_pattern_step = %x\r\n", dut2.g_pattern_step);
	       //////////////xil_printf("cc1_rp4p7k_offset =%x\r\n", cc1_rp4p7k_offset);
		   //////////////xil_printf("dut2.g_pattern_smbus_control_buf[10] =%x\r\n", dut2.g_pattern_smbus_control_buf[10]);
		   //////////////xil_printf("dut2.g_pattern_smbus_control_buf[11] =%x\r\n", dut2.g_pattern_smbus_control_buf[11]);
		   //////////////xil_printf("dut2.g_dut_pattern_status_buf[30] =%x\r\n", dut2.g_dut_pattern_status_buf[30]);
		   //////////////xil_printf("dut2.g_dut_pattern_status_buf[31] =%x\r\n", dut2.g_dut_pattern_status_buf[31]);
		   //////////////xil_printf("cc1_rp4p7k_voltage =%d\r\n", cc1_rp_voltage);
		   //////////xil_printf("dut2.g_efuse_data_buf[10] =%x\r\n", dut2.g_efuse_data_buf[10]);
		   //////////xil_printf("dut2.g_efuse_data_buf[11] =%x\r\n", dut2.g_efuse_data_buf[11]);
		   //////////////xil_printf("dut2_pattern9_read_cc1_rp4p7k_calibration_0x106\r\n");
	break;
	}

	//CC2 RP4p7K test
	case 0x0300:
	{
		   dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writephy;
		   dut2.g_pattern_smbus_control_buf[2] = 0x0e;
		   dut2.g_pattern_smbus_control_buf[3] = 0x01;
		   dut2.g_pattern_smbus_control_buf[4] = 0x01;
		   if(dut2.g_dut_pattern_status_buf[30] == 0){

				dut2.g_pattern_smbus_control_buf[5] = ((0x0f+dut2.g_dut_pattern_status_buf[31])<<5 |(0x0f+dut2.g_dut_pattern_status_buf[31])) & 0xff;
				dut2.g_pattern_smbus_control_buf[6] = (((0x0f+dut2.g_dut_pattern_status_buf[31])<<5 |0x0f)>>8)&0x03;

		   }
		   else if(dut2.g_dut_pattern_status_buf[30] == 1){
				dut2.g_pattern_smbus_control_buf[5] = ((0x0f-dut2.g_dut_pattern_status_buf[31])<<5 |(0x0f-dut2.g_dut_pattern_status_buf[31])) & 0xff;
				dut2.g_pattern_smbus_control_buf[6] = (((0x0f-dut2.g_dut_pattern_status_buf[31])<<5 |0x0f)>>8)&0x03;
		   }
		  smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);
		   if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		   {
			   break;
		   }
		   else
		   {
				   for(i=1; i<31; i++)
				   {
					   dut2.g_pattern_smbus_control_buf[i] = CLEAR_;
				   }
				       dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
				       dut2.g_pattern_step++;
		   }
		   ////////////xil_printf("dut2_rp_g_pattern_step = %x\r\n", dut2.g_pattern_step);
		   //////////////xil_printf("dut2_pattern9_write_cc2_rp4p7k_calibration_0x106\r\n");
	break;
	}

	//read cc2 voltage
	case 0x0301:
	{
		_byAD7994ReadCvsData_dut2(AD7994_ADC1_SLAVE_ADDR,0x03, 5);

    	dut2.g_dut_pattern_status_buf[26] = dut2.g_as7994Cvsdata & 0xFF;
    	dut2.g_dut_pattern_status_buf[27] = (dut2.g_as7994Cvsdata & 0xFF00)>>8;

    	cc2_rp_voltage = (u16)dut2.g_as7994Cvsdata;
    	cc_rp_voltage = (u16)dut2.g_as7994Cvsdata;
    	rp4p7k_ref = (u16)(dut2.g_dut_pattern_status_buf[12]<<8 | dut2.g_dut_pattern_status_buf[11]);
    	rp4p7k_min = (u16)(dut2.g_dut_pattern_status_buf[14]<<8 | dut2.g_dut_pattern_status_buf[13]);
    	rp4p7k_max = (u16)(dut2.g_dut_pattern_status_buf[16]<<8 | dut2.g_dut_pattern_status_buf[15]);

    	   //////////////xil_printf("dut2.g_as7994Cvsdata = %d\r\n", dut2.g_as7994Cvsdata);
		   //////////////xil_printf("cc2_rp4p7k_voltage = %d\r\n", cc2_rp_voltage);
	       //////////////xil_printf("rp4p7k_ref = %d\r\n",rp4p7k_ref );
	       //////////////xil_printf("rp4p7k_min = %d\r\n",rp4p7k_min );
	       //////////////xil_printf("rp4p7k_max = %d\r\n",rp4p7k_max );

			if((cc2_rp_voltage > rp4p7k_min) && (cc2_rp_voltage < rp4p7k_max))
			{
				dut2.g_pattern_timer = 0xfff;
				dut2.g_pattern_step = 0;
				dut2.g_dut_pattern_status_buf[2]++;
				dut2.g_uartPatternEnable = 0x00;
				dut2.g_uartPatternNum = 0x0c;
				//dut2.g_uartPatternNum = (dut2.g_uartPatternNum++ & 0xff);
				result_output_for_v50(XPAR_AXI_GPIO_dut2_1_BASEADDR,dut2.g_uartPatternNum);
				//xil_printf("dut2.g_uartPatternNum = %x\r\n\r\n",dut2.g_uartPatternNum);
				xil_printf("dut2_pattern10_rp_4p7k_test_pass!    rp4p7k_signbit= 0x%x  rp4p7k_offset=0x%x\r\n\r\n",dut2.g_efuse_data_buf[11],dut2.g_efuse_data_buf[12]);
				//xil_printf("dut2.g_result_fail = %x\r\n",dut2.g_result_fail);
			}

			else
			{
				dut2.g_result_fail = 0x01;
	   			dut2.g_result_fail_tmrcount = 0xffff;
			}
			////////////xil_printf("dut2_rp_g_pattern_step = %x\r\n", dut2.g_pattern_step);
			//////////////xil_printf("dut2_pattern9_read_cc2_rp4p7k_voltage\r\n");
			break;
	}
	}

	if(dut2.g_result_fail == 0x01)
	{
		dut2.g_ft2_test_done = 0x00;
		pattern_safe_state_dut2();	//add safe state
		xil_printf("dut2_pattern10_fail! fail_step = %x\r\n",dut2.g_pattern_step);
	    xil_printf("rp4p7k_min = %d rp4p7k_max = %d\r\n",rp4p7k_min, rp4p7k_max);
	    xil_printf("dut2_cc_rp_voltage = %d\r\n\r\n",cc_rp_voltage);
	}
	//xil_printf("dut2_pattern9_rp_step = 0x%x\r\n", dut2.g_pattern_step);
	return 0;
}



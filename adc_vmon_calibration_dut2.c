/*
 * ccdet2_calibration.c
 *
 *  Created on: 2018-06-14
 *      Author: ashley_su
 */



#include "platform.h"
#include "adc_vmon_calibration.h"

u8 _vmon_calibration_dut2()
{
	u8 i;
	switch(dut2.g_vmoncompare_process_bit)
	{
	    case vmon_process_waiting:
		{
			dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		    dut2.g_pattern_smbus_control_buf[2] = 0x47;
		    dut2.g_pattern_smbus_control_buf[3] = 0x10;
		    dut2.g_pattern_smbus_control_buf[4] = 0x01;
		    dut2.g_pattern_smbus_control_buf[5] = ((dut2.g_vmon_calib_level &0x07) |(dut2.g_vmon_temp_voltage[2]&0xF8));

		    ////////xil_printf("dut2.g_pattern_smbus_control_buf[5] = %x\r\n",dut2.g_pattern_smbus_control_buf[5]);
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
				       dut2.g_vmoncompare_process_bit = vmon_process_doing;
				       dut2.g_vmon_compare_step++;
		   }
		   ////////xil_printf("dut2.g_vmon_compare_step = %x\r\n",dut2.g_vmon_compare_step);

		break;
		}
	   case vmon_process_doing:
	{

		switch(dut2.g_vmon_compare_step)
	    {

		case 0x0000:
		{
			dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		    dut2.g_pattern_smbus_control_buf[2] = 0x47;
		    dut2.g_pattern_smbus_control_buf[3] = 0x10;
		    dut2.g_pattern_smbus_control_buf[4] = 0x01;
		    dut2.g_pattern_smbus_control_buf[5] = ((dut2.g_vmon_calib_level &0x07) |(dut2.g_vmon_temp_voltage[2]&0xF8));

		    ////////xil_printf("dut2.g_pattern_smbus_control_buf[5] = %x\r\n",dut2.g_pattern_smbus_control_buf[5]);
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
				   dut2.g_vmoncompare_process_bit = vmon_process_doing;
				   dut2.g_vmon_compare_step++;
		   }
		   ////////xil_printf("dut2.g_vmon_compare_step = %x\r\n",dut2.g_vmon_compare_step);

		break;
		}

		case 0x0001:
		{
			////xil_printf("dut2_pattern2_patternstep3_vmonstep1_start\r\n");

			i2c_max5825_output(AD7994_DEV2_ADDR,MAX5825_DEV0_SLAVE_ADDR,5,dut2.g_vmon_calib_voltage);

			usdelay(10);
	    	dut2.g_vmon_compare_step++;
	    	dut2.g_vmoncompare_process_bit = vmon_process_doing;
	    	////////xil_printf("dut2.g_vmon_compare_step = %x\r\n",dut2.g_vmon_compare_step);
	    	////xil_printf("dut2_dac_writen =%x,%x,%x\r\n",dut2.g_dac_data_buf[0],dut2.g_dac_data_buf[1],dut2.g_dac_data_buf[2]);
			break;
		}

		//write REG_VBUS_COMP_CFG4 reg 0x1049 bit[4:0] (AIF:REG_VTH2_TUNE) with 0x0f
		case 0x0002:
		{

			dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		    dut2.g_pattern_smbus_control_buf[2] = 0x49;
		    dut2.g_pattern_smbus_control_buf[3] = 0x10;
		    dut2.g_pattern_smbus_control_buf[4] = 0x01;
		    dut2.g_pattern_smbus_control_buf[5] = 0x0f;

		    ////////xil_printf("dut2.g_pattern_smbus_control_buf[5] = %x\r\n",dut2.g_pattern_smbus_control_buf[5]);
		   smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);
		   if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		   {
			   break;
		   }
		   else
		   {
		   dut2.g_vmon_calibration_data_buf[0] = 0x0f;
				   for(i=1; i<31; i++)
				   {
					   dut2.g_pattern_smbus_control_buf[i] = CLEAR_;
				   }
				   dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
				   dut2.g_vmoncompare_process_bit = vmon_process_doing;
				   dut2.g_vmon_compare_step++;
		   }
		   ////////xil_printf("dut2.g_vmon_compare_step = %x\r\n",dut2.g_vmon_compare_step);
		   ////xil_printf("dut2.g_pattern_smbus_control_buf[0] = %x\r\n",dut2.g_pattern_smbus_control_buf[0]);
		break;
		}

		//read REG_VBUS_COMP_DBC2 reg 0x1045 bit5 and set calibration register signbit

		case 0x0003:
		{
			   dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readmem;
			   dut2.g_pattern_smbus_control_buf[2] = 0x45;
			   dut2.g_pattern_smbus_control_buf[3] = 0x10;
			   dut2.g_pattern_smbus_control_buf[4] = 0x01;

			   smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);
			   if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
			   {
				   break;
			   }
			   else
			   {

				   //xil_printf("dut2.g_pattern_smbus_control_buf[10] =%x\r\n", dut2.g_pattern_smbus_control_buf[10]);
				   if(((dut2.g_pattern_smbus_control_buf[10] >>5) & 0x01) == 1)
				   {
					   dut2.g_calibrate_signbit = 0;
					   dut2.g_calibration_offset = 1;
					   dut2.g_vmon_compare_step++;
				   }
				   else
				   {
					   dut2.g_calibrate_signbit = 1;
					   dut2.g_calibration_offset = 1;
					   dut2.g_vmon_compare_step++;
				   }

				   for(i=0; i<31; i++)
				   {
					   dut2.g_pattern_smbus_control_buf[i+1] = CLEAR_;
				   }
				   dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
				   dut2.g_vmoncompare_process_bit = vmon_process_doing;

				   //xil_printf("dut2.g_calibrate_signbit =%x\r\n", dut2.g_calibrate_signbit);
				   //xil_printf("dut2.g_calibration_offset =%x\r\n", dut2.g_calibration_offset);

			   }

		       ////////xil_printf("dut2.g_vmon_compare_step = %x\r\n",dut2.g_vmon_compare_step);

		break;
		}

		  //write to calibration mem
			case 0x0004:
			{
				dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
				dut2.g_pattern_smbus_control_buf[2] = 0x49;
				dut2.g_pattern_smbus_control_buf[3] = 0x10;
				dut2.g_pattern_smbus_control_buf[4] = 0x01;

				if(dut2.g_calibrate_signbit == 0)
				{
		       			if((dut2.g_calibration_offset >=0) && (dut2.g_calibration_offset <= 0x10))
					{
					        dut2.g_pattern_smbus_control_buf[5] = (dut2.g_vmon_calibration_data_buf[0]+dut2.g_calibration_offset);
						    //////xil_printf("\r\ndut2.g_vmon_calibration_data_buf[0] =%x\r\n", dut2.g_vmon_calibration_data_buf[0]);
						    //////xil_printf("dut2.g_vmon_calibration_data_buf[1] =%x\r\n", dut2.g_vmon_calibration_data_buf[1]);
						    //////xil_printf("dut2.g_pattern_smbus_control_buf[5] = %x\r\n", dut2.g_pattern_smbus_control_buf[5]);
						    //////xil_printf("dut2.g_pattern_smbus_control_buf[6] = %x\r\n", dut2.g_pattern_smbus_control_buf[6]);
				}
					else
					{
						dut2.g_result_fail = 0x01;
						dut2.g_result_fail_tmrcount = 0xffff;
					}
				}
				else if(dut2.g_calibrate_signbit == 1)
				{
					if((dut2.g_calibration_offset >=0) && (dut2.g_calibration_offset <= 0x0f))
					{
				        dut2.g_pattern_smbus_control_buf[5] = (dut2.g_vmon_calibration_data_buf[0]-dut2.g_calibration_offset);
						}
					else
					{
						dut2.g_result_fail = 0x01;
						dut2.g_result_fail_tmrcount = 0xffff;
					}
					       //////xil_printf("\r\ndut2.g_vmon_calibration_data_buf[0] =%x\r\n", dut2.g_vmon_calibration_data_buf[0]);
					       //////xil_printf("dut2.g_vmon_calibration_data_buf[1] =%x\r\n", dut2.g_vmon_calibration_data_buf[1]);
					       //////xil_printf("dut2.g_pattern_smbus_control_buf[5] =%x\r\n", dut2.g_pattern_smbus_control_buf[5]);
					       //////xil_printf("dut2.g_pattern_smbus_control_buf[6] =%x\r\n\r\n", dut2.g_pattern_smbus_control_buf[6]);
					}
					else
					{
						dut2.g_result_fail = 0x01;
						dut2.g_result_fail_tmrcount = 0xffff;
					}
				//xil_printf("dut2.g_pattern_smbus_control_buf[5] =%x\r\n", dut2.g_pattern_smbus_control_buf[5]);
				//xil_printf("dut2.g_pattern_smbus_control_buf[6] =%x\r\n", dut2.g_pattern_smbus_control_buf[6]);

				smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);
				if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
				{

				}
				else
				{
					for(i=1; i<31; i++)
					{
						dut2.g_pattern_smbus_control_buf[i] = CLEAR_;
					}
					dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
					dut2.g_vmoncompare_process_bit = vmon_process_doing;
					dut2.g_vmon_compare_step++;

					   ////////xil_printf("dut2.g_vmon_retest_signbit =%x\r\n", dut2.g_vmon_retest_signbit);
					   //xil_printf("dut2.g_vmon_calibration_data_buf[0] =%x\r\n", dut2.g_vmon_calibration_data_buf[0]);
					   //xil_printf("dut2.g_vmon_calibration_data_buf[1] =%x\r\n", dut2.g_vmon_calibration_data_buf[1]);

					   ////////xil_printf("dut2.g_calibrate_signbit =%x\r\n", dut2.g_calibrate_signbit);
					   ////////xil_printf("dut2.g_calibration_offset =%x\r\n", dut2.g_calibration_offset);
				       ////////xil_printf("dut2.g_vmon_compare_step = %x\r\n",dut2.g_vmon_compare_step);
				}



				break;
			}

		//read 0x1045 data and calibration
		case 0x0005:
		{
			   dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readmem;
			   dut2.g_pattern_smbus_control_buf[2] = 0x45;
			   dut2.g_pattern_smbus_control_buf[3] = 0x10;
			   dut2.g_pattern_smbus_control_buf[4] = 0x01;

			   smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);
			   ////////xil_printf("\r\n dut2.g_pattern_smbus_control_buf[10] =%x\r\n", dut2.g_pattern_smbus_control_buf[10]);
			   ////////xil_printf("\r\n dut2.g_pattern_smbus_control_buf[11] =%x\r\n", dut2.g_pattern_smbus_control_buf[11]);
			   if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
			   {
				   break;
			   }
			   else
			   {

				   if(((dut2.g_pattern_smbus_control_buf[10] >> 5) & 0x01) == 0)
				   {
					  if(dut2.g_calibrate_signbit == 1)
					  {
						  if((dut2.g_calibration_offset >=0) && (dut2.g_calibration_offset < 0x0f))
						  {
							  dut2.g_calibration_offset++;
							  dut2.g_vmon_compare_step--;
						  }

						  else
						  {
							  dut2.g_result_fail = 0x01;
							  dut2.g_result_fail_tmrcount = 0xffff;
						  }
					  }

					  else if(dut2.g_calibrate_signbit == 0)
					  {
						  if((dut2.g_calibration_offset >=0) && (dut2.g_calibration_offset <= 0x10))
						  {
						  dut2.g_vmon_compare_step++;

						  }
						  else
						  {
							dut2.g_result_fail = 0x01;
							dut2.g_result_fail_tmrcount = 0xffff;
						  }
					  }
				   }


				   else if(((dut2.g_pattern_smbus_control_buf[10] >> 5) & 0x01) == 1)
				   {
					  if(dut2.g_calibrate_signbit == 0)
					  {
						  if((dut2.g_calibration_offset >=0) && (dut2.g_calibration_offset < 0x10))
						  {
							  dut2.g_calibration_offset++;
							  dut2.g_vmon_compare_step--;
						  }

						  else
						  {
							  dut2.g_result_fail = 0x01;
							  dut2.g_result_fail_tmrcount = 0xffff;
						  }
					  }

					  else if(dut2.g_calibrate_signbit == 1)
					  {
						  if((dut2.g_calibration_offset >=0) && (dut2.g_calibration_offset <= 0x0f))
						  {
						  dut2.g_vmon_compare_step++;

						  }
						  else
						  {
							dut2.g_result_fail = 0x01;
							dut2.g_result_fail_tmrcount = 0xffff;
						  }
					  }
				   }
				   //xil_printf("\r\n dut2.g_pattern_smbus_control_buf[10] =%x\r\n", dut2.g_pattern_smbus_control_buf[10]);
				   //xil_printf("\r\n dut2.g_pattern_smbus_control_buf[11] =%x\r\n", dut2.g_pattern_smbus_control_buf[11]);

				   for(i=1; i<31; i++)
				   {
					   dut2.g_pattern_smbus_control_buf[i] = CLEAR_;
				   }
				   dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;


				   ////////xil_printf("dut2.g_calibrate_signbit =%x\r\n", dut2.g_calibrate_signbit);
				   ////////xil_printf("dut2.g_calibration_offset =%x\r\n", dut2.g_calibration_offset);
				   ////////xil_printf("dut2.g_vmon_compare_step = %x\r\n",dut2.g_vmon_compare_step);
				   ////////xil_printf("dut2.g_pattern_smbus_control_buf[0] =%x\r\n", dut2.g_pattern_smbus_control_buf[0]);
				   ////////xil_printf("dut2_pattern5_ccdet_read 0xc1a9 data and calibration\r\n");
			   }



		break;
		}

		//read 0x1049 data and save to efuse buf
		case 0x0006:
		{
			   dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readmem;
			   dut2.g_pattern_smbus_control_buf[2] = 0x49;
			   dut2.g_pattern_smbus_control_buf[3] = 0x10;
			   dut2.g_pattern_smbus_control_buf[4] = 0x01;

			   smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);
			   if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
			   {
				   break;
			   }
			   else
			   {
				   dut2.g_vmon_calibration_data_buf[2] = dut2.g_pattern_smbus_control_buf[10];
				   dut2.g_vmon_calibration_data_buf[3] = dut2.g_pattern_smbus_control_buf[11];

				   dut2.g_vmoncompare_process_bit = vmon_process_done_pass;
				   dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
				   dut2.g_vmon_compare_step = 0;
			   }
		       //////////xil_printf("dut2.g_vmon_retest_signbit =%x\r\n", dut2.g_vmon_retest_signbit);
			   ////xil_printf("dut2.g_pattern_smbus_control_buf[10] =%x\r\n", dut2.g_pattern_smbus_control_buf[10]);
			   ////////xil_printf("dut2.g_pattern_smbus_control_buf[11] =%x\r\n", dut2.g_pattern_smbus_control_buf[11]);

			   //////////xil_printf("dut2.g_vmon_calibration_data_buf[2] =%x\r\n", dut2.g_vmon_calibration_data_buf[2]);
			   //////////xil_printf("dut2.g_vmon_calibration_data_buf[3] =%x\r\n", dut2.g_vmon_calibration_data_buf[3]);

			   //////////xil_printf("dut2.g_vmon_compare_step = %x\r\n",dut2.g_vmon_compare_step);


		break;
		}





	break;
	}

	}
	}
	////xil_printf("dut2.g_result_fail = %x\r\n",dut2.g_result_fail);
	//xil_printf("dut2.g_vmon_compare_step = %x\r\n\r\n",dut2.g_vmon_compare_step);

	return 0;
}


















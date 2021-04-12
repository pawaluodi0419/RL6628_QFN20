/******************************************************************************
*
* (c) Copyright 2010-2012 Xilinx, Inc. All rights reserved.
*
* This file contains confidential and proprietary information of Xilinx, Inc.
* and is protected under U.S. and international copyright and other
* intellectual property laws.
*
* DISCLAIMER
* This disclaimer is not a license and does not grant any rights to the
* materials distributed herewith. Except as otherwise provided in a valid
* license issued to you by Xilinx, and to the maximum extent permitted by
* applicable law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND WITH ALL
* FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES AND CONDITIONS, EXPRESS,
* IMPLIED, OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF
* MERCHANTABILITY, NON-INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE;
* and (2) Xilinx shall not be liable (whether in contract or tort, including
* negligence, or under any other theory of liability) for any loss or damage
* of any kind or nature related to, arising under or in connection with these
* materials, including for any direct, or any indirect, special, incidental,
* or consequential loss or damage (including loss of data, profits, goodwill,
* or any type of loss or damage suffered as a result of any action brought by
* a third party) even if such damage or loss was reasonably foreseeable or
* Xilinx had been advised of the possibility of the same.
*
* CRITICAL APPLICATIONS
* Xilinx products are not designed or intended to be fail-safe, or for use in
* any application requiring fail-safe performance, such as life-support or
* safety devices or systems, Class III medical devices, nuclear facilities,
* applications related to the deployment of airbags, or any other applications
* that could lead to death, personal injury, or severe property or
* environmental damage (individually and collectively, "Critical
* Applications"). Customer assumes the sole risk and liability of any use of
* Xilinx products in Critical Applications, subject only to applicable laws
* and regulations governing limitations on product liability.
*
* THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS PART OF THIS FILE
* AT ALL TIMES.
*
******************************************************************************/


#include "platform.h"
#include "xparameters.h"

void platform_initial()
{

    u16 i=0;
	//u16 j=0;
	//u16 k=0;

/////////////////////Dut0///////////////////////////////////////////////
    		/////////smbus initial////////
		if(dut0.g_dut_start_ready == 0x11)
		{
   			for(i=0;i<30;i++)
   			{
   				dut0.g_pattern_smbus_control_buf[i] = 0x00;
   			}
    		dut0.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
    		dut0.g_smbus_status_buf[0] = 0x00;
    		dut0.g_smbus_status_buf[1] = 0xff;
    		dut0.g_smbus_style = 0x00;
    		dut0.g_smbus_timer = 0xff;
    		dut0.g_relay_control_timer = 0xff;
    		dut0.g_smbus_getstatus_timer = 0x02;
    		/////////smbus initial////////
	 		dut0.g_vmon_compare_step=0;
	 		dut0.g_vbusocp_step =0;

    		/////////cc_det initial////////
   	   		for(i=0;i<12;i++)
   	   		{
   	   			dut0.g_ccdet_calibration_data_buf[i] = 0x88;
   	   		}

   	   	    dut0.g_retest = 0;
   	   	    dut0.g_ccdet_step = 0;
	   	    dut0.g_ccdet_retest_signbit = 0;
    		/////////cc_det initial////////

    		/////////pattern initial////////
   	   		for(i=0;i<12;i++)
   	   		{
   	 		dut0.g_dut_pattern_status_buf[i] = 0x00;
   	   		}
   	   		dut0.g_result_fail = 0x00;
  			dut0.g_uartPatternEnable = 0x01;
  			dut0.g_uartPatternNum = 0x0f;
  			dut0.g_result_output_ch = 0;
  			dut0.g_pattern_step = 0x00;
   	   	    dut0.g_pattern_timer = 0xfff;
    		/////////pattern initial////////

  			usdelay(100);
  			dut0.g_dut_start_ready = 1;
  			dut0.g_ft2_test_done = 0x01;
  			XGpio_2_WriteBit(0,dut0_FT2_PWR_CTRL_OFFSET,0x01);
  			xil_printf("\r\ndut0.platform_initial_print_finish!\r\n");
		}
  			/////////////////////Dut0///////////////////////////////////////////////

/////////////////////Dut1///////////////////////////////////////////////
    		/////////smbus initial////////
		if(dut1.g_dut_start_ready == 0x11)
		{
   			for(i=0;i<30;i++)
   			{
   				dut1.g_pattern_smbus_control_buf[i] = 0x00;
   			}
    		dut1.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
 		    dut1.g_smbus_status_buf[0] = 0x00;
 		    dut1.g_smbus_status_buf[1] = 0xff;
 		    dut1.g_smbus_style = 0x00;
 		    dut1.g_smbus_timer = 0xff;
 		    dut1.g_relay_control_timer = 0x00;
 		    dut1.g_smbus_getstatus_timer = 0x02;
    		/////////smbus initial////////
 		    dut1.g_vmon_compare_step=0;
	 		dut1.g_vbusocp_step =0;

    		/////////cc_det initial////////
   	   		for(i=0;i<12;i++)
   	   		{
   	   	    dut1.g_ccdet_calibration_data_buf[i] = 0x88;
   	   		}

   	   	    dut1.g_retest = 0;
   	   	    dut1.g_ccdet_step = 0;
   	   	    dut1.g_ccdet_retest_signbit = 0;
    		/////////cc_det initial////////

    		/////////pattern initial////////
   	   		for(i=0;i<12;i++)
   	   		{
   	 		dut1.g_dut_pattern_status_buf[i] = 0x00;
   	   		}
   	   	    dut1.g_result_fail = 0x00;
  			dut1.g_uartPatternEnable = 0x01;
  			dut1.g_uartPatternNum = 0x0f;
  			dut1.g_result_output_ch = 0;
  			dut1.g_pattern_step = 0x00;
   	   	    dut1.g_pattern_timer = 0xfff;
    		/////////pattern initial////////

  			usdelay(100);
  			dut1.g_dut_start_ready = 1;
  			dut1.g_ft2_test_done = 0x01;
   	   	    XGpio_2_WriteBit(0,dut0_FT2_PWR_CTRL_OFFSET,0x01);
			XGpio_2_WriteBit(0,dut1_FT2_PWR_CTRL_OFFSET,0x01);
  			xil_printf("\r\ndut1.platform_initial_print_finish!\r\n");
		}
/////////////////////Dut1///////////////////////////////////////////////

/////////////////////Dut2///////////////////////////////////////////////
    		/////////smbus initial////////
		if(dut2.g_dut_start_ready == 0x11)
		{
			for(i=0;i<30;i++)
			{
				dut2.g_pattern_smbus_control_buf[i] = 0x00;
			}
    		dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
	 		dut2.g_smbus_status_buf[0] = 0x00;
	 		dut2.g_smbus_status_buf[1] = 0xff;
	 		dut2.g_smbus_style = 0x00;
	 		dut2.g_smbus_timer = 0xff;
	 		dut2.g_relay_control_timer = 0x00;
	 		dut2.g_smbus_getstatus_timer = 0x02;
    		/////////smbus initial////////
	 		dut2.g_vmon_compare_step=0;
	 		dut2.g_vbusocp_step =0;

    		/////////cc_det initial////////
   	   		for(i=0;i<12;i++)
   	   		{
   	        dut2.g_ccdet_calibration_data_buf[i] = 0x88;
   	   		}

   	        dut2.g_retest = 0;
   	   	    dut2.g_ccdet_step = 0;
   	   	    dut2.g_ccdet_retest_signbit = 0;
    		/////////cc_det initial////////

    		/////////pattern initial////////
   	   		for(i=0;i<12;i++)
   	   		{
   	 		dut2.g_dut_pattern_status_buf[i] = 0x00;
   	   		}
   	   	    dut2.g_result_fail = 0x00;
  			dut2.g_uartPatternEnable = 0x01;
  			dut2.g_uartPatternNum = 0x0f;
  			dut2.g_result_output_ch = 0;
  			dut2.g_pattern_step = 0x00;
   	   	    dut2.g_pattern_timer = 0xfff;
    		/////////pattern initial////////

  			usdelay(100);
  			dut2.g_dut_start_ready = 1;
  			dut2.g_ft2_test_done = 0x01;
   	   	    XGpio_2_WriteBit(0,dut0_FT2_PWR_CTRL_OFFSET,0x01);
			XGpio_2_WriteBit(0,dut2_FT2_PWR_CTRL_OFFSET,0x01);
  			xil_printf("\r\ndut2.platform_initial_print_finish!\r\n");
		}
/////////////////////Dut2///////////////////////////////////////////////

/////////////////////Dut3///////////////////////////////////////////////
  	   		/////////smbus initial////////
		if(dut3.g_dut_start_ready == 0x11)
		{
			for(i=0;i<30;i++)
			{
				dut3.g_pattern_smbus_control_buf[i] = 0x00;
			}
    		dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
	 		dut3.g_smbus_status_buf[0] = 0x00;
	 		dut3.g_smbus_status_buf[1] = 0xff;
	 		dut3.g_smbus_style = 0x00;
	 		dut3.g_smbus_timer = 0xff;
	 		dut3.g_relay_control_timer = 0x00;
	 		dut3.g_smbus_getstatus_timer = 0x02;
    		/////////smbus initial////////

	 		dut3.g_vmon_compare_step=0;
	 		dut3.g_vbusocp_step =0;
    		/////////cc_det initial////////
   	   		for(i=0;i<12;i++)
   	   		{
   	   		dut3.g_ccdet_calibration_data_buf[i] = 0x88;
   	   		}
   	        dut3.g_retest = 0;
   	   	    dut3.g_ccdet_step = 0;
   	   	    dut3.g_ccdet_retest_signbit = 0;
    		/////////cc_det initial////////

    		/////////pattern initial////////
   	   		for(i=0;i<12;i++)
   	   		{
   	 		dut3.g_dut_pattern_status_buf[i] = 0x00;
   	   		}
   	   	    dut3.g_result_fail = 0x00;
  			dut3.g_uartPatternEnable = 0x01;
  			dut3.g_uartPatternNum = 0x0f;
  			dut3.g_result_output_ch = 0;
  			dut3.g_pattern_step = 0x00;
   	   	    dut3.g_pattern_timer = 0xfff;
    		/////////pattern initial////////

  			usdelay(100);
  			dut3.g_dut_start_ready = 1;
  			dut3.g_ft2_test_done = 0x01;
   	   	    XGpio_2_WriteBit(0,dut0_FT2_PWR_CTRL_OFFSET,0x01);
			XGpio_2_WriteBit(0,dut3_FT2_PWR_CTRL_OFFSET,0x01);
  			xil_printf("\r\ndut3.platform_initial_print_finish!\r\n");
		}
/////////////////////Dut3///////////////////////////////////////////////


  			//xil_printf("dut0.g_dut_start_ready = %x\r\n\r\n", dut0.g_dut_start_ready);
  			//xil_printf("dut1.g_dut_start_ready = %x\r\n\r\n", dut1.g_dut_start_ready);
  			//xil_printf("dut2.g_dut_start_ready = %x\r\n\r\n", dut2.g_dut_start_ready);
  			//xil_printf("dut3.g_dut_start_ready = %x\r\n\r\n", dut3.g_dut_start_ready);
  		    //xil_printf("dut0.g_result_fail = %x\r\n\r\n", dut0.g_result_fail);
  		    //xil_printf("dut1.g_result_fail = %x\r\n\r\n", dut1.g_result_fail);
  		    //xil_printf("dut2.g_result_fail = %x\r\n\r\n", dut2.g_result_fail);
  		    //xil_printf("dut3.g_result_fail = %x\r\n\r\n", dut3.g_result_fail);
  			//xil_printf("dut0.g_dut_pattern_status_buf[2] = %x\r\n\r\n", dut0.g_dut_pattern_status_buf[2]);
  			//xil_printf("dut1.g_dut_pattern_status_buf[2] = %x\r\n\r\n", dut1.g_dut_pattern_status_buf[2]);
  			//xil_printf("dut2.g_dut_pattern_status_buf[2] = %x\r\n\r\n", dut2.g_dut_pattern_status_buf[2]);
  			//xil_printf("dut3.g_dut_pattern_status_buf[2] = %x\r\n\r\n", dut3.g_dut_pattern_status_buf[2]);
  		    //xil_printf("\r\n dut0.g_pattern_step = %x\r\n", dut0.g_pattern_step);
  		    //xil_printf("\r\n dut1.g_pattern_step = %x\r\n", dut1.g_pattern_step);
  		    //xil_printf("\r\n dut2.g_pattern_step = %x\r\n", dut2.g_pattern_step);
  		    //xil_printf("\r\n dut3.g_pattern_step = %x\r\n", dut3.g_pattern_step);
		   //xil_printf("platform initial over!\r\n");


}

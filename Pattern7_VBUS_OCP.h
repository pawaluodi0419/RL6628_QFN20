/*
 * Copyright (c) 2008 Xilinx, Inc.  All rights reserved.
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#ifndef _Pattern7_VBUSOCP_DUT_H_
#define _Pattern7_VBUSOCP_DUT_H_

#include <math.h>
#include <stdio.h>

//xilix system header file
#include "platform_config.h"
#include "xparameters.h"
#include "xspi.h"
#include "xuartlite_l.h"
#include "xtmrctr_l.h"
#include "xgpio_l.h"
#include "xintc_l.h"
#include "xiic_l.h"
#include "state_define.h"

//user define header file
#include "ad7994_drv.h"
#include "gpio_drv.h"
#include "dac_5825_4822_drv.h"
#include "spi_drv.h"
#include "spi_flash_drv.h"
#include "timer_drv.h"
#include "uart_drv.h"
#include "var_initial.h"
#include "smbus_drv.h"
#include "smbus_irq_handle.h"

//extern global statement

//#define VBUSOCP_Efuse_0p9a_Min	10
//#define VBUSOCP_Efuse_0p9a_Max	50
#define VBUSOCP_Efuse_1p5a_Min	6
#define VBUSOCP_Efuse_1p5a_Max	26
#define VBUSOCP_Efuse_3a_Min	4
#define VBUSOCP_Efuse_3a_Max	9

u8 _by_Pattern7_VBUS_OCP_1p5A_dut0();
u8 _by_Pattern7_VBUS_OCP_1p5A_dut1();
u8 _by_Pattern7_VBUS_OCP_1p5A_dut2();
u8 _by_Pattern7_VBUS_OCP_1p5A_dut3();

u8 _by_Pattern8_VBUS_OCP_3A_dut0();
u8 _by_Pattern8_VBUS_OCP_3A_dut1();
u8 _by_Pattern8_VBUS_OCP_3A_dut2();
u8 _by_Pattern8_VBUS_OCP_3A_dut3();


#endif

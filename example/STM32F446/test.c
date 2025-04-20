/******************************************************************************
 * SD FAT library for STM32 ARM Cortex-M3 MCUs.
 * Copyright (C) 2021 Dilshan R Jayakody [jayakody2000lk@gmail.com]
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 * 
 *****************************************************************************
 * This library is based on OpenFAT project written by 
 * Gareth McMullin <gareth@blacksphere.co.nz>. 
 * <https://github.com/tmolteno/openfat>
 *****************************************************************************/

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include "../../openfat/include/openfat.h"
#include "../../openfat/include/openfat/mbr.h"

#include "../../logger/logger.h"
#include "../../sddriver/sddriver.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>

#define LED_PORT GPIOB
#define LED_PIN GPIO2


void blink_led(int n){
    
  	while(1){
        for(int i=0; i<n; i++){
            gpio_set(LED_PORT, LED_PIN);
            for(volatile long p = 0; p < 0x27FFFF; p++);
            gpio_clear(LED_PORT, LED_PIN);
            for(volatile long p = 0; p < 0x27FFFF; p++);            
        }
        for(volatile long p = 0; p < 0xCFFFFF; p++);            
    }
}

void blink_led_inf(int n){
    while(1){
        blink_led(n);
    }
}

int main()
{
	struct block_mbr_partition part;
	struct block_device dev;
	struct fat_vol_handle volHandle;
	struct fat_file_handle fileHandle;

	char statusBuff[30];

	char dirname[20];
	char filename[20];
	char buffer[2000];
	
	// Configure to use internal 48MHz clock.
	rcc_clock_setup_pll(&rcc_hsi_configs[RCC_CLOCK_3V3_180MHZ]);

	// Configure PortC pin13 (onboard LED) to output mode.
	rcc_periph_clock_enable(RCC_GPIOB);
    gpio_mode_setup(LED_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_PIN);
    gpio_set_output_options(LED_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, LED_PIN);
 
	// Configure USART and SPI peripherals.
	loggerInit();
	sdSPIInit();

	// Initialize SD card driver.
	if(!sdDrvInit())
	{
        loggerOutputLine("Driver initialization fail");
        blink_led_inf(1);
	}

	loggerOutputLine("Driver initialization successful");

	// Initialize OpenFAT block device structure with SD driver functions.
	dev.read_sectors = sdDrvReadSector;
	dev.write_sectors = sdDrvWriteSector;
	dev.get_sector_size = sdDrvGetSectorSize;

	// Initialize partition block device.
	if(mbr_partition_init(&part, &dev, 0) == 0)
	{
            loggerOutputLine("mbr_partition_init successful");
	}
	else
	{
        loggerOutputLine("mbr_partition_init fail");
        blink_led_inf(2);

	}

	// Initialize and mount the FAT volume.
	if(fat_vol_init((struct block_device *)&part, &volHandle) == 0)
	{
        loggerOutputLine("fat_vol_init successful");
        sprintf(statusBuff, "FAT type = %d", volHandle.type);
        loggerOutputLine(statusBuff);
	}
	else
	{
        loggerOutputLine("fat_vol_init fail");
        blink_led_inf(3);

	}

	// Start directory and file creation test (moved form example.c in original OpenFAT project).
	loggerOutputLine("Start Dir/File test...");
    
    
    int cnt = 0;
	for(int i = 0; i < 5; i++) 
	{
        sprintf(dirname, "Test%d", i);
        fat_mkdir(&volHandle, dirname);
        assert(fat_chdir(&volHandle, dirname) == 0);

		for(int j = 0; j < 5; j++) 
		{   
            cnt++;
            sprintf(filename, "File%d", j);
            assert(fat_create(&volHandle, filename, O_WRONLY, &fileHandle) == 0);
            sprintf(buffer, "This is...\n -Folder: %d\n -File: %d\n -Count: %d\n", i,j,cnt);
            assert(fat_write(&fileHandle, buffer, sizeof(buffer)) == sizeof(buffer));
        }

        assert(fat_chdir(&volHandle, "..") == 0);
	}	

	loggerOutputLine("End Dir/File test...");
	// End of directory and file creation test.

    blink_led_inf(4);

    return 0;
}

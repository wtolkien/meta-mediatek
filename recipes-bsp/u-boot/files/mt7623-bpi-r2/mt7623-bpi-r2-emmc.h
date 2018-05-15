/*
 * (C) Copyright 2012 Stephen Warren
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#include <linux/sizes.h>
//#include <configs/autoconf.h>
#include <asm/arch/mt_reg_base.h>

//#define DEBUG

/* don't touch these */
#define MT7623 1

#undef FPGA
#undef MT7623_FPGA_BOARD

#define ASIC 1
#define MT7623_ASIC_BOARD 1

#define PDMA_NEW 1
#define RX_SCATTER_GATTER_DMA 1

#define NUM_RX_DESC 24
#define NUM_TX_DESC 24
#define CONFIG_SYS_RX_ETH_BUFFER    NUM_RX_DESC

/****************************************************************************************************************
 *                                Global Configuration (change as per your hardware)
 ****************************************************************************************************************/


/* select one of the following (only eMMC has been tested recently) */
#undef  ON_BOARD_NAND_FLASH_COMPONENT
#undef  ON_BOARD_SPI_FLASH_COMPONENT
#define ON_BOARD_EMMC_COMPONENT 1
#undef  OFF_BOARD_SD_CARD_COMPONENT

#ifdef ON_BOARD_NAND_FLASH_COMPONENT
#define CONFIG_MTK_MTD_NAND 1
#define CONFIG_MTK_SLC_NAND_SUPPORT 1
#endif

/* select RAM size */
#undef  ON_BOARD_1024Mb_DRAM_COMPONENT
#undef  ON_BOARD_2048Mb_DRAM_COMPONENT
#undef  ON_BOARD_4096Mb_DRAM_COMPONENT
#undef  ON_BOARD_8192Mb_DRAM_COMPONENT
#define ON_BOARD_16384Mb_DRAM_COMPONENT 1

/* use GMAC1 or GMAC2 for Ethernet */
#define CONFIG_USE_GE1 1
#undef  CONFIG_USE_GE2

#ifdef CONFIG_USE_GE1
/* in case of GMAC1, select one of the following options depending on your hardware */
#undef  GE_MII_FORCE_100
#undef  GE_RVMII_FORCE_100
#undef  GE_MII_AN
#undef  GE_RGMII_FORCE_1000
#undef  GE_RGMII_AN
#define CONFIG_GE1_TRGMII_FORCE_2600 1

/*
 set the following helper flags and values depending on your choice above:

                              | MAC_TO_100SW_MODE | MAC_TO_100PHY_MODE | MAC_TO_GIGAPHY_MODE | MAC_TO_GIGAPHY_MODE_ADDR | MAC_TO_MT7530_MODE
 ---------------------------------------------------------------------------------------------------------------------------------------------
 GE_MII_FORCE_100             |        1          |                    |                     |                          |
 GE_RVMII_FORCE_100  		  |        1          |                    |                     |                          |
 GE_MII_AN           		  |                   |        1		   |		             |         0x1e		        |
 GE_RGMII_AN                  |                   |     			   |		  1          |         0x1e		        |
 GE_RGMII_FORCE_1000		  |                   |                    |                     |                          |         1
 CONFIG_GE1_TRGMII_FORCE_2600 |                   |                    |                     |                          |         1
*/

#undef MAC_TO_100SW_MODE
#undef MAC_TO_100PHY_MODE
#undef MAC_TO_GIGAPHY_MODE
#undef MAC_TO_GIGAPHY_MODE_ADDR
#define MAC_TO_MT7530_MODE 1

#endif  /* CONFIG_USE_GE1 */

#ifdef CONFIG_USE_GE2
/* in case of GMAC2, select one of the following options depending on your hardware */
#undef  GE_MII_FORCE_100
#undef  GE_RVMII_FORCE_100
#undef  GE_MII_AN
#undef  GE_RGMII_FORCE_1000
#undef  GE_RGMII_AN
#undef  GE_RGMII_INTERNAL_P0_AN
#define GE_RGMII_INTERNAL_P4_AN 1

/*
 set the following helper flags and values depending on your choice above:

                              | MAC_TO_100SW_MODE | MAC_TO_100PHY_MODE | MAC_TO_GIGAPHY_MODE | MAC_TO_GIGAPHY_MODE_ADDR | MAC_TO_MT7530_MODE
 ---------------------------------------------------------------------------------------------------------------------------------------------
 GE_MII_FORCE_100             |        1          |                    |                     |                          |
 GE_RVMII_FORCE_100           |        1          |                    |                     |                          |
 GE_MII_AN           		  |                   |        1		   |		             |         0x1f		        |
 GE_RGMII_AN                  |                   |     			   |		  1          |         0x1f		        |
 GE_RGMII_FORCE_1000		  |                   |                    |                     |                          |         1
 GE_RGMII_INTERNAL_P0_AN      |                   |                    |          1          |         0x00             |
 GE_RGMII_INTERNAL_P4_AN      |                   |                    |          1          |         0x04             |
*/

#undef MAC_TO_100SW_MODE
#undef MAC_TO_100PHY_MODE
#define MAC_TO_GIGAPHY_MODE 1
#define MAC_TO_GIGAPHY_MODE_ADDR 0x4
#undef MAC_TO_MT7530_MODE

#endif


/**********************************************************************************************
 *                                      ARM Cortex A7
 **********************************************************************************************/
#if 1
/*
 *  Iverson 20140326 :
 *      We can disable dcache for accessing non-cachable address instead of using mmu page table to provide noncachable address.
 */
/* Cache */
// Iverson debug
#define CONFIG_SYS_DCACHE_OFF
#endif

/* Machine ID */
#define CONFIG_MACH_TYPE                    7623

/**********************************************************************************************
 *                                          Memory
 **********************************************************************************************/
/* Memory layout */
/* DRAM definition */
/*
 * Iverson 20140521 : We detect ram size automatically.
 *      CONFIG_SYS_SDRAM_SIZE define max uboot size.
 *      The max size that auto detection support is 256MB.
 */
#define CONFIG_NR_DRAM_BANKS		        1
#define CONFIG_SYS_SDRAM_BASE		        0x80000000
#if defined(ON_BOARD_1024Mb_DRAM_COMPONENT)
#define CONFIG_SYS_SDRAM_SIZE               SZ_128M
#elif defined(ON_BOARD_2048Mb_DRAM_COMPONENT)
#define CONFIG_SYS_SDRAM_SIZE               SZ_256M
#elif defined(ON_BOARD_4096Mb_DRAM_COMPONENT)
#define CONFIG_SYS_SDRAM_SIZE               SZ_512M
#elif defined(ON_BOARD_8192Mb_DRAM_COMPONENT)
#define CONFIG_SYS_SDRAM_SIZE               SZ_1G
#elif defined(ON_BOARD_16384Mb_DRAM_COMPONENT)
#define CONFIG_SYS_SDRAM_SIZE               SZ_2G
#else
#error "memory defined error!!!")
#endif

/* Code Layout */
//#define CONFIG_SYS_TEXT_BASE		        0x80000000
#define CONFIG_SYS_TEXT_BASE		        0x81E00000

/* Uboot definition */
#define CONFIG_SYS_UBOOT_BASE		        CONFIG_SYS_TEXT_BASE
#define CONFIG_SYS_UBOOT_MAX_SIZE           SZ_2M
#define CONFIG_SYS_INIT_SP_ADDR             (CONFIG_SYS_TEXT_BASE + \
                                             CONFIG_SYS_UBOOT_MAX_SIZE - \
                                             GENERATED_GBL_DATA_SIZE)


#if (CONFIG_SYS_SDRAM_SIZE == SZ_128M) || (CONFIG_SYS_SDRAM_SIZE == SZ_256M) || \
    (CONFIG_SYS_SDRAM_SIZE == SZ_512M) || (CONFIG_SYS_SDRAM_SIZE == SZ_1G) || \
    (CONFIG_SYS_SDRAM_SIZE == SZ_2G)
#define CONFIG_SYS_MALLOC_LEN               SZ_32M

/* RichOS memory partitions */
#define CONFIG_SYS_DECOMP_ADDR              0x80008000
#define CONFIG_SYS_LOAD_ADDR                0x84000000
#define CONFIG_SYS_IMAGE_HDR_ADDR           CONFIG_SYS_LOAD_ADDR

/* Linux DRAM definition */
#define CONFIG_LOADADDR			            CONFIG_SYS_LOAD_ADDR

/*
 * For booting Linux, the board info and command line data
 * have to be in the first 64 MB of memory, since this is
 * the maximum mapped by the Linux kernel during initialization.
 */
#define CONFIG_SYS_BOOTM_LEN	            0x4000000
#else
#error "memory partition error!!!"

#endif

/**********************************************************************************************
 *                                           Board
 **********************************************************************************************/

/* Board */
#if defined(MT7623_FPGA_BOARD)
#define MACH_FPGA   1
#endif
#define CONFIG_ADD_MTK_HEADER               "y"

#define CONFIG_BOARD_LATE_INIT

/**********************************************************************************************
 *                                          Devices
 **********************************************************************************************/

/* Disable *NOR* Flash */
#define CONFIG_SYS_NO_FLASH

/****************************** SPI Flash **********************************/
#if defined(ON_BOARD_SPI_FLASH_COMPONENT)

#define CONFIG_SPI_FLASH
#define CONFIG_SF_DEFAULT_BUS               0
#define CONFIG_SF_DEFAULT_CS                0
#define CONFIG_SF_DEFAULT_SPEED             1000000
#define CONFIG_SF_DEFAULT_MODE              SPI_MODE_3

#define CONFIG_CMD_SF

#define CONFIG_ENV_IS_IN_SPI_FLASH
#define CONFIG_ENV_SIZE                     SZ_16K
#define CONFIG_ENV_SECT_SIZE                0x40000
#define CONFIG_ENV_OFFSET                   0x0000


/****************************** NAND Flash **********************************/
#elif defined(ON_BOARD_NAND_FLASH_COMPONENT)

#define CONFIG_SYS_MAX_NAND_DEVICE	        1
#define CONFIG_SYS_NAND_BASE                NFI_BASE
#define CONFIG_SYS_MAX_FLASH_SECT			256
#define CONFIG_SYS_MAX_FLASH_BANKS			1
#define CONFIG_CMD_NAND
//#define CONFIG_MTD_DEBUG
//#define CONFIG_MTD_DEBUG_VERBOSE  7
#define CONFIG_CMD_MTDPARTS	1
#define CONFIG_MTD_PARTITIONS	1
#define CONFIG_ENV_IS_IN_NAND
#define CONFIG_MTD_NAND_VERIFY_WRITE		1

#if defined (CONFIG_MTK_MLC_NAND_SUPPORT)
#define CONFIG_ENV_SIZE                     SZ_4K
#define CONFIG_ENV_OFFSET                   0x2000000
#define CONFIG_NAND_UBOOT_OFFSET            0x1000000
#define CONFIG_NAND_LINUX_OFFSET            0x4000000
#define CONFIG_MAX_NAND_PAGE_SIZE			8192
#define CONFIG_MAX_NAND_BLOCK_SIZE			2097152
#define CONFIG_MAX_UBOOT_SIZE				393216	//262144
#else
#define CONFIG_ENV_SIZE                     SZ_4K
#define CONFIG_ENV_OFFSET                   0xC0000
#define CONFIG_NAND_UBOOT_OFFSET            0x40000
#define CONFIG_NAND_LINUX_OFFSET            0x140000
#define CONFIG_MAX_NAND_PAGE_SIZE       	2048
#define CONFIG_MAX_NAND_BLOCK_SIZE      	131072
#define CONFIG_MAX_UBOOT_SIZE   			393216	//262144
#endif


/****************************** eMMC Flash **********************************/

#elif defined(ON_BOARD_EMMC_COMPONENT)

/* eMMC is MMC device #0 */
#define CONFIG_CMD_MMC
#define CONFIG_ENV_IS_IN_MMC
#define CONFIG_ENV_SIZE                     SZ_16K
#define CONFIG_ENV_OFFSET                   0x4c000 		/* U-Boot environment address in Flash (NOT block #) */
#define CONFIG_SYS_MMC_ENV_DEV              0



/******************************** SD Card ***********************************/

#elif defined(OFF_BOARD_SD_CARD_COMPONENT)

/* SD card is MMC device #1 */
#define CONFIG_CMD_MMC
#define CONFIG_ENV_IS_IN_MMC
#define CONFIG_ENV_SIZE                     SZ_16K
#define CONFIG_ENV_OFFSET                   0x4c000
#define CONFIG_SYS_MMC_ENV_DEV              1


/************************* No Flash (Really...?) *****************************/

#else
#define CONFIG_ENV_IS_NOWHERE
#define CONFIG_ENV_SIZE                     SZ_16K
#endif


/*************** SD Card and eMMC need file system support *******************/

#if defined(ON_BOARD_EMMC_COMPONENT) || defined(OFF_BOARD_SD_CARD_COMPONENT)
#define PART_DEBUG
#define CONFIG_MMC
#define CONFIG_FS_FAT
#define CONFIG_CMD_FAT
#define CONFIG_CMD_EXT4
#define CONFIG_CMD_FS_GENERIC
#define CONFIG_MEDIATEK_MMC

#define CONFIG_CMD_PART
#define CONFIG_EFI_PARTITION
#define CONFIG_DOS_PARTITION
#define CONFIG_PARTITION_UUIDS
#endif


/********************** Watchdog *************************/
#define CONFIG_WATCHDOG_OFF

/********************** Console UART *************************/
/* Uart baudrate */
#define CONFIG_BAUDRATE                     115200

/* Console configuration */
#define CONFIG_SYS_CBSIZE		            1024
#define CONFIG_SYS_PBSIZE		            (CONFIG_SYS_CBSIZE +		\
					                            sizeof(CONFIG_SYS_PROMPT) + 16)
//#define CONFIG_CONSOLE_MUX
#define CONFIG_SYS_CONSOLE_IS_IN_ENV


/********************** Ethernet *************************/
#define CONFIG_ETHADDR                      01:02:03:04:05:06
#define CONFIG_IPADDR                       10.30.0.1
#define CONFIG_SERVERIP                     10.30.0.2
#define CONFIG_CMD_NET
#define CONFIG_RT2880_ETH

#define RALINK_REG(x)		(*((volatile u32 *)(x)))



/**********************************************************************************************
 *                                       UBoot Command
 **********************************************************************************************/
/* Shell */
#define CONFIG_SYS_MAXARGS		            16
#define CONFIG_SYS_PROMPT		            "mt7623-uboot > "
#define CONFIG_COMMAND_HISTORY
#define CONFIG_SYS_BOOT_GET_CMDLINE

/* boot after 'bootdelay' seconds */
#define CONFIG_BOOTDELAY 					3
#define CONFIG_BOOTCOMMAND 					"run bootcmd"

/* Commands */
#include <config_cmd_default.h>

/* Device tree support */
#define CONFIG_OF_BOARD_SETUP
#define CONFIG_OF_LIBFDT
#define CONFIG_OF_BOOT_CPU

/* Bootz support */
#define CONFIG_CMD_BOOTZ

/* ATAGs support for bootm/bootz */
#define CONFIG_CMDLINE_TAG

#undef CONFIG_CMD_FPGA
#undef CONFIG_CMD_NFS

#define CONFIG_SYS_LONGHELP
#define CONFIG_CMD_PING

/* Enable Hush Parser */
#define CONFIG_SYS_HUSH_PARSER

/* Compression */
//#define CONFIG_LZMA

#endif

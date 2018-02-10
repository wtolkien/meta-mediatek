#include <common.h>
#include <config.h>

#include <asm/arch/mt_gpt.h>
#include <asm/arch/mtk_wdt.h>
#include <asm/arch/mt_typedefs.h>
#include <asm/arch/mt_reg_base.h>

DECLARE_GLOBAL_DATA_PTR;

extern int rt2880_eth_initialize(bd_t *bis);

/*
 *  Iverson 20140326 : DRAM have been initialized in preloader.
 */

int dram_init(void)
{
    /*
     *  Iverson 20140526 : Use static memory declaration.
     *      UBoot support memory auto detection. 
     *      However, we still use static declaration becasue we should modify many code if we use auto detection.
     */

    gd->ram_size = CONFIG_SYS_SDRAM_SIZE;
    //gd->ram_size = get_ram_size((long *)CONFIG_SYS_SDRAM_BASE, CONFIG_SYS_SDRAM_SIZE);

	return 0;
}

int board_init(void)
{
    mtk_timer_init();
    
#ifdef CONFIG_WATCHDOG_OFF
    mtk_wdt_disable();
#endif

    /* Nelson: address of boot parameters */
	gd->bd->bi_boot_params = CONFIG_SYS_SDRAM_BASE + 0x100;

    return 0;
}

int board_late_init (void)
{
    gd->env_valid = 1; //to load environment variable from persistent store
    env_relocate();
#ifdef CONFIG_CMDLINE_TAG
    char *commandline;

    /* Nelson: set linux kernel boot arguments */
    //setenv("bootargs", COMMANDLINE_TO_KERNEL);
    commandline = getenv("bootargs");
    printf("bootargs = %s\n", commandline);

    return 0;
#endif
}

/*
 *  Iverson todo
 */

void ft_board_setup(void *blob, bd_t *bd)
{
}

#ifndef CONFIG_SYS_DCACHE_OFF
void enable_caches(void)
{
    /* Enable D-cache. I-cache is already enabled in start.S */
    dcache_enable();
}
#endif

int board_eth_init(bd_t *bis)
{
    rt2880_eth_initialize(bis);

    return 0;
}


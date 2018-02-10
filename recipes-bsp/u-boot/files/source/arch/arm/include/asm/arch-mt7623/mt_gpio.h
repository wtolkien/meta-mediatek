#if (!defined(CONFIG_MT8127_FPGA))
#include <asm/arch/cust_gpio_usage.h>
#define MT_GPIO_ENABLED 1
#endif

#include <asm/arch/mt_typedefs.h>
#include <asm/arch/debug.h>
//#include <asm/mach-types.h>
//#include <common.h>
/*----------------------------------------------------------------------------*/
//  Error Code No.
#define RSUCCESS        0
#define ERACCESS        1
#define ERINVAL         2
#define ERWRAPPER		3
/*----------------------------------------------------------------------------*/
#ifndef s32
	#define s32 signed int
#endif
#ifndef s64
	#define s64 signed long long
#endif

/******************************************************************************
* Enumeration for GPIO pin
******************************************************************************/
typedef enum GPIO_PIN
{    
    GPIO_UNSUPPORTED = -1,    
        
    GPIO0  , GPIO1  , GPIO2  , GPIO3  , GPIO4  , GPIO5  , GPIO6  , GPIO7  ,
    GPIO8  , GPIO9  , GPIO10 , GPIO11 , GPIO12 , GPIO13 , GPIO14 , GPIO15 ,
    GPIO16 , GPIO17 , GPIO18 , GPIO19 , GPIO20 , GPIO21 , GPIO22 , GPIO23 ,
    GPIO24 , GPIO25 , GPIO26 , GPIO27 , GPIO28 , GPIO29 , GPIO30 , GPIO31 ,
    GPIO32 , GPIO33 , GPIO34 , GPIO35 , GPIO36 , GPIO37 , GPIO38 , GPIO39 ,
    GPIO40 , GPIO41 , GPIO42 , GPIO43 , GPIO44 , GPIO45 , GPIO46 , GPIO47 ,
    GPIO48 , GPIO49 , GPIO50 , GPIO51 , GPIO52 , GPIO53 , GPIO54 , GPIO55 ,
    GPIO56 , GPIO57 , GPIO58 , GPIO59 , GPIO60 , GPIO61 , GPIO62 , GPIO63 ,
    GPIO64 , GPIO65 , GPIO66 , GPIO67 , GPIO68 , GPIO69 , GPIO70 , GPIO71 ,
    GPIO72 , GPIO73 , GPIO74 , GPIO75 , GPIO76 , GPIO77 , GPIO78 , GPIO79 ,
    GPIO80 , GPIO81 , GPIO82 , GPIO83 , GPIO84 , GPIO85 , GPIO86 , GPIO87 ,
    GPIO88 , GPIO89 , GPIO90 , GPIO91 , GPIO92 , GPIO93 , GPIO94 , GPIO95 ,
    GPIO96 , GPIO97 , GPIO98 , GPIO99 , GPIO100, GPIO101, GPIO102, GPIO103,
    GPIO104, GPIO105, GPIO106, GPIO107, GPIO108, GPIO109, GPIO110, GPIO111,
    GPIO112, GPIO113, GPIO114, GPIO115, GPIO116, GPIO117, GPIO118, GPIO119,
    GPIO120, GPIO121, GPIO122, GPIO123, GPIO124, GPIO125, GPIO126, GPIO127,
    GPIO128, GPIO129, GPIO130, GPIO131, GPIO132, GPIO133, GPIO134, GPIO135,
    GPIO136, GPIO137, GPIO138, GPIO139, GPIO140, GPIO141, GPIO142,

#ifdef MTK_PMIC_MT6397
    GPIOEXT0, GPIOEXT1, GPIOEXT2, GPIOEXT3, GPIOEXT4, GPIOEXT5, GPIOEXT6, GPIOEXT7,	
    GPIOEXT8, GPIOEXT9, GPIOEXT10, GPIOEXT11, GPIOEXT12, GPIOEXT13, GPIOEXT14, GPIOEXT15,	
    GPIOEXT16, GPIOEXT17, GPIOEXT18, GPIOEXT19, GPIOEXT20, GPIOEXT21, GPIOEXT22, GPIOEXT23,	
    GPIOEXT24, GPIOEXT25, GPIOEXT26, GPIOEXT27, GPIOEXT28, GPIOEXT29, GPIOEXT30, GPIOEXT31,	
    GPIOEXT32, GPIOEXT33, GPIOEXT34, GPIOEXT35, GPIOEXT36, GPIOEXT37, GPIOEXT38, GPIOEXT39,
    GPIOEXT40,
#endif

    GPIO_MAX
}GPIO_PIN;         

#define MAX_GPIO_PIN    (GPIO_MAX) 

#ifdef MTK_PMIC_MT6397
#define GPIO_EXTEND_START GPIOEXT0
typedef enum GPIO_PIN_EXT
{    
    GPIO143 = GPIO_EXTEND_START,
	GPIO144, GPIO145, GPIO146, GPIO147, GPIO148, GPIO149, GPIO150, GPIO151,
	GPIO152, GPIO153, GPIO154, GPIO155, GPIO156, GPIO157, GPIO158, GPIO159,
	GPIO160, GPIO161, GPIO162, GPIO163, GPIO164, GPIO165, GPIO166, GPIO167,
	GPIO168, GPIO169, GPIO170, GPIO171, GPIO172, GPIO173, GPIO174, GPIO175,
	GPIO176, GPIO177, GPIO178, GPIO179,	GPIO180, GPIO181, GPIO182, GPIO183,
	GPIO_EXT_MAX
}GPIO_PIN_EXT;
#endif

/******************************************************************************
* Enumeration for Clock output
******************************************************************************/
/* GPIO MODE CONTROL VALUE*/
typedef enum {
    GPIO_MODE_UNSUPPORTED = -1,
    GPIO_MODE_GPIO  = 0,
    GPIO_MODE_00    = 0,
    GPIO_MODE_01    = 1,
    GPIO_MODE_02    = 2,
    GPIO_MODE_03    = 3,
    GPIO_MODE_04    = 4,
    GPIO_MODE_05    = 5,
    GPIO_MODE_06    = 6,
    GPIO_MODE_07    = 7,

    GPIO_MODE_MAX,
    GPIO_MODE_DEFAULT = GPIO_MODE_01,
} GPIO_MODE;
/*----------------------------------------------------------------------------*/
/* GPIO DIRECTION */
typedef enum {
    GPIO_DIR_UNSUPPORTED = -1,
    GPIO_DIR_IN     = 0,
    GPIO_DIR_OUT    = 1,

    GPIO_DIR_MAX,
    GPIO_DIR_DEFAULT = GPIO_DIR_IN,
} GPIO_DIR;
/*----------------------------------------------------------------------------*/
/* GPIO PULL ENABLE*/
typedef enum {
    GPIO_PULL_EN_UNSUPPORTED = -1,
    GPIO_PULL_DISABLE = 0,
    GPIO_PULL_ENABLE  = 1,

    GPIO_PULL_EN_MAX,
    GPIO_PULL_EN_DEFAULT = GPIO_PULL_ENABLE,
} GPIO_PULL_EN;
/*----------------------------------------------------------------------------*/
/* GPIO PULL-UP/PULL-DOWN*/
typedef enum {
    GPIO_PULL_UNSUPPORTED = -1,
    GPIO_PULL_DOWN  = 0,
    GPIO_PULL_UP    = 1,

    GPIO_PULL_MAX,
    GPIO_PULL_DEFAULT = GPIO_PULL_DOWN
} GPIO_PULL;
/*----------------------------------------------------------------------------*/
#ifdef MTK_PMIC_MT6397
/* GPIO INVERSION */
typedef enum {
    GPIO_DATA_INV_UNSUPPORTED = -1,
    GPIO_DATA_UNINV = 0,
    GPIO_DATA_INV   = 1,

    GPIO_DATA_INV_MAX,
    GPIO_DATA_INV_DEFAULT = GPIO_DATA_UNINV
} GPIO_INVERSION;
#endif

/*----------------------------------------------------------------------------*/
/* GPIO OUTPUT */
typedef enum {
    GPIO_OUT_UNSUPPORTED = -1,
    GPIO_OUT_ZERO = 0,
    GPIO_OUT_ONE  = 1,

    GPIO_OUT_MAX,
    GPIO_OUT_DEFAULT = GPIO_OUT_ZERO,
    GPIO_DATA_OUT_DEFAULT = GPIO_OUT_ZERO,  /*compatible with DCT*/
} GPIO_OUT;
/* GPIO INPUT */
typedef enum {
    GPIO_IN_UNSUPPORTED = -1,
    GPIO_IN_ZERO = 0,
    GPIO_IN_ONE  = 1,

    GPIO_IN_MAX,
} GPIO_IN;
/*CLOCK OUT
typedef enum {
    CLK_OUT_UNSUPPORTED = -1,
    CLK_OUT0,
    CLK_OUT1,
    CLK_OUT2,
    CLK_OUT3,
    CLK_OUT4,
    CLK_OUT5,
    CLK_MAX	
}GPIO_CLKOUT;

typedef enum CLK_SRC
{
    CLK_SRC_UNSUPPORTED = -1,	
    CLK_SRC_F32K 	= 0x2,
    CLK_SRC_F26M 	= 0x4,
    CLK_SRC_FAXI0 	= 0x6,
    CLK_SRC_FAXI1 	= 0x8,
    CLK_SRC_FAXI2,
    CLK_SRC_FAXI3,
    CLK_SRC_FAXI4,
    CLK_SRC_DPI_PIXELPLL,
    CLK_SRC_3GPLL_123M,
    CLK_SRC_FMM,

    CLK_SRC_MAX
}GPIO_CLKSRC;*/
    
/* GPIO POWER*/
typedef enum {
    GPIO_VIO28 = 0,
    GPIO_VIO18 = 1,

    GPIO_VIO_MAX,
} GPIO_POWER;
/*----------------------------------------------------------------------------*/
typedef struct {    
    u16 val;        
    u16 _align1;
    u16 set;
    u16 _align2;
    u16 rst;
    u16 _align3[3];
} VAL_REGS; /*8bytes x 2*/
/*----------------------------------------------------------------------------*/
typedef struct {
    VAL_REGS    dir[9];              /*0x0000 ~ 0x008F: 144 bytes*/
    unsigned char       rsv00[112];  /*0x0090 ~ 0x00FF: 112 bytes*/
    VAL_REGS    pullen[9];           /*0x0100 ~ 0x018F: 144 bytes*/
    unsigned char       rsv01[112];  /*0x0190 ~ 0x01FF: 112 bytes*/
    VAL_REGS    pullsel[9];          /*0x0200 ~ 0x028F: 144 bytes*/
    unsigned char       rsv02[112];  /*0x0290 ~ 0x02FF: 112 bytes*/
    unsigned char       rsv03[256];  /*0x0300 ~ 0x03FF: 256 bytes*/
    VAL_REGS    dout[9];             /*0x0400 ~ 0x048F: 144 bytes*/
    unsigned char       rsv04[112];  /*0x0490 ~ 0x04FF: 112 bytes*/
    VAL_REGS    din[9];              /*0x0500 ~ 0x058F: 144 bytes*/
    unsigned char       rsv05[112];	 /*0x0590 ~ 0x05FF: 112 bytes*/
    VAL_REGS    mode[29];            /*0x0600 ~ 0x07CF: 464 bytes*/
	unsigned char		rsv06[304];	 /*0x07D0 ~ 0x08FF: 304 bytes*/
	VAL_REGS    ies[2];            	 /*0x0900 ~ 0x091F:  32 bytes*/
    VAL_REGS    smt[2];        		 /*0x0920 ~ 0x093F:	 32 bytes*/
	unsigned char		rsv07[192];	 /*0x0940 ~ 0x09FF: 192 bytes*/
	VAL_REGS    tdsel[5];        	 /*0x0A00 ~ 0x0A4F:  80 bytes*/
	unsigned char		rsv08[48];	 /*0x0A50 ~ 0x0A7F:  48 bytes*/
	VAL_REGS    rdsel[4];        	 /*0x0A80 ~ 0x0ABF:  64 bytes*/
	unsigned char		rsv09[64];	 /*0x0AC0 ~ 0x0AFF:  64 bytes*/
	VAL_REGS    drv_mode[8];         /*0x0B00 ~ 0x0B7F: 128 bytes*/
	unsigned char		rsv10[128];	 /*0x0B80 ~ 0x0BFF: 128 bytes*/
	VAL_REGS    msdc0_ctrl[7];       /*0x0C00 ~ 0x0C6F:	112 bytes*/
	VAL_REGS    msdc1_ctrl[6];       /*0x0C70 ~ 0x0CCF:	 96 bytes*/
	VAL_REGS    msdc2_ctrl[6];       /*0x0CD0 ~ 0x0D2F:	 96 bytes*/
	VAL_REGS    tm;                  /*0x0D30 ~ 0x0D3F:	 16 bytes*/
	VAL_REGS    usb;                 /*0x0D40 ~ 0x0D4F:	 16 bytes*/
	VAL_REGS    odd33_ctrl[3];       /*0x0D50 ~ 0x0D7F:	 48 bytes*/
	unsigned char		rsv11[16];	 /*0x0D80 ~ 0x0D8F:  16 bytes*/
	VAL_REGS    kpad_ctrl[2];        /*0x0D90 ~ 0x0DAF:	 32 bytes*/
	VAL_REGS    eint_ctrl[2];        /*0x0DB0 ~ 0x0DCF:	 32 bytes*/
	unsigned char		rsv12[80];	 /*0x0DD0 ~ 0x0E1F:  80 bytes*/
	VAL_REGS    bias_ctrl[2];        /*0x0E20 ~ 0x0E3F:	 32 bytes*/
} GPIO_REGS;
/*----------------------------------------------------------------------------*/

#ifdef MTK_PMIC_MT6397
typedef struct {
    u16 val;
    u16 set;
    u16 rst;
    u16 _align;
} EXT_VAL_REGS;
typedef struct {
    EXT_VAL_REGS    dir[4];            /*0x0000 ~ 0x001F: 32 bytes*/
    EXT_VAL_REGS    pullen[4];         /*0x0020 ~ 0x003F: 32 bytes*/
    EXT_VAL_REGS    pullsel[4];        /*0x0040 ~ 0x005F: 32 bytes*/
    EXT_VAL_REGS    dinv[4];           /*0x0060 ~ 0x007F: 32 bytes*/
    EXT_VAL_REGS    dout[4];           /*0x0080 ~ 0x009F: 32 bytes*/
    EXT_VAL_REGS    din[4];            /*0x00A0 ~ 0x00BF: 32 bytes*/
    EXT_VAL_REGS    mode[10];          /*0x00C0 ~ 0x010F: 80 bytes*/
} GPIOEXT_REGS;
#endif

typedef struct {
    unsigned int no     : 16;
    unsigned int mode   : 3;    
    unsigned int pullsel: 1;
    unsigned int din    : 1;
    unsigned int dout   : 1;
    unsigned int pullen : 1;
    unsigned int dir    : 1;
    unsigned int dinv   : 1;
    unsigned int _align : 7; 
} GPIO_CFG; 
/******************************************************************************
* GPIO Driver interface 
******************************************************************************/
/*direction*/
s32 mt_set_gpio_dir(u32 pin, u32 dir);
s32 mt_get_gpio_dir(u32 pin);

/*pull enable*/
s32 mt_set_gpio_pull_enable(u32 pin, u32 enable);
s32 mt_get_gpio_pull_enable(u32 pin);
/*pull select*/
s32 mt_set_gpio_pull_select(u32 pin, u32 select);    
s32 mt_get_gpio_pull_select(u32 pin);

/*data inversion
s32 mt_set_gpio_inversion(u32 pin, u32 enable);
s32 mt_get_gpio_inversion(u32 pin);*/

/*input/output*/
s32 mt_set_gpio_out(u32 pin, u32 output);
s32 mt_get_gpio_out(u32 pin);
s32 mt_get_gpio_in(u32 pin);

/*mode control*/
s32 mt_set_gpio_mode(u32 pin, u32 mode);
s32 mt_get_gpio_mode(u32 pin);

/*clock output setting*/
s32 mt_set_clock_output(u32 num, u32 src, u32 div);
s32 mt_get_clock_output(u32 num, u32 *src, u32 *div);

/*misc functions for protect GPIO*/
//void mt_gpio_unlock_init(int all);
void mt_gpio_set_default(void);
void mt_gpio_dump(void);
void mt_gpio_load(GPIO_REGS *regs);
void mt_gpio_checkpoint_save(void);
void mt_gpio_checkpoint_compare(void);


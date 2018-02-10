#include <common.h>
#include <config.h>
#include <command.h>
#include <image.h>

#if defined(ON_BOARD_NAND_FLASH_COMPONENT)
#include <asm/arch/nand/partition_define.h>
#endif

#define PART_MAGIC          0x58881688

int do_read_image_blks (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    unsigned long blk_size;
    unsigned long img_blks;
	char *endp;
    image_header_t *img_hdr = (image_header_t *)load_addr;
    unsigned long img_size = 0;
    char s[32];

	if ((argc < 2) || (argc > 3))
		goto usage;

    blk_size = simple_strtoul(argv[1], &endp, 10);
    if (blk_size < 1)
    {
        printf("Block Size should be positive numble.\n");
        return 0;
    }
    
    if (argc == 3)
    {
        img_size = simple_strtoul(argv[2], &endp, 16);

        if (img_size < 1)
        {
            printf("Image Size should be positive numble.\n");
            return 0;
        }
    }

    if ( uimage_to_cpu(img_hdr->ih_magic) != IH_MAGIC )
    {
#if defined (CONFIG_MTK_MTD_NAND)		
	 	if (*((unsigned long*)load_addr) != PART_MAGIC)
#endif
		{	
			printf("Bad Magic Number.\n");
			sprintf((char*) s,"%lX", (unsigned long)0);
			setenv("img_blks", (const char *)s);
#if defined (CONFIG_MTK_MTD_NAND)
			sprintf((char*) s,"%lX", (unsigned long)0);
			setenv("img_align_size", (const char *)s);
#endif

			return 0;
		}
#if defined (CONFIG_MTK_MTD_NAND)		
		else
			img_size = getenv_ulong("filesize", 16, 0);
#endif
    }
	else
    {   
        /* 
         * If img_size is large than 0, it specifies that user assigns a image size.
         * Or we read image size from image header.
         */
        if (img_size == 0)
        {
            img_size = uimage_to_cpu(img_hdr->ih_size)+sizeof(image_header_t);
        }
    }
    
	img_blks = (img_size + (blk_size - 1))/blk_size;

    sprintf((char*) s,"%lX", img_blks);
    setenv("img_blks", (const char *)s);
#if defined (CONFIG_MTK_MTD_NAND)
	sprintf((char*) s,"%lX", img_blks*blk_size);
	setenv("img_align_size", (const char *)s);
#endif

    return 0;

usage:
	return CMD_RET_USAGE;    
}

int do_image_check (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    image_header_t *img_hdr = (image_header_t *)load_addr;
    char s[32];
    
    if ( uimage_to_cpu(img_hdr->ih_magic) != IH_MAGIC )
    {
        printf("Bad Magic Number.\n");        
        sprintf((char*) s, "%s", "bad");        
        setenv("img_result", (const char *)s);

        return 0;
    }

    sprintf((char*) s, "%s", "good");        
    setenv("img_result", (const char *)s);    

    return 0;
}

int do_uboot_check (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    unsigned long magic_no;    
    char s[32];
    
    magic_no = *(unsigned long*)load_addr;
    if ( magic_no != PART_MAGIC )
    {
        printf("Bad Magic Number.\n");        
        sprintf((char*) s, "%s", "bad");        
        setenv("uboot_result", (const char *)s);

        return 0;
    }

    sprintf((char*) s, "%s", "good");        
    setenv("uboot_result", (const char *)s);

    return 0;
}

U_BOOT_CMD(
	image_blks, 3,	1, do_read_image_blks,
	"read image size from img_size or image header if no specifying img_size, and divided by blk_size and save image blocks in image_blks variable.",
	"[blk_size]\n"
	"   - specify block size in decimal byte.\n"
	"[img_size]\n"
	"   - specify image size in hex byte."	
);

U_BOOT_CMD(
	image_check, 2,	1, do_image_check,
	"check if image in load_addr is normal.",
	""
);

U_BOOT_CMD(
	uboot_check, 2,	1, do_uboot_check,
	"check if uboot in load_addr is normal.",
	""
);



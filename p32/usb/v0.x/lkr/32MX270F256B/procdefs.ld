/***********************************************************************
 * PIC32MX270F256B object file
 * Contains Memory Regions definitions
 * 2014-09-10 Régis Blanchot
 **********************************************************************/

INPUT("processor.o")

/***********************************************************************
 * For interrupt vector handling
 * base address of interrupt vector table (length = 0x1000)
 * placed at the end of flash memory (0x40000 - 0x1000 = 0x3F000)
 * _ebase_address value MUST BE same as the ORIGIN value of exception_mem
 **********************************************************************/

PROVIDE(_vector_spacing = 0x00000001);

_ebase_address = 0x9D03F000;

/***********************************************************************
 * Memory Address Equates
 * _RESET_ADDR      -- Reset Vector
 * _BEV_EXCPT_ADDR  -- Boot exception Vector
 * _DBG_EXCPT_ADDR  -- In-circuit Debugging Exception Vector
 * _DBG_CODE_ADDR   -- In-circuit Debug Executive address
 * _DBG_CODE_SIZE   -- In-circuit Debug Executive size
 * _GEN_EXCPT_ADDR  -- General Exception Vector
 **********************************************************************/

/***********************************************************************
 * _RESET_ADDR value MUST BE same as the ORIGIN value of kseg1_boot_mem
 * When the PIC is reset, it goes to the reset address 0xBFC00000,
 * which is the location of the boot flash, and executes the code there:
 * crt0.S which takes care of some initialization tasks,
 * then calls the code for the bootloader,
 * which typically resides in the KSEG0 program flash (kseg0_boot_mem) memory block. 
 **********************************************************************/

_RESET_ADDR              = 0xBFC00000;

/***********************************************************************
 * map boot exception vector to kseg1_boot_mem
 * Place _BEV_EXCPT_ADDR at an offset of 0x380 to _RESET_ADDR
 **********************************************************************/

_BEV_EXCPT_ADDR          = 0xBFC00000 + 0x380;

/***********************************************************************
 * map _DBG_EXCPT_ADDR to kseg1_boot_mem
 * Place _DBG_EXCPT_ADDR at an offset of 0x480 to _RESET_ADDR
 * we don't use debug supporting code
 **********************************************************************/

_DBG_EXCPT_ADDR          = 0xBFC00000 + 0x480;
_DBG_CODE_ADDR           = 0x9FC00000 + 0x490;
_DBG_CODE_SIZE           = 0x560;

/***********************************************************************
 * 
 **********************************************************************/

_GEN_EXCPT_ADDR          = _ebase_address + 0x180;

/***********************************************************************
 * Memory Regions
 ***********************************************************************
 * - exception_mem  contains interrupt vector table starting at ebase (cacheable)
 * - kseg1_boot_mem contains reset vector, bootstrap exception handler, debug exception handler (non-cacheable)
 * - kseg0_boot_mem contains C startup module (cacheable)
 * - debug_exec_mem containing debug supporting code for running ICD3/RealICE during a debug session (non-cacheable)
 * - configuration words (non-cacheable)
 ***********************************************************************
 * Cacheable (KSEG0) and non-cacheable (KSEG1)
 * KSEG0 PROGRAM FLASH [0x9D000000:0x9D03FFFF]
 * KSEG1 PROGRAM FLASH [0xBD000000:0xBD03FFFF]
 * KSEG0    BOOT FLASH [0x9FC00000:0x9FC00BEF]
 * KSEG1    BOOT FLASH [0xBFC00000:0xBFC00BEF]
 * KSEG1           RAM [0xA0000000:0xA000FFFF]
 **********************************************************************/

MEMORY
{

/***********************************************************************
 * interrupt vector table in PROGRAM FLASH starting at ebase
 * exception_mem value MUST BE same as value of _ebase_address
 **********************************************************************/

exception_mem              : ORIGIN = 0x9D03F000, LENGTH = 0x1000

/***********************************************************************
 * Startup code (about 0x200 bytes long)
 * in cacheable BOOT FLASH
 * only 0xBF0 bytes available on PIC32MX2xx
 * 0xBF0 - 0x490 (Reset, Boot & Debug vectors) = 0x760
 **********************************************************************/

kseg0_boot_mem             : ORIGIN = 0x9FC00490, LENGTH = 0x200

/***********************************************************************
 * Reset (0x0), Boot (0x380) and Debug (0x480) vectors
 * kseg1_boot_mem value MUST BE same as the _RESET_ADDR value 
 * in non-cacheable BOOT FLASH
 * vector's length = 0x10 ?
 **********************************************************************/

kseg1_boot_mem             : ORIGIN = 0xBFC00000, LENGTH = 0x490

/***********************************************************************
 * User Program code
 * in cacheable PROGRAM FLASH
 * from 0x9D000000 to 0x9D040000 (PIC32MX270F256B = 256 KB flash = 0x40000) 
 * Bootloader is placed between 0x9D000000 and 0x9D005000
 * Interrupt Vector Table is placed between 0x9D03F000 to 0x9D040000
 * 0x3F000 - 0x0000 = 0x3F000 bytes free
 **********************************************************************/

kseg0_program_mem    (rx)  : ORIGIN = 0x9D000000, LENGTH = 0x3F000

/***********************************************************************
 * we don't use debug supporting code
 **********************************************************************/

debug_exec_mem             : ORIGIN = 0x9FC00490, LENGTH = 0x560

/***********************************************************************
 * Device Configuration Registers (configuration bits)
 **********************************************************************/

config3                    : ORIGIN = 0xBFC00BF0, LENGTH = 0x4
config2                    : ORIGIN = 0xBFC00BF4, LENGTH = 0x4
config1                    : ORIGIN = 0xBFC00BF8, LENGTH = 0x4
config0                    : ORIGIN = 0xBFC00BFC, LENGTH = 0x4
configsfrs                 : ORIGIN = 0xBFC00BF0, LENGTH = 0x10

/***********************************************************************
 * all SFRS
 **********************************************************************/

sfrs                       : ORIGIN = 0xBF800000, LENGTH = 0x100000

/***********************************************************************
 * PIC32MX270F256B has 64 KB RAM, or 0x10000
 **********************************************************************/

kseg1_data_mem       (w!x) : ORIGIN = 0xA0000000, LENGTH = 64K

}

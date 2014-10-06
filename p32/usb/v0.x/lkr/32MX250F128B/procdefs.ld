/*************************************************************************
 * PIC32MX250F128B object file
 * Contains Memory Regions definitions
 * 2014-09-10 Régis Blanchot
 *************************************************************************/

INPUT("processor.o")

/*************************************************************************
 * For interrupt vector handling
 *************************************************************************/

PROVIDE(_vector_spacing = 0x00000001);

/* base address of interrupt vector table */
_ebase_address = 0x9D000000;

/*************************************************************************
 * Memory Address Equates
 * _RESET_ADDR      -- Reset Vector
 * _BEV_EXCPT_ADDR  -- Boot exception Vector
 * _DBG_EXCPT_ADDR  -- In-circuit Debugging Exception Vector
 * _DBG_CODE_ADDR   -- In-circuit Debug Executive address
 * _DBG_CODE_SIZE   -- In-circuit Debug Executive size
 * _GEN_EXCPT_ADDR  -- General Exception Vector
 *************************************************************************/

/* match reset address to ORIGIN of kseg1_boot_mem */
_RESET_ADDR              = 0xBFC00000;

/* map _BEV_EXCPT_ADDR to kseg1_boot_mem */ 
/* we don't use boot exception vector
_BEV_EXCPT_ADDR          = (0x9D000000 + 0x1000 + 0x970 + 0x380);
*/

/* map _DBG_EXCPT_ADDR to kseg1_boot_mem */ 
/* we don't use debug supporting code
_DBG_EXCPT_ADDR          = (0xBD000000 + 0x1000 + 0x970 + 0x480);
_DBG_CODE_ADDR           = 0x9FC00490;
_DBG_CODE_SIZE           = 0x760     ;
*/

_GEN_EXCPT_ADDR          = _ebase_address + 0x180;

/*************************************************************************
 * Memory Regions
 *************************************************************************
 * NOTE1:  What is called boot_mem and program_mem below do not directly
 * correspond to boot flash and program flash.  For instance, here 
 * kseg0_boot_mem and kseg1_boot_mem both live in program flash memory.
 * (We leave the boot flash solely to the bootloader.)
 * The boot_mem names below tell the linker where the startup codes should 
 * go (here, in program flash).  The first 0x1000 + 0x970 + 0x490 = 0x1E00
 * of program flash memory is allocated to the interrupt vector table and
 * startup codes.  The remaining 0x1DD50 is allocated to the user's program.
 *************************************************************************
 * NOTE2 : The internal boot flash memory is divided into several regions.
 * It is usually too small for any meaningful bootloader firmware.
 * - exception_mem  contains interrupt vector table starting at ebase (cacheable)
 * - kseg1_boot_mem contains reset vector, bootstrap exception handler, debug exception handler (non-cacheable)
 * - kseg0_boot_mem contains C startup module (cacheable)
 * - debug_exec_mem containing debug supporting code for running ICD3/RealICE during a debug session (non-cacheable)
 * - configuration words (non-cacheable)
 *************************************************************************
 * Cacheable (KSEG0) and non-cacheable (KSEG1)
 * KSEG0 PROGRAM FLASH [0x9D000000:0x9D01FFFF]
 * KSEG1 PROGRAM FLASH [0xBD000000:0xBD01FFFF]
 * KSEG0    BOOT FLASH [0x9FC00000:0x9FC00BEF]
 * KSEG1    BOOT FLASH [0xBFC00000:0xBFC00BEF]
 * KSEG1           RAM [0xA0000000:0xA0007FFF]
 *************************************************************************/
MEMORY
{
  /* interrupt vector table in PROGRAM FLASH starting at ebase */
  /* 64 vectors * 4 bytes = 256 */
  exception_mem              : ORIGIN = 0x9D000000, LENGTH = 0x100

  /* Startup code in BOOT FLASH, only 0xBEF bytes available */
  kseg0_boot_mem             : ORIGIN = 0x9FC00000, LENGTH = 0xBEF

  /* Reset vector in BOOT FLASH, length ? */
  kseg1_boot_mem             : ORIGIN = 0xBFC00000, LENGTH = 0xBEF

  /* Bootloader is in PROGRAM FLASH, kseg0_program_mem, all cacheable */
  /* PIC32MX250F128B = 128 KB flash = 0x20000 */
  kseg0_program_mem    (rx)  : ORIGIN = 0x9D000000, LENGTH = 128K
  
  /* we don't use debug supporting code
  debug_exec_mem             : ORIGIN = 0x9FC00490, LENGTH = 0x760
  */

  /* Device Configuration Registers (configuration bits) */
  config3                    : ORIGIN = 0xBFC00BF0, LENGTH = 0x4
  config2                    : ORIGIN = 0xBFC00BF4, LENGTH = 0x4
  config1                    : ORIGIN = 0xBFC00BF8, LENGTH = 0x4
  config0                    : ORIGIN = 0xBFC00BFC, LENGTH = 0x4
  configsfrs                 : ORIGIN = 0xBFC00BF0, LENGTH = 0x10

  /* all SFRS */
  sfrs                       : ORIGIN = 0xBF800000, LENGTH = 0x100000

  /* PIC32MX250F128B has 32 KB RAM, or 0x8000 */
  kseg1_data_mem       (w!x) : ORIGIN = 0xA0000000, LENGTH = 32K

}

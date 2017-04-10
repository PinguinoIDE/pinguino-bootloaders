/**********************************************************************
 * PIC32MX470F512H object file
 * Contains Memory Regions definitions
 * Project : www.pinguino.cc
 * Last update : 2017-03-31
 * Contact : Régis Blanchot <rblanchot@gmail.com>
 **********************************************************************/

INPUT("processor.o")
OUTPUT_FORMAT("elf32-littlemips")
OUTPUT_ARCH(mips)
ENTRY(_reset)

/**********************************************************************
 * Memory Address Equates
 * _ebase_address   -- base address of interrupt vector table
 * _RESET_ADDR      -- Reset Vector
 * _GEN_EXCPT_ADDR  -- General Exception Vector
 * _RESET_ADDR value MUST BE same as the ORIGIN value of kseg1_boot_mem
 * _ebase_address value MUST BE same as the ORIGIN value of exception_mem
 * When the PIC is reset, it goes to the reset address (0xBFC00000),
 * and executes crt0.S
 **********************************************************************/

_ebase_address           = 0x9D002000;
_RESET_ADDR              = 0xBFC00000;

/**********************************************************************
 * Memory Regions
 **********************************************************************
 * - exception_mem  contains interrupt vector table starting at ebase (cacheable)
 * - kseg1_boot_mem contains reset vector, bootstrap exception handler,
 *   debug exception handler (non-cacheable)
 * - kseg0_boot_mem contains C startup module (cacheable)
 * - debug_exec_mem containing debug supporting code for running ICD3/RealICE
 *   during a debug session (non-cacheable)
 * - configuration words (non-cacheable)
 **********************************************************************
 * Cacheable (KSEG0) and non-cacheable (KSEG1)
 * KSEG0 PROGRAM FLASH [0x9D000000:0x9D03FFFF]
 * KSEG1 PROGRAM FLASH [0xBD000000:0xBD03FFFF]
 * KSEG0    BOOT FLASH [0x9FC00000:0x9FC00BEF]
 * KSEG1    BOOT FLASH [0xBFC00000:0xBFC00BEF]
 * KSEG1           RAM [0xA0000000:0xA000FFFF]
 **********************************************************************/

MEMORY
{
    /**********************************************************************
     * Bootloader code
     **********************************************************************/

    kseg0_program_mem    (rx)  : ORIGIN = 0x9D000000, LENGTH = 0x2000


    /**********************************************************************
     * Interrupt vector table (exception_mem = _ebase_address)
     **********************************************************************/

    exception_mem              : ORIGIN = 0x9D002000, LENGTH = 0xA00

    /**********************************************************************
     * Reset Vector (_RESET_ADDR = kseg1_boot_mem)
     **********************************************************************/

    kseg1_boot_mem             : ORIGIN = 0xBFC00000, LENGTH = 0x10

    /**********************************************************************
     * Startup code (about 0x200 bytes long)
     **********************************************************************/

    kseg0_boot_mem             : ORIGIN = 0x9FC00010, LENGTH = 0x200

    /**********************************************************************
     * PIC32MX470F512H has 128 KB RAM, or 0x20000
     **********************************************************************/

    boot_software_key          : ORIGIN = 0xA0000000, LENGTH = 0x4
    kseg1_data_mem       (w!x) : ORIGIN = 0xA0000004, LENGTH = 0x1FFFC

    /**********************************************************************
     * Device Configuration Registers (configuration bits)
     **********************************************************************/

    devcfg3                    : ORIGIN = 0xBFC00BF0, LENGTH = 0x4
    devcfg2                    : ORIGIN = 0xBFC00BF4, LENGTH = 0x4
    devcfg1                    : ORIGIN = 0xBFC00BF8, LENGTH = 0x4
    devcfg0                    : ORIGIN = 0xBFC00BFC, LENGTH = 0x4
    devcfg                     : ORIGIN = 0xBFC00BF0, LENGTH = 0x10

    /**********************************************************************
     * all SFRS
     **********************************************************************/

    sfrs                       : ORIGIN = 0xBF800000, LENGTH = 0x100000
}

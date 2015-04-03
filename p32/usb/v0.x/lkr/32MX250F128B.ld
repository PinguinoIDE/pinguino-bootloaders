/**********************************************************************
 * PIC32MX250F128B object file
 * Contains Memory Regions definitions
 * 2015-03-27 Régis Blanchot
 **********************************************************************/

INPUT("processor.o")
OUTPUT_FORMAT("elf32-littlemips")
OUTPUT_ARCH(mips)
ENTRY(_reset)

/**********************************************************************
 * Provide for a minimum stack and heap size
 * - _min_stack_size - represents the minimum space that must be made
 *                     available for the stack.  Can be overridden from
 *                     the command line using the linker's --defsym option.
 * - _min_heap_size  - represents the minimum space that must be made
 *                     available for the heap.  Can be overridden from
 *                     the command line using the linker's --defsym option.
 **********************************************************************/

EXTERN (_min_stack_size _min_heap_size)
PROVIDE(_min_stack_size = 0x400) ;
PROVIDE(_min_heap_size = 0) ;
PROVIDE(_vector_spacing = 0x00000001);

/**********************************************************************
 * For interrupt vector handling
 * base address of interrupt vector table (length = 0x1000)
 * placed at the end of flash memory (0x40000 - 0x1000 = 0x3F000)
 * _ebase_address value MUST BE same as the ORIGIN value of exception_mem
 **********************************************************************/

/**********************************************************************
 * Memory Address Equates
 * _RESET_ADDR      -- Reset Vector
 * _GEN_EXCPT_ADDR  -- General Exception Vector
 * _RESET_ADDR value MUST BE same as the ORIGIN value of kseg1_boot_mem
 * When the PIC is reset, it goes to the reset address 0xBFC00000,
 * and executes crt0.S
 **********************************************************************/

_ebase_address           = 0x9D003000;
_RESET_ADDR              = 0xBFC00000;


/**********************************************************************
 * Memory Regions
 **********************************************************************
 * - exception_mem  contains interrupt vector table starting at ebase (cacheable)
 * - kseg1_boot_mem contains reset vector, bootstrap exception handler, debug exception handler (non-cacheable)
 * - kseg0_boot_mem contains C startup module (cacheable)
 * - debug_exec_mem containing debug supporting code for running ICD3/RealICE during a debug session (non-cacheable)
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
     * Bootloader Interrupt Vector Table is placed between 0x9D000000 to 0x9D001000
     **********************************************************************/

    kseg0_program_mem    (rx)  : ORIGIN = 0x9D000000, LENGTH = 0x3000

    /**********************************************************************
     * interrupt vector table in PROGRAM FLASH starting at ebase
     * exception_mem value MUST BE same as value of _ebase_address
     **********************************************************************/

    exception_mem              : ORIGIN = 0x9D003000, LENGTH = 0xA00

    /**********************************************************************
     * kseg1_boot_mem value MUST BE same as the _RESET_ADDR value
     * bfc00000:    c09f1a3c     lui    k0,0x9fc0
     * bfc00004:    10005a27     addiu  k0,k0,16
     * bfc00008:    08004003     jr     k0
     * bfc0000c:    00000000     nop
     **********************************************************************/

    kseg1_boot_mem             : ORIGIN = 0xBFC00000, LENGTH = 0x10

    /**********************************************************************
     * Startup code (about 0x200 bytes long)
     * in cacheable BOOT FLASH
     **********************************************************************/

    kseg0_boot_mem             : ORIGIN = 0x9FC00010, LENGTH = 0x200

    /**********************************************************************
     * PIC32MX270F256B has 64 KB RAM, or 0x10000
     **********************************************************************/

    boot_software_key          : ORIGIN = 0xA0000000, LENGTH = 0x4
    kseg1_data_mem       (w!x) : ORIGIN = 0xA0000004, LENGTH = 0x7FFC

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

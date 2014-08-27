/*************************************************************************
 * Processor-specific object file.  Contains SFR definitions.
 * linker file for PIC32MX250F128B with Pinguino USB bootloader
 * Régis Blanchot
 *************************************************************************/
INPUT("processor.o")

/*************************************************************************
 * For interrupt vector handling
 *************************************************************************/
PROVIDE(_vector_spacing = 0x00000001);
_ebase_address  = 0x9D003000;

/*************************************************************************
 * Memory Address Equates
 * _RESET_ADDR                    -- Reset Vector
 * _BEV_EXCPT_ADDR                -- Boot exception Vector
 * _DBG_EXCPT_ADDR                -- In-circuit Debugging Exception Vector
 * _DBG_CODE_ADDR                 -- In-circuit Debug Executive address
 * _DBG_CODE_SIZE                 -- In-circuit Debug Executive size
 * _GEN_EXCPT_ADDR                -- General Exception Vector
 
_RESET_ADDR              = 0x9D004000;
_BEV_EXCPT_ADDR          = 0x9D004380;
_DBG_EXCPT_ADDR          = 0x9D004480;
_DBG_CODE_ADDR           = 0xBFC02000;
 *************************************************************************/
_RESET_ADDR                    = 0x9D004000;
_BEV_EXCPT_ADDR                = 0x9D004380;
_GEN_EXCPT_ADDR                = _ebase_address + 0x180;

/*************************************************************************
 * Memory Regions
 *
 * Memory regions without attributes cannot be used for orphaned sections.
 * Only sections specifically assigned to these regions can be allocated
 * into these regions.
 *
 * The internal boot flash memory is divided into several regions.
 * After all those allocations, it is usually too small for any meaningful bootloader firmware.
 * - exception_mem  contains interrupt vector table starting at ebase (cacheable)
 * - kseg1_boot_mem contains reset vector, bootstrap exception handler, debug exception handler (non-cacheable)
 * - kseg0_boot_mem contains C startup module (cacheable)
 * - debug_exec_mem containing debug supporting code for running ICD3/RealICE during a debug session (non-cacheable)
 * - configuration words (non-cacheable)

    kseg0_program_mem    (rx)   : ORIGIN = 0x9D004A00, LENGTH = 0x1B5FF
    exception_mem               : ORIGIN = 0x9D003000, LENGTH = 0x1000
    kseg1_boot_mem              : ORIGIN = 0x9D004000, LENGTH = 0x490
    kseg0_boot_mem              : ORIGIN = 0x9D004490, LENGTH = 0x570
    debug_exec_mem              : ORIGIN = 0xBFC02000, LENGTH = 0xFF0
 *************************************************************************/
MEMORY
{
    kseg0_program_mem    (rx)   : ORIGIN = 0x9D004C00, LENGTH = 0x1B3FF
    exception_mem               : ORIGIN = 0x9D003000, LENGTH = 0x1000
    kseg1_boot_mem              : ORIGIN = 0x9D004000, LENGTH = 0x400
    kseg0_boot_mem              : ORIGIN = 0x9D004400, LENGTH = 0x800
    configsfrs                  : ORIGIN = 0xBFC00BF0, LENGTH = 0x10
    config3                     : ORIGIN = 0xBFC00BF0, LENGTH = 0x4
    config2                     : ORIGIN = 0xBFC00BF4, LENGTH = 0x4
    config1                     : ORIGIN = 0xBFC00BF8, LENGTH = 0x4
    config0                     : ORIGIN = 0xBFC00BFC, LENGTH = 0x4
    kseg1_data_mem       (w!x)  : ORIGIN = 0xA0000000, LENGTH = 0x8000
    sfrs                        : ORIGIN = 0xBF800000, LENGTH = 0x100000
}

/*************************************************************************
 * Configuration-word sections. Map the config-pragma input sections to
 * absolute-address output sections.
 *************************************************************************/

SECTIONS
{
    .config_BFC00BF0 : {
    KEEP(*(.config_BFC00BF0))
    } > config3
    .config_BFC00BF4 : {
    KEEP(*(.config_BFC00BF4))
    } > config2
    .config_BFC00BF8 : {
    KEEP(*(.config_BFC00BF8))
    } > config1
    .config_BFC00BFC : {
    KEEP(*(.config_BFC00BFC))
    } > config0
}

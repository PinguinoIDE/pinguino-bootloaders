/*************************************************************************
 * Processor-specific object file.  Contains SFR definitions.
 *************************************************************************/
INPUT("processor.o")

/*************************************************************************
 * For interrupt vector handling
 *************************************************************************/
PROVIDE(_vector_spacing = 0x00000001);
_ebase_address  = 0x9D000000;

/*************************************************************************
 * Memory Address Equates
 *************************************************************************/
_RESET_ADDR              = 0x9D001000;
_BEV_EXCPT_ADDR          = 0x9D001380;
_DBG_EXCPT_ADDR          = 0x9D001480;
/*
_DBG_CODE_ADDR           = 0xBFC02000;
_DBG_CODE_SIZE           = 0xFF0     ;
*/
_GEN_EXCPT_ADDR          = _ebase_address + 0x180;

/*************************************************************************
 * Memory Regions
 *
 * Memory regions without attributes cannot be used for orphaned sections.
 * Only sections specifically assigned to these regions can be allocated
 * into these regions.
 *************************************************************************/
MEMORY
{
    /* Prog Flash from 0x9D000000 to 0x9D01FFFF */
    exception_mem              : ORIGIN = 0x9D000000, LENGTH = 0x1000
    kseg0_program_mem    (rx)  : ORIGIN = 0x9D001C00, LENGTH = 0x1E3FF

    /* Boot Flash from 0x9FC00000 to 0x9FC00BEF */
    kseg1_boot_mem             : ORIGIN = 0x9D001000, LENGTH = 0x490
    kseg0_boot_mem             : ORIGIN = 0x9D001490, LENGTH = 0x570

    /* debug_exec_mem             : ORIGIN = 0xBFC02000, LENGTH = 0xFF0 */

    /* Config Bits are from 0xBFC00BF0 to 0xBFC00BFF */ 
    config3                    : ORIGIN = 0xBFC00BF0, LENGTH = 0x4
    config2                    : ORIGIN = 0xBFC00BF4, LENGTH = 0x4
    config1                    : ORIGIN = 0xBFC00BF8, LENGTH = 0x4
    config0                    : ORIGIN = 0xBFC00BFC, LENGTH = 0x4

    /* RAM is from 0xA00000000 to 0xA0007FFF */
    kseg1_data_mem       (w!x) : ORIGIN = 0xA0000000, LENGTH = 0x8000
    sfrs                       : ORIGIN = 0xBF800000, LENGTH = 0x100000
}
SECTIONS
{
  .config_BFC00BF0 : {
    *(.config_BFC00BF0)
  } > config3
  .config_BFC00BF4 : {
    *(.config_BFC00BF4)
  } > config2
  .config_BFC00BF8 : {
    *(.config_BFC00BF8)
  } > config1
  .config_BFC00BFC : {
    *(.config_BFC00BFC)
  } > config0
}

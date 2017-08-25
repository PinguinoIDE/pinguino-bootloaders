/***********************************************************************
    Title:  USB Pinguino Bootloader
    File:   config.h
    Descr.: configuration bits for supported PIC32MX
    Author: Régis Blanchot <rblanchot@gmail.com>

    This file is part of Pinguino (http://www.pinguino.cc)
    Released under the LGPL license (http://www.gnu.org/licenses/lgpl.html)
***********************************************************************/

#ifndef _CONFIG_H_
#define _CONFIG_H_

/*
 * Config0 register
 */

#define DEVCFG0_DEBUG_DISABLED  3           /* Debugger disabled */
#define DEVCFG0_DEBUG_ENABLED   0           /* Debugger enabled */
#define DEVCFG0_JTAG_DISABLED   0           /* JTAG disabled */
#define DEVCFG0_JTAG_ENABLED    1           /* JTAG enabled */
#if defined(__32MX440F256H__)
#define DEVCFG0_ICESEL1         0           /* PGEC1/PGED1 pair is used */
#define DEVCFG0_ICESEL2         1           /* PGEC2/PGED2 pair is used */
#else
#define DEVCFG0_ICESEL1         3           /* Use PGC1/PGD1 */
#define DEVCFG0_ICESEL2         2           /* Use PGC2/PGD2 */
#define DEVCFG0_ICESEL3         1           /* Use PGC3/PGD3 */
#define DEVCFG0_ICESEL4         0           /* Use PGC4/PGD4 (not available on all devices )*/
#endif

#if defined(__PIC32MX2__)
#define DEVCFG0_PWP_DISABLED    0x1FF       /* Program flash write protect */
#define DEVCFG0_PWP_BELOW_2000  0x1F7       /* Memory below 0x2000 (8K) address is write-protected */
#else
#define DEVCFG0_PWP_DISABLED    0xFF        /* Program flash write not protected */
#define DEVCFG0_PWP_BELOW_2000  0xFD        /* Memory below 0x2000 (8K) address is write-protected */
#endif
#define DEVCFG0_PWP_ENABLED     0           /* Program flash write protected */
#define DEVCFG0_BWP_DISABLED    1           /* Boot flash write not protected */
#define DEVCFG0_BWP_ENABLED     0           /* Boot flash write protected */
#define DEVCFG0_CP_DISABLED     1           /* Code not protected */
#define DEVCFG0_CP_ENABLED      0           /* Code protected */

/*
 * Config1 register
 */

#define DEVCFG1_FNOSC_FRC       0           /* Fast RC */
#define DEVCFG1_FNOSC_FRCDIVPLL 1           /* Fast RC with divide-by-N and PLL */
#define DEVCFG1_FNOSC_PRI       2           /* Primary oscillator XT, HS, EC */
#define DEVCFG1_FNOSC_PRIPLL    3           /* Primary with PLL */
#define DEVCFG1_FNOSC_SEC       4           /* Secondary oscillator */
#define DEVCFG1_FNOSC_LPRC      5           /* Low-power RC */
#define DEVCFG1_FNOSC_FRCDIV16  6           /* Fast RC with divide-by-16 */
#define DEVCFG1_FNOSC_FRCDIV    7           /* Fast RC with divide-by-N */

#define DEVCFG1_FSOSCEN_DISABLED 0          /* Secondary oscillator disable */
#define DEVCFG1_FSOSCEN_ENABLED 1           /* Secondary oscillator enable */

#define DEVCFG1_IESO_DISABLED   0           /* Internal-external switch over */
#define DEVCFG1_IESO_ENABLED    0           /* Internal-external switch over */

#define DEVCFG1_POSCMOD_EXT     0           /* External mode */
#define DEVCFG1_POSCMOD_XT      1           /* XT oscillator */
#define DEVCFG1_POSCMOD_HS      2           /* HS oscillator */
#define DEVCFG1_POSCMOD_DISABLED 3          /* Disabled */

#define DEVCFG1_OSCIOFNC_DISABLED 1         /* CLKO output active */
#define DEVCFG1_OSCIOFNC_ENABLED 0          /* CLKO output active */

#define DEVCFG1_FPBDIV_1        0           /* SYSCLK / 1 */
#define DEVCFG1_FPBDIV_2        1           /* SYSCLK / 2 */
#define DEVCFG1_FPBDIV_4        2           /* SYSCLK / 4 */
#define DEVCFG1_FPBDIV_8        3           /* SYSCLK / 8 */

#define DEVCFG1_CSDCMD          2           /* Clock switching is disabled, Fail-Safe Clock Monitor is disabled */
#define DEVCFG1_CSECMD          1           /* Clock switching is enabled, Fail-Safe Clock Monitor is disabled */
#define DEVCFG1_CSECME          0           /* Clock switching is enabled, Fail-Safe Clock Monitor is enabled */

#define DEVCFG1_WDTPS_1         0           /* 1:1 */
#define DEVCFG1_WDTPS_2         1           /* 1:2 */
#define DEVCFG1_WDTPS_4         2           /* 1:4 */
#define DEVCFG1_WDTPS_8         3           /* 1:8 */
#define DEVCFG1_WDTPS_16        4           /* 1:16 */
#define DEVCFG1_WDTPS_32        5           /* 1:32 */
#define DEVCFG1_WDTPS_64        6           /* 1:64 */
#define DEVCFG1_WDTPS_128       7           /* 1:128 */
#define DEVCFG1_WDTPS_256       8           /* 1:256 */
#define DEVCFG1_WDTPS_512       9           /* 1:512 */
#define DEVCFG1_WDTPS_1024      10          /* 1:1024 */
#define DEVCFG1_WDTPS_2048      11          /* 1:2048 */
#define DEVCFG1_WDTPS_4096      12          /* 1:4096 */
#define DEVCFG1_WDTPS_8192      13          /* 1:8192 */
#define DEVCFG1_WDTPS_16384     14          /* 1:16384 */
#define DEVCFG1_WDTPS_32768     15          /* 1:32768 */
#define DEVCFG1_WDTPS_65536     16          /* 1:65536 */
#define DEVCFG1_WDTPS_131072    17          /* 1:131072 */
#define DEVCFG1_WDTPS_262144    18          /* 1:262144 */
#define DEVCFG1_WDTPS_524288    19          /* 1:524288 */
#define DEVCFG1_WDTPS_1048576   20          /* 1:1048576 */

#define DEVCFG1_WINDIS_ENABLED  0           /* Watchdog Window mode enable */
#define DEVCFG1_WINDIS_DISABLED 1           /* Watchdog Window mode disabled */

#define DEVCFG1_FWDTEN_ENABLED  1           /* Watchdog enable */
#define DEVCFG1_FWDTEN_DISABLED 0           /* Watchdog disable */

#define DEVCFG1_FWDTWINSZ_25    3           /* Window size is 25% */
#define DEVCFG1_FWDTWINSZ_37    2           /* Window size is 37.5% */
#define DEVCFG1_FWDTWINSZ_50    1           /* Window size is 50% */
#define DEVCFG1_FWDTWINSZ_75    0           /* Window size is 75% */

/*
 * Config2 register
 */

#define DEVCFG2_FPLLIDIV_1      0           /* 1x */
#define DEVCFG2_FPLLIDIV_2      1           /* 2x */
#define DEVCFG2_FPLLIDIV_3      2           /* 3x */
#define DEVCFG2_FPLLIDIV_4      3           /* 4x */
#define DEVCFG2_FPLLIDIV_5      4           /* 5x */
#define DEVCFG2_FPLLIDIV_6      5           /* 6x */
#define DEVCFG2_FPLLIDIV_10     6           /* 10x */
#define DEVCFG2_FPLLIDIV_12     7           /* 12x */

#define DEVCFG2_FPLLMUL_15      0           /* 15x */
#define DEVCFG2_FPLLMUL_16      1           /* 16x */
#define DEVCFG2_FPLLMUL_17      2           /* 17x */
#define DEVCFG2_FPLLMUL_18      3           /* 18x */
#define DEVCFG2_FPLLMUL_19      4           /* 19x */
#define DEVCFG2_FPLLMUL_20      5           /* 20x */
#define DEVCFG2_FPLLMUL_21      6           /* 21x */
#define DEVCFG2_FPLLMUL_24      7           /* 24x */

#define DEVCFG2_UPLLIDIV_1      0           /* 1x */
#define DEVCFG2_UPLLIDIV_2      1           /* 2x */
#define DEVCFG2_UPLLIDIV_3      2           /* 3x */
#define DEVCFG2_UPLLIDIV_4      3           /* 4x */
#define DEVCFG2_UPLLIDIV_5      4           /* 5x */
#define DEVCFG2_UPLLIDIV_6      5           /* 6x */
#define DEVCFG2_UPLLIDIV_10     6           /* 10x */
#define DEVCFG2_UPLLIDIV_12     7           /* 12x */

#define DEVCFG2_UPLLEN_ENABLED  0           /* Enable USB PLL */
#define DEVCFG2_UPLLEN_DISABLED 1           /* Disable USB PLL */

#define DEVCFG2_FPLLODIV_1      0           /* 1x */
#define DEVCFG2_FPLLODIV_2      1           /* 2x */
#define DEVCFG2_FPLLODIV_4      2           /* 4x */
#define DEVCFG2_FPLLODIV_8      3           /* 8x */
#define DEVCFG2_FPLLODIV_16     4           /* 16x */
#define DEVCFG2_FPLLODIV_32     5           /* 32x */
#define DEVCFG2_FPLLODIV_64     6           /* 64x */
#define DEVCFG2_FPLLODIV_256    7           /* 256x */

/*
 * Config3 register
 */

#define DEVCFG3_USERID(x)       ((x) & 0xffff)
#define DEVCFG3_PMDL1WAY_ONE    1           /* Peripheral Module only one reconfiguration */
#define DEVCFG3_PMDL1WAY_MULTI  0           /* Peripheral Module multiple reconfiguration */
#define DEVCFG3_IOL1WAY_ONE     1           /* Peripheral Pin only one reconfiguration */
#define DEVCFG3_IOL1WAY_MULTI   0           /* Peripheral Pin multiple reconfiguration */
#define DEVCFG3_FUSBIDIO_USB    1           /* USBID pin: controlled by USB */
#define DEVCFG3_FUSBIDIO_PORT   0           /* USBID pin: controlled by port function */
#define DEVCFG3_FVBUSONIO_USB   1           /* VBuson pin: controlled by USB */
#define DEVCFG3_FVBUSONIO_PORT  0           /* VBuson pin: controlled by port function */
#define DEVCFG3_PRIORITY_7      7           /* Shadow Register Set Priority Select bit */
#define DEVCFG3_PRIORITY_6      6           /* Shadow Register Set Priority Select bit */
#define DEVCFG3_PRIORITY_5      5           /* Shadow Register Set Priority Select bit */
#define DEVCFG3_PRIORITY_4      4           /* Shadow Register Set Priority Select bit */
#define DEVCFG3_PRIORITY_3      3           /* Shadow Register Set Priority Select bit */
#define DEVCFG3_PRIORITY_2      2           /* Shadow Register Set Priority Select bit */
#define DEVCFG3_PRIORITY_1      1           /* Shadow Register Set Priority Select bit */
#define DEVCFG3_PRIORITY_0      0           /* Shadow Register Set Priority Select bit */

#endif	/* _CONFIG_H_ */

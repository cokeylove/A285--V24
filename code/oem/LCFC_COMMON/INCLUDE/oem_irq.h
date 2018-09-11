//*****************************************************************************
//
//  oem_irq.h
//
//  Copyright (c) 2012-
//
//  Created on: 2012/11/21
//
//      Author: 
//
//*****************************************************************************

#ifndef	OEM_IRQ_H
#define OEM_IRQ_H

extern void Hook_IRQ_INT1_WKO20(void);
extern void Hook_IRQ_INT2_KBCOBE(void);
extern void Hook_IRQ_INT3_PMCPMC1OBE(void);
extern void Hook_IRQ_INT4_SMBusD(void);
extern void Hook_IRQ_INT5_WKINTAD(void);
extern void Hook_IRQ_INT6_WKO23(void);
extern void Hook_IRQ_INT7_PWM(void);
extern void Hook_IRQ_INT8_ADC(void);
extern void Hook_IRQ_INT9_SMBusA(void);
extern void Hook_IRQ_INT10_SMBusB(void);
extern void Hook_IRQ_INT11_KBMatrixScan(void);
extern void Hook_IRQ_INT12_WKO26(void);
extern void Hook_IRQ_INT13_WKINTC(void);
extern void Hook_IRQ_INT14_WKO25(void);
extern void Hook_IRQ_INT15_CIR(void);
extern void Hook_IRQ_INT16_SMBusC(void);
extern void Hook_IRQ_INT17_WKO24(void);
extern void Hook_IRQ_INT18_PS2Interrupt2(void);
extern void Hook_IRQ_INT19_PS2Interrupt1(void);
extern void Hook_IRQ_INT20_PS2Interrupt0(void);
extern void Hook_IRQ_INT21_WKO22(void);
extern void Hook_IRQ_INT22_SMFISemaphore(void);
extern void Hook_IRQ_INT23_Null(void);
extern void Hook_IRQ_INT24_KBCIBF(void);
extern void Hook_IRQ_INT25_PMCPMC1IBF(void);
extern void Hook_IRQ_INT26_PMC2OBE(void);
extern void Hook_IRQ_INT27_PMC2IBF(void);
extern void Hook_IRQ_INT28_GINTofGPD5(void);
extern void Hook_IRQ_INT29_EGPC(void);
extern void Hook_IRQ_INT30_ET1(void);
extern void Hook_IRQ_INT31_WKO21(void);
extern void Hook_IRQ_INT32_GPINT0(void);
extern void Hook_IRQ_INT33_GPINT1(void);
extern void Hook_IRQ_INT34_GPINT2(void);
extern void Hook_IRQ_INT35_GPINT3(void);
extern void Hook_IRQ_INT36_CIRGPINT(void);
extern void Hook_IRQ_INT37_SSPI(void);
extern void Hook_IRQ_INT38_UART1(void);
extern void Hook_IRQ_INT39_UART2(void);
extern void Hook_IRQ_INT40_WKO50(void);
extern void Hook_IRQ_INT41_WKO51(void);
extern void Hook_IRQ_INT42_WKO52(void);
extern void Hook_IRQ_INT43_WKO53(void);
extern void Hook_IRQ_INT44_WKO54(void);
extern void Hook_IRQ_INT45_WKO55(void);
extern void Hook_IRQ_INT46_WKO56(void);
extern void Hook_IRQ_INT47_WKO57(void);
extern void Hook_IRQ_INT48_WKO60(void);
extern void Hook_IRQ_INT49_WKO61(void);
extern void Hook_IRQ_INT50_WKO62(void);
extern void Hook_IRQ_INT51_WKO63(void);
extern void Hook_IRQ_INT52_WKO64(void);
extern void Hook_IRQ_INT53_WKO65(void);
extern void Hook_IRQ_INT54_WKO66(void);
extern void Hook_IRQ_INT55_WKO67(void);
extern void Hook_IRQ_INT56_Null(void);
extern void Hook_IRQ_INT57_Null(void);
extern void Hook_IRQ_INT58_ET2(void);
extern void Hook_IRQ_INT59_DeferredSPIInstruction(void);
extern void Hook_IRQ_INT60_TMRINTA0(void);
extern void Hook_IRQ_INT61_TMRINTA1(void);
extern void Hook_IRQ_INT62_TMRINTB0(void);
extern void Hook_IRQ_INT63_TMRINTB1(void);
extern void Hook_IRQ_INT64_PMC2EXOBE(void);
extern void Hook_IRQ_INT65_PMC2EXIBF(void);
extern void Hook_IRQ_INT66_PMC3OBE(void);
extern void Hook_IRQ_INT67_PMC3IBF(void);
extern void Hook_IRQ_INT68_PMC4OBE(void);
extern void Hook_IRQ_INT69_PMC4IBF(void);
extern void Hook_IRQ_INT70_Null(void);
extern void Hook_IRQ_INT71_I2BRAM(void);
extern void Hook_IRQ_INT72_WKO70(void);
extern void Hook_IRQ_INT73_WKO71(void);
extern void Hook_IRQ_INT74_WKO72(void);
extern void Hook_IRQ_INT75_WKO73(void);
extern void Hook_IRQ_INT76_WKO74(void);
extern void Hook_IRQ_INT77_WKO75(void);
extern void Hook_IRQ_INT78_WKO76(void);
extern void Hook_IRQ_INT79_WKO77(void);

extern void Hook_EnableInterrupt(void);

extern void Hook_IRQ_INT80_ET8(void);
extern void Hook_IRQ_INT81_SMBusClockHeld(void);
extern void Hook_IRQ_INT82_CEC(void);
extern void Hook_IRQ_INT83_H2RAMLPC(void);
extern void Hook_IRQ_INT84_Null(void);
extern void Hook_IRQ_INT85_WKO88(void);
extern void Hook_IRQ_INT86_WKO89(void);
extern void Hook_IRQ_INT87_WKO90(void);
extern void Hook_IRQ_INT88_WKO80(void);
extern void Hook_IRQ_INT89_WKO81(void);
extern void Hook_IRQ_INT90_WKO82(void);
extern void Hook_IRQ_INT91_WKO83(void);
extern void Hook_IRQ_INT92_WKO84(void);
extern void Hook_IRQ_INT93_WKO85(void);
extern void Hook_IRQ_INT94_WKO86(void);
extern void Hook_IRQ_INT95_WKO87(void);
extern void Hook_IRQ_INT96_WKO91(void);
extern void Hook_IRQ_INT97_WKO92(void);
extern void Hook_IRQ_INT98_WKO93(void);
extern void Hook_IRQ_INT99_WKO94(void);
extern void Hook_IRQ_INT100_WKO95(void);
extern void Hook_IRQ_INT101_WKO96(void);
extern void Hook_IRQ_INT102_WKO97(void);
extern void Hook_IRQ_INT103_WKO98(void);
extern void Hook_IRQ_INT104_WKO99(void);
extern void Hook_IRQ_INT105_WKO100(void);
extern void Hook_IRQ_INT106_WKO101(void);
extern void Hook_IRQ_INT107_WKO102(void);
extern void Hook_IRQ_INT108_WKO103(void);
extern void Hook_IRQ_INT109_WKO104(void);
extern void Hook_IRQ_INT110_WKO105(void);
extern void Hook_IRQ_INT111_WKO106(void);
extern void Hook_IRQ_INT112_WKO107(void);
extern void Hook_IRQ_INT113_WKO108(void);
extern void Hook_IRQ_INT114_WKO109(void);
extern void Hook_IRQ_INT115_WKO110(void);
extern void Hook_IRQ_INT116_WKO111(void);
extern void Hook_IRQ_INT117_WKO112(void);
extern void Hook_IRQ_INT118_WKO113(void);
extern void Hook_IRQ_INT119_WKO114(void);
extern void Hook_IRQ_INT120_WKO115(void);
extern void Hook_IRQ_INT121_WKO116(void);
extern void Hook_IRQ_INT122_WKO117(void);
extern void Hook_IRQ_INT123_WKO118(void);
extern void Hook_IRQ_INT124_WKO119(void);
extern void Hook_IRQ_INT125_WKO120(void);
extern void Hook_IRQ_INT126_WKO121(void);
extern void Hook_IRQ_INT127_WKO122(void);
extern void Hook_IRQ_INT128_WKO128(void);
extern void Hook_IRQ_INT129_WKO129(void);
extern void Hook_IRQ_INT130_WKO130(void);
extern void Hook_IRQ_INT131_WKO131(void);
extern void Hook_IRQ_INT132_WKO132(void);
extern void Hook_IRQ_INT133_WKO133(void);
extern void Hook_IRQ_INT134_WKO134(void);
extern void Hook_IRQ_INT135_Null(void);
extern void Hook_IRQ_INT136_WKO136(void);
extern void Hook_IRQ_INT137_WKO137(void);
extern void Hook_IRQ_INT138_WKO138(void);
extern void Hook_IRQ_INT139_WKO139(void);
extern void Hook_IRQ_INT140_WKO140(void);
extern void Hook_IRQ_INT141_WKO141(void);
extern void Hook_IRQ_INT142_WKO142(void);
extern void Hook_IRQ_INT143_WKO143(void);
extern void Hook_IRQ_INT144_Null(void);
extern void Hook_IRQ_INT145_Null(void);
extern void Hook_IRQ_INT146_Null(void);
extern void Hook_IRQ_INT147_Null(void);
extern void Hook_IRQ_INT148_Null(void);
extern void Hook_IRQ_INT149_PMC5OBE(void);
extern void Hook_IRQ_INT150_PMC5IBF(void);
extern void Hook_IRQ_INT151_VCI(void);
extern void Hook_IRQ_INT152_SMBusE(void);
extern void Hook_IRQ_INT153_SMBusF(void);
extern void Hook_IRQ_INT154_OSCDMA(void);
extern void Hook_IRQ_INT155_ET3(void);
extern void Hook_IRQ_INT156_ET4(void);
extern void Hook_IRQ_INT157_ET5(void);
extern void Hook_IRQ_INT158_ET6(void);
extern void Hook_IRQ_INT159_ET7(void);
extern void Hook_IRQ_INT160_Null(void);
extern void Hook_IRQ_INT161_Null(void);
extern void Hook_IRQ_INT165_USBPD0(void);
extern void Hook_IRQ_INT166_USBPD1(void);

extern void Hook_Isr_Int1(void);
extern void EnablePMC3IBFInt(void);

#endif


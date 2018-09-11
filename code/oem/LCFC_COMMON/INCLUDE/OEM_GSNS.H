#ifndef OEM_GSNS_H
#define OEM_GSNS_H


#define  GS_D0  BRAM20//BRAM10
#define  GS_D1  BRAM21//BRAM11
#define  GS_D2  BRAM22//BRAM12
#define  GS_D3  BRAM23//BRAM13
#define  GS_D4  BRAM24//BRAM14
#define  GS_D5  BRAM25//BRAM15
#define  GS_D6  BRAM26//BRAM16
#define  GS_D7  BRAM27//BRAM17
#define  GS_D8  BRAM28//BRAM18
#define  GS_D9  BRAM29//BRAM19
#define  GS_DA  BRAM2A//BRAM1A
#define  GS_DB  BRAM2B//BRAM1B
#define  GS_DC  BRAM2C//BRAM1C
#define  GS_DD  BRAM2D//BRAM1D
#define  GS_DE  BRAM2E//BRAM1E
#define  GS_DF  BRAM2F//BRAM1F

#define  GS_D10  BRAM30//BRAM20
// BIT6 -- Gravity sensor data is changed.
// BIT5 -- LID status is changed.
// BIT4 -- An input device (keyboard or pointing device) is used.
// All other bits are reserved and EC should set to 0.



#define Who_AM_I                            0x0F


#define LIS3DH_WHO_AM_I                     0x33
#define LIS3DH_ADDRESS                      (0x19 << 1)           // Slave Address (LIS3DH 0x18(SEL/SDO LOW) oor 0x19(SEL/SDO HIGH))
#define LIS3DH_CTRL_REG1                    0x20
	#define LIS3DH_CTRL_REG1_ODR_400HZ      (7 << 4)                // Normal/ low power mode (400Hz)
	#define LIS3DH_CTRL_REG1_Z_EN           (1 << 2)                // Z axis enable
	#define LIS3DH_CTRL_REG1_Y_EN           (1 << 1)                // y axis enable
	#define LIS3DH_CTRL_REG1_X_EN           (1 << 0)                // x axis enable
#define LIS3DH_CTRL_REG4                    0x23
	#define LIS3DH_CTRL_REG4_FS_2G          (0 << 4)                // +-4G
	#define LIS3DH_CTRL_REG4_BDU            (1 << 7)                // Block data update
	#define LIS3DH_CTRL_REG4_HR             (1 << 3)                // High resolution output mode
	#define LIS3DH_CTRL_REG4_ST_MASK        (3 << 1)                // Self test enable
	#define LIS3DH_CTRL_REG4_ST_SELF_TEST0  BIT1                    // Self test 0
#define LIS3DH_OUT_X_L                      0x28
#define LIS3DH_OUT_X_H                      0x29
#define LIS3DH_OUT_Y_L                      0x2A
#define LIS3DH_OUT_Y_H                      0x2B



#define LIS3DSH_WHO_AM_I                     0x3F
#define LIS3DSH_ADDRESS	                    (0x1D << 1)           // Slave Address (LIS3DSH 0x1E(SEL/SDO LOW) or 0x1D(SEL/SDO HIGH))
#define LIS3DSH_CTRL_REG4                   0x20
	#define LIS3DSH_CTRL_REG4_ODR_400HZ     (7 << 4)                // 400HZ
	#define LIS3DSH_CTRL_REG4_BDU           (1 << 3)                // Block data update
	#define LIS3DSH_CTRL_REG4_Z_EN          (1 << 2)                // Z axis enable
	#define LIS3DSH_CTRL_REG4_Y_EN          (1 << 1)                // y axis enable
	#define LIS3DSH_CTRL_REG4_X_EN          (1 << 0)                // x axis enable
#define LIS3DSH_CTRL_REG5                   0x24
	#define LIS3DSH_CTRL_REG5_FS_2G         (0 << 4)                // +-2G
	#define LIS3DSH_CTRL_REG5_HR            (1 << 3)                // High resolution output mode
	#define LIS3DSH_CTRL_REG5_ST_MASK       (3 << 1)                // Self test enable
	#define LIS3DSH_CTRL_REG5_ST_SELF_TEST0 (1 << 1)                // Self test 0 (positive sign)
#define LIS3DSH_OUT_X_L                     0x28
#define LIS3DSH_OUT_X_H	                    0x29
#define LIS3DSH_OUT_Y_L                     0x2A
#define LIS3DSH_OUT_Y_H                     0x2B



#define KX022_WHO_AM_I                      0x14
#define KX022_ADDRESS                       0x3C	//(0x1E << 1)           // Slave Address (KX023 0x1E(SEL/SDO LOW) or 0x1F(SEL/SDO HIGH))

#define KX023_WHO_AM_I                      0x15
#define KX023_ADDRESS                       (0x1F << 1)           // Slave Address (KX023 0x1E(SEL/SDO LOW) or 0x1F(SEL/SDO HIGH))
#define KX023_ODCNTL                        0x1B  
	#define KX023_ODCNTL_OSA_400            (5 << 0)                // 400Hz
#define KX023_CNTL1                         0x18
	#define KX023_CNTL1_GSEL_2G             (0 << 3)                // +-2G
	#define KX023_CNTL1_RES                 (1 << 6)                // high current, 12-bit or 14-bit valid. Banndwidth=ODR/2
	#define KX023_CNTL1_PC1                 (1 << 7)                // Operating/ standby mode
#define KX023_XOUT_L                        0x06
#define KX023_XOUT_H                        0x07
#define KX023_YOUT_L                        0x08
#define KX023_YOUT_H                        0x09
#define KX023_SELF_TEST                     0x60

#define GSENSOR_NO                          0



extern void APS_Cmd_10(BYTE nPort);
extern void APS_Cmd_11(BYTE nPort);
extern void APS_Cmd_12(BYTE nPort);
extern void APS_Cmd_13(BYTE nPort);
extern void APS_Cmd_14(BYTE nPort);
extern void APS_Cmd_15(BYTE nPort);
extern void APS_Cmd_16(BYTE nPort);
extern void APS_Cmd_17(BYTE nPort);
extern void APS_Cmd_18(BYTE nPort);


extern void ScanGsensorADC(void);
extern void Gsensor_Initial(void);
extern void GSensor_DIAG(void);
void GSensorDeviceID (void);
void InitGSensor (void);
void ScanGsensorI2C (void);


extern void LnvApsProtect(void);
#endif

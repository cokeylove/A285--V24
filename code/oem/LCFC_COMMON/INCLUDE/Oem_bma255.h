/*
********************************************************************************
* Lenovo Confidential                                                          
*                                                                                             
* Microcode Source Materials                                                         
*  
* ThinkPad Embedded Controller Microcode
* 
* COPYRIGHT LENOVO 2016 ALL RIGHTS RESERVED
*
* Filename: Bma255.h
* 
* Description:
*    definition of BOSCH BMA255 Digital, triaxial acceleration sensor
* 
* History:
*   7/06/2016    Tetsuji Nakamura
*      change address of BMA255a to 0x18(0x30) from 0x19(0x32)
*   2/29/2016    Tetsuji Nakamura
*      created as new
********************************************************************************
*/

#ifndef __BMA255_H
   #define __BMA255_H
//#include <Gsensor.h>
/*
*********************************************************
*     #define Constants
*********************************************************
*/

#ifndef BMA255_ADDRESS 
#define BMA255_ADDRESS                   0x30         // Slave Address (BMA250 0x30 or 0x32)
#endif

#define BMA255_BGW_CHIPID                0x00         // Chip identication code  
   #define BMA255_BGW_CHIPID_VALUE             0xfa         
#define BMA255_ACCD_X_LSB                0x02         // least significant bits of X channel
   #define BMA255_ACCD_LSB_MASK                0xf0       // least significant bits mask (bit 0 - 3)
   #define BMA255_ACCD_LSB_SHIFT               4                   
   #define BMA255_ACCD_LSB_NEW_DATA            (1 << 0)   // acceleration value has been updated                 
#define BMA255_ACCD_X_MSB                0x03         // most significant bits of X channel (bit 4 - 11)
#define BMA255_ACCD_Y_LSB                0x04         // least significant bits of Y channel (bit 0 - 3)
#define BMA255_ACCD_Y_MSB                0x05         // most significant bits of Y channel (bit 4 - 11)
#define BMA255_ACCD_Z_LSB                0x06         // least significant bits of Z channel (bit 0 - 3)
#define BMA255_ACCD_Z_MSB                0x07         // most significant bits of Z channel (bit 4 - 11)
#define BMA255_ACCD_TEMP                 0x08         // temperature from 23 celsius (0 means 23C)
#define BMA255_INT_STATUS_0              0x09         // interrupt status flags
   #define BMA255_INT_STATUS_0_FLAT_INT         (1 << 7)  // flat interrupt status
   #define BMA255_INT_STATUS_0_ORIENT_INT       (1 << 6)  // orientation interrupt status
   #define BMA255_INT_STATUS_0_S_TAP_INT        (1 << 5)  // single tap interrupt status
   #define BMA255_INT_STATUS_0_D_TAP_INT        (1 << 4)  // double tap interrupt status
   #define BMA255_INT_STATUS_0_SLO_NO_MOT_INT   (1 << 3)  // slow/no-motion interrupt status
   #define BMA255_INT_STATUS_0_SLOPE_INT        (1 << 2)  // slow/no-motion interrupt status
   #define BMA255_INT_STATUS_0_HIGH_INT         (1 << 1)  // high-g interrupt status
   #define BMA255_INT_STATUS_0_LOW_INT          (1 << 0)  // low-g interupt status
#define BMA255_INT_STATUS_1              0x0a         // interrupt status flags
   #define BMA255_INT_STATUS_1_DATA_INT         (1 << 7)  // data ready interrupt status
   #define BMA255_INT_STATUS_1_FIFO_WM_INT      (1 << 6)  // FIFO watermark interrupt status
   #define BMA255_INT_STATUS_1_FIFO_FULL_INT    (1 << 5)  // FIFO full interrupt status
#define BMA255_INT_STATUS_2              0x0b         // interrupt status flags
   #define BMA255_INT_STATUS_2_TAP_SIGN         (1 << 7)  // sign of single/double tap triggering signal
   #define BMA255_INT_STATUS_2_TAP_FIRST_Z      (1 << 6)  // single/double tap interrupt by z-axis
   #define BMA255_INT_STATUS_2_TAP_FIRST_Y      (1 << 5)  // single/double tap interrupt by y-axis
   #define BMA255_INT_STATUS_2_TAP_FIRST_X      (1 << 4)  // single/double tap interrupt by x-axis
   #define BMA255_INT_STATUS_2_SLOPE_SIGN       (1 << 3)  // slope sign of slope tap triggering signal
   #define BMA255_INT_STATUS_2_SLOPE_FIRST_Z    (1 << 2)  // slope interrupt triggered by z-axis
   #define BMA255_INT_STATUS_2_SLOPE_FIRST_Y    (1 << 1)  // slope interrupt triggered by y-axis
   #define BMA255_INT_STATUS_2_SLOPE_FIRST_X    (1 << 0)  // slope interrupt triggered by x-axis
#define BMA255_INT_STATUS_3              0x0c         // interrupt status flags
   #define BMA255_INT_STATUS_3_FLAT             (1 << 7)  // device is flat obly valid if FLAT_EN is 1
   #define BMA255_INT_STATUS_3_ORIENT_DOWN      (1 << 6)  // orienttion value of z-axis, downward
   #define BMA255_INT_STATUS_3_ORIENT_MASK      (3 << 4)  // orienttion value of x, y axis
   #define BMA255_INT_STATUS_3_ORIENT_PORTRAIT_UPRIGHT     (0 << 4)
   #define BMA255_INT_STATUS_3_ORIENT_PORTRAIT_UPSIDE_DOWN (1 << 4)
   #define BMA255_INT_STATUS_3_ORIENT_LANDSCAPE_LEFT       (2 << 4)
   #define BMA255_INT_STATUS_3_ORIENT_LANDSCAPE_RIGHT      (3 << 4)
   #define BMA255_INT_STATUS_3_HIGH_SIGN        (1 << 3)  // sign of acceleration signal triggered high-g interrupt
   #define BMA255_INT_STATUS_3_HIGH_FIRST_Z     (1 << 2)  // high-g interrupt triggered by z-axis
   #define BMA255_INT_STATUS_3_HIGH_FIRST_Y     (1 << 1)  // high-g interrupt triggered by y-axis
   #define BMA255_INT_STATUS_3_HIGH_FIRST_X     (1 << 0)  // high-g interrupt triggered by x-axis
#define BMA255_FIFO_STATUS               0x0e         // FIFO status flags
   #define BMA255_FIFO_STATUS_FIFO_OVERRUN      (1 << 7)  // FIFO overrun
   #define BMA255_FIFO_STATUS_FIFO_FRAME_COUNTER_MASK  0x7f
   #define BMA255_FIFO_STATUS_FIFO_FRAME_COUNTER_SHIFT 0
#define BMA255_PMU_RANGE                 0x0f         // accelerometer g-range
   #define BMA255_PMU_RANGE_2G                  0x3       // +-2G
   #define BMA255_PMU_RANGE_4G                  0x5       // +-4G
   #define BMA255_PMU_RANGE_8G                  0x8       // +-8G
   #define BMA255_PMU_RANGE_16G                 0xc       // +-16G
#define BMA255_PMU_BW                    0x10         // acceleration filter bandwidhth
   #define BMA255_PMU_BW_7HZ                    0x8       // 7.81Hz
   #define BMA255_PMU_BW_15HZ                   0x9       // 15.63Hz
   #define BMA255_PMU_BW_31HZ                   0xa       // 31.25Hz
   #define BMA255_PMU_BW_62HZ                   0xb       // 62.5Hz
   #define BMA255_PMU_BW_125HZ                  0xc       // 125Hz 
   #define BMA255_PMU_BW_250HZ                  0xd       // 250Hz 
   #define BMA255_PMU_BW_500HZ                  0xe       // 500Hz
   #define BMA255_PMU_BW_1000Hz                 0xf       // 1000Hz
#define BMA255_PMU_LPW                   0x11         // low power sleep period
   #define BMA255_PMU_LPW_SUSPEND               (1 << 7)  // 000: NORMAL mode
   #define BMA255_PMU_LPW_LOWPOWER_EN           (1 << 6)  // 001: DEEP_SUSPEND mode
   #define BMA255_PMU_LPW_DEEP_SUSPEND          (1 << 5)  // 010: LOW_POWER mode
                                                         // 100: SUSPEND mode
   #define BMA255_PMU_SLEEP_DURATION_MASK     (0x1f << 0) // sleep phase duration in LOW_POWER mode
   #define BMA255_PMU_SLEEP_DURATION_500US      0         // 0.5ms
   #define BMA255_PMU_SLEEP_DURATION_1MS        6         // 1ms
   #define BMA255_PMU_SLEEP_DURATION_2MS        7         // 2ms
   #define BMA255_PMU_SLEEP_DURATION_4MS        8         // 4ms
   #define BMA255_PMU_SLEEP_DURATION_6MS        9         // 6ms
   #define BMA255_PMU_SLEEP_DURATION_10MS       0xa       // 10ms
   #define BMA255_PMU_SLEEP_DURATION_25MS       0xb       // 25ms
   #define BMA255_PMU_SLEEP_DURATION_50MS       0xc       // 50ms
   #define BMA255_PMU_SLEEP_DURATION_100MS      0xd       // 100ms
   #define BMA255_PMU_SLEEP_DURATION_500MS      0xe       // 500ms
   #define BMA255_PMU_SLEEP_DURATION_1S         0xf       // 1 second
#define BMA255_PMU_LOW_POWER             0x12         // settings for low power mode
   #define BMA255_PMU_LOW_POWER_LOWPOWER_MODE   (1 << 6)  // LPM2/LPM1 configuration for suspend and low power mode
   #define BMA255_PMU_LOW_POWER_SLEEPTIMER_MODE (1 << 5)  // equidistant sampling/ event driven time-base mode
#define BMA255_ACCD_HBW                  0x13         // Acceleration data qcuisition and data output format
   #define BMA255_ACCD_HBW_DATA_HIGH_BW         (1 << 7)  // unfiltered/filtered data
   #define BMA255_ACCD_HBW_SHADOW_DIS           (1 << 6)  // disable the shadowing mechanism for
                                                           // acceleration data output register
#define BMA255_BGW_SOFTRESET             0x14         // Controls user triggered reset of the sensor
   #define BMA255_BGW_SOFTRESET_VALUE           0xb6      // trigger a reset
#define BMA255_INT_EN_0                  0x16         // interrupt enable
   #define BMA255_INT_EN_0_FLAT_EN              (1 << 7)  // flat interrupt enable
   #define BMA255_INT_EN_0_ORIENT_EN            (1 << 6)  // orientation interrupt enable
   #define BMA255_INT_EN_0_S_TAP_EN             (1 << 5)  // single tap interrupt enable
   #define BMA255_INT_EN_0_D_TAP_EN             (1 << 4)  // double tap interrupt enable
   #define BMA255_INT_EN_0_SLOPE_EN_Z           (1 << 2)  // slope interrupt z-axis
   #define BMA255_INT_EN_0_SLOPE_EN_Y           (1 << 1)  // slope interrupt y-axis
   #define BMA255_INT_EN_0_SLOPE_EN_X           (1 << 0)  // slope interrupt x-axis
#define BMA255_INT_EN_1                  0x17         // interrupt enable
   #define BMA255_INT_EN_1_INT_FWM_EN           (1 << 6)  // FIFO watermark interrupt enable
   #define BMA255_INT_EN_1_INT_FFULL_EN         (1 << 5)  // FIFO full interrupt enable
   #define BMA255_INT_EN_1_DATA_EN              (1 << 4)  // data ready interrupt enale
   #define BMA255_INT_EN_1_LOW_EN               (1 << 3)  // low-g interrupt
   #define BMA255_INT_EN_1_HIGH_EN_Z            (1 << 2)  // high-g interrupt z-axis
   #define BMA255_INT_EN_1_HIGH_EN_Y            (1 << 1)  // high-g interrupt y-axis
   #define BMA255_INT_EN_1_HIGH_EN_X            (1 << 0)  // high-g interrupt x-axis
#define BMA255_INT_EN_2                  0x18         // interrupt enable
   #define BMA255_INT_EN_2_SLO_NO_MOT_SEL       (1 << 3)  // select mo-motion/slow motion intterupt
   #define BMA255_INT_EN_2_SLO_NO_MOT_EN_Z      (1 << 2)  // slow/no-motion interrupt z-axis
   #define BMA255_INT_EN_2_SLO_NO_MOT_EN_Y      (1 << 1)  // slow/no-motion interrupt y-axis
   #define BMA255_INT_EN_2_SLO_NO_MOT_EN_X      (1 << 0)  // slow/no-motion interrupt x-axis
#define BMA255_INT_MAP_0                 0x19         // interrupt signals are mapped to the INT1 pin
   #define BMA255_INT_MAP_0_INT1_FLAT           (1 << 7)  // map flat interrupt to INT1
   #define BMA255_INT_MAP_0_INT1_ORIENT         (1 << 6)  // map orientation interrupt to INT1
   #define BMA255_INT_MAP_0_INT1_S_TAP          (1 << 5)  // map single tap interrupt to INT1
   #define BMA255_INT_MAP_0_INT1_D_TAP          (1 << 4)  // map double tap interrupt to INT1
   #define BMA255_INT_MAP_0_INT1_SLO_NO_MOT     (1 << 3)  // map slow/no-motion interrupt to INT1
   #define BMA255_INT_MAP_0_INT1_SLOPE          (1 << 2)  // map slope interrupt to INT1
   #define BMA255_INT_MAP_0_INT1_HIGH           (1 << 1)  // map high-g interrupt to INT1
   #define BMA255_INT_MAP_0_INT1_LOW            (1 << 0)  // map low-g interrupt to INT1
#define BMA255_INT_MAP_1                 0x1a         // interrupt signals are mapped to the INT1 and INT2 pins
   #define BMA255_INT_MAP_1_INT2_DATA           (1 << 7)  // map data ready interrupt to INT2
   #define BMA255_INT_MAP_1_INT2_FWM            (1 << 6)  // map FIFO watermark interrupt to INT2
   #define BMA255_INT_MAP_1_INT2_FFULL          (1 << 5)  // map FIFO full interrupt to INT2
   #define BMA255_INT_MAP_1_INT1_FFULL          (1 << 2)  // map FIFO full interrupt to INT1
   #define BMA255_INT_MAP_1_INT1_FWM            (1 << 1)  // map FIFO watermark interrupt to INT1
   #define BMA255_INT_MAP_1_INT1_DATA           (1 << 0)  // map data ready interrupt to INT1
#define BMA255_INT_MAP_2                 0x1b         // interrupt signals are mapped to the INT2 pin.
   #define BMA255_INT_MAP_2_INT2_FLAT           (1 << 7)  // map flat interrupt to INT2
   #define BMA255_INT_MAP_2_INT2_ORIENT         (1 << 6)  // map orientation interrupt to INT2
   #define BMA255_INT_MAP_2_INT2_S_TAP          (1 << 5)  // map single tap interrupt to INT2
   #define BMA255_INT_MAP_2_INT2_D_TAP          (1 << 4)  // map double tap interrupt to INT2
   #define BMA255_INT_MAP_2_INT2_SLO_NO_MOT     (1 << 3)  // map slow/no-motion interrupt to INT2
   #define BMA255_INT_MAP_2_INT2_SLOPE          (1 << 2)  // map slope interrupt to INT2
   #define BMA255_INT_MAP_2_INT2_HIGH           (1 << 1)  // map high-g interrupt to INT2
   #define BMA255_INT_MAP_2_INT2_LOW            (1 << 0)  // map low-g interrupt to INT2
#define BMA255_INT_SRC                   0x1e         // data source definition for interrupts
   #define BMA255_INT_SRC_DATA                  (1 << 5)  // unfiltered/filtered data for new data interrupt
   #define BMA255_INT_SRC_TAP                   (1 << 4)  // unfiltered/filtered data for single/double tap interrupt
   #define BMA255_INT_SRC_SLO_NO_MOT            (1 << 3)  // unfiltered/filtered data for slow/no-motion interrupt
   #define BMA255_INT_SRC_SLOPE                 (1 << 2)  // unfiltered/filtered data for slope interrupt
   #define BMA255_INT_SRC_HIGH                  (1 << 1)  // unfiltered/filtered data for high-g interrupt
   #define BMA255_INT_SRC_LOW                   (1 << 0)  // unfiltered/filtered data for low-g interrupt
#define BMA255_INT_OUT_CTRL              0x20         // behavioural configuration of the interrupts pins
   #define BMA255_INT_OUT_CTRL_INT2_OD          (1 << 3)  // open drain/push-pull behavior for INT2 pin
   #define BMA255_INT_OUT_CTRL_INT2_LVL         (1 << 2)  // active high/ low level for INT2 pin
   #define BMA255_INT_OUT_CTRL_INT1_OD          (1 << 1)  // open drain/push-pull behavior for INT1 pin
   #define BMA255_INT_OUT_CTRL_INT1_LVL         (1 << 0)  // active high/ low level for INT1 pin
#define BMA255_INT_RST_LATCH             0x21         // interrupt reset bit and interrupt mode selection
   #define BMA255_INT_RST_LATCH_RESET_INT       (1 << 7)  // clear any latched interrupt
   #define BMA255_INT_RST_LATCH_LATCH_INT_MASK  0x0f      // latch mode
   #define BMA255_INT_RST_LATCH_LATCH_INT_NON_LATCHED 0   // non-latched
   #define BMA255_INT_RST_LATCH_LATCH_INT_250MS 1         // temporary 250ms
   #define BMA255_INT_RST_LATCH_LATCH_INT_500MS 2         // temporary 500ms
   #define BMA255_INT_RST_LATCH_LATCH_INT_1S    3         // temporary 1s
   #define BMA255_INT_RST_LATCH_LATCH_INT_2S    4         // temporary 2s
   #define BMA255_INT_RST_LATCH_LATCH_INT_4S    5         // temporary 4s
   #define BMA255_INT_RST_LATCH_LATCH_INT_8S    6         // temporary 8s
   #define BMA255_INT_RST_LATCH_LATCH_INT_LATCHED 7       // latched
   #define BMA255_INT_RST_LATCH_LATCH_INT_250US 9         // temporary 250us
   #define BMA255_INT_RST_LATCH_LATCH_INT_500US 0xa       // temporary 500us
   #define BMA255_INT_RST_LATCH_LATCH_INT_1MS   0xb       // temporary 1ms
   #define BMA255_INT_RST_LATCH_LATCH_INT_12MS  0xc       // temporary 12.5ms
   #define BMA255_INT_RST_LATCH_LATCH_INT_25MS  0xd       // temporary 25ms
   #define BMA255_INT_RST_LATCH_LATCH_INT_50MS  0xe       // temporary 50ms
#define BMA255_INT_0                     0x22         // delay time definition for the low-g interrupt
                                                       // delay according to (this value + 1) * 2ms
#define BMA255_INT_1                     0x23         // threshold definition for the low-g interrupt
                                                       // threshold according to (this value * 7.81 mg) in 2G range
#define BMA255_INT_2                     0x24         // low-g mode selection, hysteresis and high-g histeresis
   #define BMA255_INT_2_HIGH_HY_MASK            (3 << 6)  // hysteresis of high-g interrupt according to
                                                           // this value * 125mg in 2G range
   #define BMA255_INT_2_HIGH_HY_SHIFT           6       
   #define BMA255_INT_2_LOW_MODE                (1 << 2)  // axis-summing mode/ single-axis mode
   #define BMA255_INT_2_LOW_HY_MASK             (3 << 0)  // hysteresis of low-g interrupt according to
                                                           // this value * 125mg in 2G range
   #define BMA255_INT_2_LOW_HY_SHIFT            0       
#define BMA255_INT_3                     0x25         // delay time definition for the high-g interrupt
                                                       // delay according to (this value + 1) * 2ms
#define BMA255_INT_4                     0x26         // threshold definition for the high-g interrupt
                                                       // threshold according to (this value * 7.81 mg) in 2G range
#define BMA255_INT_5                     0x27         // number of samples to be evaluated for slope interrupt and
                                                       // slow/no-motion interrupt
   #define BMA255_INT_5_SLO_NO_MOT_DUR_MASK     0xfc      // b5,4: 00 -> b0-3 + 1
   #define BMA255_INT_5_SLO_NO_MOT_DUR_SHIFT    2         // b5,4: 01 -> (b0-3 * 4) + 20
                                                           // b5: 1    -> (b0-4 * 8) + 88
   #define BMA255_INT_5_SLOPE_DUR_MASK          0x3       // b1,0 + 1
   #define BMA255_INT_5_SLOPE_DUR_SHIFT         0x0   
#define BMA255_INT_6                     0x28         // threshold for any-motion interrupt
                                                       // threshold is according to this value * 3.91mg in 2G range
#define BMA255_INT_7                     0x29         // threshold for slow/no-motion interrupt
                                                       // threshold is according to this value * 3.91mg in 2G range
#define BMA255_INT_8                     0x2a         // timing definitions for signle tap and doule tap interrupts
   #define BMA255_INT_8_TAP_QUIET               (1 << 7)  // select a tap quiet duration to 20ms/30ms
   #define BMA255_INT_8_TAP_SHOCK               (1 << 6)  // select a tap shock duration to 75ms/50ms
   #define BMA255_INT_8_TAP_DUR_MASK            0x7       // select the length of the witome window for 
                                                           // second shock event for double tap detection
   #define BMA255_INT_8_TAP_DUR_50MS            0
   #define BMA255_INT_8_TAP_DUR_100MS           1
   #define BMA255_INT_8_TAP_DUR_150MS           2
   #define BMA255_INT_8_TAP_DUR_200MS           3
   #define BMA255_INT_8_TAP_DUR_250MS           4
   #define BMA255_INT_8_TAP_DUR_375MS           5
   #define BMA255_INT_8_TAP_DUR_500MS           6
   #define BMA255_INT_8_TAP_DUR_700MS           7
#define BMA255_INT_9                     0x2b         // number of samples processed by signle/double-tap interrupt
                                                       // after wake up in low-power mode
                                                       // threshold for single/double tap interrupt
   #define BMA255_INT_9_TAP_SAMP_MASK           (3 << 6)  // select number of samples processed after wake-up
   #define BMA255_INT_9_TAP_SAMP_2              (0 << 6)
   #define BMA255_INT_9_TAP_SAMP_4              (1 << 6)
   #define BMA255_INT_9_TAP_SAMP_8              (2 << 6)
   #define BMA255_INT_9_TAP_SAMP_16             (3 << 6)
   #define BMA255_INT_9_TAP_TH_MASK             0x1f      // threshold of single/double tap interrupr
   #define BMA255_INT_9_TAP_TH_SHIFT            0         // corresponding to an acceleration difference of tap,
                                                           // this value * 125mg in 2G mode
#define BMA255_INT_A                     0x2c         // hysteresis, blocking and mode for orientation interrupt
   #define BMA255_INT_A_ORIENT_HYST_MASK        (7 << 4)  // hysteresis of the orientation interrupt
                                                           // accoording to this value * 62.5mg in all G mode
   #define BMA255_INT_A_ORIENT_HYST_SHIFT       4
   #define BMA255_INT_A_ORIENT_BLOCKING_MASK    (3 << 2)  // blocking mode of orientation interrupt
   #define BMA255_INT_A_ORIENT_BLOCKING_NONE    0
   #define BMA255_INT_A_ORIENT_BLOCKING_MODE0   1         // theta blocking or acceleration in any axis > 1.5G
   #define BMA255_INT_A_ORIENT_BLOCKING_MODE1   2         // theta blocking or
                                                           // acceleration slope in any axis > 0.2G or
                                                           // acceleration in any axis > 1.5G
   #define BMA255_INT_A_ORIENT_BLOCKING_MODE2   3         // theta blocking or
                                                           // acceleration slope in any axis > 0.4G or
                                                           // acceleration in any axis > 1.5G and
                                                           // value of orient is not stable for at least 100ms
   #define BMA255_INT_A_ORIENT_MODE_SYMMETRICAL       0   // symmetrical mode 
   #define BMA255_INT_A_ORIENT_MODE_HIGH_ASYMMETRICAL 1   // high-asymmetrical mode
   #define BMA255_INT_A_ORIENT_MODE_LOW_ASYMMETRICAL  2   // low-asymmetrical mode
#define BMA255_INT_B                     0x2d         // axis orientation, up/down masking and the theta blocking
                                                       // angle for the orientation interrupt
   #define BMA255_INT_B_ORIENT_UD_EN            (1 << 6)  // generate an orientation interrupt on change of up/down
   #define BMA255_INT_B_ORIENT_THETA_MASK       0x3f      // defines blocking angle between 0 and 44.8
   #define BMA255_INT_B_ORIENT_THETA_SHIFT      0         
#define BMA255_INT_C                     0x2e         // flat threshold angle for the flat interrupt
   #define BMA255_INT_C_FLAT_THETA_MASK         0x3f      // defines blocking angle between 0 and 44.8
   #define BMA255_INT_C_FLAT_THETA_SHIFT        0    
#define BMA255_INT_D                     0x2f         // flat interrupt hold time and flat interrupt hysteresis
   #define BMA255_INT_D_FLAT_HOLD_TIME_MASK     (3 << 4)  // delay time for which the flat value must remain 
                                                           // stable for the flat interrupt to be generated
   #define BMA255_INT_D_FLAT_HOLD_TIME_0MS      (0 << 4)
   #define BMA255_INT_D_FLAT_HOLD_TIME_512MS    (1 << 4)
   #define BMA255_INT_D_FLAT_HOLD_TIME_1024MS   (2 << 4)
   #define BMA255_INT_D_FLAT_HOLD_TIME_2048MS   (3 << 4)
   #define BMA255_INT_D_FLAT_HY_MASK            7         // flat interrupt syteresis
   #define BMA255_INT_D_FLAT_HY_SHIFT           0         
#define BMA255_FIFO_CONFIG_0             0x30         // FIFO watermark level
   #define BMA255_FIFO_CONFIG_0_FIFO_WATER_MARK_LEVEL_TRIGGER_RETAIN_MASK  0x3f  
   #define BMA255_FIFO_CONFIG_0_FIFO_WATER_MARK_LEVEL_TRIGGER_RETAIN_SHIFT 0  
#define BMA255_PMU_SELF_TEST             0x32         // settings of the sensor self-test configuration and trigger
   #define BMA255_PMU_SELF_TEST_SIGN            (1 << 2)  // select sign of self-test excitation as positive/negative
   #define BMA255_PMU_SELF_TEST_AXIS_MASK       (3 << 0)  // select axis of self-test
   #define BMA255_PMU_SELF_TEST_AXIS_DISABLE    (0 << 0)  //   when a self-test is performed, only the acceleration
   #define BMA255_PMU_SELF_TEST_AXIS_X          (1 << 0)  //   data readout value of the selected axis is valid.
   #define BMA255_PMU_SELF_TEST_AXIS_Y          (2 << 0)  //   after the self-test has been enable a delay of a least
   #define BMA255_PMU_SELF_TEST_AXIS_Z          (3 << 0)  //   50ms is neccessary for the read-out value to settle
#define BMA255_TRIM_NVM_CTRL             0x33         // settings of the few-time programmable non-volatile memory
   #define BMA255_TRIM_NVM_CTRL_NVM_REAMIN_MASK 0xf0      // number of remain write cycles permitted for NVM (RO)
   #define BMA255_TRIM_NVM_CTRL_NVM_REAMIN_SHIFT 4        
   #define BMA255_TRIM_NVM_CTRL_NVM_LOAD        (1 << 3)  // trigger to update of all configuration registers
   #define BMA255_TRIM_NVM_CTRL_NVM_RDY         (1 << 2)  // ready to accept a new write or update trigger (RO)
   #define BMA255_TRIM_NVM_CTRL_NVM_PROG_TRIG   (1 << 1)  // trigger to write (WO)
   #define BMA255_TRIM_NVM_CTRL_NVM_PROG_MODE   (1 << 0)  // unlock/lock to write (WO)
#define BMA255_BGW_SPI3_WDT              0x34         // settings for the digital interface
   #define BMA255_BGW_SPI3_WDT_I2C_WDT_EN       (1 << 2)  // enable the watchdog at SDI pin if I2C is selected
   #define BMA255_BGW_SPI3_WDT_I2C_WDT_SEL      (1 << 1)  // select an I2C watchdog timer period of 50ms/1ms
   #define BMA255_BGW_SPI3_WDT_SPI3             (1 << 0)  // 3 wire/ 4wire SPI mode
#define BMA255_OFC_CTRL                  0x36         // settings for the fast and the slow offset compensation
   #define BMA255_OFC_CTRL_OFFSET_RESET         (1 << 7)  // set all compensation registers to 0
   #define BMA255_OFC_CTRL_OFFSET_TRIGGER_MASK  (3 << 5)  // select axis of trigger the fast compensation for
   #define BMA255_OFC_CTRL_OFFSET_TRIGGER_X     (1 << 5)  // x-axis
   #define BMA255_OFC_CTRL_OFFSET_TRIGGER_Y     (2 << 5)  // y-axis
   #define BMA255_OFC_CTRL_OFFSET_TRIGGER_Z     (3 << 5)  // z-axis
   #define BMA255_OFC_CTRL_CAL_RDY              (1 << 4)  // offset compensation is ready
   #define BMA255_OFC_CTRL_HP_Z_EN              (1 << 2)  // enable slow offset compensation for z-axis
   #define BMA255_OFC_CTRL_HP_Y_EN              (1 << 1)  // enable slow offset compensation for y-axis
   #define BMA255_OFC_CTRL_HP_X_EN              (1 << 0)  // enable slow offset compensation for x-axis
#define BMA255_OFC_SETTING               0x37         // settings for the fast and the slow offset compensation
   #define BMA255_OFC_SETTING_OFFSET_TARGET_Z_MASK  (3 << 5)
   #define BMA255_OFC_SETTING_OFFSET_TARGET_Z_SHIFT 5
   #define BMA255_OFC_SETTING_OFFSET_TARGET_Y_MASK  (3 << 3)
   #define BMA255_OFC_SETTING_OFFSET_TARGET_Y_SHIFT 3
   #define BMA255_OFC_SETTING_OFFSET_TARGET_X_MASK  (3 << 1)
   #define BMA255_OFC_SETTING_OFFSET_TARGET_X_SHIFT 1
   #define BMA255_OFC_SETTING_OFFSET_TARGET_0G      0
   #define BMA255_OFC_SETTING_OFFSET_TARGET_1G      1
   #define BMA255_OFC_SETTING_OFFSET_TARGET_1G_N    2
   #define BMA255_OFC_SETTING_CUT_OFF               (1 << 0)
#define BMA255_OFC_OFFSET_X              0x38         // offset compensation value for x-axis
#define BMA255_OFC_OFFSET_Y              0x39         // offset compensation value for y-axis
#define BMA255_OFC_OFFSET_Z              0x3a         // offset compensation value for z-axis
#define BMA255_TRIM_GP1                  0x3c         // Genral purpose data register with NVM back-up
#define BMA255_FIFO_CONFIG_1             0x3e         // Genral purpose data register with NVM back-up
   #define BMA255_FIFO_CONFIG_1_FIFO_MODE_MASK        (3 << 6)  // FIFO operating mode
   #define BMA255_FIFO_CONFIG_1_FIFO_MODE_BYPASS      (0 << 6) // BYPASS (buffer depth of 1 frame)
   #define BMA255_FIFO_CONFIG_1_FIFO_MODE_FIFO        (1 << 6) // FIFO (data collection stops when buffer is full)
   #define BMA255_FIFO_CONFIG_1_FIFO_MODE_STREAM      (2 << 6) // STREAM (sampling contitues when buffer is full)
   #define BMA255_FIFO_CONFIG_1_FIFO_DATA_SELECT_MASK (3 << 0) // select data of x, y, z-axis to stored into FIFO
   #define BMA255_FIFO_CONFIG_1_FIFO_DATA_SELECT_XYZ  (0 << 0) // all of x, y, z-axis acceleration data
   #define BMA255_FIFO_CONFIG_1_FIFO_DATA_SELECT_X    (1 << 0) // only x-axis acceleration data
   #define BMA255_FIFO_CONFIG_1_FIFO_DATA_SELECT_Y    (2 << 0) // only y-axis acceleration data
   #define BMA255_FIFO_CONFIG_1_FIFO_DATA_SELECT_Z    (3 << 0) // only z-axis acceleration data
#define BMA255_FIFO_DATA                 0x3f         // FIFO read-out register
                                                       //   The format of the LSB and MSB components corresponds
                                                       //   to that of acceleration data read-out registers.
                                                       //   Read burst access may be used since the address
                                                       //   counter will not increment when the read burst is 
                                                       //   started at the address of FIFO_DATA. 
                                                       //   The entire frame is discarded when a frame is only
                                                       //   partially read out.
#define BMA255_WaitCommandMS     1
/*
*********************************************************
*     Global Macros
*********************************************************
*/


/*
*********************************************************
*     Global Data Types
*********************************************************
*/

/*
*********************************************************
*     Global Variables
*********************************************************
*/

/*
*********************************************************
*     Externals
*********************************************************
*/

/*
*********************************************************
*     Global Function Prototypes
*********************************************************
*/
extern void BMA255_SelfTest(void);
extern void sendBMA255_SelfTestCommand(BYTE axis);

#endif

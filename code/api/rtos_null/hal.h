
#ifndef __HAL_H__
#define __HAL_H__

#include "../../nds/include/os_cpu.h"
#include "../../nds/include/nds32_regs.h"
#include "../../nds/include/mem_layout.h"

extern volatile unsigned char gucI2CINTFlag[3];

//****************************************************************************
//  Define Macro
//****************************************************************************
#define HAL_DISABLE_INTERRUPTS		        (0)
#define HAL_ENABLE_INTERRUPTS		        (1)

#define HAL_NULL			                (0)
#define HAL_TRUE			                (1)
#define HAL_FALSE			                (0)
                                            
#define HAL_SUSPEND			                (-1) /* wait for object timeout */
#define HAL_DEL_NO_PEND                     (0)
#define HAL_DEL_ALWAYS                      (1)
                                            
#define HAL_SUCCESS			                (0)
#define HAL_FAILURE			                (-1)
                                            
#define HAL_ERR_UNHANDLED_INTERRUPT	        (-1)
#define HAL_ERR_INVALID_POINTER		        (-2)
#define HAL_ERR_NOT_PRESENT		            (-3)
#define HAL_ERR_UNAVAILABLE		            (-4)
#define HAL_ERR_TIMEOUT			            (-5)
#define HAL_ERR_NO_MEMORY		            (-6)
#define HAL_ERR_INVALID_ENTRY		        (-7)
#define HAL_ERR_INVALID_OPERATION	        (-8)
#define HAL_ERR_INVALID_DRIVER		        (-9)
#define HAL_ERR_INVALID_START               (-23)

//****************************************************************************
//  Macro Define
//****************************************************************************
#define DEF_DIRECT_MAP_CODE                 0	// alex modify to disable ILM for 8186
#define DM_S_IDX                            (2)
#define DM_MAX_NUM                          (6)//(9)
#define SRAM_SEG_SIZE                       (0x1000)
#define SRAM_SEG_ALIGN                      (12)

#define vPortEnterCritical()
#define vPortExitCritical()

//****************************************************************************
//  Structure Declaration
//****************************************************************************
typedef struct hal_semaphore {

	void	*obj;

} hal_semaphore_t;

typedef struct hal_mutex {

	void	*obj;

} hal_mutex_t;

typedef struct hal_mbox {

	void	*obj;

} hal_mbox_t;

typedef struct hal_queue {

	void	*obj;
	void	**start;
	void	*msg;
	INT32U  size;
	INT32U	opt;
	INT32U  timeout;
	void	*event;
	INT32S	err;

} hal_queue_t;

typedef struct hal_message {

	void	*obj;

} hal_message_t;

typedef struct hal_thread {

	void		(*fn)(void *);
	void		*arg;
	INT8U		prio;
	void		*ptos;		    /* high_address should be assigned to NULL 
					               if you want kernel to allocate stack. */
	INT32U		stack_size;	    /* in bytes */
	char		*name;
	void		*task;
    void        *pstack;        /* Stack start pointer */

} hal_thread_t;

typedef struct hal_bh {

	hal_semaphore_t	sem;
	hal_thread_t	th;

} hal_bh_t;

typedef struct hal_event {

	void	*obj;

} hal_event_t;

inline void  hal_hw_module_int(void);
inline unsigned int hal_global_int_ctl(int int_op);
inline int RTOS_CODE_H hal_wait_for_mutex(hal_mutex_t *mutex, unsigned int timeout);
inline int RTOS_CODE_H hal_release_mutex(hal_mutex_t *mutex);
inline int RTOS_CODE_L hal_create_mutex(hal_mutex_t *mutex, const void *param);
inline int RTOS_CODE_H hal_suspend_all_thread(void);
inline int RTOS_CODE_H hal_resume_all_thread(void);
inline int RTOS_CODE_H hal_sleep(int ms);
//inline void RTOS_CODE_H hal_hid_irq(void);
//inline void RTOS_CODE_H hal_fusion_irq(void);
inline void RTOS_CODE_L hal_init_os(void);
inline void  hal_yield_thread(void);

//**************************
//  HID & FUSION IRQ
//**************************
inline void  hal_SMbusD_irq(void);
inline void  hal_SMbusE_irq(void);
inline void  hal_SMbusF_irq(void);
inline void  hal_usb_irq(void);
inline void  hal_sensor_hub_off(void);
inline void  hal_sensor_hub_on(void);

//**************************
//  WUI IRQ
//**************************
inline void  hal_WUI_SmbF_swap2ch0_irq(void);
inline void  hal_WUI_SmbD_irq(void);
inline void  hal_WUI_SmbE_irq(void);
inline void  hal_WUI_SmbF_irq(void);
inline void  hal_WUI_enable_mask(void);
inline void  hal_WUI_USBDminus_irq(void);
inline void  hal_WUI_USBDplus_irq(void);
inline unsigned long hal_get_sys_ticks(void);

inline unsigned long hal_get_free_heep_size(void);
inline void* hal_malloc(unsigned long size);
inline void  hal_free(void *ptr);

extern void vApplicationTickHook(void);

extern void EC_Handle_Task(void);
extern void pure_ec_sensor_device_power_off(void);
extern void usb_isr_service_flag(void);
extern void service_usb_isr(void);
#endif

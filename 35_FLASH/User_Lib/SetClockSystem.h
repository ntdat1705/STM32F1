#ifndef __SETCLOCKSYSTEM_H
#define __SETCLOCKSYSTEM_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Uncomment the line corresponding to the desired System clock (SYSCLK)
   frequency (after reset the HSI is used as SYSCLK source) */
//#define SYSCLK_HSE
#define SYSCLK_FREQ_72MHz

#if !defined STM32F10X_LD_VL && !defined STM32F10X_MD_VL && !defined STM32F10X_HD_VL
  //#define SYSCLK_FREQ_36MHz
  //#define SYSCLK_FREQ_48MHz
  //#define SYSCLK_FREQ_56MHz
  #define SYSCLK_FREQ_72MHz
#endif

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void SetSysClock(void);
void NVIC_Configuration(void);
#endif

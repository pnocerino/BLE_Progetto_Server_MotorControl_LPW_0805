/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    MSS_app.c
  * @author  MCD Application Team
  * @brief   MSS_app application definition.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "app_common.h"
#include "log_module.h"
#include "app_ble.h"
#include "ll_sys_if.h"
#include "dbg_trace.h"
#include "mss_app.h"
#include "mss.h"
#include "stm32_rtos.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

typedef enum
{
  Msp_NOTIFICATION_OFF,
  Msp_NOTIFICATION_ON,
  /* USER CODE BEGIN Service2_APP_SendInformation_t */

  /* USER CODE END Service2_APP_SendInformation_t */
  MSS_APP_SENDINFORMATION_LAST
} MSS_APP_SendInformation_t;

typedef struct
{
  MSS_APP_SendInformation_t     Msp_Notification_Status;
  /* USER CODE BEGIN Service2_APP_Context_t */

  /* USER CODE END Service2_APP_Context_t */
  uint16_t              ConnectionHandle;
} MSS_APP_Context_t;

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* External variables --------------------------------------------------------*/
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Private macros ------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
static MSS_APP_Context_t MSS_APP_Context;

uint8_t a_MSS_UpdateCharData[247];

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static void MSS_Msp_SendNotification(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Functions Definition ------------------------------------------------------*/
void MSS_Notification(MSS_NotificationEvt_t *p_Notification)
{
  /* USER CODE BEGIN Service2_Notification_1 */

  /* USER CODE END Service2_Notification_1 */
  switch(p_Notification->EvtOpcode)
  {
    /* USER CODE BEGIN Service2_Notification_Service2_EvtOpcode */

    /* USER CODE END Service2_Notification_Service2_EvtOpcode */

    case MSS_MSP_READ_EVT:
      /* USER CODE BEGIN Service2Char1_READ_EVT */

      /* USER CODE END Service2Char1_READ_EVT */
      break;

    case MSS_MSP_NOTIFY_ENABLED_EVT:
      /* USER CODE BEGIN Service2Char1_NOTIFY_ENABLED_EVT */

      /* USER CODE END Service2Char1_NOTIFY_ENABLED_EVT */
      break;

    case MSS_MSP_NOTIFY_DISABLED_EVT:
      /* USER CODE BEGIN Service2Char1_NOTIFY_DISABLED_EVT */

      /* USER CODE END Service2Char1_NOTIFY_DISABLED_EVT */
      break;

    case MSS_MC_WRITE_EVT:
      /* USER CODE BEGIN Service2Char2_WRITE_EVT */

      /* USER CODE END Service2Char2_WRITE_EVT */
      break;

    default:
      /* USER CODE BEGIN Service2_Notification_default */

      /* USER CODE END Service2_Notification_default */
      break;
  }
  /* USER CODE BEGIN Service2_Notification_2 */

  /* USER CODE END Service2_Notification_2 */
  return;
}

void MSS_APP_EvtRx(MSS_APP_ConnHandleNotEvt_t *p_Notification)
{
  /* USER CODE BEGIN Service2_APP_EvtRx_1 */

  /* USER CODE END Service2_APP_EvtRx_1 */

  switch(p_Notification->EvtOpcode)
  {
    /* USER CODE BEGIN Service2_APP_EvtRx_Service2_EvtOpcode */

    /* USER CODE END Service2_APP_EvtRx_Service2_EvtOpcode */
    case MSS_CONN_HANDLE_EVT :
      /* USER CODE BEGIN Service2_APP_CONN_HANDLE_EVT */

      /* USER CODE END Service2_APP_CONN_HANDLE_EVT */
      break;

    case MSS_DISCON_HANDLE_EVT :
      /* USER CODE BEGIN Service2_APP_DISCON_HANDLE_EVT */

      /* USER CODE END Service2_APP_DISCON_HANDLE_EVT */
      break;

    default:
      /* USER CODE BEGIN Service2_APP_EvtRx_default */

      /* USER CODE END Service2_APP_EvtRx_default */
      break;
  }

  /* USER CODE BEGIN Service2_APP_EvtRx_2 */

  /* USER CODE END Service2_APP_EvtRx_2 */

  return;
}

void MSS_APP_Init(void)
{
  UNUSED(MSS_APP_Context);
  MSS_Init();

  /* USER CODE BEGIN Service2_APP_Init */

  /* USER CODE END Service2_APP_Init */
  return;
}

/* USER CODE BEGIN FD */

/* USER CODE END FD */

/*************************************************************
 *
 * LOCAL FUNCTIONS
 *
 *************************************************************/
__USED void MSS_Msp_SendNotification(void) /* Property Notification */
{
  MSS_APP_SendInformation_t notification_on_off = Msp_NOTIFICATION_OFF;
  MSS_Data_t mss_notification_data;

  mss_notification_data.p_Payload = (uint8_t*)a_MSS_UpdateCharData;
  mss_notification_data.Length = 0;

  /* USER CODE BEGIN Service2Char1_NS_1 */

  /* USER CODE END Service2Char1_NS_1 */

  if (notification_on_off != Msp_NOTIFICATION_OFF)
  {
    MSS_UpdateValue(MSS_MSP, &mss_notification_data);
  }

  /* USER CODE BEGIN Service2Char1_NS_Last */

  /* USER CODE END Service2Char1_NS_Last */

  return;
}

/* USER CODE BEGIN FD_LOCAL_FUNCTIONS */

/* USER CODE END FD_LOCAL_FUNCTIONS */

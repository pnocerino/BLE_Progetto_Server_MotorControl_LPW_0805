/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    WSS_app.c
  * @author  MCD Application Team
  * @brief   WSS_app application definition.
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
#include "wss_app.h"
#include "wss.h"
#include "stm32_rtos.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

typedef enum
{
  Wgt_NOTIFICATION_OFF,
  Wgt_NOTIFICATION_ON,
  /* USER CODE BEGIN Service3_APP_SendInformation_t */

  /* USER CODE END Service3_APP_SendInformation_t */
  WSS_APP_SENDINFORMATION_LAST
} WSS_APP_SendInformation_t;

typedef struct
{
  WSS_APP_SendInformation_t     Wgt_Notification_Status;
  /* USER CODE BEGIN Service3_APP_Context_t */

  /* USER CODE END Service3_APP_Context_t */
  uint16_t              ConnectionHandle;
} WSS_APP_Context_t;

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
static WSS_APP_Context_t WSS_APP_Context;

uint8_t a_WSS_UpdateCharData[247];

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static void WSS_Wgt_SendNotification(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Functions Definition ------------------------------------------------------*/
void WSS_Notification(WSS_NotificationEvt_t *p_Notification)
{
  /* USER CODE BEGIN Service3_Notification_1 */

  /* USER CODE END Service3_Notification_1 */
  switch(p_Notification->EvtOpcode)
  {
    /* USER CODE BEGIN Service3_Notification_Service3_EvtOpcode */

    /* USER CODE END Service3_Notification_Service3_EvtOpcode */

    case WSS_WGT_READ_EVT:
      /* USER CODE BEGIN Service3Char1_READ_EVT */

      /* USER CODE END Service3Char1_READ_EVT */
      break;

    case WSS_WGT_NOTIFY_ENABLED_EVT:
      /* USER CODE BEGIN Service3Char1_NOTIFY_ENABLED_EVT */

      /* USER CODE END Service3Char1_NOTIFY_ENABLED_EVT */
      break;

    case WSS_WGT_NOTIFY_DISABLED_EVT:
      /* USER CODE BEGIN Service3Char1_NOTIFY_DISABLED_EVT */

      /* USER CODE END Service3Char1_NOTIFY_DISABLED_EVT */
      break;

    default:
      /* USER CODE BEGIN Service3_Notification_default */

      /* USER CODE END Service3_Notification_default */
      break;
  }
  /* USER CODE BEGIN Service3_Notification_2 */

  /* USER CODE END Service3_Notification_2 */
  return;
}

void WSS_APP_EvtRx(WSS_APP_ConnHandleNotEvt_t *p_Notification)
{
  /* USER CODE BEGIN Service3_APP_EvtRx_1 */

  /* USER CODE END Service3_APP_EvtRx_1 */

  switch(p_Notification->EvtOpcode)
  {
    /* USER CODE BEGIN Service3_APP_EvtRx_Service3_EvtOpcode */

    /* USER CODE END Service3_APP_EvtRx_Service3_EvtOpcode */
    case WSS_CONN_HANDLE_EVT :
      /* USER CODE BEGIN Service3_APP_CONN_HANDLE_EVT */

      /* USER CODE END Service3_APP_CONN_HANDLE_EVT */
      break;

    case WSS_DISCON_HANDLE_EVT :
      /* USER CODE BEGIN Service3_APP_DISCON_HANDLE_EVT */

      /* USER CODE END Service3_APP_DISCON_HANDLE_EVT */
      break;

    default:
      /* USER CODE BEGIN Service3_APP_EvtRx_default */

      /* USER CODE END Service3_APP_EvtRx_default */
      break;
  }

  /* USER CODE BEGIN Service3_APP_EvtRx_2 */

  /* USER CODE END Service3_APP_EvtRx_2 */

  return;
}

void WSS_APP_Init(void)
{
  UNUSED(WSS_APP_Context);
  WSS_Init();

  /* USER CODE BEGIN Service3_APP_Init */

  /* USER CODE END Service3_APP_Init */
  return;
}

/* USER CODE BEGIN FD */

/* USER CODE END FD */

/*************************************************************
 *
 * LOCAL FUNCTIONS
 *
 *************************************************************/
__USED void WSS_Wgt_SendNotification(void) /* Property Notification */
{
  WSS_APP_SendInformation_t notification_on_off = Wgt_NOTIFICATION_OFF;
  WSS_Data_t wss_notification_data;

  wss_notification_data.p_Payload = (uint8_t*)a_WSS_UpdateCharData;
  wss_notification_data.Length = 0;

  /* USER CODE BEGIN Service3Char1_NS_1 */

  /* USER CODE END Service3Char1_NS_1 */

  if (notification_on_off != Wgt_NOTIFICATION_OFF)
  {
    WSS_UpdateValue(WSS_WGT, &wss_notification_data);
  }

  /* USER CODE BEGIN Service3Char1_NS_Last */

  /* USER CODE END Service3Char1_NS_Last */

  return;
}

/* USER CODE BEGIN FD_LOCAL_FUNCTIONS */

/* USER CODE END FD_LOCAL_FUNCTIONS */

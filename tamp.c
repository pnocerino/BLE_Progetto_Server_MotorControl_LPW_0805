/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    tamp.c
  * @brief   This file provides code for the configuration
  *          of the TAMP instances.
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
#include "tamp.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* TAMP init function */

void MX_TAMP_RTC_Init(void)
{

  /* USER CODE BEGIN TAMP_Init 0 */

  /* USER CODE END TAMP_Init 0 */

  RTC_PrivilegeStateTypeDef privilegeState = {0};
  RTC_ActiveTampersTypeDef sAllTamper = {0};

  /* USER CODE BEGIN TAMP_Init 1 */

  /* USER CODE END TAMP_Init 1 */

  /** Enable the RTC Active Tamper
  */
  privilegeState.tampPrivilegeFull = TAMP_PRIVILEGE_FULL_YES;
  if (HAL_RTCEx_PrivilegeModeSet(&hrtc, &privilegeState) != HAL_OK)
  {
    Error_Handler();
  }
  sAllTamper.ActiveFilter = RTC_ATAMP_FILTER_DISABLE;
  sAllTamper.ActiveAsyncPrescaler = RTC_ATAMP_ASYNCPRES_RTCCLK;
  sAllTamper.TimeStampOnTamperDetection = RTC_TIMESTAMPONTAMPERDETECTION_ENABLE;
  sAllTamper.ActiveOutputChangePeriod = 0;
  sAllTamper.Seed[0] = 0x00000000;
  sAllTamper.Seed[1] = 0x00000000;
  sAllTamper.Seed[2] = 0x00000000;
  sAllTamper.Seed[3] = 0x00000000;
  sAllTamper.TampInput[RTC_ATAMP_4].Enable = RTC_ATAMP_ENABLE;
  sAllTamper.TampInput[RTC_ATAMP_4].Interrupt = RTC_ATAMP_INTERRUPT_DISABLE;
  sAllTamper.TampInput[RTC_ATAMP_4].Output = RTC_ATAMP_4;
  sAllTamper.TampInput[RTC_ATAMP_4].NoErase = RTC_TAMPER_ERASE_BACKUP_ENABLE;
  if (HAL_RTCEx_SetActiveTampers(&hrtc, &sAllTamper) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TAMP_Init 2 */

  /* USER CODE END TAMP_Init 2 */

}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

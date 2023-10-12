/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    App/custom_stm.c
  * @author  MCD Application Team
  * @brief   Custom Example Service.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "common_blesvc.h"
#include "custom_stm.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef struct{
  uint16_t  CustomUartshortHdle;                    /**< uartlong handle */
  uint16_t  CustomTxdHdle;                  /**< txdlong handle */
  uint16_t  CustomRxdHdle;                  /**< rxdlong handle */
}CustomContext_t;

/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private defines -----------------------------------------------------------*/
#define UUID_128_SUPPORTED  1

#if (UUID_128_SUPPORTED == 1)
#define BM_UUID_LENGTH  UUID_TYPE_128
#else
#define BM_UUID_LENGTH  UUID_TYPE_16
#endif

#define BM_REQ_CHAR_SIZE    (3)

/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macros ------------------------------------------------------------*/
#define CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET         2
#define CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET              1
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
uint8_t SizeTxd = 32;
uint8_t SizeRxd = 32;

/**
 * START of Section BLE_DRIVER_CONTEXT
 */
PLACE_IN_SECTION("BLE_DRIVER_CONTEXT") static CustomContext_t CustomContext;

/**
 * END of Section BLE_DRIVER_CONTEXT
 */

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static SVCCTL_EvtAckStatus_t Custom_STM_Event_Handler(void *pckt);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Functions Definition ------------------------------------------------------*/
/* USER CODE BEGIN PFD */

/* USER CODE END PFD */

/* Private functions ----------------------------------------------------------*/

#define COPY_UUID_128(uuid_struct, uuid_15, uuid_14, uuid_13, uuid_12, uuid_11, uuid_10, uuid_9, uuid_8, uuid_7, uuid_6, uuid_5, uuid_4, uuid_3, uuid_2, uuid_1, uuid_0) \
do {\
    uuid_struct[0] = uuid_0; uuid_struct[1] = uuid_1; uuid_struct[2] = uuid_2; uuid_struct[3] = uuid_3; \
    uuid_struct[4] = uuid_4; uuid_struct[5] = uuid_5; uuid_struct[6] = uuid_6; uuid_struct[7] = uuid_7; \
    uuid_struct[8] = uuid_8; uuid_struct[9] = uuid_9; uuid_struct[10] = uuid_10; uuid_struct[11] = uuid_11; \
    uuid_struct[12] = uuid_12; uuid_struct[13] = uuid_13; uuid_struct[14] = uuid_14; uuid_struct[15] = uuid_15; \
}while(0)

/* Hardware Characteristics Service */
/*
 The following 128bits UUIDs have been generated from the random UUID
 generator:
 D973F2E0-B19E-11E2-9E96-0800200C9A66: Service 128bits UUID
 D973F2E1-B19E-11E2-9E96-0800200C9A66: Characteristic_1 128bits UUID
 D973F2E2-B19E-11E2-9E96-0800200C9A66: Characteristic_2 128bits UUID
 */
#define COPY_UARTLONG_UUID(uuid_struct)          COPY_UUID_128(uuid_struct,0x9e,0xca,0xdc,0x24,0x0e,0xe5,0xa9,0xe0,0x93,0xf3,0xa3,0xb5,0x01,0x00,0x40,0x6e)
#define COPY_TXDLONG_UUID(uuid_struct)    COPY_UUID_128(uuid_struct,0x9e,0xca,0xdc,0x24,0x0e,0xe5,0xa9,0xe0,0x93,0xf3,0xa3,0xb5,0x03,0x00,0x40,0x6e)
#define COPY_RXDLONG_UUID(uuid_struct)    COPY_UUID_128(uuid_struct,0x9e,0xca,0xdc,0x24,0x0e,0xe5,0xa9,0xe0,0x93,0xf3,0xa3,0xb5,0x02,0x00,0x40,0x6e)

/* USER CODE BEGIN PF */

/* USER CODE END PF */

/**
 * @brief  Event handler
 * @param  Event: Address of the buffer holding the Event
 * @retval Ack: Return whether the Event has been managed or not
 */
static SVCCTL_EvtAckStatus_t Custom_STM_Event_Handler(void *Event)
{
  SVCCTL_EvtAckStatus_t return_value;
  hci_event_pckt *event_pckt;
  evt_blecore_aci *blecore_evt;
  aci_gatt_attribute_modified_event_rp0 *attribute_modified;
  aci_gatt_write_permit_req_event_rp0   *write_perm_req;
  aci_gatt_read_permit_req_event_rp0    *read_req;
  Custom_STM_App_Notification_evt_t     Notification;
  /* USER CODE BEGIN Custom_STM_Event_Handler_1 */

  /* USER CODE END Custom_STM_Event_Handler_1 */

  return_value = SVCCTL_EvtNotAck;
  event_pckt = (hci_event_pckt *)(((hci_uart_pckt*)Event)->data);

  switch (event_pckt->evt)
  {
    case HCI_VENDOR_SPECIFIC_DEBUG_EVT_CODE:
      blecore_evt = (evt_blecore_aci*)event_pckt->data;
      switch (blecore_evt->ecode)
      {
        case ACI_GATT_ATTRIBUTE_MODIFIED_VSEVT_CODE:
          /* USER CODE BEGIN EVT_BLUE_GATT_ATTRIBUTE_MODIFIED_BEGIN */

          /* USER CODE END EVT_BLUE_GATT_ATTRIBUTE_MODIFIED_BEGIN */
          attribute_modified = (aci_gatt_attribute_modified_event_rp0*)blecore_evt->data;
          if (attribute_modified->Attr_Handle == (CustomContext.CustomTxdHdle + CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /* USER CODE BEGIN CUSTOM_STM_Service_1_Char_1 */

            /* USER CODE END CUSTOM_STM_Service_1_Char_1 */
            switch (attribute_modified->Attr_Data[0])
            {
              /* USER CODE BEGIN CUSTOM_STM_Service_1_Char_1_attribute_modified */

              /* USER CODE END CUSTOM_STM_Service_1_Char_1_attribute_modified */

              /* Disabled Notification management */
              case (!(COMSVC_Notification)):
                /* USER CODE BEGIN CUSTOM_STM_Service_1_Char_1_Disabled_BEGIN */

                /* USER CODE END CUSTOM_STM_Service_1_Char_1_Disabled_BEGIN */
                Notification.Custom_Evt_Opcode = CUSTOM_STM_TXD_NOTIFY_DISABLED_EVT;
                Custom_STM_App_Notification(&Notification);
                /* USER CODE BEGIN CUSTOM_STM_Service_1_Char_1_Disabled_END */

                /* USER CODE END CUSTOM_STM_Service_1_Char_1_Disabled_END */
                break;

              /* Enabled Notification management */
              case COMSVC_Notification:
                /* USER CODE BEGIN CUSTOM_STM_Service_1_Char_1_COMSVC_Notification_BEGIN */

                /* USER CODE END CUSTOM_STM_Service_1_Char_1_COMSVC_Notification_BEGIN */
                Notification.Custom_Evt_Opcode = CUSTOM_STM_TXD_NOTIFY_ENABLED_EVT;
                Custom_STM_App_Notification(&Notification);
                /* USER CODE BEGIN CUSTOM_STM_Service_1_Char_1_COMSVC_Notification_END */

                /* USER CODE END CUSTOM_STM_Service_1_Char_1_COMSVC_Notification_END */
                break;

              default:
                /* USER CODE BEGIN CUSTOM_STM_Service_1_Char_1_default */

                /* USER CODE END CUSTOM_STM_Service_1_Char_1_default */
              break;
            }
          }  /* if (attribute_modified->Attr_Handle == (CustomContext.CustomTxdHdle + CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET))*/

          else if (attribute_modified->Attr_Handle == (CustomContext.CustomRxdHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /* USER CODE BEGIN CUSTOM_STM_Service_1_Char_2_ACI_GATT_ATTRIBUTE_MODIFIED_VSEVT_CODE */

            /* USER CODE END CUSTOM_STM_Service_1_Char_2_ACI_GATT_ATTRIBUTE_MODIFIED_VSEVT_CODE */
          } /* if (attribute_modified->Attr_Handle == (CustomContext.CustomRxdHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))*/
          /* USER CODE BEGIN EVT_BLUE_GATT_ATTRIBUTE_MODIFIED_END */

          /* USER CODE END EVT_BLUE_GATT_ATTRIBUTE_MODIFIED_END */
          break;

        case ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE :
          /* USER CODE BEGIN EVT_BLUE_GATT_READ_PERMIT_REQ_BEGIN */

          /* USER CODE END EVT_BLUE_GATT_READ_PERMIT_REQ_BEGIN */
          read_req = (aci_gatt_read_permit_req_event_rp0*)blecore_evt->data;
          if (read_req->Attribute_Handle == (CustomContext.CustomTxdHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /*USER CODE BEGIN CUSTOM_STM_Service_1_Char_1_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_1 */

            /*USER CODE END CUSTOM_STM_Service_1_Char_1_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_1*/
            aci_gatt_allow_read(read_req->Connection_Handle);
            /*USER CODE BEGIN CUSTOM_STM_Service_1_Char_1_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_2 */

            /*USER CODE END CUSTOM_STM_Service_1_Char_1_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_2*/
          } /* if (read_req->Attribute_Handle == (CustomContext.CustomTxdHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))*/
          /* USER CODE BEGIN EVT_BLUE_GATT_READ_PERMIT_REQ_END */

          /* USER CODE END EVT_BLUE_GATT_READ_PERMIT_REQ_END */
          break;

        case ACI_GATT_WRITE_PERMIT_REQ_VSEVT_CODE:
          /* USER CODE BEGIN EVT_BLUE_GATT_WRITE_PERMIT_REQ_BEGIN */

          /* USER CODE END EVT_BLUE_GATT_WRITE_PERMIT_REQ_BEGIN */
          write_perm_req = (aci_gatt_write_permit_req_event_rp0*)blecore_evt->data;
          if (write_perm_req->Attribute_Handle == (CustomContext.CustomRxdHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /* Allow or reject a write request from a client using aci_gatt_write_resp(...) function */
            /*USER CODE BEGIN CUSTOM_STM_Service_1_Char_2_ACI_GATT_WRITE_PERMIT_REQ_VSEVT_CODE */

            /*USER CODE END CUSTOM_STM_Service_1_Char_2_ACI_GATT_WRITE_PERMIT_REQ_VSEVT_CODE*/
          } /*if (write_perm_req->Attribute_Handle == (CustomContext.CustomRxdHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))*/

          /* USER CODE BEGIN EVT_BLUE_GATT_WRITE_PERMIT_REQ_END */

          /* USER CODE END EVT_BLUE_GATT_WRITE_PERMIT_REQ_END */
          break;
        /* USER CODE BEGIN BLECORE_EVT */

        /* USER CODE END BLECORE_EVT */
        default:
          /* USER CODE BEGIN EVT_DEFAULT */

          /* USER CODE END EVT_DEFAULT */
          break;
      }
      /* USER CODE BEGIN EVT_VENDOR*/

      /* USER CODE END EVT_VENDOR*/
      break; /* HCI_VENDOR_SPECIFIC_DEBUG_EVT_CODE */

      /* USER CODE BEGIN EVENT_PCKT_CASES*/

      /* USER CODE END EVENT_PCKT_CASES*/

    default:
      /* USER CODE BEGIN EVENT_PCKT*/

      /* USER CODE END EVENT_PCKT*/
      break;
  }

  /* USER CODE BEGIN Custom_STM_Event_Handler_2 */

  /* USER CODE END Custom_STM_Event_Handler_2 */

  return(return_value);
}/* end Custom_STM_Event_Handler */

/* Public functions ----------------------------------------------------------*/

/**
 * @brief  Service initialization
 * @param  None
 * @retval None
 */
void SVCCTL_InitCustomSvc(void)
{

  Char_UUID_t  uuid;
  tBleStatus ret = BLE_STATUS_INVALID_PARAMS;
  /* USER CODE BEGIN SVCCTL_InitCustomSvc_1 */

  /* USER CODE END SVCCTL_InitCustomSvc_1 */

  /**
   *  Register the event handler to the BLE controller
   */
  SVCCTL_RegisterSvcHandler(Custom_STM_Event_Handler);

  /**
   *          uartlong
   *
   * Max_Attribute_Records = 1 + 2*2 + 1*no_of_char_with_notify_or_indicate_property + 1*no_of_char_with_broadcast_property
   * service_max_attribute_record = 1 for uartlong +
   *                                2 for txdlong +
   *                                2 for rxdlong +
   *                                1 for txdlong configuration descriptor +
   *                              = 6
   */

  COPY_UARTLONG_UUID(uuid.Char_UUID_128);
  ret = aci_gatt_add_service(UUID_TYPE_128,
                             (Service_UUID_t *) &uuid,
                             PRIMARY_SERVICE,
                             6,
                             &(CustomContext.CustomUartshortHdle));
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_gatt_add_service command: uartshort, error code: 0x%x \n\r", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_gatt_add_service command: uartshort \n\r");
  }

  /**
   *  txdlong
   */
  COPY_TXDLONG_UUID(uuid.Char_UUID_128);
  ret = aci_gatt_add_char(CustomContext.CustomUartshortHdle,
                          UUID_TYPE_128, &uuid,
                          SizeTxd,
                          CHAR_PROP_READ | CHAR_PROP_NOTIFY,
                          ATTR_PERMISSION_NONE,
                          GATT_NOTIFY_ATTRIBUTE_WRITE | GATT_NOTIFY_WRITE_REQ_AND_WAIT_FOR_APPL_RESP | GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP,
                          0x10,
                          CHAR_VALUE_LEN_VARIABLE,
                          &(CustomContext.CustomTxdHdle));
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_gatt_add_char command   : TXD, error code: 0x%x \n\r", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_gatt_add_char command   : TXD \n\r");
  }
  /**
   *  rxdlong
   */
  COPY_RXDLONG_UUID(uuid.Char_UUID_128);
  ret = aci_gatt_add_char(CustomContext.CustomUartshortHdle,
                          UUID_TYPE_128, &uuid,
                          SizeRxd,
                          CHAR_PROP_WRITE,
                          ATTR_PERMISSION_NONE,
                          GATT_NOTIFY_ATTRIBUTE_WRITE | GATT_NOTIFY_WRITE_REQ_AND_WAIT_FOR_APPL_RESP | GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP,
                          0x10,
                          CHAR_VALUE_LEN_VARIABLE,
                          &(CustomContext.CustomRxdHdle));
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_gatt_add_char command   : RXD, error code: 0x%x \n\r", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_gatt_add_char command   : RXD \n\r");
  }

  /* USER CODE BEGIN SVCCTL_InitCustomSvc_2 */

  /* USER CODE END SVCCTL_InitCustomSvc_2 */

  return;
}

/**
 * @brief  Characteristic update
 * @param  CharOpcode: Characteristic identifier
 * @param  Service_Instance: Instance of the service to which the characteristic belongs
 *
 */
tBleStatus Custom_STM_App_Update_Char(Custom_STM_Char_Opcode_t CharOpcode, uint8_t *pPayload)
{
  tBleStatus ret = BLE_STATUS_INVALID_PARAMS;
  /* USER CODE BEGIN Custom_STM_App_Update_Char_1 */

  /* USER CODE END Custom_STM_App_Update_Char_1 */

  switch (CharOpcode)
  {

    case CUSTOM_STM_TXD:
      ret = aci_gatt_update_char_value(CustomContext.CustomUartshortHdle,
                                       CustomContext.CustomTxdHdle,
                                       0, /* charValOffset */
                                       SizeTxd, /* charValueLen */
                                       (uint8_t *)  pPayload);
      if (ret != BLE_STATUS_SUCCESS)
      {
        APP_DBG_MSG("  Fail   : aci_gatt_update_char_value TXD command, result : 0x%x \n\r", ret);
      }
      else
      {
        APP_DBG_MSG("  Success: aci_gatt_update_char_value TXD command\n\r");
      }
      /* USER CODE BEGIN CUSTOM_STM_App_Update_Service_1_Char_1*/

      /* USER CODE END CUSTOM_STM_App_Update_Service_1_Char_1*/
      break;

    case CUSTOM_STM_RXD:
      ret = aci_gatt_update_char_value(CustomContext.CustomUartshortHdle,
                                       CustomContext.CustomRxdHdle,
                                       0, /* charValOffset */
                                       SizeRxd, /* charValueLen */
                                       (uint8_t *)  pPayload);
      if (ret != BLE_STATUS_SUCCESS)
      {
        APP_DBG_MSG("  Fail   : aci_gatt_update_char_value RXD command, result : 0x%x \n\r", ret);
      }
      else
      {
        APP_DBG_MSG("  Success: aci_gatt_update_char_value RXD command\n\r");
      }
      /* USER CODE BEGIN CUSTOM_STM_App_Update_Service_1_Char_2*/

      /* USER CODE END CUSTOM_STM_App_Update_Service_1_Char_2*/
      break;

    default:
      break;
  }

  /* USER CODE BEGIN Custom_STM_App_Update_Char_2 */

  /* USER CODE END Custom_STM_App_Update_Char_2 */

  return ret;
}

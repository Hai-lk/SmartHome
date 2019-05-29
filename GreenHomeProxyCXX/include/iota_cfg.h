/**
 * @file iota_cfg.h
 * Indicates IOTA init Interface Functions
 */
#ifndef _IOTA_CFG_H_
#define _IOTA_CFG_H_

#include "hw_sys.h"

#ifdef __cplusplus
extern "C" {
#endif

#define IOTA_CFG_LOGID         _USTR("IOTA_CFG")

typedef enum enum_IOTA_CFG_TYPE
{
    EN_IOTA_CFG_DEVICEID     = 0,  //ƽ̨������߼��豸ID
    EN_IOTA_CFG_DEVICESECRET = 1,  //�豸����ļ�Ȩ��Կ
    EN_IOTA_CFG_APPID        = 2,  //������Ӧ��ID
    EN_IOTA_CFG_IOCM_ADDR    = 3,  //IoCM��������ַ
    EN_IOTA_CFG_IOCM_PORT    = 4,  //IoCM�������˿�
    EN_IOTA_CFG_MQTT_ADDR    = 5,  //MQTT��������ַ
    EN_IOTA_CFG_MQTT_PORT    = 6,  //MQTT�������˿�
    EN_IOTA_CFG_IODM_ADDR    = 7,  //IoDM��������ַ
    EN_IOTA_CFG_IODM_PORT    = 8,  //IoDM�������˿�
}EN_IOTA_CFG_TYPE;

HW_API_FUNC HW_INT IOTA_ConfigSetStr(HW_INT iItem, HW_CHAR *pValue);
HW_API_FUNC HW_INT IOTA_ConfigSetUint(HW_INT iItem, HW_UINT uiValue);

#ifdef __cplusplus
}
#endif

#endif


/**
 * @file Iota_Device.h
 * Indicates IOTA device control Interface Functions
 */
#ifndef _IOTA_DATATRANS_H_
#define _IOTA_DATATRANS_H_

#ifdef __cplusplus
extern "C" {
#endif


#define IOTA_TOPIC_SERVICE_DATA_REPORT_RET    "IOTA_TOPIC_SERVICE_DATA_REPORT_RET"
#define IOTA_TOPIC_SERVICE_COMMAND_RECEIVE    "IOTA_TOPIC_SERVICE_COMMAND_RECEIVE"
#define IOTA_TOPIC_DATATRANS_REPORT_RSP       "IOTA_TOPIC_DATATRANS_REPORT_RSP"

HW_API_FUNC HW_INT IOTA_ServiceDataReport(HW_UINT uiCookie, HW_CHAR *pcRequstId, 
            HW_CHAR *pcDeviceId, HW_CHAR *pcServiceId, HW_CHAR *pcServiceProperties);

typedef enum enum_EN_IOTA_DATATRANS_IE_TYPE
{
    EN_IOTA_DATATRANS_IE_RESULT     = 0,//nsigned int  ����ִ�з��ؽ��
    EN_IOTA_DATATRANS_IE_DEVICEID   = 1,//String        �豸ID
    EN_IOTA_DATATRANS_IE_REQUESTID  = 2,//String        ����ID
    EN_IOTA_DATATRANS_IE_SERVICEID  = 3,//String        ����ID
    EN_IOTA_DATATRANS_IE_METHOD     = 4,//String        ���񷽷�
    EN_IOTA_DATATRANS_IE_CMDCONTENT = 5,//String        ��������
}EN_IOTA_DATATRANS_IE_TYPE;

#ifdef __cplusplus
}
#endif

#endif

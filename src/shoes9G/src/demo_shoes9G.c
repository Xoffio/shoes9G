/*
 * @File  demo_gpsSimpleTestn.c
 * @Info:
 *      Main code of shoes9G. shoes GPS tracker.
 * @More info: https://ai-thinker-open.github.io/GPRS_C_SDK_DOC/en/c-sdk/first-code.html
 * 
 * @Author: Ricx8 
 * @Date: 01-20-2019 11:59:50 (Lunar eclipse) 
 * @Last Modified by: Ricx8
 * @Last Modified time: 01-22-2019 12:22:40
 */

#include "stdbool.h"
#include "stdint.h"

#include <string.h>
#include <stdio.h>
#include <api_os.h>
#include <api_event.h>
#include <api_socket.h>
#include <api_network.h>
#include <api_debug.h>
#include <api_gps.h>

//#include "api_os.h"
//#include "api_debug.h"
//#include "api_event.h"
#include "api_hal_gpio.h"
#include "gps_parse.h"
#include "gps.h"
#include "api_ssl.h"

/////////////////////////socket configuration////////////////////////
#define SERVER_IP           "caracascoder.com" // Replace this with the host to connect
#define SERVER_PORT         "443"   // Replace this with the port to connect
#define SERVER_PATH_POST    "/test/setLocationPOST.php" // [POST] Replace this with your path

// Replace the next certificate with the certificate of the page you will connet to.
const char* ca_cert = "-----BEGIN CERTIFICATE-----\n\
MIIFbDCCBFSgAwIBAgISA0gXQ0cnsv7p31+BZWAULSL1MA0GCSqGSIb3DQEBCwUA\n\
MEoxCzAJBgNVBAYTAlVTMRYwFAYDVQQKEw1MZXQncyBFbmNyeXB0MSMwIQYDVQQD\n\
ExpMZXQncyBFbmNyeXB0IEF1dGhvcml0eSBYMzAeFw0xODEyMDUwNTQyMjRaFw0x\n\
OTAzMDUwNTQyMjRaMBsxGTAXBgNVBAMTEGNhcmFjYXNjb2Rlci5jb20wggEiMA0G\n\
CSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQC65kt6A41pEicBD1xzlHRmZU3DfZcB\n\
lp8D/xLI2ye0uEKX4qexrsuUycyUnAu6RTUHs6zb5dnXdYHUyQj6cUhq/bDn1td8\n\
rpj3+m1tSAXSIFEoxG+oT9w+HbjGO6chitI07k7pR5UVwXNVuzEO1Oq3PqBrYK6I\n\
5EEjmW3ABS2NZlgAF/xaDHGaLswrOnbouT+tkrnGN74FRc6prNw5e+dBcdhJpu16\n\
9OqfrZo1/e8qLC3knuGBPJtY1m09vYlyMq/acW2G4b+6bLsdWHohPOijDxVvxy/X\n\
NG7W6/EkebsImS3UfiMCqnredmiI2TZs10dRHE2j7FBO3zvNfMaCmuzDAgMBAAGj\n\
ggJ5MIICdTAOBgNVHQ8BAf8EBAMCBaAwHQYDVR0lBBYwFAYIKwYBBQUHAwEGCCsG\n\
AQUFBwMCMAwGA1UdEwEB/wQCMAAwHQYDVR0OBBYEFFzYUb1zmdjpUc7mi4+pxv83\n\
Nl+zMB8GA1UdIwQYMBaAFKhKamMEfd265tE5t6ZFZe/zqOyhMG8GCCsGAQUFBwEB\n\
BGMwYTAuBggrBgEFBQcwAYYiaHR0cDovL29jc3AuaW50LXgzLmxldHNlbmNyeXB0\n\
Lm9yZzAvBggrBgEFBQcwAoYjaHR0cDovL2NlcnQuaW50LXgzLmxldHNlbmNyeXB0\n\
Lm9yZy8wMQYDVR0RBCowKIIQY2FyYWNhc2NvZGVyLmNvbYIUd3d3LmNhcmFjYXNj\n\
b2Rlci5jb20wTAYDVR0gBEUwQzAIBgZngQwBAgEwNwYLKwYBBAGC3xMBAQEwKDAm\n\
BggrBgEFBQcCARYaaHR0cDovL2Nwcy5sZXRzZW5jcnlwdC5vcmcwggECBgorBgEE\n\
AdZ5AgQCBIHzBIHwAO4AdQB0ftqDMa0zEJEhnM4lT0Jwwr/9XkIgCMY3NXnmEHvM\n\
VgAAAWd9GnQfAAAEAwBGMEQCIFHSIub5jcIkvtboWzMNDnC7mNJDoO4dhu3FfoMn\n\
gD2ZAiAGNaEo2olXcLYY3NnMMFfoTSvigsFvpFBUVt1g8l692AB1ACk8UZZUyDll\n\
uqpQ/FgH1Ldvv1h6KXLcpMMM9OVFR/R4AAABZ30adF4AAAQDAEYwRAIge+whROY5\n\
Z6nHBJPdofSd5zPpu5qSz8Vu75kyg78g7hICIBud81Z8D8dQXe9oJN2leOU/gfLv\n\
XwwI3NXcLx3A6QGGMA0GCSqGSIb3DQEBCwUAA4IBAQB48k0mw9GaR/YOazNCYGfl\n\
ZA5QhGPANVmCSHgE+6kXPWJXwMmPg1/IP8WL+PNdeid8S48aHt1hO+pEqUjdVF+Z\n\
FO2r397GDJ42CQVUIfPUx8C1s+ZLcsNaukqhyMgzzUc8jw9Fu1z6sPxalJpbLvje\n\
54XeL++s3jm4exnPQc8mkJyugCZUIWrlg2GrzCGAgAe7NJYlGW3UDlCpdHKrDZTp\n\
bS+2G9dRIhOWfVsUJ67bGfyE1xb2eq9aNrogc7kKFoeFM/Ye6dxWBKET1EAVtBuR\n\
tB5nq0N8j20gka7hpYoS99GrHXNyFO5rRJH0Of1jzEfT+S4hCDFG7tTbA5IDejVP\n\
-----END CERTIFICATE-----";


#define MAIN_TASK_STACK_SIZE    (2048 * 2)
#define MAIN_TASK_PRIORITY      0
#define MAIN_TASK_NAME          "Main Test Task"

#define SECOND_TASK_STACK_SIZE    (2048 * 2)
#define SECOND_TASK_PRIORITY      1
#define SECOND_TASK_NAME          "Second Test Task"

static HANDLE mainTaskHandle = NULL;
static HANDLE secondTaskHandle = NULL;
static HANDLE semStart = NULL;

//convert unit ddmm.mmmm to degree(°) 
double convertCoordinates(double nmeaValue, double nmeaScale){
    double  tmp = nmeaValue/nmeaScale/100.0;
    int     dd  = (int)tmp;
    double  mm  = (tmp - dd) * 100.0 / 60.0;

    tmp = dd+mm;
    if (tmp < 0) tmp*=-1;

    return(tmp);
}

//https POST
int Https_Post(const char* domain, const char* port,const char* path, const char* data, uint16_t dataLen, char* retBuffer, int* bufferLen){
    uint8_t buffer[2048];
    int retBufferLen = *bufferLen;
    int ret;
    SSL_Error_t error;
    SSL_Config_t config = {
        .caCert = ca_cert,
        .caCrl  = NULL,
        .clientCert = NULL,
        .clientKey  = NULL,
        .clientKeyPasswd = NULL,
        .hostName   = domain,
        .minVersion = SSL_VERSION_SSLv3,
        .maxVersion = SSL_VERSION_TLSv1_2,
        .verifyMode = SSL_VERIFY_MODE_OPTIONAL, // SSL_VERIFY_MODE_REQUIRED
        .entropyCustom = "GPRS"
    };

    // Build the package
    memset(retBuffer, 0, sizeof(retBuffer));
    snprintf(retBuffer,retBufferLen,"POST %s HTTP/1.1\r\nHost: %s\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: %d\r\n%s\r\n\r\n", path, domain, dataLen, data);
    char* pData = retBuffer;
    Trace(1,"#LOG: Package: %s", pData);

    error = SSL_Init(&config);
    if(error != SSL_ERROR_NONE){
        Trace(1,"#LOG: ssl init error:%d",error);
        return -1;
    }

    // Connect to server
    error = SSL_Connect(&config, domain, port);
    if(error != SSL_ERROR_NONE){
        Trace(1,"#LOG: ssl connect error:%d",error);
        Trace(1,"#LOG: ssl destroy");
        SSL_Destroy(&config);
        return -1;
    }

    // Send package
    Trace(1,"#LOG: Write len:%d data:%s", strlen(pData), pData);
    ret = SSL_Write(&config, pData, strlen(pData),5000);
    if(ret <= 0){
        error = ret;
        Trace(1,"#LOG: ssl write fail:%d",error);
        Trace(1,"#LOG: ssl close");
        SSL_Close(&config);
        return -1;
    }

    // Read response
    memset(buffer,0,sizeof(buffer));
    ret = SSL_Read(&config, buffer, strlen(pData), 2000);
    if(ret <= 0){
        error = ret;
        Trace(1,"#LOG: ssl read fail:%d",error);
        Trace(1,"#LOG: ssl close");
        SSL_Close(&config);
        return -1;
    }
    Trace(1,"#LOG: read len:%d, data:%s",ret,buffer);

    SSL_Close(&config);
    return 0;
}

void EventDispatch(API_Event_t* pEvent){
    switch(pEvent->id){
        case API_EVENT_ID_NO_SIMCARD:
            Trace(2, "#LOG: NO SIM CARD%d!",pEvent->param1);
            //networkFlag = false;
            break;

        case API_EVENT_ID_NETWORK_REGISTER_SEARCHING:
            Trace(2, "#LOG: Network register searching");
            //networkFlag = false;
            break;

        case API_EVENT_ID_NETWORK_REGISTER_DENIED:
            Trace(2, "#LOG: Network register denied.");
            break;

        case API_EVENT_ID_NETWORK_REGISTER_NO:
            Trace(2, "#LOG: no network register.");
            break;

        case API_EVENT_ID_GPS_UART_RECEIVED:
            // Trace(1,"received GPS data,length:%d, data:%s,flag:%d",pEvent->param1,pEvent->pParam1,flag);
            GPS_Update(pEvent->pParam1,pEvent->param1);
            break;

        case API_EVENT_ID_NETWORK_REGISTERED_HOME:
        case API_EVENT_ID_NETWORK_REGISTERED_ROAMING:{
            uint8_t status;

            Trace(2,"#LOG: network register success");
            bool ret = Network_GetAttachStatus(&status);

            if(!ret) Trace(1,"#LOG: get attach staus fail");
            Trace(1,"#LOG: attach status:%d",status);

            if(status == 0){
                ret = Network_StartAttach();
                if(!ret) Trace(1,"network attach fail");
            }
            else{
                Network_PDP_Context_t context = {
                    .apn        ="wholesale", // Replace this with your APN name
                    .userName   = ""    ,
                    .userPasswd = ""
                };
                Network_StartActive(context);
            }
            break;
        }
        case API_EVENT_ID_NETWORK_ATTACHED:{
            Trace(2,"#LOG: network attach success");
            Network_PDP_Context_t context = {
                .apn        ="wholesale", // Replace this with your APN name
                .userName   = ""    ,
                .userPasswd = ""
            };
            Network_StartActive(context);
            break;
        }

        case API_EVENT_ID_NETWORK_ACTIVATED:
            Trace(2,"#LOG: network activate success");
            OS_ReleaseSemaphore(semStart);
            break;
        default:
            break;
    }
}

// Secondary task where I'm going to execute everything.
void SecondTask(void *pData){
    
    GPS_Info_t* gpsInfo = Gps_GetInfo();
    char buffer[2048];
    char locationBuffer[40];
    int len = sizeof(buffer);
    int locationBufferLen = 25+11+1;

    // GPIO configuration
    /*GPIO_config_t gpioLedBlue = {
        .mode         = GPIO_MODE_OUTPUT,
        .pin          = GPIO_PIN27,
        .defaultLevel = GPIO_LEVEL_LOW
    };*/

    //GPIO_Init(gpioLedBlue); // Initialize GPIO

    // Open GPS hardware(UART2 open either)
    GPS_Init();
    GPS_Open(NULL);

    // Wait for gps start up, or gps will not response command
    while(gpsInfo->rmc.latitude.value == 0){
        Trace(1, "#LOG: GPS starting up...");
        OS_Sleep(1000);
    }

    //wait for gprs network connection ok
    Trace(1, "#LOG: waiting to connect to a network.");
    semStart = OS_CreateSemaphore(0);
    OS_WaitForSemaphore(semStart,OS_TIME_OUT_WAIT_FOREVER);
    OS_DeleteSemaphore(semStart);

    while(1){
        // Convert the coordinates
        double latitude =  convertCoordinates(gpsInfo->rmc.latitude.value, gpsInfo->rmc.latitude.scale);
        double longitude =  convertCoordinates(gpsInfo->rmc.longitude.value, gpsInfo->rmc.longitude.scale);

        snprintf(locationBuffer, locationBufferLen, "\r\nlocation=(%.6f N %.6f W)", latitude, longitude);
        Trace(1, "#LOG: %s", locationBuffer);
        //char* pData = retBuffer;

        if(Https_Post(SERVER_IP, SERVER_PORT, SERVER_PATH_POST, locationBuffer, locationBufferLen, buffer, &len) < 0){
            Trace(1,"http get fail");
        }
        OS_Sleep(5000);
    }
}

// Main task
void MainTask(void *pData){
    API_Event_t* event=NULL;

    // Create another task (secondary task).
    secondTaskHandle = OS_CreateTask(SecondTask,
        NULL, NULL, SECOND_TASK_STACK_SIZE, SECOND_TASK_PRIORITY, 0, 0, SECOND_TASK_NAME);

    while(1){
        // Wait for events from the lower system and is handle in EventDispatch
        if(OS_WaitEvent(mainTaskHandle, (void**)&event, OS_TIME_OUT_WAIT_FOREVER)){
            EventDispatch(event);
            OS_Free(event->pParam1);
            OS_Free(event->pParam2);
            OS_Free(event);
        }
    }
}

// Main entrance
void shoes9G_Main(void){
    // Create the main task. It will return the pointer os that task.
    mainTaskHandle = OS_CreateTask(MainTask,
        NULL, NULL, MAIN_TASK_STACK_SIZE, MAIN_TASK_PRIORITY, 0, 0, MAIN_TASK_NAME);
    OS_SetUserMainHandle(&mainTaskHandle); // Set the main handle.
}

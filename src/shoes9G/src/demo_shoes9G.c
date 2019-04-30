/*
 * @File  demo_gpsSimpleTestn.c
 * @Info:
 *      Main code of shoes9G. shoes GPS tracker.
 * @More info: https://ai-thinker-open.github.io/GPRS_C_SDK_DOC/en/c-sdk/first-code.html
 * 
 * @Author: Ricx8 
 * @Date: 01-20-2019 11:59:50 (Lunar eclipse) 
 * @Last Modified by: Ricx8
 * @Last Modified time: 04-29-2019 05:53 pm
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
#include "api_hal_pm.h"
#include "gps_parse.h"
#include "gps.h"
#include "api_ssl.h"

/////////////////////////socket configuration////////////////////////
#define SERVER_IP           "oikyo.com" // Replace this with the host to connect
#define SERVER_PORT         "443"   // Replace this with the port to connect
#define SERVER_PATH_POST    "/test/shoes9G/setLocationPOST.php" // [POST] Replace this with your path

// Replace the next certificate with the certificate of the page you will connet to.
const char* ca_cert = "-----BEGIN CERTIFICATE-----\n\
MIIFWzCCBEOgAwIBAgISA5xcctEV9oROKZhY/HuEipIYMA0GCSqGSIb3DQEBCwUA\n\
MEoxCzAJBgNVBAYTAlVTMRYwFAYDVQQKEw1MZXQncyBFbmNyeXB0MSMwIQYDVQQD\n\
ExpMZXQncyBFbmNyeXB0IEF1dGhvcml0eSBYMzAeFw0xOTAxMjgyMTI2MjBaFw0x\n\
OTA0MjgyMTI2MjBaMBQxEjAQBgNVBAMTCW9pa3lvLmNvbTCCASIwDQYJKoZIhvcN\n\
AQEBBQADggEPADCCAQoCggEBAMv3ujST4Kj6yfddwXF31cR2brIC0pGqNCwVI/xo\n\
rVFJdyszG1Qcx98M5/MERymDBUFrvrlK4Sv+w+x4sGx0DgikGn/iZOFOccnrI8Id\n\
7OXSpdi/tPEehYX0c0dOjEQozOjYg2ewOeWIwvDjpDG1ALQnJkC3DlUdSl9AjsKX\n\
Z3v5EPT8pupIhzKSGvdXyLZukik6il8Iyuf6+t1g38HhZbPTq8KLHszWIolTvTKG\n\
3wY+PkLkACZzj5W7289DonAOhHQcZLz+FhKNouj3AIbcdECuhaUHR7u7KidJx+Kp\n\
nRFQfgEyR8OSCbyJlOdLQgnjMuCi4JRmf0Udc1blS9k8OikCAwEAAaOCAm8wggJr\n\
MA4GA1UdDwEB/wQEAwIFoDAdBgNVHSUEFjAUBggrBgEFBQcDAQYIKwYBBQUHAwIw\n\
DAYDVR0TAQH/BAIwADAdBgNVHQ4EFgQUhGTX2EFV9zSSk/rn0THRHfyG1gkwHwYD\n\
VR0jBBgwFoAUqEpqYwR93brm0Tm3pkVl7/Oo7KEwbwYIKwYBBQUHAQEEYzBhMC4G\n\
CCsGAQUFBzABhiJodHRwOi8vb2NzcC5pbnQteDMubGV0c2VuY3J5cHQub3JnMC8G\n\
CCsGAQUFBzAChiNodHRwOi8vY2VydC5pbnQteDMubGV0c2VuY3J5cHQub3JnLzAj\n\
BgNVHREEHDAagglvaWt5by5jb22CDXd3dy5vaWt5by5jb20wTAYDVR0gBEUwQzAI\n\
BgZngQwBAgEwNwYLKwYBBAGC3xMBAQEwKDAmBggrBgEFBQcCARYaaHR0cDovL2Nw\n\
cy5sZXRzZW5jcnlwdC5vcmcwggEGBgorBgEEAdZ5AgQCBIH3BIH0APIAdwApPFGW\n\
VMg5ZbqqUPxYB9S3b79Yeily3KTDDPTlRUf0eAAAAWiWkgt0AAAEAwBIMEYCIQC+\n\
6pXhDeU95pJx1Qnw8Rz6iFxTjshChRCjzqqctA7EoQIhAI+0XASWuQpiR3ITv77O\n\
OcsmUEeJ8Ko+4MJxaVO8HJ2mAHcAdH7agzGtMxCRIZzOJU9CcMK//V5CIAjGNzV5\n\
5hB7zFYAAAFolpINNAAABAMASDBGAiEAwKYZ8o7qrQcc/RmGwpLCRj+t+IxNXqWF\n\
YibgEjCob/4CIQDtVBRag4Nkbg9CeqSywP40jsh1W8q1M0cTTh9TUWuXZzANBgkq\n\
hkiG9w0BAQsFAAOCAQEACP7bH/RNeRQP9KKg7LdBT/HZ/JqIOiE3woPpZoZEoTm3\n\
YB7lxcbj2YIfzd2R1YE1qtsWhUT+4Tv6K8OLIOgxdLXD0R7lh+F+WkOmA68SnN3V\n\
DUNdYE88f1r/nNAn8/nv6DtSnBB69PlHgqkZod6Uq1uRwOZVEtekTpXztYm6OitN\n\
bg+ldWG2XxQ6fXOEEXoU+KMx363quEHrImOIMZtrlRj0E6H9gzajwjKeIpmuAjxt\n\
RQrYvcbiT/9jdVYIMjESmv3FLOaNG8zhJQGdmIaI/JB0HSkjhRwbU1g1eahqRzVU\n\
rmhKqgapRhgYVpnFtzfXqJizB4HyCv6zrlQj8qg4gQ==\n\
-----END CERTIFICATE-----";

uint8_t pass[41] = "01"; // TMP password.

#define MAIN_TASK_STACK_SIZE    (2048 * 2)
#define MAIN_TASK_PRIORITY      0
#define MAIN_TASK_NAME          "Main Task"

#define SECOND_TASK_STACK_SIZE    (2048 * 2)
#define SECOND_TASK_PRIORITY      1
#define SECOND_TASK_NAME          "Second Test Task"

static HANDLE mainTaskHandle = NULL;
static HANDLE secondTaskHandle = NULL;
static HANDLE semStart = NULL;

unsigned int sslFailCounter = 0;
unsigned int sslFailRCounter = 0;
unsigned int sslFailReset = 4; // Restart chip after failing 'sslFailReset' times consecutive

// GPIO configuration
GPIO_config_t gpioLedBlue = {
    .mode         = GPIO_MODE_OUTPUT,
    .pin          = GPIO_PIN27,
    .defaultLevel = GPIO_LEVEL_LOW
};

// Blink function
// Led Will blink nTimes per nMilli
void blinkLed(GPIO_config_t led, UINT32 nMilli, uint16_t nTimes){
    if (nTimes > 1){
        nMilli = nMilli/nTimes/2;
        for (int i=0; i<nTimes; i++){
            GPIO_SetLevel(led, GPIO_LEVEL_HIGH);
            OS_Sleep(nMilli);
            GPIO_SetLevel(led, GPIO_LEVEL_LOW);
            OS_Sleep(nMilli);
        }
    }
    else{
        GPIO_SetLevel(led, GPIO_LEVEL_HIGH);
        OS_Sleep(nMilli);
        GPIO_SetLevel(led, GPIO_LEVEL_LOW);
    }
}

//convert unit ddmm.mmmm to degree(°) 
double convertCoordinates(double nmeaValue, double nmeaScale){
    // nmeaValue=40408191, nmeaScale=10000
    double  tmp = nmeaValue/nmeaScale/100.0;
    int     dd  = (int)tmp;
    double  mm  = (tmp - dd) * 100.0 / 60.0;

    tmp = dd+mm;

    return(tmp);
}

//https POST
int Https_Post(const char* domain, const char* port,const char* path, const char* data, uint16_t dataLen, uint8_t* retBuffer, int bufferLen){
    const int bufferSize = 2048;

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
    char* buffer = OS_Malloc(bufferSize);
    snprintf(buffer, bufferSize, "POST %s HTTP/1.1\r\nHost: %s\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: %d\r\n%s \r\n\r\n", path, domain, (dataLen-2), data);

    char* pData = buffer;
    Trace(1,"#LOG: Package: %s", pData);

    error = SSL_Init(&config);
    if(error != SSL_ERROR_NONE){
        Trace(1,"#LOG: ssl init error:%d",error);
        OS_Free(buffer);
        return -1;
    }
    else{
        // If SSL init, Connect to server
        error = SSL_Connect(&config, domain, port); // BUG HERE: crash after 8min aprox.
        if(error != SSL_ERROR_NONE){
            Trace(1,"#LOG: ssl connect error:%d",error);
            sslFailCounter++;
            goto exit02;
        }
        else{
            // If it is connected, Send package
            Trace(1, "#LOG: Connected to the server with ssl");
            
            Trace(1,"#LOG: Write len:%d data:%s", strlen(pData), pData);
            ret = SSL_Write(&config, pData, strlen(pData),5000);
            if(ret <= 0){
                error = ret;
                Trace(1,"#LOG: ssl write fail:%d",error);
                goto exit01;
            }
            else{
                blinkLed(gpioLedBlue, 200, 1);
                //int retBufferLen = *bufferLen;

                // Reset the sslFailCounter since the package is sent
                sslFailCounter = 0;

                // If package sent then read response.
                memset(retBuffer, 0, sizeof(retBuffer[0])*bufferLen);
                ret = SSL_Read(&config, retBuffer, bufferLen, 2000);
                if(ret <= 0){
                    error = ret;
                    Trace(1,"#LOG: ssl read fail:%d",error);
                    sslFailRCounter++;
                    goto exit01;
                }
                else{
                    sslFailRCounter = 0;
                }
                Trace(1,"#LOG: read len:%d, data:%s", ret, retBuffer);
            }
        }
    }

    exit01:
        Trace(1,"#LOG: ssl close");
        SSL_Close(&config);
    exit02:
        Trace(1,"#LOG: ssl destroy");
        SSL_Destroy(&config);
    OS_Free(buffer);

    // Restart the chip if can not be connected 'sslFailReset' times consecutive
    if (sslFailCounter >= sslFailReset || sslFailRCounter >= sslFailReset){
        Trace(1, "#LOG: Restarting chip.");
        PM_Restart();
    }

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
            //Trace(1,"#LOG: received GPS data,length:%d, data:%s",pEvent->param1,pEvent->pParam1);
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
    int bufferSize = 2048;
    uint8_t buffer[bufferSize];
    uint8_t imei[16];
    int locationBufferLen = 88; // 3 \r\n. 45 of latitude, longitude. 40 of password
    char locationBuffer[locationBufferLen+2]; 
    uint8_t vPercent;

    GPIO_Init(gpioLedBlue); // Initialize GPIO

    // Get imei
    //memset(imei, 0, sizeof(imei));
    //INFO_GetIMEI(imei);

    // Open GPS hardware(UART2 open either)
    GPS_Init();
    GPS_Open(NULL);

    // Wait for gps start up, or gps will not response command
    while(gpsInfo->rmc.latitude.value == 0){
        Trace(1, "#LOG: GPS starting up...");
        OS_Sleep(1000);
    }

    // Set gps nmea output interval
    for(uint8_t i = 0;i<5;++i){
        bool ret = GPS_SetOutputInterval(10000);
        Trace(1,"#LOG: set gps ret:%d",ret);
        if(ret)
            break;
        OS_Sleep(1000);
    }

    // Set the GPS to Fix mode (Needs at least 4 satellites to get accuate position and time)
    if (!GPS_SetFixMode(GPS_FIX_MODE_NORMAL))//GPS_FIX_MODE_LOW_SPEED
        Trace(1, "#LOG: set fix mode fail");
    else
        Trace(1, "#LOG: set fix mode");

    // Set gps nmea output interval
    if(!GPS_SetOutputInterval(1000))
        Trace(1,"set nmea output interval fail");

    //wait for gprs network connection ok
    Trace(1, "#LOG: waiting to connect to a network.");
    blinkLed(gpioLedBlue, 2000, 10);
    semStart = OS_CreateSemaphore(0);
    OS_WaitForSemaphore(semStart,OS_TIME_OUT_WAIT_FOREVER);
    OS_DeleteSemaphore(semStart);

    // Low power mode. Frequency = 32kHz
    PM_SleepMode(true);

    while(1){
        uint8_t status;
        Network_GetActiveStatus(&status);
        int nOfSatellites = gpsInfo->gga.satellites_tracked;

        Trace(1, "#LOG: number of satellites tracked: %i", nOfSatellites);

        uint16_t v = PM_Voltage(&vPercent);
        Trace(1,"voltage:%dmV,%d percent last,times count:",v,vPercent);

        // Send location if is connected to the network and the number of satellites tracked are more than 3
        if (status && (nOfSatellites > 3)){
            // Convert the coordinates
            double latitude =  convertCoordinates(gpsInfo->rmc.latitude.value, gpsInfo->rmc.latitude.scale);
            double longitude = convertCoordinates(gpsInfo->rmc.longitude.value, gpsInfo->rmc.longitude.scale);

            // Do an average of locations to avoid unexpected values.
            /*int nAverage = 5;
            float sumSatellites = 0;
            double latitude =  0, longitude =  0, lastLat = 0, lastLon = 0;
            for (int nTime=1; nTime<=nAverage; nTime++){
                nOfSatellites = gpsInfo->gga.satellites_tracked;
                sumSatellites += nOfSatellites;

                if ((nOfSatellites >= (sumSatellites/nTime)-2) && (nOfSatellites > 3)){
                    // Convert the coordinates
                    lastLat = convertCoordinates(gpsInfo->rmc.latitude.value, gpsInfo->rmc.latitude.scale);
                    lastLon = convertCoordinates(gpsInfo->rmc.longitude.value, gpsInfo->rmc.longitude.scale);
                    
                    latitude += lastLat;
                    longitude += lastLon;
                }
                else{
                    if (lastLat == 0 && lastLon == 0 ){ // Future problem...
                        Trace(1," ERROR!! CHECK THE CODE #01.");
                    }
                    else{
                        latitude += lastLat;
                        longitude += lastLon;
                    }
                }

                OS_Sleep(1000);
            }

            nOfSatellites = (int)(sumSatellites/nAverage);
            latitude /= nAverage;
            longitude /= nAverage;*/

            snprintf(locationBuffer, locationBufferLen, "\r\nd=%s,%.6f,%.6f,%i,%i,%i", pass, latitude, longitude, nOfSatellites, v, vPercent);
            Trace(1, "#LOG: %s", locationBuffer);

            blinkLed(gpioLedBlue, 200, 2);

            if(Https_Post(SERVER_IP, SERVER_PORT, SERVER_PATH_POST, locationBuffer, strlen(locationBuffer), buffer, bufferSize) < 0){
                Trace(1,"http get fail");
            }

            OS_Sleep(15000); // Send coordinates 4 times per minute
        }
        else{
            OS_Sleep(1000);
        }
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

#include "server_setup.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/api.h"

#include "libesphttpd/httpdespfs.h"
#include "libesphttpd/cgiwifi.h"
#include "libesphttpd/cgiflash.h"
#include "libesphttpd/auth.h"
#include "libesphttpd/espfs.h"
#include "libesphttpd/captdns.h"
#include "libesphttpd/webpages-espfs.h"
#include "libesphttpd/cgiwebsocket.h"
#include "libesphttpd/httpd-freertos.h"
#include "libesphttpd/route.h"

#include "stepper.h"
#include "cgi.h"

#define BEEBOT_SSID               CONFIG_ESP_WIFI_SSID
#define BEEBOT_WIFI_PASS          CONFIG_ESP_WIFI_PASSWORD
#define BEEBOT_MAX_STA_CONN       CONFIG_MAX_STA_CONN

#define LISTEN_PORT     80u
#define MAX_CONNECTIONS 32u

// WIFI STATION START
static const char *TAG = "wifi setup";
static int s_retry_num = 0;
const static int WIFI_CONNECTED_BIT = BIT0;
const static int maximum_retry = 5;

static EventGroupHandle_t s_wifi_event_group;
static HttpdFreertosInstance httpdFreertosInstance;
static char connectionMemory[sizeof(RtosConnType) * MAX_CONNECTIONS];

HttpdBuiltInUrl cgiUrls[]={
    ROUTE_REDIRECT("/", "/index.tpl"),
    ROUTE_CGI("/connect.cgi", cgiParseWifiInfo),
    ROUTE_CGI("/resetnvs.cgi", cgiResetNVS),
    ROUTE_REDIRECT("/control", "/controls.tpl"),
    ROUTE_CGI("/sendsteps.cgi", cgiSendSteps),

    ROUTE_FILESYSTEM(),

    ROUTE_END()
};


void clearNVS()
{
    nvs_handle my_handle;
    esp_err_t ret;
    ret = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (ret != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(ret));
    }

    else
    {
        nvs_erase_all(my_handle);
        nvs_commit(my_handle);
    }
}

static esp_err_t s_event_handler(void *ctx, system_event_t *event)
{
    switch(event->event_id) {
    case SYSTEM_EVENT_STA_START:
        esp_wifi_connect();
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
        ESP_LOGI(TAG, "got ip:%s",
                 ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        {
            if (s_retry_num < maximum_retry) {
                esp_wifi_connect();
                xEventGroupClearBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
                s_retry_num++;
                ESP_LOGI(TAG,"retry to connect to the AP");
            }
            else {
                ESP_LOGI(TAG,"Trying to switch to AP mode");
                clearNVS();
                esp_restart();
            }
            ESP_LOGI(TAG,"connect to the AP fail\n");
            break;
        }
    default:
        break;
    }
    return ESP_OK;
}

void wifi_init_sta(char * m_ssid, char * m_password)
{
    s_wifi_event_group = xEventGroupCreate();

    tcpip_adapter_init();
    ESP_ERROR_CHECK(esp_event_loop_init(s_event_handler, NULL) );

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    wifi_config_t wifi_config = {
        .sta = {
        },
    };
    strcpy((char *)wifi_config.sta.ssid,m_ssid);
    strcpy((char *)wifi_config.sta.password,m_password);
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
    ESP_ERROR_CHECK(esp_wifi_start() );

    ESP_LOGI(TAG, "wifi_init_sta finished.");
    ESP_LOGI(TAG, "connect to ap SSID:%s password:%s",
             BEEBOT_SSID, BEEBOT_WIFI_PASS);
}


void setup_sta_mode(char *ssid,char *password)
{
        ESP_LOGI(TAG, "ESP_WIFI_MODE_STA");
        wifi_init_sta(ssid,password);
        // SET GPIO FOR STEPPER OUTPUT
        initStepper();
        // INIT HTTPD
        espFsInit((void*)(webpages_espfs_start));
        tcpip_adapter_init();
        httpdFreertosInit(&httpdFreertosInstance,
                          cgiUrls,
                          LISTEN_PORT,
                          connectionMemory,
                          MAX_CONNECTIONS,
                          HTTPD_FLAG_NONE);
        httpdFreertosStart(&httpdFreertosInstance);
}


// WIFI STATION END

// WIFI ACCESS POINT START

/* FreeRTOS event group to signal when we are connected*/

static esp_err_t ap_event_handler(void *ctx, system_event_t *event)
{
    switch(event->event_id) {
    case SYSTEM_EVENT_AP_STACONNECTED:
        ESP_LOGI(TAG, "station: "MACSTR" join, AID=%d",
                 MAC2STR(event->event_info.sta_connected.mac),
                 event->event_info.sta_connected.aid);
        break;
    case SYSTEM_EVENT_AP_STADISCONNECTED:
        ESP_LOGI(TAG, "station: "MACSTR" leave, AID=%d",
                 MAC2STR(event->event_info.sta_disconnected.mac),
                 event->event_info.sta_disconnected.aid);
        break;
    default:
        break;
    }
    return ESP_OK;
}


void wifi_init_softap()
{
    s_wifi_event_group = xEventGroupCreate();

    tcpip_adapter_init();
    ESP_ERROR_CHECK(esp_event_loop_init(ap_event_handler, NULL));

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    wifi_config_t wifi_config = {
        .ap = {
            .ssid = BEEBOT_SSID,
            .ssid_len = strlen(BEEBOT_SSID),
            .password = BEEBOT_WIFI_PASS,
            .max_connection = BEEBOT_MAX_STA_CONN,
            .authmode = WIFI_AUTH_WPA_WPA2_PSK
        },
    };
    if (strlen(BEEBOT_WIFI_PASS) == 0) {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    }

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "wifi_init_softap finished.SSID:%s password:%s",
             BEEBOT_SSID, BEEBOT_WIFI_PASS);
}


void setup_ap_mode() {
    ESP_LOGI(TAG, "ESP_WIFI_MODE_AP");
    wifi_init_softap();
    espFsInit((void*)(webpages_espfs_start));
    // httpd init
    tcpip_adapter_init();
    httpdFreertosInit(&httpdFreertosInstance,
                        cgiUrls,
                        LISTEN_PORT,
                        connectionMemory,
                        MAX_CONNECTIONS,
                        HTTPD_FLAG_NONE);
    httpdFreertosStart(&httpdFreertosInstance);
}
// WIFI ACCESS POINT END
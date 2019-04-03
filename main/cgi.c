#include <libesphttpd/esp.h>
#include "cgi.h"
#include "perform.h"
#include "nvs_flash.h"
#include "stepper.h"


//Cgi that turns the LED on or off according to the 'led' param in the POST data
CgiStatus ICACHE_FLASH_ATTR cgiParseWifiInfo(HttpdConnData *connData) {
	char ssid[128];
	char pass[128];

	if (connData->isConnectionClosed) {
		return HTTPD_CGI_DONE;
	}
	httpdFindArg(connData->post.buff, "ssid", ssid, sizeof(ssid));
	httpdFindArg(connData->post.buff, "pass", pass, sizeof(pass));

	//strncpy((char*)stconf.ssid, ssid, 32);
	//strncpy((char*)stconf.password, pass, 64);

    //Initialize NVS
    nvs_handle my_handle;
    esp_err_t ret;
    ret = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (ret != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(ret));
    }

    else
    {

        ret = nvs_set_str(my_handle, "ssid", ssid);
        ret = nvs_set_str(my_handle, "password", pass);
    }

    esp_restart();
	httpdRedirect(connData, "connected.tpl");
	return HTTPD_CGI_DONE;
}

CgiStatus ICACHE_FLASH_ATTR cgiResetNVS(HttpdConnData *connData) {
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
	httpdRedirect(connData, "index.tpl");
	return HTTPD_CGI_DONE;
}

CgiStatus ICACHE_FLASH_ATTR cgiSendSteps(HttpdConnData *connData) {
	char steps[128];
	httpdFindArg(connData->post.buff, "steps", steps, sizeof(steps));
	printf("Steps -> %s \n", steps);
	prepair_program(steps);
	return HTTPD_CGI_DONE;
}

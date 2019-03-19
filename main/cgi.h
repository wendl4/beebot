#ifndef CGI_H
#define CGI_H

#include "libesphttpd/httpd.h"

CgiStatus cgiParseWifiInfo(HttpdConnData *connData);
CgiStatus cgiResetNVS(HttpdConnData *connData);
CgiStatus cgiSendSteps(HttpdConnData *connData);
CgiStatus tplLed(HttpdConnData *connData, char *token, void **arg);

#endif

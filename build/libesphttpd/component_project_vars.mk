# Automatically generated build file. Do not edit.
COMPONENT_INCLUDES += $(IDF_PATH)/components/libesphttpd/core $(IDF_PATH)/components/libesphttpd/espfs $(IDF_PATH)/components/libesphttpd/util $(IDF_PATH)/components/libesphttpd/include $(IDF_PATH)/components/libesphttpd/lib/heatshrink
COMPONENT_LDFLAGS += -L$(BUILD_DIR_BASE)/libesphttpd -lwebpages-espfs -llibesphttpd
COMPONENT_LINKER_DEPS += 
COMPONENT_SUBMODULES += 
COMPONENT_LIBRARIES += libesphttpd
component-libesphttpd-build: 

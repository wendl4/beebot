deps_config := \
	/Users/sam/esp/esp-idf/components/app_trace/Kconfig \
	/Users/sam/esp/esp-idf/components/aws_iot/Kconfig \
	/Users/sam/esp/esp-idf/components/bt/Kconfig \
	/Users/sam/esp/esp-idf/components/driver/Kconfig \
	/Users/sam/esp/esp-idf/components/esp32/Kconfig \
	/Users/sam/esp/esp-idf/components/esp_adc_cal/Kconfig \
	/Users/sam/esp/esp-idf/components/esp_http_client/Kconfig \
	/Users/sam/esp/esp-idf/components/ethernet/Kconfig \
	/Users/sam/esp/esp-idf/components/fatfs/Kconfig \
	/Users/sam/esp/esp-idf/components/freertos/Kconfig \
	/Users/sam/esp/esp-idf/components/heap/Kconfig \
	/Users/sam/esp/esp-idf/components/http_server/Kconfig \
	/Users/sam/esp/esp-idf/components/libesphttpd/Kconfig \
	/Users/sam/esp/esp-idf/components/libsodium/Kconfig \
	/Users/sam/esp/esp-idf/components/log/Kconfig \
	/Users/sam/esp/esp-idf/components/lwip/Kconfig \
	/Users/sam/esp/esp-idf/components/mbedtls/Kconfig \
	/Users/sam/esp/esp-idf/components/mdns/Kconfig \
	/Users/sam/esp/esp-idf/components/mqtt/Kconfig \
	/Users/sam/esp/esp-idf/components/openssl/Kconfig \
	/Users/sam/esp/esp-idf/components/pthread/Kconfig \
	/Users/sam/esp/esp-idf/components/spi_flash/Kconfig \
	/Users/sam/esp/esp-idf/components/spiffs/Kconfig \
	/Users/sam/esp/esp-idf/components/tcpip_adapter/Kconfig \
	/Users/sam/esp/esp-idf/components/vfs/Kconfig \
	/Users/sam/esp/esp-idf/components/wear_levelling/Kconfig \
	/Users/sam/esp/esp-idf/components/bootloader/Kconfig.projbuild \
	/Users/sam/esp/esp-idf/components/esptool_py/Kconfig.projbuild \
	/Users/sam/Documents/diplomovka/kod/beebot/main/Kconfig.projbuild \
	/Users/sam/esp/esp-idf/components/partition_table/Kconfig.projbuild \
	/Users/sam/esp/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)

ifneq "$(IDF_CMAKE)" "n"
include/config/auto.conf: FORCE
endif

$(deps_config): ;

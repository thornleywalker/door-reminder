set(srcs "main.c"
         "bluetooth.c"
         "data_cache.c"
         "database_proxy.c"
         "sensor.c"
         "wifi.c")

set(include_dirs ".")

idf_component_register(SRCS "${srcs}"
                       INCLUDE_DIRS "${include_dirs}"
                       REQUIRES esp_hid)

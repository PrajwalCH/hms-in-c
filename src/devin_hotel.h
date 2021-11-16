#ifndef DEVIN_HOTEL_H

#define DEVIN_HOTEL_H

#define LOG_ERR_FMT(fmt, ...) \
    fprintf(stderr, "\033[1;31mError:\033[0m \033[1;37m"fmt"\033[0m\n", __VA_ARGS__);

#define LOG_ERR(m) \
    LOG_ERR_FMT("%s", m)

#define DATA_FILES_DIR "./data_files"
#define ROOMS_DATA_FILENAME "rooms.dat"
#define CUSTOMERS_DATA_FILENAME "customers.dat"

#endif /* DEVIN_HOTEL_H */


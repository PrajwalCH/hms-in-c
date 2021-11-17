#ifndef CUSTOMER_H

#define CUSTOMER_H

#include <stdbool.h>

struct Customer {
    int  id;
    char firstname[21];
    char lastname[21];
    char address[21];
    char phnumber[11];
    char email[51];
};

void customer_take_details(struct Customer *customer);
struct Customer customer_New(void);
void customer_add_new_record(struct Customer *new_customer);
bool customer_is_record_exist(int id_to_lookup);
void customer_show_all_records(void);
void customer_search_record_by_id(int id_to_lookup);
void customer_search_record_by_name(const char *firstname, const char *lastname);
void customer_update_record(int id_to_update);
void customer_delete_record(int id_to_delete);
void customer_delete_all_records(void);

#endif /* CUSTOMER_H */


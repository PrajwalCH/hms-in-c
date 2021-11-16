#include "customer.h"

#include <stdio.h>
#include <string.h>

#include "devin_hotel.h"
#include "util.h"

static void print_customer_data(struct Customer *customer)
{
    printf("%-2d | ", customer->id);
    printf("%-20s | ", customer->firstname);
    printf("%-20s | ", customer->lastname);
    printf("%-20s | ", customer->address);
    printf("%-10s | ", customer->phnumber);
    printf("%s |\n", customer->email);
}

static const char *temp_file_name(void)
{
    const char *path = DATA_FILES_DIR"/"CUSTOMERS_DATA_FILENAME".temp";
    return path;
}

static const char *data_file_name(void)
{
    const char *path = DATA_FILES_DIR"/"CUSTOMERS_DATA_FILENAME;
    return path;
}

static bool open_data_file(FILE **data_file)
{
    *data_file = fopen(data_file_name(), "r");

    if (*data_file == NULL) {
        printf("Error: customers data file '%s' not found on '%s' dir", CUSTOMERS_DATA_FILENAME, DATA_FILES_DIR);
        return false;
    }

    if (is_file_empty(*data_file)) {
        fclose(*data_file);
        printf("Empty records\n");
        return false;
    }

    return true;
}

typedef bool (*CallbackFn)(FILE *, struct Customer *, int);
static bool modify_data_file(int id, CallbackFn cb)
{
    FILE *data_file = NULL;
    FILE *temp_file = NULL;

    if (!open_data_file(&data_file))
        return false;

    temp_file = fopen(temp_file_name(), "w");
    if (temp_file == NULL) {
        printf("Error: failed to create or open temp file\n");
        return false;
    }

    bool has_record = false;
    struct Customer customer_old_details = customer_New();

    while (fread(&customer_old_details, sizeof(struct Customer), 1, data_file) != 0) {
        if (cb(temp_file, &customer_old_details, id)) {
            has_record = true;
            continue;
        }
        fwrite(&customer_old_details, sizeof(struct Customer), 1, temp_file);
    }
    fclose(data_file);
    fclose(temp_file);

    remove(data_file_name());
    rename(temp_file_name(), data_file_name());

    if (!has_record) {
        printf("customer with id '%d' not found on records\n", id);
        return false;
    }

    return true;
}

struct Customer customer_New(void)
{
    return (struct Customer) {
        .id = 0,
        .firstname = {0},
        .lastname = {0},
        .address = {0},
        .phnumber = {0},
        .email = {0}
    };
}

void customer_take_details(struct Customer *customer)
{
    print_header("ADD CUSTOMER DETAILS");

    /*
     * don'use fflush(stdin) to clear the input buffer
     * as per the c standard the behavior is undefined
     *
     * link:
     * http://port70.net/~nsz/c/c11/n1570.html#7.21.5.2
     *
     * stackoverflow question:
     * https://stackoverflow.com/questions/2979209/using-fflushstdin#:~:text=According%20to%20the%20C%20standard,not%20use%20fflush(stdin)%20.
     */

    bool is_existing_id = false;
    do {
        flush_input_buffer();
        printf("Id: ");
        scanf("%d", &customer->id);

        if (customer_is_record_exist(customer->id)) {
            printf("Error: id already exist\n");
            is_existing_id = true;
            continue;
        } else {
            is_existing_id = false;
        }

        printf("First Name: ");
        scanf("%20s", customer->firstname);
        flush_input_buffer();

        printf("Last Name: ");
        scanf("%20s", customer->lastname);
        flush_input_buffer();

        printf("Address: ");
        scanf("%20s", customer->address);
        flush_input_buffer();

        printf("Ph Number: ");
        scanf("%10s", customer->phnumber);
        flush_input_buffer();

        printf("Email: ");
        scanf("%50s", customer->email);
        flush_input_buffer();
    } while (is_existing_id);
}

void customer_add_new_record(struct Customer *new_customer)
{
    FILE *data_file = fopen(data_file_name(), "a");

    if (data_file == NULL) {
        printf("Error: customers data file '%s' not found on '%s' dir\n", CUSTOMERS_DATA_FILENAME, DATA_FILES_DIR);
        return;
    }

    if (fwrite(new_customer, sizeof(struct Customer), 1, data_file) != 1) {
        printf("Error: failed to add new customer record\n");
    }
    fclose(data_file);
}

bool customer_is_record_exist(int id_to_lookup)
{
    struct Customer customer = customer_New();
    FILE *data_file = fopen(data_file_name(), "r");

    if (data_file == NULL)
        return false;

    if (is_file_empty(data_file)) {
        fclose(data_file);
        return false;
    }

    while (fread(&customer, sizeof(struct Customer), 1, data_file) != 0) {
        if (customer.id == id_to_lookup) {
            fclose(data_file);
            return true;
        }
    }
    fclose(data_file);
    return false;
}

void customer_show_all_records(void)
{
    print_header("ALL CUSTOMERS RECORD");

    struct Customer customer = customer_New();
    FILE *data_file = NULL;

    if (!open_data_file(&data_file))
        return;

    int num_chars_printed = printf("%-2s | %-20s | %-20s | %-20s | %-10s | %s\n", "Id", "First name", "Last name", "Address", "Ph number", "Email");
    print_horizontal_line(num_chars_printed);

    while (fread(&customer, sizeof(struct Customer), 1, data_file) != 0) {
        print_customer_data(&customer);
    }
    fclose(data_file);
}

void customer_search_record_by_id(int id_to_lookup)
{
    print_header("SEARCH RESULT");

    struct Customer customer = customer_New();
    FILE *data_file = NULL;

    if (!open_data_file(&data_file))
        return;

    while (fread(&customer, sizeof(struct Customer), 1, data_file) != 0) {
        if (customer.id == id_to_lookup) {
            print_customer_data(&customer);
            fclose(data_file);
            return;
        }
    }
    fclose(data_file);
    printf("customer with id '%d' not found on records\n", id_to_lookup);
}

void customer_search_record_by_name(const char *firstname, const char *lastname)
{
    print_header("SEARCH RESULT");
    struct Customer customer = customer_New();
    FILE *data_file = NULL;

    if (!open_data_file(&data_file))
        return;

    while (fread(&customer, sizeof(struct Customer), 1, data_file) != 0) {
        if (strcmp(customer.firstname, firstname) == 0 && strcmp(customer.lastname, lastname) == 0) {
            print_customer_data(&customer);
            fclose(data_file);
            return;
        }
    }
    fclose(data_file);
    printf("customer with first name '%s' and last name '%s' not found on records\n", firstname, lastname);
}

static bool update_callback(FILE *temp_file, struct Customer *customer_old_details, int id)
{
    if (customer_old_details->id == id) {
        struct Customer customerNewDetails = customer_New();

        customer_take_details(&customerNewDetails);
        fwrite(&customerNewDetails, sizeof(struct Customer), 1, temp_file);
        return true;
    }
    return false;
}

void customer_update_record(int id_to_update)
{
    if (modify_data_file(id_to_update, update_callback))
        printf("\nupdate success\n");
}

static bool delete_callback(FILE *temp_file, struct Customer *customer_old_details, int id)
{
    (void) temp_file; // to supress unused argument warning

    if (customer_old_details->id == id)
        return true;
    return false;
}

void customer_delete_record(int id_to_delete)
{
    if (modify_data_file(id_to_delete, delete_callback))
        printf("\ndelete success\n");
}

void customer_delete_all_records(void)
{
    if (remove(data_file_name()) == -1) {
        printf("Error: failed to delete all records\n");
        return;
    }

    FILE *new_data_file = fopen(data_file_name(), "w");

    if (new_data_file == NULL) {
        printf("Error: failed to create new data file\n");
        return;
    }
    printf("\nDeleted all customers record\n");
}

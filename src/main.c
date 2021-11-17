#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define USERNAME_LEN 4
#define PASSWORD_LEN 7

struct Customer {
    int  id;
    char firstname[21];
    char lastname[21];
    char address[21];
    char phnumber[11];
    char email[51];
} customer;

char data_file_name[] = "./data_files/customers.dat";
char temp_file_name[] = "./data_files/customers.dat.tmp";

/*-----UTILITY FUNCTIONS-----*/

void flush_input_buffer(void)
{
    int ch = 0;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

void print_horizontal_line(int width)
{
    for (int i = 0; i < width; i++)
        printf("-");
    printf("\n");
}

void print_header(const char *txt)
{
    int num_chars_printed = printf("\n %s\n", txt);
    print_horizontal_line(num_chars_printed);
}

bool is_file_empty(FILE *fp)
{
    fseek(fp, 0, SEEK_END);
    long chunk_size = ftell(fp);

    if (chunk_size == 0) {
        rewind(fp);
        return true;
    }
    rewind(fp);
    return false;
}

/*-----CUSTOMER RELATED FUNCTIONS-----*/

void print_table_header(void)
{
    int num_chars_printed = printf("%-2s | %-20s | %-20s | %-20s | %-10s | %s\n", "Id", "First name", "Last name", "Address", "Ph number", "Email");
    print_horizontal_line(num_chars_printed);
}

void print_customer_data(struct Customer *customer)
{
    printf("%-2d | ", customer->id);
    printf("%-20s | ", customer->firstname);
    printf("%-20s | ", customer->lastname);
    printf("%-20s | ", customer->address);
    printf("%-10s | ", customer->phnumber);
    printf("%s |\n", customer->email);
}

bool customer_is_record_exist(int id_to_lookup)
{
    FILE *data_file = fopen(data_file_name, "r");

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

void customer_take_details(struct Customer *customer)
{
    print_header("ADD CUSTOMER DETAILS");

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
    FILE *data_file = fopen(data_file_name, "a");

    if (data_file == NULL) {
        printf("Error: customers data file '%s' not found\n", data_file_name);
        return;
    }

    if (fwrite(new_customer, sizeof(struct Customer), 1, data_file) != 1) {
        printf("Error: failed to add new customer record\n");
    }
    fclose(data_file);
}

void customer_show_all_records(void)
{
    print_header("ALL CUSTOMERS RECORD");

    FILE *data_file = fopen(data_file_name, "r");

    if (data_file == NULL) {
        printf("Error: customers data file '%s' not found", data_file_name);
        return;
    }

    if (is_file_empty(data_file)) {
        fclose(data_file);
        printf("Empty records\n");
        return;
    }

    print_table_header();
    while (fread(&customer, sizeof(struct Customer), 1, data_file) != 0) {
        print_customer_data(&customer);
    }
    fclose(data_file);
}

void customer_search_record_by_id(int id_to_lookup)
{
    print_header("SEARCH RESULT");
    FILE *data_file = fopen(data_file_name, "r");

    if (data_file == NULL) {
        printf("Error: customers data file '%s' not found", data_file_name);
        return;
    }

    if (is_file_empty(data_file)) {
        fclose(data_file);
        printf("Empty records\n");
        return;
    }

    print_table_header();
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
    FILE *data_file = fopen(data_file_name, "r");

    if (data_file == NULL) {
        printf("Error: customers data file '%s' not found", data_file_name);
        return;
    }

    if (is_file_empty(data_file)) {
        fclose(data_file);
        printf("Empty records\n");
        return;
    }

    print_table_header();
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

void customer_update_record(int id_to_update)
{
    FILE *data_file = NULL;
    FILE *temp_file = NULL;

    data_file = fopen(data_file_name, "r");

    if (data_file == NULL) {
        printf("Error: customers data file '%s' not found\n", data_file_name);
        return;
    }

    if (is_file_empty(data_file)) {
        printf("Empty records\n");
        fclose(data_file);
        return;
    }

    temp_file = fopen(temp_file_name, "w");

    if (temp_file == NULL) {
        printf("Error: customers temp file '%s' not found\n", temp_file_name);
        return;
    }

    bool has_record = false;
    struct Customer customer_old_details;

    while (fread(&customer_old_details, sizeof(struct Customer), 1, data_file) != 0) {
        if (customer_old_details.id == id_to_update) {
            struct Customer customerNewDetails;

            customer_take_details(&customerNewDetails);
            fwrite(&customerNewDetails, sizeof(struct Customer), 1, temp_file);
            has_record = true;
            continue;
        }
        fwrite(&customer_old_details, sizeof(struct Customer), 1, temp_file);
    }
    fclose(data_file);
    fclose(temp_file);

    remove(data_file_name);
    rename(temp_file_name, data_file_name);

    if (!has_record) {
        printf("customer with id '%d' not found on records\n", id_to_update);
        return;
    }
    printf("Update success\n");
}

void customer_delete_record(int id_to_delete)
{
    FILE *data_file = NULL;
    FILE *temp_file = NULL;

    data_file = fopen(data_file_name, "r");

    if (data_file == NULL) {
        printf("Error: customers data file '%s' not found\n", data_file_name);
        return;
    }

    if (is_file_empty(data_file)) {
        printf("Empty records\n");
        fclose(data_file);
        return;
    }

    temp_file = fopen(temp_file_name, "w");

    if (temp_file == NULL) {
        printf("Error: customers temp file '%s' not found\n", temp_file_name);
        return;
    }

    bool has_record = false;
    struct Customer customer_old_details;

    while (fread(&customer_old_details, sizeof(struct Customer), 1, data_file) != 0) {
        if (customer_old_details.id == id_to_delete) {
            has_record = true;
            continue;
        }
        fwrite(&customer_old_details, sizeof(struct Customer), 1, temp_file);
    }
    fclose(data_file);
    fclose(temp_file);

    remove(data_file_name);
    rename(temp_file_name, data_file_name);

    if (!has_record) {
        printf("customer with id '%d' not found on records\n", id_to_delete);
        return;
    }

    printf("Delete success\n");
}

void customer_delete_all_records(void)
{
    if (remove(data_file_name) == -1) {
        printf("Error: failed to delete all records\n");
        return;
    }

    FILE *new_data_file = fopen(data_file_name, "w");

    if (new_data_file == NULL) {
        printf("Error: failed to create new data file\n");
        return;
    }
    printf("\nDeleted all customers record\n");
}

/*------------------------------------*/

void print_main_menu(void)
{
    print_header("MAIN MENU");

    printf("1. Add new customer record\n");
    printf("2. View all customers record\n");
    printf("3. Search customer record by id\n");
    printf("4. Search customer record by name\n");
    printf("5. Update customer record\n");
    printf("6. Delete customer record\n");
    printf("7. Delete all customers record\n");
    printf("8. Logout\n");
}

/*------------------------------------*/

void start_menu_loop(void)
{
    int user_choosed_opt = 0;
    bool exit_program = false;

    do {
        print_main_menu();
        printf("> ");
        scanf("%d", &user_choosed_opt);

        switch (user_choosed_opt) {
        case 1: {
            struct Customer new_customer;
            customer_take_details(&new_customer);
            customer_add_new_record(&new_customer);
            break;
        }
        case 2:
            customer_show_all_records();
            break;
        case 3: {
            int customer_id = 0;
            printf("Customer id: ");
            scanf("%d", &customer_id);
            customer_search_record_by_id(customer_id);
            break;
        }
        case 4: {
            char firstname[256] = {0};
            char lastname[256] = {0};

            printf("First name: ");
            scanf("%255s", firstname);
            flush_input_buffer();

            printf("Last name: ");
            scanf("%255s", lastname);
            flush_input_buffer();

            customer_search_record_by_name(firstname, lastname);
            break;
        }
        case 5: {
            int customer_id = 0;
            printf("Customer id: ");
            scanf("%d", &customer_id);
            customer_update_record(customer_id);
            break;
        }
        case 6: {
            int customer_id = 0;
            printf("Customer id: ");
            scanf("%d", &customer_id);
            customer_delete_record(customer_id);
            break;
        }
        case 7:
            customer_delete_all_records();
            break;
        case 8:
            exit_program = true;
            break;
        default:
            printf("Unknown option :(\n");
            break;
        }
        printf("\n");
    } while (!exit_program);
}

/*-----LOGIN RELATED FUNCTIONS-----*/

bool check_username_and_password(char *taken_username, char *taken_password)
{
    char username[] = "root";
    char password[] = "rootmin";

    if (strncmp(username, taken_username, USERNAME_LEN) == 0 &&
        strncmp(password, taken_password, PASSWORD_LEN) == 0) {
        return true;
    }
    return false;
}

bool authenticate_user(void)
{
    char taken_username[USERNAME_LEN + 1] = {0};
    char taken_password[PASSWORD_LEN + 1] = {0};

    print_header("LOGIN");

    printf("Username: ");
    scanf("%4s", taken_username);
    flush_input_buffer();

    printf("Password: ");
    scanf("%7s", taken_password);
    flush_input_buffer();

    return check_username_and_password(taken_username, taken_password);
}

/*------------------------------------*/

int main(void)
{
    int remaining_chances = 5;

    do {
        printf("remaining chances: %d\n", remaining_chances);

        if (!authenticate_user()) {
            printf("Error: Incorrect username or password\n\n");
        } else {
            break;
        }

        remaining_chances--;
    } while (remaining_chances != 0);

    if (remaining_chances == 0) {
        printf("Out of chances, try again\n");
        return 1;
    } else {
        start_menu_loop();
    }
    return 0;
}


#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "customer.h"
#include "util.h"

#define USERNAME_LEN 4
#define PASSWORD_LEN 7

static void print_main_menu(void)
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

static void start_menu_loop(void)
{
    int user_choosed_opt = 0;
    bool exit_program = false;

    do {
        print_main_menu();
        printf("> ");
        scanf("%d", &user_choosed_opt);

        switch (user_choosed_opt) {
        case 1: {
            struct Customer new_customer = customer_New();
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
            scanf("%s", lastname);
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

static bool check_username_and_password(char *taken_username, char *taken_password)
{
    static char username[] = "root";
    static char password[] = "rootmin";

    if (strncmp(username, taken_username, USERNAME_LEN) == 0 &&
        strncmp(password, taken_password, PASSWORD_LEN) == 0) {
        return true;
    }
    return false;
}

static bool authenticate_user(void)
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


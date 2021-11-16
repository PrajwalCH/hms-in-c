#include "room.h"

#include <stdio.h>
#include <time.h>

#include "customer.h"
#include "devin_hotel.h"
#include "util.h"

static void today_date(char *output, int output_size)
{
    time_t raw_time;
    struct tm *time_info;
    time(&raw_time);
    time_info = localtime(&raw_time);

    snprintf(output, output_size, "%02d/%02d/%02d", time_info->tm_year + 1900, time_info->tm_mon + 1, time_info->tm_mday);
}

static void print_room_data(struct Room *room)
{
    printf("| %-2d | %-10.1f | %-10s | %-6c |\n", room->num, room->rate, room->type, room->status);
}

static void print_table_header(void)
{
    int num_chars_printed = printf("  %-2s   %-10s   %-10s   %-6s \n", "No", "Rate", "Type", "Status");
    print_horizontal_line(num_chars_printed);
}

static const char *data_file_name(void)
{
    const char *path = DATA_FILES_DIR"/"ROOMS_DATA_FILENAME;
    return path;
}

static bool open_data_file(FILE **data_file)
{
    *data_file = fopen(data_file_name(), "r");

    if (*data_file == NULL) {
        printf("Error: rooms data file '%s' not found '%s' dir\n", ROOMS_DATA_FILENAME, DATA_FILES_DIR);
        return false;
    }

    if (is_file_empty(*data_file)) {
        printf("Empty records\n");
        return false;
    }

    return true;
}

void room_take_info(struct Room *room)
{
    print_header("ADD NEW ROOM INFO");

    printf("Room num: ");
    scanf("%d", &room->num);
    printf("Room rate: ");
    scanf("%f", &room->rate);
    printf("Room type: ");
    scanf("%50s", room->type);
    flush_input_buffer();
    printf("Room status: ");
    scanf("%c", &room->status);
}

struct Room room_new(void)
{
    return (struct Room) {
        .num = 0,
        .rate = 0.0f,
        .type = {0},
        .status = '0'
    };
}

void room_add_new_record(struct Room *room_info)
{
    FILE *data_file = fopen(data_file_name(), "a");

    if (data_file == NULL) {
        printf("Error: rooms data file '%s' not found on '%s' dir\n", ROOMS_DATA_FILENAME, DATA_FILES_DIR);
        return;
    }

    if (fwrite(room_info, sizeof(struct Room), 1, data_file) != 1) {
        printf("Error: failed to add new room record\n");
    }
    fclose(data_file);
}

void room_show_all_rooms(void)
{
    print_header("ALL ROOMS INFO");

    struct Room room = room_new();
    FILE *data_file = NULL;

    if (!open_data_file(&data_file))
        return;

    print_table_header();
    while (fread(&room, sizeof(struct Room), 1, data_file) != 0) {
        print_room_data(&room);
    }
    fclose(data_file);
}

void room_show_available_rooms(void)
{
    print_header("AVAILABLE ROOMS");

    struct Room room = room_new();
    FILE *data_file = NULL;

    if (!open_data_file(&data_file))
        return;

    print_table_header();
    while (fread(&room, sizeof(struct Room), 1, data_file) != 0) {
        if (room.status == 'Y')
            print_room_data(&room);
    }
    fclose(data_file);
}

static void print_reservation_details(struct Reservation *reservation)
{
    printf("Reservation details\n");
    printf("%d\n", reservation->customer_id);
    printf("%d\n", reservation->room_num);
    printf("%s\n", reservation->date_of_arrival);
    printf("%s\n", reservation->date_of_departure);
}

static void take_reservation_details(struct Reservation *reservation)
{
    print_header("RESERVATION DETAILS");

    printf("Customer id: ");
    scanf("%d", &reservation->customer_id);

    /*
     * check if we already have a customer with given id on records->
     * if we don't have any record then s/he is a new customer so first take
     * a new details of that customer, save it on customers records and take
     * the id of that new customer
     */

    if (!customer_is_record_exist(reservation->customer_id)) {
        printf("customer with id '%d' not found on records\n", reservation->customer_id);

        struct Customer new_customer = customer_New();
        customer_take_details(&new_customer);
        customer_add_new_record(&new_customer);
        reservation->customer_id = new_customer.id;
    }

    printf("Customer id: %d\n", reservation->customer_id);
    room_show_available_rooms();
    printf("choice room number: ");
    scanf("%d", &reservation->room_num);

    today_date(reservation->date_of_arrival, 26);
    printf("Date of arrival: %s\n", reservation->date_of_arrival);
    printf("Date of departure (leaving) [YYYY/MMM/DD]: ");
    scanf("%s", reservation->date_of_departure);
}

void room_reserve(void)
{
    struct Reservation reservation = {
        .customer_id = 0,
        .room_num = 0,
        .date_of_arrival = {0},
        .date_of_departure = {0}
    };

    take_reservation_details(&reservation);
    print_reservation_details(&reservation);
}


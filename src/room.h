#ifndef ROOM_H

#define ROOM_H

struct Room {
    int   num;
    float rate;
    char  type[51];
    char  status; // Y for available and N for not available
};

struct Reservation {
    int  customer_id;
    int  room_num;
    char date_of_arrival[256];
    char date_of_departure[256];
};

struct Room room_new(void);
void room_take_info(struct Room *room);
void room_add_new_record(struct Room *room_info);
void room_show_all_rooms(void);
void room_show_available_rooms(void);
void room_reserve(void);

#endif /* ROOM_H */


#include "util.h"

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


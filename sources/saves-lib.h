typedef struct record_info{
    int seconds;
    char *name;
    char *date1;
    char *date2;
} record_info;

void convert_to_file(game_data *g, char *filename);

game_data *convert_to_game(char *filename);

char *info_about_file(char *filename);

record_info *record_constructor(int seconds, char *name, char *date1, char *date2);

bool record_compare(record_info *a, record_info *b);

void update_records(game_data *game);

GtkWidget *get_records(int height);
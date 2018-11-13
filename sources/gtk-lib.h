#include <stdbool.h>

typedef struct tiles {
    int opened;
    int number;
} tiles;

typedef struct game_data {
    GtkWidget *playerA, *playerB;
    int type, turn, clicked;
    int height, width;
    int pictures, seconds;
    tiles **table;
    int **computer;
    int scoreA, scoreB;
    bool blocked;
} game_data;

typedef struct info {
    game_data *game;
    GtkWidget *widget;
    int type;
} info;

typedef struct move_info {
    game_data *game;
    GtkWidget *window, ***buttons;
    GtkWidget *label, *label2;
    char *text;
    int i, j;
} move_info;

void quit(GtkWidget *widget, gpointer data);

GtkWidget *button_constructor(char *label, void (*function)(GtkWidget*, gpointer), gpointer data);

void button_swap(GtkWidget *button, int height, int type, int number);

GtkWidget *label_constructor(char *text);

GtkWidget *text_constructor(int max_lenght);

info *info_constructor(game_data *game, GtkWidget *widget, int type);

move_info *move_info_constructor(game_data *game, GtkWidget *window, GtkWidget ***buttons, GtkWidget *label, GtkWidget *label2, char *text, int i, int j);

char *get_text(GtkWidget *text, int max_lenght);

GtkWidget *get_window();

void configurate_window(GtkWidget *window, GtkWidget *box, GtkWidget *grid);

void set_size(GtkWidget *widget, gpointer data);

void set_images(GtkWidget *widget, gpointer data);

void random_shuffle(int *array, int n);

void prepare_tiles(game_data *game);

char *int_to_char(int x);

void delay(int miliseconds);

char *seconds(int x);

int update_time(gpointer data);

void run_timer(GtkWidget *widget, gpointer data);

void stop_timer(GtkWidget *widget, gpointer data);

int make_second_move_computer(gpointer data);

int make_move_computer(gpointer data);

GtkWidget *save_frame(int number, gpointer data);

int run_wait(gpointer data);

void make_move(GtkWidget *widget, gpointer data);

void run_game(GtkWidget *widget, gpointer data);

void run_make_save(GtkWidget *widget, gpointer data);

void run_select_game(GtkWidget *widget, gpointer data);

void run_records(GtkWidget *widget, gpointer data);

void run_menu(GtkWidget *widget, gpointer data);

void run_game_over(GtkWidget *widget, gpointer data);

void run_new_game(GtkWidget *widget, gpointer data);
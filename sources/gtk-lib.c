#include <string.h>
#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

#include "gtk-lib.h"
#include "saves-lib.h"

void quit(GtkWidget *widget, gpointer data)
{
    gtk_main_quit();
}

GtkWidget *button_constructor(char *label, void (*function)(GtkWidget*, gpointer), gpointer data)
{
    GtkWidget *button = gtk_button_new_with_label(label);

    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(function), data);

    return button;
}

void button_swap(GtkWidget *button, int height, int type, int number)
{
    if(gtk_button_get_label(GTK_BUTTON(button)) == NULL)
    {
        gtk_button_set_label(GTK_BUTTON(button), "");
    }
    else
    {
        int pixels;

        if(height == 4)
        {
            pixels = 120;
        }
        if(height == 5)
        {
            pixels = 100;
        }
        if(height == 6)
        {
            pixels = 70;
        }

        gtk_button_set_label(GTK_BUTTON(button), NULL);

        char *filename = (char *)malloc(100 * sizeof(char));
        
        if(type == 1)
        {
            sprintf(filename, "pictures/pies%d.jpg", number + 1);
        }
        if(type == 2)
        {
            sprintf(filename, "pictures/zam%d.jpg", number + 1);
        }
        if(type == 3)
        {
            sprintf(filename, "pictures/sam%d.jpg", number + 1);
        }
        
        GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(filename, NULL);
        pixbuf = gdk_pixbuf_scale_simple(pixbuf, pixels, pixels, GDK_INTERP_BILINEAR);

        GtkWidget *picture = gtk_image_new_from_pixbuf(pixbuf);

        gtk_button_set_image(GTK_BUTTON(button), picture);
    
        free(filename);
    }
}

GtkWidget *label_constructor(char *text)
{
    GtkWidget *label = gtk_label_new(text);

    return label;
}

GtkWidget *text_constructor(int max_lenght)
{
    GtkWidget *text = gtk_entry_new();
    
    gtk_entry_set_max_length(GTK_ENTRY(text), max_lenght);
    gtk_entry_set_alignment(GTK_ENTRY(text), 0.5);
    gtk_entry_set_text(GTK_ENTRY(text), "");

    return text;
}

info *info_constructor(game_data *game, GtkWidget *widget, int type)
{
    info *result = (info *)malloc(sizeof(info));
    
    result->game = game;
    result->widget = widget;
    result->type = type;

    return result;
}

move_info *move_info_constructor(game_data *game, GtkWidget *window, GtkWidget ***buttons, GtkWidget *label, GtkWidget *label2, char *text, int i, int j)
{
    move_info *result = (move_info *)malloc(sizeof(move_info));

    result->game = game;
    result->window = window;
    result->buttons = buttons;
    result->label = label;
    result->label2 = label2;
    result->text = text;
    result->i = i;
    result->j = j;

    return result;
}

char *get_text(GtkWidget *text, int max_lenght)
{
    char *result = (char *)malloc(max_lenght * sizeof(char));
    
    strcpy(result, gtk_entry_get_text(GTK_ENTRY(text)));

    return result;
}

GtkWidget *get_window()
{
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_window_set_title(GTK_WINDOW(window),"Memory game");
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    return window;
}

void configurate_window(GtkWidget *window, GtkWidget *box, GtkWidget *grid)
{
    gtk_box_pack_start(GTK_BOX(box), grid, TRUE, TRUE, 0);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);

    gtk_container_add(GTK_CONTAINER(window), box);
    gtk_window_set_default_size(GTK_WINDOW(window), 512, 384);
}

void set_size(GtkWidget *widget, gpointer data)
{
    int type = ((info *)data)->type;
    GtkLabel *label = GTK_LABEL(((info *)data)->widget);

    game_data *game = ((info *)data)->game;

    if(type == 1)
    {
        gtk_label_set_text(label, "4x5");
        game->height = 4, game->width = 5;
    }
    if(type == 2)
    {
        gtk_label_set_text(label, "5x6");
        game->height = 5, game->width = 6;
    }
    if(type == 3)
    {
        gtk_label_set_text(label, "6x8");
        game->height = 6, game->width = 8;
    }
}

void set_images(GtkWidget *widget, gpointer data)
{
    int type = ((info *)data)->type;
    GtkLabel *label = GTK_LABEL(((info *)data)->widget);

    game_data *game = ((info *)data)->game;

    if(type == 1)
    {
        gtk_label_set_text(label, "Pieski");
        game->pictures = 1;
    }
    if(type == 2)
    {
        gtk_label_set_text(label, "Zabytki");
        game->pictures = 2;
    }
    if(type == 3)
    {
        gtk_label_set_text(label, "Samochody");
        game->pictures = 3;
    }
}

void random_shuffle(int *array, int n)
{
    srand(time(NULL));

    for(int i = 0; i < n - 1; i++)
    {
        int j = i + rand() % (n - i);

        int tmp = array[i];
        array[i] = array[j];
        array[j] = tmp;
    }
}

void prepare_tiles(game_data *game)
{
    game->table = (tiles **)malloc(game->height * sizeof(tiles *));

    for(int i = 0; i < game->height; i++)
    {
        game->table[i] = (tiles *)malloc(game->width * sizeof(tiles));
    }

    int *permutation = (int *)malloc(game->width * game->height * sizeof(int));
    
    for(int i = 0; i < game->width * game->height; i++)
    {
        permutation[i] = i;
    }

    game->computer = (int **)malloc(game->height * sizeof(int *));

    for(int i = 0; i < game->height; i++)
    {
        game->computer[i] = (int *)malloc(game->width * sizeof(int));
        
        for(int j = 0; j < game->width; j++)
        {
            game->computer[i][j] = -1;
        }
    }
    
    random_shuffle(permutation, game->width * game->height);

    for(int i = 0; i < game->height; i++)
    {
        for(int j = 0; j < game->width; j++)
        {
            game->table[i][j].opened = false;
            game->table[i][j].number = permutation[i * game->width + j] / 2;
        }
    }
}

char *int_to_char(int x)
{
    char *result = (char *)malloc(10 * sizeof(char));
    
    sprintf(result, "%d", x);

    return result;
}

void delay(int miliseconds)
{
    clock_t finish = miliseconds + clock();
    while(finish > clock());
}

char *seconds(int x)
{
    char *text = (char *)malloc(10 * sizeof(char));
    if(x == 1)
    {
        sprintf(text, "sekunda");
        return text;
    }

    x %= 100;
    
    if(!(10 <= x && x <= 20) && 2 <= x % 10 && x % 10 <= 4)
    {
        sprintf(text, "sekundy");
    }
    else
    {
        sprintf(text, "sekund");
    }

    return text;
}

bool flag;
int update_time(gpointer data)
{
    if(flag == 0)
    {
        game_data *game = ((info *)data)->game;
        GtkWidget *label = ((info *)data)->widget;
        char *text = (char *)malloc(50 * sizeof(char));

        ++game->seconds;

        sprintf(text, "Czas: %d %s -- Punkty: %d", game->seconds, seconds(game->seconds), game->scoreA);
        gtk_label_set_text(GTK_LABEL(label), text);

        free(text);
        return true;
    }
    return false;
}

void run_timer(GtkWidget *widget, gpointer data)
{
    flag = 0;

    game_data *game = ((info *)data)->game;
    GtkWidget *label = ((info *)data)->widget;
    
    g_timeout_add_seconds(1, update_time, info_constructor(game, label, 0));
}

void stop_timer(GtkWidget *widget, gpointer data)
{
    flag = 1;
}

int make_second_move_computer(gpointer data)
{
    gtk_button_clicked(GTK_BUTTON(data)); 

    return false;
}

int make_move_computer(gpointer data)
{
    game_data *game = ((move_info *)data)->game;    
    GtkWidget *window = ((move_info *)data)->window;
    GtkWidget ***buttons = ((move_info *)data)->buttons;
    GtkWidget *label = ((move_info *)data)->label;
    GtkWidget *label2 = ((move_info *)data)->label2;
    char *text = ((move_info *)data)->text;

    if(game->scoreA + game->scoreB == (game->height * game->width / 2))
    {
        return false;
    }

    srand(time(NULL));

    int x1 = -1, x2, y1, y2;
    for(int i = 0; i < game->height; i++)
    {
        for(int j = 0; j < game->width; j++)
        {
            for(int i2 = 0; i2 < game->height; i2++)
            {
                for(int j2 = 0; j2 < game->width; j2++)
                {
                    if(i == i2 && j == j2) continue;
                    if(game->computer[i][j] >= 0 && game->computer[i][j] == game->computer[i2][j2])
                    {
                        x1 = i, x2 = i2, y1 = j, y2 = j2;
                    }
                }
            }
        }
    }

    if(x1 != -1)
    {
        gtk_button_clicked(GTK_BUTTON(buttons[x1][y1]));

        g_timeout_add(800, make_second_move_computer, buttons[x2][y2]);
    }
    else
    {
        int a, b;
        
        a = rand() % game->height, b = rand() % game->width;
        
        while(game->computer[a][b] != -1)
            a = rand() % game->height, b = rand() % game->width;
        
        gtk_button_clicked(GTK_BUTTON(buttons[a][b]));

        for(int i = 0; i < game->height; i++)
        {
            for(int j = 0; j < game->width; j++)
            {
                if(i == a && j == b) continue;
                if(game->computer[a][b] == game->computer[i][j])
                {
                    g_timeout_add(800, make_second_move_computer, buttons[i][j]);
                    
                    return false;
                }
            }
        }       

        a = rand() % game->height, b = rand() % game->width;
        
        while(game->computer[a][b] != -1)
            a = rand() % game->height, b = rand() % game->width;

        g_timeout_add(800, make_second_move_computer, buttons[a][b]);
    }
    
    return false;
}

GtkWidget *save_frame(int number, gpointer data)
{
    char *text = (char *)malloc(30 * sizeof(char));
    sprintf(text, "Slot %d", number);

    GtkWidget *frame, *label;

    frame = gtk_frame_new(text);
    GtkWidget *button = button_constructor(NULL, run_game, data);
    
    char *text2 = (char *)malloc(30 * sizeof(char));
    sprintf(text2, "save%d", number);
    label = gtk_label_new(info_about_file(text2));
    gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);

    gtk_container_add(GTK_CONTAINER(button), label);
    gtk_container_add(GTK_CONTAINER(frame), button);

    return frame;
}

int run_wait(gpointer data)
{
    game_data *game = ((move_info *)data)->game;    
    GtkWidget *window = ((move_info *)data)->window;
    GtkWidget ***buttons = ((move_info *)data)->buttons;
    GtkWidget *label = ((move_info *)data)->label;
    GtkWidget *label2 = ((move_info *)data)->label2;
    char *text = ((move_info *)data)->text;
    int i = ((move_info *)data)->i, j = ((move_info *)data)->j;

    game->clicked--;

    int last_i = game->clicked / game->width, last_j = game->clicked - game->width * last_i;

    game->clicked = 0;

    if(game->table[i][j].number != game->table[last_i][last_j].number)
    {
        game->table[i][j].opened = false;
        game->table[last_i][last_j].opened = false;
        
        button_swap(buttons[i][j], game->height, game->pictures, game->table[i][j].number);
        button_swap(buttons[last_i][last_j], game->height, game->pictures, game->table[last_i][last_j].number);
    }
    else
    {
        if(game->type == 2)
        {
            game->computer[i][j] = -2;
            game->computer[last_i][last_j] = -2;
        }

        if(game->turn == 0)
        {
            game->scoreA++;
        }
        else
        {
            game->scoreB++;
        }

        if(game->scoreA + game->scoreB == game->width * game->height / 2)
        {
            run_game_over(window, data);
        }

        if(game->type != 1)
        {
            sprintf(text, "Wynik: %d - %d", game->scoreA, game->scoreB);
        }
        else
        {
            sprintf(text, "Czas: %d %s -- Punkty: %d", game->seconds, seconds(game->seconds), game->scoreA);
        }
        gtk_label_set_text(GTK_LABEL(label2), text);
    }

    if(game->type != 1)
    {
        (game->turn) ^= 1;
    }
    
    if(game->turn == 0)
    {
        sprintf(text, "Ruch: %s", get_text(game->playerA, 20));  
    }
    else
    {
        sprintf(text, "Ruch: %s", get_text(game->playerB, 20));
    }
    gtk_label_set_text(GTK_LABEL(label), text); 

    game->blocked = false;

    if(game->type == 2 && game->turn == 1)
    {
        g_timeout_add(800, make_move_computer, data);
    }

    return false;
}

void make_move(GtkWidget *widget, gpointer data)
{
    game_data *game = ((move_info *)data)->game;    
    GtkWidget *window = ((move_info *)data)->window;
    GtkWidget ***buttons = ((move_info *)data)->buttons;
    GtkWidget *label = ((move_info *)data)->label;
    GtkWidget *label2 = ((move_info *)data)->label2;
    char *text = ((move_info *)data)->text;
    int i = ((move_info *)data)->i, j = ((move_info *)data)->j;

    if(game->blocked == true || game->table[i][j].opened == true)
    {
        return;
    }

    if(game->type == 2)
    {
        game->computer[i][j] = game->table[i][j].number;
    }

    if(game->clicked == 0)
    {
        game->table[i][j].opened = true;
    
        button_swap(buttons[i][j], game->height, game->pictures, game->table[i][j].number);
        game->clicked = 1 + i * game->width + j;
    }
    else
    {
        game->table[i][j].opened = true;
        button_swap(buttons[i][j], game->height, game->pictures, game->table[i][j].number);
        game->blocked = true;

        g_timeout_add(500, run_wait, data);
    }
}

void run_game(GtkWidget *widget, gpointer data)
{
    GtkWidget *window = get_window();
    
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget *grid = gtk_grid_new();

    configurate_window(window, box, grid);

    game_data *game = ((info*)data)->game;

    int nr = ((info*)data)->type;

    if(nr != 0)
    {
        char *filename = (char *)malloc(10 * sizeof(char));
        sprintf(filename, "save%d", nr);

        if(game != NULL)
        {
            convert_to_file(game, filename);
        }

        game = convert_to_game(filename);
    }

    if(game == NULL)
    {
        return;
    }

    // ************** GAME ************** //
    
    const int MAX = 720;

    int tile_width = MAX / game->width, tile_height = tile_width, options_height = 60;
    gtk_window_set_default_size(GTK_WINDOW(window), MAX, game->height * tile_height + 2 * options_height);

    char *text = (char *)malloc(50 * sizeof(char));
    
    if(game->turn == 0)
    {
        sprintf(text, "Ruch: %s", get_text(game->playerA, 20));
    }
    else
    {
        sprintf(text, "Ruch: %s", get_text(game->playerB, 20));        
    }
    GtkWidget *label = label_constructor(text);
    
    if(game->type != 1)
    {
        sprintf(text, "Wynik: %d - %d", game->scoreA, game->scoreB);
    }
    else
    {
        sprintf(text, "Czas: %d %s -- Punkty: %d", game->seconds, seconds(game->seconds), game->scoreA);
    }
    GtkWidget *label2 = label_constructor(text);

    if(game->type == 1)
    {
        g_signal_connect(G_OBJECT(window), "show", G_CALLBACK(run_timer), info_constructor(game, label2, 0));
        g_signal_connect(G_OBJECT(window), "hide", G_CALLBACK(stop_timer), NULL);
    }

    gtk_grid_attach(GTK_GRID(grid), label_constructor(get_text(game->playerA, 20)), 0, 0, MAX / 3, options_height);
    gtk_grid_attach(GTK_GRID(grid), label, MAX / 3, 0, MAX / 3, options_height / 2);
    gtk_grid_attach(GTK_GRID(grid), label2, MAX / 3, options_height / 2, MAX / 3, options_height / 2);
    gtk_grid_attach(GTK_GRID(grid), label_constructor(get_text(game->playerB, 20)), 2 * MAX / 3, 0, MAX / 3, options_height);

    GtkWidget ***button;

    button = (GtkWidget ***)malloc(game->height * sizeof(GtkWidget **));
    for(int i = 0; i < game->height; i++)
    {
        button[i] = (GtkWidget **)malloc(game->width * sizeof(GtkWidget *));
    }

    for(int i = 0; i < game->height; i++)
    {
        for(int j = 0; j < game->width; j++)
        {
            button[i][j] = gtk_button_new_with_label("");
            if(game->table[i][j].opened == true)
            {
                button_swap(button[i][j], game->height, game->pictures, game->table[i][j].number);
            }

            g_signal_connect(G_OBJECT(button[i][j]), "clicked", G_CALLBACK(make_move),
            move_info_constructor(game, window, button, label, label2, text, i, j));
            gtk_grid_attach(GTK_GRID(grid), button[i][j], j * tile_width, options_height + i * tile_height, tile_width, tile_height);
        }
    }

    // ************** END ************** //

    gtk_grid_attach(GTK_GRID(grid), button_constructor("Menu Główne", run_menu, window),
    0, options_height + game->height * tile_height, MAX / 3, options_height);
    gtk_grid_attach(GTK_GRID(grid), button_constructor("Wyjście", quit, NULL),
    MAX / 3, options_height + game->height * tile_height, MAX / 3, options_height);
    gtk_grid_attach(GTK_GRID(grid), button_constructor("Zapis", run_make_save, info_constructor(game, window, 2)),
    2 * MAX / 3, options_height + game->height * tile_height, MAX / 3, options_height);

    gtk_widget_show_all(window);
    
    if(((info*)data)->widget != NULL)
    {
        gtk_widget_hide(GTK_WIDGET(((info*)data)->widget));
    }
}

void run_make_save(GtkWidget *widget, gpointer data)
{
    GtkWidget *window = get_window();
    
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget *grid = gtk_grid_new();

    configurate_window(window, box, grid);

    game_data *game = ((info*)data)->game;

    if(((info*)data)->type == 1)
    {
        if(strcmp(get_text(game->playerA, 20), "") == 0)
        {
            gtk_entry_set_text(GTK_ENTRY(game->playerA), "BezimiennyA");
            return;
        }
        if(strcmp(get_text(game->playerB, 20), "") == 0)
        {
            gtk_entry_set_text(GTK_ENTRY(game->playerB), "BezimiennyB");
            return;
        }
        prepare_tiles(game);
    }

    GtkWidget *label = gtk_label_new(NULL);
    if(((info*)data)->type != 0)
    {
        gtk_label_set_markup(GTK_LABEL(label), "<span font_desc=\"15.0\">Wybór miejsca zapisu</span>");
    }
    else
    {
        gtk_label_set_markup(GTK_LABEL(label), "<span font_desc=\"15.0\">Kontynuuj rozpoczętą grę</span>");
    }

    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 2, 1);

    gtk_grid_attach(GTK_GRID(grid), save_frame(1, info_constructor(game, window, 1)), 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), save_frame(2, info_constructor(game, window, 2)), 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), save_frame(3, info_constructor(game, window, 3)), 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), save_frame(4, info_constructor(game, window, 4)), 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), save_frame(5, info_constructor(game, window, 5)), 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), save_frame(6, info_constructor(game, window, 6)), 1, 3, 1, 1);

    if(((info*)data)->type == 0)
    {
        gtk_grid_attach(GTK_GRID(grid), button_constructor("Powrót", run_menu, window), 0, 5, 1, 1);        
    }
    if(((info*)data)->type == 1)
    {
        gtk_grid_attach(GTK_GRID(grid), button_constructor("Powrót", run_new_game, window), 0, 5, 1, 1);        
    }
    if(((info*)data)->type == 2)
    {
        gtk_grid_attach(GTK_GRID(grid), button_constructor("Powrót", run_game, info_constructor(game, window, 0)), 0, 5, 1, 1);        
    }
    gtk_grid_attach(GTK_GRID(grid), button_constructor("Wyjście", quit, NULL), 1, 5, 1, 1);

    gtk_widget_show_all(window);
    if(((info*)data)->widget != NULL)
    {
        gtk_widget_hide(GTK_WIDGET(((info*)data)->widget));
    }
}

void run_select_game(GtkWidget *widget, gpointer data)
{
    GtkWidget *window = get_window();
    
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget *grid = gtk_grid_new();

    configurate_window(window, box, grid);

    game_data *new_game = (game_data *)malloc(sizeof(game_data));

    int type = ((info*)data)->type;
    new_game->type = type;
    new_game->turn = 0;
    new_game->clicked = 0;
    new_game->blocked = false;

    new_game->playerA = text_constructor(20);
    new_game->playerB = text_constructor(20);

    new_game->height = 4, new_game->width = 5;
    new_game->pictures = 1;
    new_game->seconds = 0;
    new_game->scoreA = 0, new_game->scoreB = 0;

    GtkWidget *frame1 = gtk_frame_new("Gracz 1");
    GtkWidget *frame2 = gtk_frame_new("Gracz 2");
    gtk_grid_attach(GTK_GRID(grid), frame1, 0, 0, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), frame2, 2, 0, 2, 1);

    gtk_container_add(GTK_CONTAINER(frame1), new_game->playerA);

    if(type == 1)
    {
        gtk_container_add(GTK_CONTAINER(frame2), label_constructor("Czas"));
        gtk_entry_set_text(GTK_ENTRY(new_game->playerB), "Czas");   
    }
    if(type == 2)
    {
        gtk_container_add(GTK_CONTAINER(frame2), label_constructor("Komputer"));
        gtk_entry_set_text(GTK_ENTRY(new_game->playerB), "Komputer");
    }
    if(type == 3)
    {
        gtk_container_add(GTK_CONTAINER(frame2), new_game->playerB);
    }

    GtkWidget *label_size = label_constructor("4x5");
    
    GtkWidget *frame_size = gtk_frame_new("Rozmiar");
    gtk_container_add(GTK_CONTAINER(frame_size), label_size);

    gtk_grid_attach(GTK_GRID(grid), frame_size, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button_constructor("4x5", set_size, info_constructor(new_game, label_size, 1)), 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button_constructor("5x6", set_size, info_constructor(new_game, label_size, 2)), 2, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button_constructor("6x8", set_size, info_constructor(new_game, label_size, 3)), 3, 2, 1, 1);
    
    GtkWidget *label_images = label_constructor("Pieski");
    
    GtkWidget *frame_images = gtk_frame_new("Obrazki");
    gtk_container_add(GTK_CONTAINER(frame_images), label_images);
    
    gtk_grid_attach(GTK_GRID(grid), frame_images, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button_constructor("Pieski", set_images, info_constructor(new_game, label_images, 1)), 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button_constructor("Zabytki", set_images, info_constructor(new_game, label_images, 2)), 2, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button_constructor("Samochody", set_images, info_constructor(new_game, label_images, 3)), 3, 3, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), button_constructor("Powrót", run_new_game, window), 0, 5, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), button_constructor("Wyjście", quit, NULL), 2, 5, 2, 1);

    GtkWidget *start = gtk_label_new(NULL);
    
    gtk_label_set_markup(GTK_LABEL(start), "<span font_desc=\"13.0\"><b>START</b></span>");
    GtkWidget *button_start = button_constructor(NULL, run_make_save, info_constructor(new_game, window, 1));
    gtk_container_add(GTK_CONTAINER(button_start), start);

    gtk_grid_attach(GTK_GRID(grid), button_start, 0, 4, 4, 1);

    gtk_widget_show_all(window);
    if(((info*)data)->widget != NULL)
    {
        gtk_widget_hide(GTK_WIDGET(((info*)data)->widget));
    }
}

void run_records(GtkWidget *widget, gpointer data)
{
    GtkWidget *window = get_window();

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget *grid = gtk_grid_new();

    configurate_window(window, box, grid);

    GtkWidget *notebook = gtk_notebook_new();

    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), get_records(4), label_constructor("Rozmiar 4x5"));
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), get_records(5), label_constructor("Rozmiar 5x6"));
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), get_records(6), label_constructor("Rozmiar 6x8"));

    gtk_grid_attach(GTK_GRID(grid), notebook, 0, 0, 2, 4);

    gtk_grid_attach(GTK_GRID(grid), button_constructor("Powrót", run_menu, window), 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button_constructor("Wyjście", quit, NULL), 1, 4, 1, 1);

    gtk_widget_show_all(window);
    if(data != NULL)
    {
        gtk_widget_hide(GTK_WIDGET(data));
    }
}

void run_menu(GtkWidget *widget, gpointer data)
{
    GtkWidget *window = get_window();

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget *grid = gtk_grid_new();
    GtkWidget *frame = gtk_frame_new("Michał Górniak");

    gtk_box_pack_start(GTK_BOX(box), frame, TRUE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(frame), grid);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);

    gtk_container_add(GTK_CONTAINER(window), box);
    gtk_window_set_default_size(GTK_WINDOW(window), 512, 384);

    GtkWidget *label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label), "<span font_desc=\"20.0\"><b>Memory Game</b></span>");
    
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button_constructor("Nowa gra", run_new_game, window), 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button_constructor("Kontynuuj rozpoczętą grę", run_make_save, info_constructor(NULL, window, 0)), 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button_constructor("Rekordy", run_records, window), 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button_constructor("Wyjście", quit, NULL), 0, 4, 1, 1);

    gtk_widget_show_all(window);
    if(data != NULL)
    {
        gtk_widget_hide(GTK_WIDGET(data));
    }
}

void run_game_over(GtkWidget *widget, gpointer data)
{
    GtkWidget *window = get_window();
    
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget *grid = gtk_grid_new();

    configurate_window(window, box, grid);

    game_data *game = ((move_info*)data)->game;

    GtkWidget *label = gtk_label_new(NULL);
    
    if(game->type == 1)
    {
        update_records(game);

        char *text = (char *)malloc(100 * sizeof(char));
        
        sprintf(text, "<span font_desc=\"20.0\">Gracz: <b>%s</b> - Czas: <b>%d %s</b></span>", get_text(game->playerA, 20), game->seconds, seconds(game->seconds));

        gtk_label_set_markup(GTK_LABEL(label), text);
    }
    else
    {
        char *text = (char *)malloc(100 * sizeof(char));

        if(game->scoreA == game->scoreB)
            sprintf(text, "<span font_desc=\"20.0\">Wynik: <b>%s</b> ( <b>%d</b> - <b>%d</b> ) <b>%s</b></span>", get_text(game->playerA, 20), game->scoreA, game->scoreB, get_text(game->playerB, 20));
        if(game->scoreA > game->scoreB)
            sprintf(text, "<span font_desc=\"20.0\">Wynik: <b>%s</b> ( <b>%d</b> - %d ) %s</span>", get_text(game->playerA, 20), game->scoreA, game->scoreB, get_text(game->playerB, 20));
        if(game->scoreA < game->scoreB)
            sprintf(text, "<span font_desc=\"20.0\">Wynik: %s ( %d - <b>%d</b> ) <b>%s</b></span>", get_text(game->playerA, 20), game->scoreA, game->scoreB, get_text(game->playerB, 20));
        
        gtk_label_set_markup(GTK_LABEL(label), text);
    }

    GtkWidget *title = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(title), "<span font_desc=\"20.0\">KONIEC GRY</span>");
    gtk_grid_attach(GTK_GRID(grid), title, 0, 0, 2, 1);

    GtkWidget *frame = gtk_frame_new("Wynik");
    gtk_container_add(GTK_CONTAINER(frame), label);
    gtk_grid_attach(GTK_GRID(grid), frame, 0, 1, 2, 2);

    title = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(title), "<span font_desc=\"20.0\">Zagraj ponownie! &#9786;</span>");
    gtk_grid_attach(GTK_GRID(grid), title, 0, 3, 2, 1);

    gtk_grid_attach(GTK_GRID(grid), button_constructor("Menu Główne", run_menu, window), 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button_constructor("Rekordy", run_records, NULL), 1, 4, 1, 1);

    gtk_widget_show_all(window);
    if(((move_info*)data)->window != NULL)
    {
        gtk_widget_hide(GTK_WIDGET(((move_info*)data)->window));
    }
}

void run_new_game(GtkWidget *widget, gpointer data)
{
    GtkWidget *window = get_window();

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget *grid = gtk_grid_new();

    configurate_window(window, box, grid);

    GtkWidget *label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label), "<span font_desc=\"15.0\">Nowa gra</span>");
    
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), button_constructor("Gra na czas", run_select_game, info_constructor(NULL, window, 1)), 0, 1, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), button_constructor("Gra jednoosobowa z komputerem", run_select_game, info_constructor(NULL, window, 2)), 0, 2, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), button_constructor("Gra dwusobowa", run_select_game, info_constructor(NULL, window, 3)), 0, 3, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), button_constructor("Powrót", run_menu, window), 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button_constructor("Wyjście", quit, NULL), 1, 4, 1, 1);

    gtk_widget_show_all(window);
    if(data != NULL)
    {
        gtk_widget_hide(GTK_WIDGET(data));
    }
}
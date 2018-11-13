#include <string.h>
#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

#include "gtk-lib.h"
#include "saves-lib.h"

void convert_to_file(game_data *g, char *filename)
{
    char *path = (char *)malloc(30 * sizeof(char));
    sprintf(path, "saves/%s.txt", filename);

    FILE *file = fopen(path, "w");
    
    fprintf(file, "%s - %s\n", get_text(g->playerA, 20), get_text(g->playerB, 20));

    time_t tmp = time(NULL);
    struct tm tm = *localtime(&tmp);
    fprintf(file, "%d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

    fprintf(file, "%d %d %d\n", g->type, g->turn, g->clicked);

    fprintf(file, "%d %d\n", g->height, g->width);

    fprintf(file, "%d %d\n", g->pictures, g->seconds);

    fprintf(file, "%d %d\n", g->scoreA, g->scoreB);

    for(int i = 0; i < g->height; i++)
    {
        for(int j = 0; j < g->width; j++)
        {
            fprintf(file, "%d %d\n", g->table[i][j].opened, g->table[i][j].number);
        }
    }

    if(g->type == 2)
    {
        for(int i = 0; i < g->height; i++)
        {
            for(int j = 0; j < g->width; j++)
            {
                fprintf(file, "%d\n", g->computer[i][j]);
            }
        }
    }

    free(g);
    fclose(file);
}

game_data *convert_to_game(char *filename)
{
    char *path = (char *)malloc(30 * sizeof(char));
    sprintf(path, "saves/%s.txt", filename);

    FILE *file = fopen(path, "r");

    if(file == NULL)
    {
        return NULL;
    }

    game_data *game = (game_data *)malloc(sizeof(game_data));

    char *playerA = (char *)malloc(20 * sizeof(char));
    char *playerB = (char *)malloc(20 * sizeof(char));
    char *date1 = (char *)malloc(10 * sizeof(char));
    char *date2 = (char *)malloc(10 * sizeof(char));

    fscanf(file, "%s - %s\n%s %s\n", playerA, playerB, date1, date2);

    game->playerA = text_constructor(20);
    game->playerB = text_constructor(20);
    
    gtk_entry_set_text(GTK_ENTRY(game->playerA), playerA);
    gtk_entry_set_text(GTK_ENTRY(game->playerB), playerB);

    fscanf(file, "%d %d %d\n", &game->type, &game->turn, &game->clicked);

    fscanf(file, "%d %d\n", &game->height, &game->width);

    fscanf(file, "%d %d\n", &game->pictures, &game->seconds);

    fscanf(file, "%d %d\n", &game->scoreA, &game->scoreB);

    game->table = (tiles **)malloc(game->height * sizeof(tiles *));

    for(int i = 0; i < game->height; i++)
    {
        game->table[i] = (tiles *)malloc(game->width * sizeof(tiles));
    }

    for(int i = 0; i < game->height; i++)
    {
        for(int j = 0; j < game->width; j++)
        {
            fscanf(file, "%d %d\n", &game->table[i][j].opened, &game->table[i][j].number);
        }
    }

    game->computer = (int **)malloc(game->height * sizeof(int *));

    for(int i = 0; i < game->height; i++)
    {
        game->computer[i] = (int *)malloc(game->width * sizeof(int));
    }

    if(game->type == 2)
    {
        for(int i = 0; i < game->height; i++)
        {
            for(int j = 0; j < game->width; j++)
            {
                fscanf(file, "%d\n", &game->computer[i][j]);
            }
        }
    }

    game->blocked = 0;

    fclose(file);
    
    return game;
}

char *info_about_file(char *filename)
{
    char *path = (char *)malloc(30 * sizeof(char));
    sprintf(path, "saves/%s.txt", filename);

    FILE *file = fopen(path, "r");

    char *result = (char *)malloc(100 * sizeof(char));

    if(file == NULL)
    {
        strcpy(result, "Pusty");
    }
    else
    {
        char *playerA = (char *)malloc(20 * sizeof(char));
        char *playerB = (char *)malloc(20 * sizeof(char));
        char *date1 = (char *)malloc(10 * sizeof(char));
        char *date2 = (char *)malloc(10 * sizeof(char));

        fscanf(file, "%s - %s\n%s %s\n", playerA, playerB, date1, date2);
        sprintf(result, "%s - %s\n%s %s", playerA, playerB, date1, date2);
    }
    return result;
}

record_info *record_constructor(int seconds, char *name, char *date1, char *date2)
{
    record_info *result = (record_info *)malloc(sizeof(record_info));

    result->seconds = seconds;
    result->name = name;
    result->date1 = date1;
    result->date2 = date2;

    return result;
}

bool record_compare(record_info *a, record_info *b)
{
    if(a->seconds == b->seconds && strcmp(a->date1, b->date1) == 0)
    {
        if(strcmp(a->date2, b->date2) <= 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    if(a->seconds == b->seconds)
    {
        if(strcmp(a->date1, b->date1) <= 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    return a->seconds < b->seconds;
}

void update_records(game_data *game)
{
    if(game->type != 1)
    {
        return;
    }

    char *filename = (char *)malloc(50 * sizeof(char));
    sprintf(filename, "records/records%dx%d.txt", game->height, game->width);

    FILE *file = fopen(filename, "a");
    
    time_t tmp = time(NULL);
    struct tm tm = *localtime(&tmp);
    fprintf(file, "%d %s %d-%02d-%02d %02d:%02d:%02d\n", game->seconds, get_text(game->playerA, 20), tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

    fclose(file);
}

GtkWidget *get_records(int height)
{
    GtkWidget *label = gtk_label_new(NULL);

    char *filename = (char *)malloc(50 * sizeof(char));
    
    if(height == 4)
        sprintf(filename, "records/records4x5.txt");
    if(height == 5)
        sprintf(filename, "records/records5x6.txt");
    if(height == 6)
        sprintf(filename, "records/records6x8.txt");
    
    FILE *file = fopen(filename, "r");

    char *text = (char *)malloc(1000 * sizeof(char));
    sprintf(text, "<span font_desc=\"12.0\">");

    record_info **tab = (record_info **)malloc(10 * sizeof(record_info *));

    for(int i = 0; i < 10; i++)
    {
        tab[i] = NULL;
    }

    while(feof(file) == false)
    {
        int sec;
        char *name = (char *)malloc(20 * sizeof(char));
        char *date1 = (char *)malloc(20 * sizeof(char));
        char *date2 = (char *)malloc(20 * sizeof(char));
        
        fscanf(file, "%d %s %s %s\n", &sec, name, date1, date2);

        record_info *tmp = record_constructor(sec, name, date1, date2);

        for(int i = 0; i < 10; i++)
        {
            if(tab[i] == NULL || record_compare(tmp, tab[i]) == 1)
            {
                record_info *cp = tab[i];
                tab[i] = tmp;
                tmp = cp;
            }
        }
    }

    for(int i = 0; i < 10; i++)
    {
        if(tab[i] != NULL)
        {
            sprintf(text + strlen(text), "%d) Czas: <b>%d %s</b> Gracz: <b>%s</b> Data: <b>%s %s</b>\n",
            i + 1, tab[i]->seconds, seconds(tab[i]->seconds), tab[i]->name, tab[i]->date1, tab[i]->date2);
        }
    }

    sprintf(text + strlen(text), "</span>");

    gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
    //gtk_label_set_xalign(GTK_LABEL(label), 0.1);
    //gtk_label_set_yalign(GTK_LABEL(label), 0.05);

    gtk_label_set_markup(GTK_LABEL(label), text);

    fclose(file);

    return label;
}

/*
*******************************************************************
*                                                                 *
*                          MEMORY GAME                            *
*                        Michał Górniak                           *
*                                                                 *
*             Projekt przygotowany na potrzeby kursu:             *
*               Wstęp do programowania w języku C                 *
*                                                                 *
*                      Wrocław, 01.02.2018                        *
*                                                                 *
*******************************************************************
*/

#include <string.h>
#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

#include "gtk-lib.h"
#include "saves-lib.h"

int main(int argc, char **argv)
{
    gtk_init(&argc, &argv);

    run_menu(NULL, NULL);

    gtk_main();

    return 0;
}
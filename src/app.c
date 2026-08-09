#include "app.h"
#include <stdlib.h>

struct App {
    bool is_running;
};

App *app_create(void)
{
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    
    App *app = malloc(sizeof(App));
    if (!app) {
        endwin();
        return NULL;
    }
    
    app->is_running = true;
    return app;
}

void app_run(App *app)
{
    if (!app) {
        return;
    }
    
    while (app->is_running) {
        int ch = getch();
        
        if (ch == 'q' || ch == 'Q') {
            app->is_running = false;
        }
        
        napms(10);
    }
}

void app_destroy(App *app)
{
    if (!app) {
        return;
    }
    
    endwin();
    free(app);
}

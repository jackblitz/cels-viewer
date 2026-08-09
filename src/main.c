#include "app.h"

int main(void)
{
    App *app = app_create();
    if (!app) {
        fprintf(stderr, "Failed to create application\n");
        return 1;
    }

    app_run(app);
    app_destroy(app);

    return 0;
}

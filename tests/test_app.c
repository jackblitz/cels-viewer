#include "test_app.h"
#include "app.h"
#include <stdio.h>
#include <stdlib.h>

#define TEST_ASSERT(condition, message)                                        \
    do {                                                                       \
        if (!(condition)) {                                                    \
            printf("FAIL: %s\n", message);                                     \
            return false;                                                      \
        }                                                                      \
    } while (0)

static bool test_app_create_destroy(void)
{
    App *app = app_create();
    TEST_ASSERT(app != NULL, "app_create should return a valid pointer");
    app_destroy(app);
    return true;
}

static bool test_app_destroy_null(void)
{
    app_destroy(NULL);
    return true;
}

static bool test_app_run_null(void)
{
    app_run(NULL);
    return true;
}

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    
    int passed = 0;
    int total = 3;
    
    printf("Running app tests...\n");
    
    if (test_app_create_destroy()) {
        printf("PASS: test_app_create_destroy\n");
        passed++;
    }
    
    if (test_app_destroy_null()) {
        printf("PASS: test_app_destroy_null\n");
        passed++;
    }
    
    if (test_app_run_null()) {
        printf("PASS: test_app_run_null\n");
        passed++;
    }
    
    printf("\n%d/%d tests passed\n", passed, total);
    
    return (passed == total) ? 0 : 1;
}

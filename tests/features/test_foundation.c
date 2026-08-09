/**
 * @file test_foundation.c
 * @brief Foundation smoke test: app lifecycle and dependency linking.
 *
 * Verifies the app_create -> app_run -> app_destroy sequence is stable and
 * that the vendored dependencies (libcurl, cJSON) and system ncurses are
 * correctly linked. This binary is also the Valgrind target for the
 * foundation memory audit (see foundation_audit.sh).
 */

#include "app.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include <cJSON.h>
#include <curl/curl.h>

#define TEST_ASSERT(condition, message)                                        \
    do {                                                                       \
        if (!(condition)) {                                                    \
            printf("FAIL: %s\n", message);                                     \
            return false;                                                      \
        }                                                                      \
    } while (0)

/**
 * @brief Verifies app_create allocates an instance and app_destroy frees it.
 */
static bool test_app_create_destroy(void)
{
    App *app = app_create();
    TEST_ASSERT(app != NULL, "app_create should return a valid pointer");
    app_destroy(app);
    return true;
}

/**
 * @brief Verifies app_destroy(NULL) is a safe no-op.
 */
static bool test_app_destroy_null(void)
{
    app_destroy(NULL);
    return true;
}

/**
 * @brief Verifies app_run(NULL) is a safe no-op.
 */
static bool test_app_run_null(void)
{
    app_run(NULL);
    return true;
}

/**
 * @brief Verifies app_run enters its loop and exits on the 'q' key.
 *
 * Pushes 'q' into the ncurses input queue with ungetch so the loop
 * terminates on its first iteration without a real keyboard.
 */
static bool test_app_run_exits_on_q(void)
{
    App *app = app_create();
    TEST_ASSERT(app != NULL, "app_create should return a valid pointer");
    ungetch('q');
    app_run(app);
    app_destroy(app);
    return true;
}

/**
 * @brief Verifies repeated create/destroy cycles do not corrupt state.
 *
 * Exercised under Valgrind to prove the lifecycle leaks nothing per cycle.
 */
static bool test_app_lifecycle_repeated(void)
{
    for (int i = 0; i < 3; i++) {
        App *app = app_create();
        TEST_ASSERT(app != NULL, "app_create should succeed on every cycle");
        app_destroy(app);
    }
    return true;
}

/**
 * @brief Verifies the vendored libcurl is linked and callable.
 */
static bool test_curl_linked(void)
{
    const char *version = curl_version();
    TEST_ASSERT(version != NULL, "curl_version should return a string");
    TEST_ASSERT(strlen(version) > 0, "curl_version should be non-empty");
    return true;
}

/**
 * @brief Verifies the vendored cJSON is linked and functional.
 *
 * Parses a small document so the allocation paths run under Valgrind.
 */
static bool test_cjson_linked(void)
{
    const char *version = cJSON_Version();
    TEST_ASSERT(version != NULL, "cJSON_Version should return a string");

    cJSON *json = cJSON_Parse("{\"foundation\": true}");
    TEST_ASSERT(json != NULL, "cJSON_Parse should parse a valid document");

    const cJSON *flag = cJSON_GetObjectItemCaseSensitive(json, "foundation");
    const bool isTrue = cJSON_IsTrue(flag);
    cJSON_Delete(json);
    TEST_ASSERT(isTrue, "parsed document should contain foundation=true");
    return true;
}

typedef struct TestCase {
    const char *name;
    bool (*fn)(void);
} TestCase;

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    const TestCase tests[] = {
        { "test_app_create_destroy", test_app_create_destroy },
        { "test_app_destroy_null", test_app_destroy_null },
        { "test_app_run_null", test_app_run_null },
        { "test_app_run_exits_on_q", test_app_run_exits_on_q },
        { "test_app_lifecycle_repeated", test_app_lifecycle_repeated },
        { "test_curl_linked", test_curl_linked },
        { "test_cjson_linked", test_cjson_linked },
    };
    const int total = (int)(sizeof(tests) / sizeof(tests[0]));
    int passed = 0;

    printf("Running foundation smoke tests...\n");

    for (int i = 0; i < total; i++) {
        if (tests[i].fn()) {
            printf("PASS: %s\n", tests[i].name);
            passed++;
        }
    }

    printf("\n%d/%d tests passed\n", passed, total);

    return (passed == total) ? 0 : 1;
}

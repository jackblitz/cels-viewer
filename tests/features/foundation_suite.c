/**
 * @file foundation_suite.c
 * @brief Foundation suite: app lifecycle and dependency linking.
 *
 * Verifies the app_create -> app_run -> app_destroy sequence is stable
 * and that the vendored dependencies (libcurl, cJSON) and system ncurses
 * are correctly linked. Run it alone with `cels_tests -s foundation`;
 * this is also the Valgrind target for the foundation memory audit
 * (see foundation_audit.sh).
 */

#include "app.h"

#include <string.h>

#include <cJSON.h>
#include <curl/curl.h>
#include <ncurses.h>

#include "greatest.h"

/**
 * @brief Verifies app_create allocates an instance and app_destroy
 * frees it.
 */
TEST app_create_destroy(void)
{
    App *app = app_create();
    ASSERTm("app_create should return a valid pointer", app != NULL);
    app_destroy(app);
    PASS();
}

/**
 * @brief Verifies app_destroy(NULL) is a safe no-op.
 */
TEST app_destroy_null(void)
{
    app_destroy(NULL);
    PASS();
}

/**
 * @brief Verifies app_run(NULL) is a safe no-op.
 */
TEST app_run_null(void)
{
    app_run(NULL);
    PASS();
}

/**
 * @brief Verifies app_run enters its loop and exits on the 'q' key.
 *
 * Pushes 'q' into the ncurses input queue with ungetch so the loop
 * terminates on its first iteration without a real keyboard.
 */
TEST app_run_exits_on_q(void)
{
    App *app = app_create();
    ASSERTm("app_create should return a valid pointer", app != NULL);
    ungetch('q');
    app_run(app);
    app_destroy(app);
    PASS();
}

/**
 * @brief Verifies repeated create/destroy cycles do not corrupt state.
 *
 * Exercised under Valgrind to prove the lifecycle leaks nothing per
 * cycle.
 */
TEST app_lifecycle_repeated(void)
{
    for (int i = 0; i < 3; i++) {
        App *app = app_create();
        ASSERTm("app_create should succeed on every cycle", app != NULL);
        app_destroy(app);
    }
    PASS();
}

/**
 * @brief Verifies the vendored libcurl is linked and callable.
 */
TEST curl_linked(void)
{
    const char *version = curl_version();
    ASSERTm("curl_version should return a string", version != NULL);
    ASSERTm("curl_version should be non-empty", strlen(version) > 0);
    PASS();
}

/**
 * @brief Verifies the vendored cJSON is linked and functional.
 *
 * Parses a small document so the allocation paths run under Valgrind.
 */
TEST cjson_linked(void)
{
    const char *version = cJSON_Version();
    ASSERTm("cJSON_Version should return a string", version != NULL);

    cJSON *json = cJSON_Parse("{\"foundation\": true}");
    ASSERTm("cJSON_Parse should parse a valid document", json != NULL);

    const cJSON *flag = cJSON_GetObjectItemCaseSensitive(json, "foundation");
    const bool isTrue = cJSON_IsTrue(flag);
    cJSON_Delete(json);
    ASSERTm("parsed document should contain foundation=true", isTrue);
    PASS();
}

SUITE(foundation)
{
    RUN_TEST(app_create_destroy);
    RUN_TEST(app_destroy_null);
    RUN_TEST(app_run_null);
    RUN_TEST(app_run_exits_on_q);
    RUN_TEST(app_lifecycle_repeated);
    RUN_TEST(curl_linked);
    RUN_TEST(cjson_linked);
}

/**
 * @file main.c
 * @brief cels_tests: the CLI test harness for all feature suites.
 *
 * Single entry point for the test hierarchy. Each feature under
 * tests/features/ contributes a suite, registered here with RUN_SUITE.
 * The greatest runner provides suite/test selection from the command
 * line:
 *
 *     cels_tests                  run every suite
 *     cels_tests -s foundation    run one feature's suite
 *     cels_tests -t curl_linked   run tests matching a name
 *     cels_tests -l               list tests without running
 *     cels_tests -v               verbose output
 *     cels_tests -f               stop at first failure
 *
 * Exits non-zero when any test fails, so scripts (e.g. Valgrind audits)
 * can gate on the result.
 */

#include "greatest.h"

SUITE_EXTERN(foundation);

GREATEST_MAIN_DEFS();

int main(int argc, char **argv)
{
    GREATEST_MAIN_BEGIN();

    RUN_SUITE(foundation);

    GREATEST_MAIN_END();
}

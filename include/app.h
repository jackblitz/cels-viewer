#pragma once

#include <stdbool.h>
#include <ncurses.h>

/**
 * @file app.h
 * @brief Application state container and TUI lifecycle management.
 *
 * Typical usage:
 * @code
 *     App *app = app_create();
 *     if (!app) {
 *         fprintf(stderr, "Failed to create application\n");
 *         return 1;
 *     }
 *
 *     app_run(app);
 *
 *     app_destroy(app);
 * @endcode
 *
 * The App struct owns the ncurses state and manages the main event loop.
 * All UI interactions must occur on the main thread.
 */

/**
 * @brief Application state container.
 *
 * This is an opaque handle. The full definition is in app.c.
 * All members are managed internally; use the provided functions to interact.
 */
typedef struct App App;

/**
 * @brief Creates and initializes a new App instance.
 *
 * Initializes the ncurses environment and allocates the App struct.
 * Must be called before any other App functions.
 *
 * @return A pointer to the new App instance, or NULL on failure.
 */
App *app_create(void);

/**
 * @brief Runs the main application loop.
 *
 * Enters the ncurses event loop. The loop continues until is_running
 * is set to false (e.g., via 'q' key press).
 *
 * @param app The App instance. Must not be NULL.
 */
void app_run(App *app);

/**
 * @brief Destroys the App instance and cleans up resources.
 *
 * Calls endwin() to restore the terminal and frees the App instance.
 * After calling this, the App pointer must not be used again.
 *
 * @param app The App instance to destroy. May be NULL.
 */
void app_destroy(App *app);

/* SPDX-License-Identifier: GPL-3.0-only */
/*  ╔════════════════════════════════════════════════════╗
 *  ║                     __             ._      ._      ║
 *  ║ _______ ____ _____./  |_ ___       |_| ____| |__   ║
 *  ║ \___  / __  \\___ \_  __/ _ \  ____| |/  __| |  \  ║
 *  ║  / __/\  ___/| |_> |  || <_> |/___/| |\__ \|  Y  \ ║
 *  ║ /____ \\___ \|  __/|__| \___/      |_/___  |__|  / ║
 *  ║      \/    \/|_|                         \/    \/  ║
 *  ╚════════════════════════════════════════════════════╝
 */

//
// WARNING: This task ***is not portable***.
//          It relies on a Linux host system to work.
//

#include <itron.h>
#include <kernel.h>

// Non-portable host APIs
#include <linenoise.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// zepto-dlopen interface
extern ER vkerext_load(char* lib);

static struct linenoiseState *g_ls = NULL;
static void console_before_output() {
	if (g_ls) {
		linenoiseHide(g_ls);
	}
}
static void console_after_output() {
	if (g_ls) {
		linenoiseShow(g_ls);
	}
}

static T_DINH io_handler_info;
static void io_handler();

static void task_console(VP_INT exinf);
static int CONSOLE_TASK_ID = 1;
__attribute__((constructor)) void console_init() {
	ER ret;
	ret = vconsole_reghooks(console_before_output, console_after_output);
	if (ret < 0) {
		PANIC("Could not register console output hooks...", ret);
	}
	DBG("Initializing console...");
	static T_CTSK task_console_info;
	task_console_info.task = (FP)task_console;
	task_console_info.exinf = NULL;
	task_console_info.tskatr |= TA_ACT;
	ret = cre_tsk(CONSOLE_TASK_ID, &task_console_info);
	if (ret < 0) {
		PANIC("Could not create console task...", ret);
	}

	io_handler_info.inthdr = io_handler;
	ret = def_inh(INTNO_IO, &io_handler_info);
	if (ret < 0) {
		PANIC("Could not define IO handler...", ret);
	}
	if (fcntl(STDIN_FILENO, F_SETOWN, getpid()) < 0) {
		PANIC("fcntl: Could not F_SETOWN on STDIN_FILENO", 3);
	}
	if (fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK|O_ASYNC) < 0) {
		PANIC("fcntl: Could not F_SETFL on STDIN_FILENO", 3);
	}
}

static void io_handler() {
	// Just kick back our task into gear!
	iwup_tsk(CONSOLE_TASK_ID);
}

static void command_handler(char* str);
static void task_console(VP_INT exinf) {
	for (;;) {
		struct linenoiseState ls;
		g_ls = &ls;
		char *line = NULL;
		char buf[1024];
		linenoiseEditStart(&ls, -1, -1, buf, sizeof(buf), "[zepto]-> ");
		for (;;) {
			int ret;
			fd_set readfds;

			FD_ZERO(&readfds);
			FD_SET(ls.ifd, &readfds);

			struct timeval tv;
			tv.tv_sec = 0;
			tv.tv_usec = 0;

			ret = select(ls.ifd+1, &readfds, NULL, NULL, &tv);
			if (ret == -1) {
				perror("select()");
				PANIC("Error with `select()` host API...", E_SYS);
			}
			else if (ret) {
				line = linenoiseEditFeed(&ls);
				/* A NULL return means: line editing is continuing.
				 * Otherwise the user hit enter or stopped editing
				 * (CTRL+C/D). */
				if (line != linenoiseEditMore) break;
			}
			else {
				// Wait until there is I/O
				slp_tsk();
			}
		}
		linenoiseEditStop(&ls);
		g_ls = NULL;

		/* Ctrl+D/C. */
		if (line == NULL) {
			vprocess_exit(0);
		}

		linenoiseHistoryAdd(line);
		command_handler(line);
		free(line);
	}
}

/************************
 *   Command handlers   *
 ************************/

#define TRY_CMD(scope, cmd) if (token != NULL && strcmp(token, #cmd) == 0) { cmd_##scope##_##cmd(); return; }

static void cmd_main_help() {
	vconsole_stdoutf(
		"\n"
		"Commands supported by this shell:\n"
		"  help        Prints this help.\n"
		"  kerext      manages kernel extensions.\n"
		"\n"
		"  exit        stops this zepto-ish instance.\n"
		"\n"
	);
}

static void cmd_kerext_help() {
	vconsole_stdoutf(
		"\n"
		"kerext <command>...\n"
		"\n"
		"sub-commands:\n"
		"   help                 Prints this help\n"
		"   load <kerext.so>     Used to load a kernel extension\n"
		"\n"
	);
}

static void cmd_kerext_load() {
	char* token = strtok(NULL, " ");
	if (token == NULL) {
		vconsole_stderrf("error: kerext load expects one argument...\n");
		cmd_kerext_help();
		return;
	}

	vkerext_load(token);
}

static void cmd_main_kerext() {
	char* token = strtok(NULL, " ");

	TRY_CMD(kerext, load);
	TRY_CMD(kerext, help);

	if (token == NULL) {
		cmd_kerext_help();
		return;
	}

	vconsole_stderrf("'%s': kerext subcommand unknown...\n", token);
}

static void cmd_main_exit() {
	vprocess_exit(0);
}

static void command_handler(char* line) {
	char* token = strtok(line, " ");
	if (token == NULL) {
		return;
	}

	TRY_CMD(main, exit);
	TRY_CMD(main, help);
	TRY_CMD(main, kerext);
	
	vconsole_stderrf("'%s': command unknown...\n", token);
}

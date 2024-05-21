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

/**
 * Entrypoint of the zepto-dlopen tool.
 */

#include <kernel.h>

// Non-portable host APIs
#include <dlfcn.h>
#include <stdlib.h>

/**
 * The only additional API exposed by this program.
 * Use the following `extern` when referencing it.
 *
 * ```c
 * extern ER vkerext_load(char* lib);
 * ```
 */
ER vkerext_load(char* lib) {
	DBG("Opening given kernel extension (%s)...", lib);

	void* handle = dlopen(lib, RTLD_LAZY | RTLD_LOCAL);
	if (!handle) {
		vconsole_stderrf("Could not dlopen(%s); ERR: %s\n", lib, dlerror());
		return E_PAR;
	}
	DBG("... finished opening given kernel extension (%s)...", lib);

	return E_OK;
}

ER static_setup(int argc, char** argv) {
	ER ret;

	if (argc != 2) {
		vconsole_stderrf("ERROR: No kernel extension provided\n");
		vconsole_stdoutf("Usage: %s <./kernel_extension.so>\n", argv[0]);
		vconsole_stdoutf("\n");
		vconsole_stdoutf("       NOTE: the parameter is given to `dlopen`, and\n");
		vconsole_stdoutf("       follows its path and LD_*_PATH semantics.\n");
		exit(1);
	}

	ret = vkerext_load(argv[1]);

	if (ret != E_OK) {
		return ret;
	}

	return E_OK;
}

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

#include <itron.h>
#include <kernel.h>

extern ER vkerext_load(char* lib);

__attribute__((constructor)) void test_init() {
	DBG("Would initialize the kernel extension here!");
	// Shows that loading the same kernel extension doesn't cause a loop.
	vkerext_load("./kerexts/simplest.so");
}

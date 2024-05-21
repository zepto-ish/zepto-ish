`zepto-dlopen`
==============

```c
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
```

*`zepto-dlopen` is part of the zepto-ish project...*

The `zepto-dlopen` tool implements a vendor-defined *kernel extension* system.
The kernel extensions are defined as `.so` file, which will be loaded via `dlopen`.
This allows complex tasks to be composed at runtime.

Ignoring the fact it uses `.so` files, the API is not portable as it implies usage of a *filesystem* outside of the scope managed by the kernel.

This demo also shows how one can extend zepto-ish and its µITRON API surface in their own application.

Note that `zepto-dlopen` provides only a single feature: the `vkerext_load` service call.

Implementation details
----------------------

The `.so` kernel extensions are expected to self-define their usage at load by using `__attribute__((constructor))`.
This can be used to add tasks, interrupts, or really anything one would do in the *initialization routines*.

Note that this follows the usual zepto-ish limitations and drawbacks.
E.g. no automatic assignation of task identifiers.

It is expected that the `vkerext_load` service call will be used through `extern` symbol resolution.
This is because there is no service calls support in zepto-ish.

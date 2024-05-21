`zepto-ish`
===========

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

A minimum-functionality satisfactory implementation of *µITRON4.0* (according to *5.1.2 Minimum Required Functionalities*).

It implements this minimum required functionality:

 - Task creation/activation/termination (states: RUNNING, READY, DORMANT)
 - Task scheduling (conforming, with acceptable limitations)
 - Interrupt handling

And additionally:

 - Task sleep/wake-up
 - A couple [vendor-specific service calls](SERVICES.md) for convenience

All of this implemented as userspace Linux application.

> See the [Implementation Details](DETAILS.md) for more details.


## What does it do?

For now, not much.

If you run the current demo program (without reading its source), you will see it appears to hang after an hello world.
It is not hanging, it has no tasks to run anymore.
You can send `SIGINT` or `SIGUSR1` to start `Task#1`, and `SIGUSR2` to start `Task#2`.

This is only a silly demo, though, so don't expect too much.


## Interrupts? In my userspace???

Since this is all for demonstration purposes only, an "interrupt model" in userspace has been defined.

> See [INTERRUPTS.md](INTERRUPTS.md) for details


## Why?

Why what?


### Why as a Linux application?

Because running this on real hardware would be cumbersome, especially at first!

In addition, I think designing it this way has value, as you could simulate applications on the host system, and hopefully run it later on embedded.


### Why do this?

Have you never wanted to write your own µITRON4.0 specification OS?
*Weird*.


### Why C?

Proof of concept at first.

Let's *C* where it goes from there...


### Why are you like this?

No one stopped me.


### Why zepto?

Blame the <del>French</del> [metric system](https://en.wikipedia.org/wiki/Metric_prefix#List_of_SI_prefixes).
Smaller than micro, and sounds neat.


### Why ish?

¯\\\_(ツ)\_/¯

Though mainly because I don't want to have *TRON* be part of the name of the project, since the TRON projects defines *specifications*.

And here, I'm roughly making something.

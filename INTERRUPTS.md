Interrupts under zepto-ish
==========================

The minimum required functionalities for µITRON 4.0 requires *registration of interrupt handlers (or interrupt service routines).
It is thus required to handle them in zepto-ish to be complete.

Interrupts for a userspace application are unexpected, normally.
The closest you would get normally are *`signal(7)`*.

For the purpose of this “toy” µITRON 4.0 implementation, all signals are implicitly available as interrupts.


`signal(7)` interrupts
-----------------------

The first `SIGRTMAX` interrupts (usually 64) are reserved for signals.

Not all signals are available, or make sense to use.

As usual, `SIGKILL` and `SIGSTOP` cannot be intercepted or handled.

The usual signal names should NOT be used, as their numbers are not necessarily portable.

Instead, use the `INTNO_*` equivalent.


Other interrupts
----------------

(TBD)

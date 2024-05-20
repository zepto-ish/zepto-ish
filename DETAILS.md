Implementation Details
======================

## Specification

The µITRON 4.0 Specification can be found on the TRON Forum website.

 - https://www.tron.org/specifications/

Search for `µITRON 4.0 Specification` to find the link.


## Conformance

> ```
>   µITRON3.0 Spec     µITRON4.0 Spec
>   --------------     --------------
>                    o- Full set
>        Level E -o  |
>                 |  o- Standard Profile
>        Level S -o  |
>                 |  o- Automotive Control Profile
>        Level R -o  |
>                    o- Minimum set
> ```
> 
> — (Section 1.4.4)

`zepto-ish` conforms to the minimum set, broadly speaking.

Conformance:

 - Creation of tasks
 - Task states RUNNING, READY and DORMANT
 - A method to activate tasks (`TA_ACT`, `act_tsk`, `iact_tsk`)
 - A method for tasks to terminate (`ext_tsk` and `return` from main routine)

Current caveats:

 - No interrupt handlers yet
 - Scheduling incomplete
   - Limited to a single priority level (valid)
   - No queue for task scheduling (invalid)
   - As such, the task id works as task priority would (invalid!)

Other notes not affecting conformance:

 - No *static API*.
 - No *configurator*.


## About Task ID 0

There is an implicit task ID 0 "available" in the array of tasks.
It is currently not used, and actual Task ID 0 is a reference to `self`.
As such, it would be invalid to expose the "available" ID.

Why does it exist there?
Convenience!
This way, I can map task IDs to array indices.

With that said, task ID 0 might be useful in the future.
It might be used for dogfooding tasks for the main loop runner?
It might be used for other purposes too.

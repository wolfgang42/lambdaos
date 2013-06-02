**LambdaOS** will be an operating system based around a MOO engine, specifically
LambdaMOO. It's largely a project to allow me to explore operating system
development; it's not necessarily intended to be a practical OS for everyday
use.

**Why a MOO?** LambdaMOO already includes a task scheduler and permissions
system, meaning that I don't have to program any of this. Basically, LambdaOS
is just a *very* thin layer of glue between the hardware and the moo engine.

Roadmap
-------
* Get keyboard driver working
* Create console driver to connect keyboard & display
* Integrate malloc()
* Begin integrating LambdaMOO engine, probably stunt.io derivative

Sources
-------
* IDT, GTD, keyboard routines derived from [Brian's Kernel Development Tutorial](http://www.osdever.net/bkerndev/Docs/gettingstarted.htm)
* Core & VGA routines from (Bare Bones)[http://wiki.osdev.org/Bare_Bones] on the OSDev Wiki

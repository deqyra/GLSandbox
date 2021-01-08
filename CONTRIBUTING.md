First of all, **thank you** for considering contributing (and thank you even if you're not).  
RenderBoi was written with a few things in mind, regarding architecture as well as code style:
* **Keep things simple.** If it makes sense, go for it. If it's convoluted, break it down. Even if it does make sense as is. Complex is better than complicated.
* **Explicit over implicit.** Verbosity is (almost) never a problem. `using` directives for whole namespaces are frowned upon, especially in the global scope.
* Naming stuff:
  * Make it clear. Keep in mind: explicit over implicit.
  * Local variables:
    * `regularVariable`
    * `ConstVariable`
    * `_staticVariable`
  * Class attributes (public, both static and non-static):
    * `regularVariable`
    * `ConstVariable`
  * Class attributes (protected or private, both static and non-static):
    * Same as above, but with a leading `_`: `_regularVariable`, etc.
  * Class methods (public):
    * `myNonStaticMethod`
    * `MyStaticMethod`
  * Class methods (protected or private):
    * Same as above, but with a leading `_`: `_MyStaticMethod`, etc.
* Provide Doxygen readable doc above each function *declaration* (header, not source).
* The recommended maximum line width is 80 for docs or comments in general, and 120 for code (130 is fine though).

These guidelines were merely put up here for the sake of having them around, and they should probably be taken with a grain of salt.
I personally do my best to stick to them, but I won't go out of my way to enforce them. No big deal if they aren't followed. :)

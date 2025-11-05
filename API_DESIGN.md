API & ADT Design
================

## Lists & Collections
- All entities
- All entities by nation
- All Entities by sector
- Entities by nation plus fleet


## Maps
The world map will be in array, as well, the nation map.  Map entries will have pointers to the sectors that are to the left right above and below.

Each sector probably needs an observed method that generates data for a non-friendly nation that they would be able to see. That generates with a non-friendly nation would be able to see.


## Instrumentation

In addition to a really good logger, we should also build in a metrics and instrumentation capability to monitor the performance of the system. It should have periodic performance counters that measure the number of operations as well as timers that measure the duration of operations. That information should be made visibleand should could provide benchmarks and indications for health management.


## Configuration

Commandline options and global configuration and knowledge of the platform

When serializing, store the fixed, compiled-in configuration.  When restoring
from a saved state, compare the current configuration with the stored state and
report any changes.

## Design Notes to be Organized

Every unit gets a nation, serialized ID, and a random UID that will not change.

Have a way of destroying and re-creating a Singleton.  This is for testing purposes only.

I think our namespace strategy is lacking

Hang class UID_Generator off of Core.   It can generate one or contiguous blocks of ids.

Consider creating a serialized number class, a Singleton actually. And then assigning a unique ID number to every single object ever created. And that would be the primary key for the serialization or marshaling mechanism.

We definitely need something like a model view controller architecture model is the colonels internal database. The view is each users capital centric view of the world. And everything the user interacts with needs to interact based on that map. In other words, users should never be exposed directly to the model itself.

Put most of the commodity counters in the base class, or at least a large fraction of them, so they’re always there and in a predictable cashed place


## Singletons

I've read [Singletons are Pathological Liars], [Where Have All the Singletons Gone?]
and [Performant Singletons].

> The Singleton is probably one of the most controversial design patterns,
> sparking perennial debate on forums and discussion boards, and prompting
> analysis and dissection in many articles and papers.

> -- <cite>Performant Singletons</cite>

The Singleton pattern should be applied only when:
1. Singularity must exist
2. Global accessibility must exist

I think this applies to the objects listed above.

The arguments against Singletons...
- Well-designed systems isolate responsibility.  Singletons encourage tight
  coupling by making it easy for components to depend on them.
    - Agreed:  I'd have to think on this, but my initial reaction is that, for
      the things I intend to make Singletons, those dependencies are inherent.
- Whether there needs to be a single instance of an object depends on the
  context that object is being used.
    - I will not be running multiple instances of Empire in the same process space.
    - I won't be testing multiple Empire programs concurrently.  One Empire at a time.
    - We should have the ability to shutdown and restart the core in the same process.
- Have the self-control and awareness to create as many instances as you
  need — one, if that’s all it takes.
    - I'd rather have the object guarantee this.  Isn't that the argument for
      Encapsulation; The object hides its data to protect itself.
- When you encounter a scenario where you need to depend on another object or
  subsystem, stop and ask yourself why you need that dependency.
    - Yea, sometimes we really do.  Even if I have Coreable, Loggable, and Nationable
      interfaces, sooner or later, I'll need an actual object to reference.
    - It's global state.  It is what it is.  The trick is to make is usable,
      extendable **and** safe.
- How do you create mock worlds for testing?
    - How will we test multiple world sizes?  How will we test creating and
      destroying a world?
        - To achieve optimal performance, we'd hardcode configuration (for
          example). But this sacrifices a lot of unit testing capabilities.  
          Therefore, we will make a testability-over-performance tradeoff and make
          configuration dynamic.
        - empire::Singleton will have an `erase()` method which will destroy it
          and allow it to be recreated.
- You loose control of initializations
    - Not true.  The Core will initialize all the Singletons.  That's what
      the core does.  Also, I will be using instances of Singletons (or the
      contents therein) as parameters when I create other objects.
    - The test harness will manage the test Singletons.
- Explicit reference passing makes garbage collection work.
    - All of these objects will begin with Empire and end with Empire.
    - The `erase()` method on Singleton will destroy.  There's nothing wrong
      with that.  The Singleton guarantees only one object will
      exist after you ask for it (which is still true).  It also makes it
      globally available, which will also continue to be true.
- Singletons are nothing more than global state. Global state makes it so your
  objects can secretly get hold of things which are not declared in their APIs,
  and, as a result, Singletons make your APIs into pathological liars.
    - It's true... I'm collaborating with more objects than the API claims.  I'll
      give this some thought.
- As the original author of the code, I know the true dependencies.  Anyone
  who comes after is baffled, since not all of the dependencies are
  declared and information flows in some secret paths which are not clear from
  the API.  You live in a society full of liars.
    - That's a little dramatic.  Especially, if you tell everyone up-front what
      our Singletons are and they are logical.  Still, I think it's good point.
      When do we pass a parameter and when do I grab from a Singleton?
- Globals provide invisible lines of influence across all the code
    - Let's distinguish between reading & writing globals.  Unexpected writing
      to a global is bad.  We can all agree on that.  Reading from read-only
      globals are not too different from constants, which is fine.
    - So, let's not make native datatypes a Singleton.
    - Agreed, in the context of raw variables...  but these globals are objects
      that have all of the protections an encapsulated class should have.
- Globals lead to spaghetti code.
    - Agreed, when they are abused.  All global state will be maintained in the
      object model.
    - I'm not creating `static int i, j, k` as global iterators.
- When you go to include 3rd party libraries in your code, sometimes they use
  the same names as yours.
    - All of our globals will be in the Empire namespace.

Detractors say that Singletons actively work against separation of duties by
introducing implicit, hidden dependencies on external systems and allowing
instant access from any location, even if that location is illogical.  True, but
I can't see why having access to any of the above would be illogical.

I also do _**not**_ want to pass these into every function, constructor, et. al.
under the sun.  That's not happening.

I agree that Globals should be used with utmost care, and functions should
clearly scope out the variables they use via an API, locals, what have you.

Another way to manage this is to carefully control the creation of Singletons.  We aren't
letting just any variable, say `auto i`, to be used as a global iterator.

The important thing is to remember the overall goal: clarity

The "no global variables" rule is there because most of the time, global
variables make the meaning of code less clear.  However, like many rules, people
remember the rule, and not what the rule was intended to do.

I've seen programs that seem to double the size of the code by passing an
enormous number of parameters around simply to avoid the evil of global variables.
In the end, using globals would have made the program clearer to those reading it.
By mindlessly adhering to the word of the rule, the original programmer had
failed the intent of the rule.

So, yes, globals are often bad. But if you feel that in the end, the intent of
the programmer is made clearer by the use of global variables, then go ahead.
However, remember the drop in clarity that automatically ensues when you force
someone to access a second piece of code (the globals) to understand how the
first piece works.

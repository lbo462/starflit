# How to continue the project

Being here for you means that you probably read all the other
documentation, or that you are ready to do so. Here, we'll cover
what could be done in order to continue the project from we left
it, together with some ideas and technical details of how to do
so.

First things first, if you want to start where we left of, you
should [fork](https://en.wikipedia.org/wiki/Fork) our GitHub repo
at https://github.com/lbo462/starflit, as we described on the
getting started section of this documentation. This will allow
you to have your own Starflit version that you will be able to
modify at wish.

> If you didn't read the `Getting Started`, you definitely should
> as all the installation process lies in there. Do not
> underestimate this section !

## Best practices

When working on the project, one should keep to best practices.
As we're students, we don't have much responsibility about doing
things clean, but that's a good habit to have anyway.

When writing code, check how we did things to keep a continuous
and homogenous workflow. Use git with care, create virtual env
when working with Python, cut your code in clearly defined and
independent classes.

When writing C++ code, check the C++ best practices. When
writing python code, check the python best practices. Be clean,
be thoughtful, be professional.

But most of all, __write documentation at any step of the__
__project!__ Whenever you're doing something, write down what
you're doing. It will help you have a clearer view of what you're
doing and will help others to work and help you. And this makes
a project maintainable and live through the years!

## Technical improving

There are many ways to improve this project on a technical basis.
In every section of the documentation of the project, you should
find a section about potential improvements that could be made
for these particular sections.

Here are few examples:
- Add some LIDAR sensors to better the exploration and mapping.
- Implement a complementary or a Kalman filter for the control
engineering.
- Fine-tune a model to run on the Raspberry PI to detect
different objects.
- etc ...

## Hardware upgrading

The strandbeest have multiple sensors that can be upgraded, like
the ultrasonic sensors or the gyroscope. Upgrading them will
allow you to have a better precision on the different presented
features.

One could also imagine replacing the legs with tank
tracks. This will make it have cleaner movements, easier to
control through control engineering.

Here's an example on AliExpress : 
https://fr.aliexpress.com/item/32996181258.html

## Add some mapping

The Starflit crew actually couldn't implement any form of mapping
within the given time. But we had time to make some research
about it, and write a full document concatenating what we found
useful.

__Why add some mapping ?__

First things, because it's very cool. Second thing, one could
imagine a scenario in which the stranbeest will be aware of its
environment to improve its searching process.

Plus, the mapping process should be a shared process between all
the robots, so that all the robots are building a same map,
preferably using no central unit to avoid SPOF.

Plus, if the robots use to have a map, one will be able to add
path algorithm so that they're able to reach pinned points, such
as the location of a found object.

__Be aware!__

If you're able to implement a solution for mapping the
environment, there are few things that you might want to consider.
You should be very thoughtful about how you're going to model
the map, as you're probably going to need to send it to the other
robots. To do so, you'll need to update the exchanged frames
to support more complex data structures.

This is a tricky, yet very cool feature to implement.

## Add some fine-tuning

There's a whole documentation about fine-tuning in the object
recognition as this has been a non-negligible part of our work.
Therefore, we couldn't end up with a fully functional transfer
learning process.

This could improve the project is a significative way since it
would add some flexibility about what the strandbeest could be
detecting.

If you're trying to implement this, try reading what we tried
first, and what we recommend in the object recognition doc.


## Add some arms

Haven't you always wished to see the strandbeest with some arms ?
This is definitely the option I prefer. Find a way to add arms
to the strandbeest and make so that it's able to grab the object
it's searching. It would be a very useful and cool feature to
have, right ?

This would require a nice implementation of image segmentation
and a great control engineering system.

This would not only be a software project, but also an electrical
and mechanical one, ambitious and perfectly fitted for tinkerer
people.

---

If you want to add all of this, we might see one day, a fleet
of strandbeest searching for lost items, drawing a map in real
time of the area, and bringing back lots of treasures in no time!

Have fun~
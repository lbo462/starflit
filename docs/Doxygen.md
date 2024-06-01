# Generating the documentation

Here, we use the famously know [Doxygen](https://www.doxygen.nl) to generate the documentation.
In addition, we use [doxygen-awesome](https://jothepro.github.io/doxygen-awesome-css/) for the styling,
and __Github Actions__ to upload the documentation on Github Pages.

To use Doxygen locally, install it for your OS by following the instruction on their [website](https://www.doxygen.nl/i).

In order to generate the graphs as well, one should have the `dot` command available.
For that purpose, install [Graphviz](https://graphviz.org/download/) and make sure it's reachable before you call Doxygen.

Then, use the `Doxyfile` file to generate the documentation.

> On Linux, call `doxygen Doxyfile` at the root of this project.

It will create / update the folder `docs/html` that will contain the generated doc.
Open `docs/html/index.html` in your browser to read through it.

> The folder `docs/html` is ignored by git and should stay that way.
> Instead, the doc is generated through Github Actions and deployed on [Github Pages](https://lbo462.github.io/starflit/).
> See the file `.github/workflows/static.yml` to explore the deeps of the job doing this magic trick.

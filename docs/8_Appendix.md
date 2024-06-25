# Appendix

Welcome to the appendix 🤯 !

Here, you'll find all the miscellaneous documentation we couldn't fit anywhere
else. You're encouraged to use the navigation tree to read through this, as 
order doesn't exist here.

## How to work with git and GitHub

You need to have `git` installed on your computer (Linux is preferred but `git bash` for Windows works just fine).

Clone the repo with `git clone git@github.com:lbo462/starflit.git`, 
after having a clean [SSH-key access set-up](https://docs.github.com/en/authentication/connecting-to-github-with-ssh/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent)

The other useful and only commands you'll ever need for this project are:
- `git add -v .` to stage your local changes
- `git commit -m "Your message"` to commit these changes
- `git push origin <your-branch>` to push your commit to GitHub
- `git fetch origin -v` to fetch changes and new branches from GitHub
- `git checkout <branch>` to change the current branch
- `git rebase master -i` to rebase your branch commits on the commits added to `master`

And these ones might be of use:
- `git status` the see a global view of changed files
- `git diff` to see in details what changed
- `git checkout .` to reset your changes to the previous commit. __To use carefully__ 

Here's the standard working process:

__Don't push to__ `master`.
Instead ...

- Create an issue at [https://github.com/lbo462/starflit/issues](https://github.com/lbo462/starflit/issues)
and describe what needs to be done.

- On this issue, create a branch (checkout locally), at the bottom right of the page (under the Development section).

- From now on, `git fetch origin -v` to fetch the created branch and `git checkout <branch>` to work on the given branch.
You can freely make commits and push to that branch to implement what's needed.

- From that issue and once you made changes, create a pull request at 
[https://github.com/lbo462/starflit/pulls](https://github.com/lbo462/starflit/pulls)
and select the branch created for the issue.

- Once your work is done, assign someone to review your work and approve your changes.

- Once approved, you can __squash and merge__ your branch via the pull request you created.

- A good practice is to delete the merged branch once the pull request is accepted, and can be done via the pull request.

### Rebase

Rebasing is the tricky part of this process.

It's required when some changes happened on `master` after you created your branch.
One way to solve this would be to solve merge conflict but it might be complex and
won't always work as expected if done via GitHub.

Instead, we propose here to rebase the incoming branch.

Before going any further, it is required to pull the last changes from `master`.
To do so:

```shell
git checkout master
git pull origin master
git checkout <your-branch>
```

Now you can proceed with `git rebase master -i` and follow the interactive documentation.

This process replays your branches commits from `master` so there's can't be conflict 
when merging the two branches and one keep a linear commit history.

Once the rebase is done, just push your branch to GitHub and have someone review your
changes.

---

## Code organization

As for any project, it's important to keep a nice and clean organization. And
that goes for the code too.

### Partioning

Partioning the different part of a project is really important while working
on big projects. This means that we could have multiple repository : one for 
the Arduino part, an other for the Raspberry part, an other for documentation,
etc.

But, the starflit isn't that big, and we try to keep things simple for
newcomers. Still, we tried to organize the project in different sections, in 
different folders:
- The Arduino section, keeping all the code the Arduino cards
- The Python section, for the Raspberry part
- The documentation section

### About the Arduino code ...

Usually, when working on Arduino on small projects, we keep it simple, with no
partioning whatsoever. But the starflit project is not that small, and we
have to get things straight from the start.

We decided to write our code with `.cpp` and `.h` files, as in C++.

> The arduino language is a layer of top of C++, created specially for these
> controllers. This means that we can directly write some C++ code and compile
> it for Arduino, via importing the `Arduino.h` library.

For the ones having no idea of what are these files, the `.h` files holds the
class or function definitions, aka signatures. See more at
[https://www.learncpp.com/cpp-tutorial/header-files/](https://www.learncpp.com/cpp-tutorial/header-files/).

We kept a Java-style code writing, meaning that we have one file = one class.

We regrouped the files in a same folder when they're only used by the main 
class of the folder. For example:

```
SmartMotors/
    SmartMotors[.h / .cpp]     # Main class of the folder (has the same name)
    AxelGyroSensor[.h / .cpp]  # Only used by SmartMotors
    Motors[.h / .cpp]          # Only used by SmartMotors
```

Also note that we have put all our files in a `src/` folder, which is a
requirement in order to have the possibility of having nested folders.

With this files architecture, we have a single `.ino` file that imports our
custom classes from the `src/` folder, and we can keep this script trivial, as
the logic stands in our C++ files.

> Most of the class have a `setup()` and `update()` method. The `setup()` 
> method should be called in the `setup()` function on the Arduino `.ino`
> sketch, while the `update()` method should be called in the `loop()` 
> function.

Our main interface between the executed `.ino` sketch and the C++ code is the
`RecueBot` class, which is instantiated in the sketch, setted-up in `setup()`
and updated at each iteration in `loop()`.

---

## Generating the documentation

Here, we use the famously know [Doxygen](https://www.doxygen.nl) to generate
the documentation. In addition, we use
[doxygen-awesome](https://jothepro.github.io/doxygen-awesome-css/)
for the styling, and __Github Actions__ to upload the documentation on Github
Pages.

> We're currently using
> [Doxygen 1.9.1](https://github.com/doxygen/doxygen/releases/tag/Release_1_9_1)
> It seems that versions 1.9.8 and above are making the `Modules` section disappear.
> Hopefully, that won't be the case, but otherwise, check `DoxygenLayout.xml`.

To use Doxygen locally, install it for your OS by following the instruction on
their [website](https://www.doxygen.nl/i).

In order to generate the graphs as well, one should have the `dot` command
available. For that purpose, install
[Graphviz](https://graphviz.org/download/) and make sure it's reachable before
you call Doxygen.

Then, use the `Doxyfile` file to generate the documentation.

> On Linux, call `doxygen Doxyfile` at the root of this project.

It will create / update the folder `docs/html` that will contain the generated
doc. Open `docs/html/index.html` in your browser to read through it.

> The folder `docs/html` is ignored by git and should stay that way.
> Instead, the doc is generated through Github Actions and deployed on
> [Github Pages](https://lbo462.github.io/starflit/). Check the file
> `.github/workflows/static.yml` to explore the deeps of the job doing this
> magic trick.

### About the pythonic doc

Doxygen doesn't really seem to have been designed for Python. The
documentation it provides is not exhaustive and one might want to look
directly inside the code to check all the written comments.

If a bored soul's reading this, note that
[Sphinx](https://www.sphinx-doc.org) is a nicer solution to generate the
Python documentation, and that there exist 
[Breathe](https://breathe.readthedocs.io/) to combine the documentation
generated by Doxygen and Sphinx.

Here, we made the choice not to implement such a solution for the sake of
the simplicity of this project, but you are free to do so.

### How we wrote the documentation

All the documentation you're reading was written in
[Markdown](https://www.markdownguide.org/) under the `.md` extension. You
can find the raw version on the GitHub in the folder `docs/`.

In order to work as a team on these documentation, we hosted a collaborative 
markdown editor tool, named [CodiMD](https://github.com/hackmdio/codimd) on a 
server we owned. It helped us to have a similar workflow as Google Doc, but 
for markdown documentation writing.

---

## SHS - Sciences Humaines et Sociales

### Introduction

Notre projet a pour objectif de faire du « search and rescue » avec une flotte de robots. En situation réelle, ce genre d’implémentation peut servir pour faire du sauvetage de personnes en zone de guerre ou de catastrophe naturelle. 

Nous pouvons imaginer déployer un million de robots pour parcourir un site dangereux. Dès qu’ils ont localisé une personne en vie, ils remontent les coordonnées GPS à un centre de contrôle pour dépêcher efficacement des équipes de sauvetage. De cette manière, on risquerait moins de vies humaines en cherchant les rescapés.

### Questions éthiques

Cependant, cela soulève quelques questions éthiques. Comment faire confiance à un robot pour détecter une personne vivante qu’on peut encore sauver, d’une personne décédée ? 

Les robots doivent être équipés de capteurs extrêmement précis pour distinguer les signes de vie (comme la chaleur corporelle, le mouvement ou le rythme cardiaque). Une erreur de détection pourrait entraîner des pertes humaines évitables. 

De plus, les algorithmes utilisés pour analyser les données des capteurs et prendre des décisions doivent être rigoureusement testés et validés. Ils doivent être capables de fonctionner dans des conditions variées et extrêmes.

En cas de défaillance des robots, qui serait tenu pour responsable ? Le fabricant du robot, les programmeurs, ou l’organisation déployant les robots ? Ces questions sont de nos jours beaucoup soulevées dans le cas de la voiture autonome. 

En 2020, le Conseil de l'Europe a adopté des lignes directrices pour assurer la sécurité des véhicules autonomes et clarifier les responsabilités en cas d'accident. L'objectif principal est de garantir que les constructeurs automobiles soient responsables de la sécurité et de la performance de leurs systèmes autonomes. Mais tout cela se complique si les programmeurs du robot travaillent dans une société externe au fabricant et qu’on ne peut pas prouver la faute de l’accident.

Sur une zone de guerre, des questions de cybersécurité s’imposent également. La survie de la population ou l’armée attaquée est une donnée très sensible qui doit être transmise de manière sécurisée. Les robots doivent être protégés contre les cyberattaques et l’utilisation malveillante de leurs données. Les systèmes de communication doivent être chiffrés et résilients face aux tentatives de piratage.

### Questions écologiques

Les questions écologiques ne sont pas à négliger. Nos robots sont cassables et majoritairement composés de plastique. S’ils se cassent sur le terrain, des débris vont être laissés et constituer des déchets.

La fabrication d’un million de robots nécessitera des ressources naturelles et de l’énergie, contribuant potentiellement à une empreinte carbone significative. Utiliser des matériaux recyclables et des sources d'énergie renouvelable pourrait aider à atténuer cet impact. Une fois les robots en fin de vie, des stratégies doivent être mises en place pour recycler efficacement leurs composants, afin de réduire les déchets électroniques.

### Conclusion

Le déploiement d’une flotte de robots pour des opérations de "search and rescue" en zones de guerre ou de catastrophe naturelle représente une avancée technologique majeure, promettant d'améliorer l'efficacité des opérations de sauvetage tout en réduisant les risques pour les secouristes humains. Cependant, il est crucial de considérer les questions éthiques et écologiques liées à cette technologie.

Les questions éthiques touchent à la fiabilité des robots, la responsabilité en cas de défaillance, et le rôle des humains dans les décisions critiques. En parallèle, les questions écologiques incluent l'impact environnemental de la production et du déploiement des robots, la gestion des sources d’énergie et des déchets.

---

## (unofficial) SHS - Version lyonnaise

### Introduction

Notre projet, c’est d’faire du « search and rescue » avec une flotte de robots, pélo. En situation réelle, ce genre d’truc peut servir à sauver des gonzes en zone de guerre ou après une catastrophe naturelle.

Imagine, on balance un million de robots pour fouiller un endroit craignos. Dès qu’ils choppent quelqu’un en vie, ils renvoient les coordonnées GPS au centre de contrôle pour qu’on envoie les équipes de secours direct. Comme ça, on risque moins de vies humaines en cherchant les survivants.

### Questions éthiques

Mais bon, ça soulève des questions éthiques, tu vois le genre. Comment on peut faire confiance à un robot pour détecter quelqu’un de vivant qu’on peut encore sauver, plutôt qu’un trépassé ?

Les robots doivent être équipés de capteurs ultra-précis pour capter les signes de vie (comme la chaleur corporelle, les mouvements ou le rythme cardiaque). Une erreur de détection pourrait entraîner des pertes humaines évitables.

En plus, les algos utilisés pour analyser les données des capteurs et prendre des décisions doivent être béton et validés à fond. Ils doivent tourner dans des conditions variées et extrêmes.

Et si les robots partent en sucette, qui est responsable ? Le fabricant du robot, les codeurs, ou l’organisation qui les balance sur le terrain ? Ces questions, on les entend aussi pour les bagnoles autonomes.

En 2020, le Conseil de l'Europe a pondu des lignes directrices pour assurer la sécurité des véhicules autonomes et clarifier les responsabilités en cas d'accident. L’idée, c’est que les constructeurs doivent être responsables de la sécurité et des perfs de leurs systèmes autonomes. Mais tout ça devient relou si les codeurs du robot bossent pour une boîte externe au fabricant et qu’on ne peut pas prouver la faute de l'accident.

En zone de guerre, des questions de cybersécurité s’imposent aussi. La survie de la populace ou de l’armée attaquée, c’est une info hyper sensible qui doit être transmise en toute sécurité. Les robots doivent être blindés contre les cyberattaques et l’utilisation malveillante de leurs données. Les systèmes de communication doivent être chiffrés et béton face aux tentatives de piratage.

### Questions écologiques

Les questions écolos, c’est pas à zapper. Nos robots, ils peuvent péter et ils sont bourrés de plastique. S’ils se cassent sur le terrain, ça fera des débris et des déchets.

La fabrication d’un million de robots va bouffer des ressources naturelles et de l’énergie, ce qui peut alourdir l’empreinte carbone. Utiliser des matériaux recyclables et des sources d'énergie renouvelable pourrait aider à alléger cet impact. Quand les robots arrivent en fin de vie, il faudra des plans pour recycler leurs composants efficacement, pour réduire les déchets électroniques.

### Conclusion

Déployer une flotte de robots pour des opérations de "search and rescue" en zones de guerre ou après une catastrophe naturelle, c’est une avancée techno de ouf, promettant d'améliorer l'efficacité des opérations de sauvetage tout en réduisant les risques pour les secouristes humains. Mais faut pas oublier les questions éthiques et écolos liées à cette techno.

Les questions éthiques touchent à la fiabilité des robots, la responsabilité en cas de pépin, et le rôle des humains dans les décisions critiques. En parallèle, les questions écolos incluent l'impact sur l’environnement de la production et du déploiement des robots, la gestion des sources d’énergie et des déchets.

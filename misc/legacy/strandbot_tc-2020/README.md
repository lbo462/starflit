# Vers une flotte de strandbeests autonomes et intelligents… 
```mots-clés: Motion planning, Swarm robotic, SLAM, Visual SLAM.```
L’objet de cette étude s’inscrit dans un projet ambitieux :  construire une flotte de robots- strandbeests autonomes, intelligents et capables de coopérer pour réaliser une mission commune.
Les scenarios mettant en œuvre des flottes de robots sont nombreux : tâches dangereuses à l'être humain, déploiement de secours post-catastrophe, gestion des entrepôts, nettoyage de sites, applications militaires, etc. 	
Cependant pour naviguer de manière autonome, planifier des trajectoires et effectuer des tâches de manière efficace et sûre, les robots doivent être capables de se localiser dans leur environnement, construire une cartographie des informations spatiales de l'environnement avec ou sans connaissance préalable et partager leurs informations.
Ce projet vise à dresser dans un premier temps un état de l’art des travaux de recherche en lien avec les systèmes multi-robots autonomes. Durant la phase projet, les efforts seront focalisés sur une première implantation permettant aux strandbeests d’évoluer librement et sans collision dans un espace défini.
## Charte de développement

Cette application est destinée à être mise en service et maintenue, de plus les données qui vont y transiter ont un caractère sensible. En tant que développeur nous vous demandons donc d'observer les règles suivantes:

- Il vous sera impossible de _push_ vos modifications sur la branche **master**, ce afin de respecter l'intégrité du projet ainsi que les règles élémentaires de l'intégration continue.
- Chaque _commit_ doit contenir des modifications _cohérentes_ avec l'objectif du projet, et être **minutieusement commenté**. Ce afin de faciliter la relecture et la navigations par le reste de l'équipe.
- Une modification importante du code doit suivre cette méthode:
  1. Créer une _issue_ bien titrée et comportant une **description claire** de l'amélioration. L'assigner et la labeliser correctement.
  2. Créer une branche à partir de la branche désirée avec un nom bref et clair.
  3. Créer une _pull request_ sur la branche créée vers la branche d'origine.
  4. Associer la _pull request_ à l'_issue_ (interface de l'issue).
  5. Lier chaque _commit_ à l'_issue_ créée à l'aide de son numéro. (Astuce: écrire **#23** dans le commentaire du _commit_ pour qu'il apparaisse dans l'_issue_ numéro 23 après le _push_).
  6. Une fois l'objectif atteint, fermer l'_issue_ et effectuer le _merge_ de la branche.
- Nous encourageons l'utilisation des _milestones_ (groupe d'issues à réaliser avant une date donnée) afin de mieux plannifier le travail.
- Pour toute utilisation d'identifiants de connexion ou autres données sensibles dans le code, merci d'avoir recours aux **secrets github**.
- En cas de détection d'un **bug**, merci de créer une _issue_ décrivant très précisement le cas d'erreur afin qu'un autre développeur puisse le corriger.

Merci

### Quelques commandes git utiles
```
git add .
git commit -m "commentaire détaillant les modifications apportées"
git pull
git push
git checkout nom_branche
git branch nom_nouvelle_branche
```

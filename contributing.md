# Comment contribuer à ft_irc

## Sommaire
1. [Branches](#branches)
2. [Pull requests](#pull-requests)
3. [Norme des commit](#commits)

## Branches

Pour creer une branche, il faut utiliser la commande suivante:
```
git checkout -b dev-{nom}
```

- `master` : branche principale, contient le code de "production"

- `dev-{nom}` : une branche de développement, contient le code en cours de développement à fusionner dans `master` avec une [pull request](#pull-requests)

## Pull requests

Pull requests (PR) servent à fusionner une branche de développement dans `master`. Pour créer une PR, il faut :
- Créer une branche de développement à partir de `master`
- Faire des commits sur cette branche
- Pousser la branche sur le dépôt distant
- Créer une PR sur [GitHub](https://github.com/harmonie-durrant/ft_irc/pulls)
- si il y a des conflits de fusion, les résoudre, si tu ne sais pas comment, demande de l'aide de quelqu'un d'autre.
- Fusionner la PR si tout les tests passent (le boutton est vert)

## Commits

Dans les messages de commit, habituellement les équipes utilisent une convention pour faciliter la lecture et la compréhension des changements.

Par exemple, pour ajouter une fonctionnalité de login, le message de commit pourrait être :
```
Add: login feature
```

Pour corriger un bug, le message pourrait être :
```
Fix: login bug
```

Pour ajouter une documentation, le message pourrait être :
```
Doc: add login documentation
```

Pour ajouter une fonctionnalité, corriger un bug et ajouter de la documentation, le message pourrait être :
```
Edit: login
- Add: login feature
- Fix: login bug
- Doc: add login documentation
```


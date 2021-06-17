# Programme de coordination

Le programme de coordination communique avec le gestionnaire de ressources 
ainsi que l'automate pour coordonner deux trains.

Pour le lancer, il faut fournir deux fichiers de commande de train.

# Utiliser le programme :

Pour compiler le programme, il faut executer le commade suivante :

```
make
```

Puis, vous pouvez lancer le programme avec la commande suivante :

```
./coordination.exe FICHIER_CONFIG_TRAIN_A FICHIER_CONFIG_TRAIN_B XWAY_A XWAY_B ADDR_IP_GR
```

# Le fichier de configuration

## Exemple

Le fichier de configuration a la forme suivante :

```
TRAIN 1

DR 1
AIGUILLAGE 17
INVERSION 12
TRONCON 12
RR 1
```

## Les commandes possibles

Voici les commandes possibles pour le fichier de configuration.

### Le numéro du train

Il s'agit de la seule ligne obligatoire du fichier. Voici sa forme générale : 
```
TRAIN $NUMERO_TRAIN
```

Voici un exemple :

```
TRAIN 1
```

### La commande d'un troncon

Voici sa forme générale :
```
TRONCON $NUMERO_TRONCON
```

Voici un exemple :

```
TRONCON 17
```

### La commande d'un aiguillage

Voici sa forme générale :
```
AIGUILLAGE $NUMERO_AIGUILLAGE
```

Note : `$NUMERO_AIGUILLAGE` doit être remplacé par le numéro défini dans l'automate (celui qui va permettre à 
l'automate de savoir quel aiguillage piloter).

Voici un exemple :

```
AIGUILLAGE 82
```

### La commande d'une inversion

Voici sa forme générale :
```
INVERSION $NUMERO_TRONCON
```

Voici un exemple :

```
INVERSION 1
```

### La commande d'une prise de ressource(s)

Voici sa forme générale :
```
DR $NUM_RESSOURCE_1 $NUM_RESSOURCE_2 ... $NUM_RESSOURCE_N
```

Note : `DR` signifie Demande Ressource.

Voici un exemple :

```
DR 2 4 5
```

Ici, on demande de prendre les ressources 2, 4 et 5.

### La commande pour rendre des ressources

Voici sa forme générale :
```
RR $NUM_RESSOURCE_1 $NUM_RESSOURCE_2 ... $NUM_RESSOURCE_N
```

Note : `RR` signifie Rendre Ressource.

Voici un exemple :

```
RR 2
```

Ici, on demande de rendre la ressource 2.

## Les commentaires

Vous pouvez également ajouter des commentaires dans votre fichier de configuration.
Voici la forme qu'ils doivent prendre :
```
AIGUILLAGE 17 #CECI EST UN COMMENTAIRE
AIGUILLAGE 17 # CECI EST UN COMMENTAIRE
AIGUILLAGE 31 # A1b
```

# Configurer le programme

Dans le fichier `utils.h`, vous trouverez toutes les variables de 
paramètres qui permettent de configurer le programme.

# Tester le programme

Pour tester le programme, vous pouvez commenter les deux lignes suivantes dans `utils.h` :

```
// #define COMMUNICATE_FOR_REAL_GR
// #define COMMUNICATE_FOR_REAL_AUTOMATE
```

Puis exécuter les commandes suivantes :
```
make
./coordination.exe train1.txt train2.txt 27 28 192.168.1.4
```

La dernière commande va simplement simuler des échanges avec l'automate pour le pilotage du train1. L'adresse IP à la 
fin de la commande n'est pas importante sachant que la communication avec le gestionnaire de ressources n'est pas 
activée. Vous pouvez la laisser telle quelle.

# Les fichiers

## coordination.c

Ce fichier contient le programme principal. 

Il se connecte à l'automate par le biai d'une socket et il initialise les variables globales qui seront utiles pour 
le bon fonctionnement du programme. Il lit également les variables passées en paramètre du programe.

Puis il créé 3 threads différents :
- 1 thread pour la gestion d'un premier train
- 1 thread pour la gestion d'un second train
- 1 thread pour la lecture de la socket avec le train. Il écoute tous les messages reçus et les dispatch entre les 
différents threads.

## code.h

Ce fichier définit les codes qui me serviront à identifier le type de trame envoyé par l'automate.

## utils.h

Ce fichier contient les constantes utiles au bon fonctionnement du programme. Il contient également la définition des
types de variables crée pour le programme. 
C'est dans ce programe que l'on paramétrer certaines choses telles que l'affichage des trames envoyé et reçu par 
exemple.

## commande_train.h/.c

Ce fichier contient toutes les fonctions haut niveau utiles pour la communication avec l'automate. On a par exemple 
les fonctions `envoieMonNombreDeTours` ou `commandeTroncon`.

## gestionRessources.h/.c

Ce fichier contient toutes les fonctions haut niveau utiles pour la communication avec le gestionnaire de ressources. 
On a par exemple les fonctions `demandeRessource` ou `rendRessource`.

## console.h/.c

Ce fichier contient des fonctions d'affichage utile pour l'application.
On a par exemple les fonctions `afficheTrameEnvoyeeAutomate` ou `afficheTrameRecuGR`.

## file_parser.h/.c

Ce fichier contient des fonctions utiles pour l'extraction des commandes présentes dans les fichiers de configuration 
des trains.

## pilotageTrain.h/.c

Ce fichier contient la fonction `pilotageTrain` qui permet de gérer le pilotage d'un train. Cette fonction crée une 
socket de connection pour communiquer avec le gestionnaire de ressources.

Il gére ensuite la communication avec le GR et l'automate pour piloter un train.

## test_troncon.h/.c

Ce programme est un programme de test qui permet de tester rapidement une fonction souhaitée.

## trame.h/.c

Ce programme contient les fonctions utiles à la création d'une trame.
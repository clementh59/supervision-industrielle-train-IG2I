# Programme de coordination

Le programme de coordination communique avec le gestionnaire de ressources 
ainsi que l'automate pour coordonner un train.

Pour le lancer, il faut fournir un fichier de commande de train.

# Utiliser le programme :

Pour compiler le programme, il faut executer le commade suivante :

```
make
```

Puis, vous pouvez lancer le programme avec la commande suivante :

```
./coordination MON_FICHIER_DE_CONFIG
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
./coordination.exe train1.txt train2.txt 27 28 192.168.1.1
```

La dernière commande va simplement simuler des échanges avec l'automate pour le pilotage du train1.
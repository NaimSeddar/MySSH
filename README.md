# Projet MySSH
Projet de *Réseau* et *Système* pour le semestre 1 du Master Informatique.

## Utilisation  
### Obtenir tout les exécutables
    make all  
    
### Remarque  
Lors de la compilation, le droit de root sera demandé afin de pouvoir appliquer les droits nécessaires à myssh-server.  
Ce projet à été réalisé et testé sous le sous système Ubuntu sur Windows 10.  

### La partie réseau  
Afin de pouvoir utiliser myssh, il faut que mysshd soit en cours :
  ./mysshd &
Ensuite :
  ./myssh [Hostname]
myssh-server sera automatiquement exécuté via mysshd.  

### La partie système
Chaque exécutable de cette partie peut être utilisé de manière conventionnelle.

## 1. Part de travail en %
    100% SEDDAR NaÏm

## 2. Executables
## *mysh*  
### Fonctionnalités :
    [x] Petit prompt sympa
    [x] Changer de répertoire (cd)  
    [x] Les signaux (CTRL+C et CTRL+Z)  
    [x] Enchainement de commande via les ";"  
    [x] Application des && et ||  
    [x] Wildcards  
    [x] Pipeliner bien comme il faut  
    [x] Redirection vers ou depuis un fichier (>, >>, 2>, 2>>, >&, >>&, <)  
    [x] Prendre en charge des variables   
    [x] Utliser myls  
    [x] Utliser myps  
    [x] Commande "status" 
    [x] Lancer un prog en background  
    [x] Commande interne myjobs  
    [x] Commande mybg (fg/stopped ==> bg)  
    [x] Commande myfg (bg         ==> fg)  


## *myls (ls -l)*
### Fonctionnalités :
    [x] Type du fichier  
    [x] Permissions  
    [x] Option -a  
    [x] Option -R  
    [x] Dates  
    [x] Propriétaire (user) du fichier  
    [x] Propriétaire (group) du fichier   
    [x] Couleurs pour chaque type de fichier  

## *myps (ps aux)*
### Fonctionnalités :
    [x] User  
    [x] PID  
    [x] %CPU  
    [x] %MEM  
    [x] VSZ  
    [x] RRS  
    [x] TTY  
    [x] État processus  
    [x] Start  
    [x] Time  
    [x] Command  

## *Partie réseau*
### Fonctionnalités :
    [x] Connexion multi-clients  
    [x] Connexion via le daemon  
    [x] Authentification (shadow password)  
    [x] Protocole d'authentification  
    [x] Execution de commande sur le serveur
    [x] Redirection du stdout sur la socket (mysh)  
    [x] Protocole execution de commande  
    [x] One shot command (-c [command])  
    [x] Fichier config dans le dossier $HOME/.myssh
    [ ] Envp en mémoire partagé
    [ ] Signaux

Par manque de temps, je n'ai pas pu implémenter le envp en mémoire partagée et le transfert de signaux du client vers le serveur.

## 3. Bugs
Lors de mes tests sur la partie réseau, j'ai eu, de manière aléatoire, des problèmes de synchronisation entre le client et serveur. Ce faisant, le serveur attendait une commande du client pendant que le client attendait la réponse du serveur.  
Rarement, le resultat de la commande ne s'affiche pas. 
L'ajout d'ACK et de do-while pour recevoir m'ont aidé à diminuer le nombre de problèmes aléatoires.

L'affichage du myps via myssh bug, je n'ai pas su trouver la raison de ce bug.  
Toutefois, je n'ai detecté aucun bug sur myps avec son exécutable ou via mysh.  

Il y a aussi des bugs d'affichage dans mysh lorsqu'une comme ls est lancé en background.  

## 4. La petite blague
Alors... J'ai pas les meilleurs blagues, mais j'ai de bons memes.  
https://twitter.com/c0dehard/status/1327718161848872960?s=20  
Ça me fais plaisir d'enfin pouvoir le montrer à quelqu'un qui puisse le comprendre.

Bonne année !  

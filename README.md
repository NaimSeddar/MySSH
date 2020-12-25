# Projet MySSH
Projet de *Réseau* et *Système* pour le semestre 1 du Master Informatique.

## Executables
## *mysh*  
### Fonctionnalités :
    [x] Petit prompt sympa
    [x] Changer de répertoire (cd)  
    [x] Les signaux  
    [x] Enchainement de commande via les ";"  
    [x] Application des && et ||  
    [x] Wildcards  
    [x] Pipeliner bien comme il faut  
    [x] Redirection vers ou depuis un fichier (>, >>, 2>, 2>>, >&, >>&, <)  
    [x] Prendre en charge des variables   
    [x] Utliser myls  
    [x] Utliser myps  
    [ ] Commande "status" 
    [ ] Lancer un prog en background  
    [ ] Commande interne myjobs  
    [ ] Passer une commande de foreground à background (et inversement)  


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
    [ ] Envp en mémoire partagé
    [ ] Signaux
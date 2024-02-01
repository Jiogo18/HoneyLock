# HoneyLock

Verrouiller son ordinateur en faisant croire qu'il ne l'est pas.

L'objectif est d'avoir un écran de veille pouvant être déveouillé par le propriétaire uniquement,
sans passer par l'écran de connexion utilisateur.
Même si l'application empêche l'utilisation de l'ordinateur,
l'utilisateur n'est pas déconnecté, ce qui peut présenter un risque de sécurité.

Attention, une fois ouverte, cette application ne peut pas être fermée de manière classique.

La fenêtre principale est un cadre transparent qui prend l'entièreté de l'écran et qui ne peut pas être réduit.

## Les actions suivantes sont contrées

- accès à la barre des tâches => fenêtre en plein écran avec showFullScreen
- fermeture avec alt+f4 ou autre méthode "douce" => closeEvent refusé
- changement de bureau virtuel => Qt::SplashScreen
- ouverture du menu démarrer => échappe
- ouverture du sélecteur d'application alt+tab ou win+tab => clic gauche régulier
- prise de focus par une autre application (gestionnaire des tâches, y compris via ctrl+alt+sup)
						=> reprise du focus et verrouillage avec l'api Windows
- utilisation de la souris => curseur masqué et placé à droite de l'écran en (inf;500)
							(ces 2 sécurités ne sont pas vraiment importantes)

## Quitter l'application

- Réaliser une combinaison de toûches/boutons de souris spécifique.
- Fermer la session utilisateur

Par défaut il existe deux combinaisons :
- Un clic droit
- Insert ; O

## Configuration

Le fichier de configuration `config.ini` permet de définir les touches pour quitter l'application.
Format : `<clé inutilisée> = "<suite ; de ; touches>"` dans la séction `[Combinaisons]`
Exemple de fichier de configuration :

```ini
# Combinaisons de touches pour déverrouiller honeylock

[Combinaisons]
1 = "Alt ; P ; _"
2 = "RightClick ; O"
# ...
```

Une combinaison est réalisée en appuyant sur toutes les touches l'une après l'autre (seule l'appuie des touches est compté).

### Touches spéciales

- Clics de souris communs : LeftClick, RightClick, MiddleClick
- Clics de souris spéciaux : `Mouse_[code dans Qt::MouseButton]`
- Touches de clavier modifieurs : Ctrl, Alt, Shift
- Touches de clavier communes : utilise le format de QKeySequence, une touche à la fois
- Touches de clavier spéciales : `Key_[code dans Qt::Key]`

Si une touche n'est pas reconnue, l'application ne démarre pas.

Si le fichier n'existe pas ou est vide, les raccourcis par défaut sont utilisés.

Les lignes commençant par `#` sont ignorées.

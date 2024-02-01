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

- Faire une combinaison de toûches/boutons de souris spécifique (par défaut un clic droit)
- Fermer la session utilisateur

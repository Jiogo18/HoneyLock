#ifndef FENETRE_H
#define FENETRE_H

#include <QWidget>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QCloseEvent>
#include <QTimer>
#include <QTime>

/**
 * Fenêtre transparente, mais empêchant toute utilisation de l'ordinateur.
 * Les actions suivantes sont contrées :
 *   - accès à la barre des tâches => fenêtre en plein écran avec showFullScreen
 *   - fermeture avec alt+f4 ou autre méthode "douce" => closeEvent refusé
 *   - changement de bureau virtuel => Qt::SplashScreen
 *   - ouverture du menu démarrer => échappe
 *   - ouverture du sélecteur d'application alt+tab ou win+tab => clic gauche régulier
 *   - prise de focus par une autre application (gestionnaire des tâches, y compris via ctrl+alt+sup)
 *                              => reprise du focus et verrouillage avec l'api Windows
 *   - utilisation de la souris => curseur masqué et placé à droite de l'écran en (inf;500)
 *                                  (ces 2 sécurités ne sont pas vraiment importantes)
 *
 * L'unique solution pour arrêter l'application et de faire un clic droit.
 * Ou de redémarrer la session.
 *
 * Il n'est pas recommandé d'utiliser le debugage pour des raisons évidentes.
 */

class TransparentFrame : public QWidget
{
public:
    TransparentFrame();

    bool isActiveWindow() const;

protected:
    void closeEvent(QCloseEvent *event);

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private slots:
    /**
     * @brief Forcer la fenêtre au premier plan (principalement lorsqu'une nouvelle application a le focus)
     */
    void raise();
    /**
     * @brief Envoyer une action clavier via l'api Windows
     * @param key Numéro virtuel de la clé (VK_)
     */
    void sendClavier(int key);
    /**
     * @brief Vérifie que l'application a le focus
     */
    void focusTimeout();

private:
    bool allowCloseEvent = false;
    HWND hWnd;
    QTimer *timerFocus;
};

#endif // FENETRE_H

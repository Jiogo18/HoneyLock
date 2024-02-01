#include "fenetre.h"
#include <windows.h>

TransparentFrame::TransparentFrame() : QWidget()
{
    setCursor(Qt::BlankCursor);
    setMouseTracking(true);

    // SplashScreen : fenêtre présente sur tous les bureaux virtuels
    setWindowFlags(Qt::SplashScreen | Qt::WindowStaysOnTopHint);

    setWindowOpacity(0.01);

    // Permet de contourner Alt+Tab et Win+Tab en redonnant le focus à l'application
    timerFocus = new QTimer(this);
    timerFocus->setInterval(100);
    connect(timerFocus, &QTimer::timeout, this, &TransparentFrame::focusTimeout);
    timerFocus->start();

    hWnd = (HWND)QWidget::winId();
    raise(); // lock foreground
}

bool TransparentFrame::isActiveWindow() const
{
    return GetForegroundWindow() == hWnd;
}

void TransparentFrame::closeEvent(QCloseEvent *event)
{
    if(allowCloseEvent) {
        event->accept();
        timerFocus->stop();
    } else {
        event->ignore();
    }
}

void TransparentFrame::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "key press event" << event;
    // Permet d'effectuer des actions pour reprendre le focus
    switch(event->key()) {
    case Qt::Key::Key_Meta:
        sendClavier(VK_ESCAPE); // Annule l'ouverture du menu démarrer
        break;
    default:
        break;
    }
}

void TransparentFrame::keyReleaseEvent(QKeyEvent *event)
{
    Q_UNUSED(event)
}

void TransparentFrame::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "mouse press event" << event->button();
    switch(event->button()) {
    case Qt::RightButton:
        allowCloseEvent = true;
        exit(0);
        break;
    default:
        break;
    }
}

void TransparentFrame::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
}

void TransparentFrame::mouseMoveEvent(QMouseEvent *event)
{
    // Fixe la souris à droite de l'écran pour ne pas faire de dégâts
    Q_UNUSED(event)
    SetCursorPos(100000, 500);
}

void TransparentFrame::raise()
{
    // Permet de récupérer le focus dans le cas d'une nouvelle app ouverte
    LockSetForegroundWindow(LSFW_UNLOCK);
    SetForegroundWindow(hWnd);
    LockSetForegroundWindow(LSFW_LOCK);
}

void TransparentFrame::sendClavier(int key)
{
    INPUT ip;
    ip.type = INPUT_KEYBOARD;
    ip.ki.wVk = key;
    ip.ki.wScan = MapVirtualKeyEx(key, MAPVK_VK_TO_VSC, (HKL)0xf0010413);

    ip.ki.dwFlags = 0; //key press
    SendInput(1, &ip, sizeof(INPUT));
    ip.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &ip, sizeof(INPUT));
}

void TransparentFrame::focusTimeout()
{
    if(!isActiveWindow()) {
        // Envoyer un clic gauche (sur l'application)
        mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
        mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
        // Fenêtres spéciales (gestionnaire des tâches)
        raise();
    }
}

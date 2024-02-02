#include "fenetre.h"
#include "combinaisonParseur.h"
#include <windows.h>
#include <QProcess>

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

    // Chargement des combinaisons de déverrouillage
    QString fichier = "honeylock.ini";
    try {
        QSettings settings(fichier, QSettings::IniFormat);
        
        CombinaisonParseur parseur(settings);
        combinaisonsDeverrouillage = parseur.getCombinaisons();
        qDebug() << "Combinaisons : " << combinaisonsDeverrouillage;

        settings.beginGroup("Contre-mesures");
        paramDelaiArretSuspicion = settings.value("delaiArretSuspicion", 10000).toInt();
        paramNombreSuspicionsMax = settings.value("nombreSuspicions", 100).toInt();
        paramSingleContreMesure = settings.value("activateOnlyOneTime", true).toBool();
        commandeContreMesure = settings.value("commandeContreMesure", "").toString();
        settings.endGroup();

    } catch(QString e) {
        qDebug() << "Impossible de lire le fichier de configuration " << fichier << " : " << e;
        exit(1);
    }
    touchesAppuyees = QList<int>();

    // Combinaisons par défaut
    if(combinaisonsDeverrouillage.isEmpty()) {
        Combinaison combinaisonSecurite({Qt::MouseButton::RightButton});
        combinaisonsDeverrouillage.append(combinaisonSecurite);
        Combinaison combinaisonSecurite2({Qt::Key::Key_Insert, Qt::Key::Key_O});
        combinaisonsDeverrouillage.append(combinaisonSecurite2);
    }

    // Permet de déterminer le nombre de touches maximum à garder en mémoire
    for(const Combinaison &combinaison : combinaisonsDeverrouillage) {
        if(combinaison.size() > longueurCombinaisonMax) {
            longueurCombinaisonMax = combinaison.size();
        }
    }
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
        actionSuspecte(10);
    }
}

void TransparentFrame::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "key press event" << event;
    // Permet d'effectuer des actions pour reprendre le focus
    switch(event->key()) {
    case Qt::Key::Key_Meta:
        sendClavier(VK_ESCAPE); // Annule l'ouverture du menu démarrer
        actionSuspecte(10);
        break;
    default:
        break;
    }
    toucheAppuyee(event->key());
}

void TransparentFrame::keyReleaseEvent(QKeyEvent *event)
{
    Q_UNUSED(event)
}

void TransparentFrame::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "mouse press event" << event->button();
    toucheAppuyee(event->button());
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
        actionSuspecte(10);
    }
}

void TransparentFrame::toucheAppuyee(int touche)
{
    touchesAppuyees.push_back(touche);
    if(touchesAppuyees.size() > longueurCombinaisonMax) {
        touchesAppuyees.pop_front();
    }
    for(const Combinaison &combinaison : combinaisonsDeverrouillage) {
        if(combinaison.match(touchesAppuyees)) {
            allowCloseEvent = true;
            exit(0);
        }
    }
}

void TransparentFrame::actionSuspecte(int poids)
{
    qDebug() << "Action suspecte";
    QTime now = QTime::currentTime();
    if(tempsDerniereActionSuspecte.msecsTo(now) >= paramDelaiArretSuspicion) {
        nombreActionsSuspectes = 0;
        tempsPremiereActionSuspecte = now;
    }
    else {
        nombreActionsSuspectes += poids;
        if(nombreActionsSuspectes >= paramNombreSuspicionsMax) {
            qDebug() << "Lancement des contres mesures !";
            QProcess::startDetached("pwsh.exe", QStringList() << "-Command" << commandeContreMesure);
            nombreActionsSuspectes = 0;
            if(paramSingleContreMesure) {
                paramDelaiArretSuspicion = 0; // court-circuit pour ne pas relancer de commande
            }
        }
    }

    tempsDerniereActionSuspecte = now;
}

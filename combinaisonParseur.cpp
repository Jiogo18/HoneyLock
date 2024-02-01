#include "combinaisonParseur.h"
#include <QKeySequence>

CombinaisonParseur::CombinaisonParseur(QSettings &settings)
{
    this->combinaisons = QList<Combinaison>();

    // Format :
    // [Combinaisons]
    // <clé inutilisée> = "<suite ; de ; touches>"
    // ...
    settings.beginGroup("Combinaisons");
    QStringList combinaisonsStr = settings.childKeys();
    foreach(QString cle, combinaisonsStr) {
        QString ligne = settings.value(cle).toString();
        qDebug() << "combi" << cle << ligne;
        if(ligne.isEmpty()) continue;
        QStringList touches = ligne.split(";");
        QList<int> combinaisonTouches;
        for(QString touche : touches) {
            touche = touche.trimmed();
            int toucheInt = keyNameToByte(touche);
            combinaisonTouches.append(toucheInt);
        }
        Combinaison combinaison(combinaisonTouches);
        combinaisons.append(combinaison);
    }
    settings.endGroup();
}

QList<Combinaison> CombinaisonParseur::getCombinaisons()
{
    return combinaisons;
}

int CombinaisonParseur::keyNameToByte(QString key)
{
    if(key == "LeftClick") return Qt::MouseButton::LeftButton;
    else if(key == "RightClick") return Qt::MouseButton::RightButton;
    else if(key == "MiddleClick") return Qt::MouseButton::MiddleButton;
    else if(key.startsWith("Mouse_")) {
        // Traiter en tant qu'int
        key = key.mid(6);
        int keyInt = key.toInt();
        qDebug() << "Mouse_" + key + " => " << keyInt;
    }

    if (0 == key.compare("Alt", Qt::CaseInsensitive)) {
        return Qt::Key_Alt;
    } else if (0 == key.compare("Ctrl", Qt::CaseInsensitive)) {
        return Qt::Key_Control;
    } else if (0 == key.compare("Shift", Qt::CaseInsensitive)) {
        return Qt::Key_Shift;
    } else if (0 == key.compare("Meta", Qt::CaseInsensitive)) {
        return Qt::Key_Meta;
    } else {
        const QKeySequence keySeq(key);
        if (1 == keySeq.count()) {
            return keySeq[0].key();
        }
    }

    throw "Impossible de convertir la touche '" + key + "' en byte de touche souris ou clavier";
}

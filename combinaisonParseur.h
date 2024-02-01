#ifndef COMBINAISONPARSEUR_H
#define COMBINAISONPARSEUR_H

#include <QSettings>
#include "combinaison.h"

class CombinaisonParseur
{
public:
    CombinaisonParseur(QSettings &settings);

    QList<Combinaison> getCombinaisons();

private:
    static int keyNameToByte(QString key);
    QList<Combinaison> combinaisons;
};

#endif // COMBINAISONPARSEUR_H

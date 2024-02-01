#ifndef COMBINAISON_H
#define COMBINAISON_H

#include "qdebug.h"
#include <QObject>

class Combinaison : public QObject
{
public:
    explicit Combinaison(QList<int> combinaison, QObject *parent = nullptr);
    Combinaison(const Combinaison &c);
    bool match(QList<int> touches) const;
    int size() const { return combinaisonTouches.size(); }

    Combinaison &operator=(const Combinaison &c);
    friend QDebug operator<<(QDebug debug, const Combinaison &c);

private:
    QList<int> combinaisonTouches;

};

#endif // COMBINAISON_H

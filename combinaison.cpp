#include "combinaison.h"

Combinaison::Combinaison(QList<int> combinaison, QObject *parent)
    : QObject{parent}, combinaisonTouches{combinaison}
{

}

Combinaison::Combinaison(const Combinaison &c)
    : QObject{c.parent()}, combinaisonTouches{c.combinaisonTouches}
{

}

bool Combinaison::match(QList<int> touches) const
{
    if(touches.size() < combinaisonTouches.size()) {
        return false;
    }
    for(int i = 0; i < combinaisonTouches.size(); i++) {
        if(touches[i] != combinaisonTouches[i]) {
            return false;
        }
    }
    return true;
}

Combinaison &Combinaison::operator=(const Combinaison &c)
{
    setParent(c.parent());
    combinaisonTouches = c.combinaisonTouches;
    return *this;
}

QDebug operator<<(QDebug debug, const Combinaison &c)
{
    debug.nospace() << c.combinaisonTouches;

    return debug;
}

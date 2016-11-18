#ifndef NOTIFIERMANAGER_H_INCLUDED
#define NOTIFIERMANAGER_H_INCLUDED


#include "Notifier.h"
#include <map>


namespace PirateSimulator
{
    class NotifierManager
    {
    public:
        static NotifierManager singleton;


    private:
        std::map<NotifiedElement::Name, NotifiedRef> m_notifiers;


    public:
        template<class NotificationType>
        NotifiedRef addANotifier(NotifiedElement::const_NameReference name)
        {
            NotifiedRef notifier = std::make_shared<Notifier<NotificationType>>(Notifier<NotificationType>(name));
            m_notifiers.emplace(std::pair<NotifiedElement::Name, NotifiedRef>(name, notifier));
            return notifier;
        }

        void removeANotifier(NotifiedElement::const_NameReference name)
        {
            m_notifiers.erase(name);
        }

        NotifiedRef getNotifier(NotifiedElement::const_NameReference name)
        {
            return m_notifiers.at(name);
        }
    };
}


#endif //!NOTIFIERMANAGER_H_INCLUDED
#ifndef NOTIFIER_H_INCLUDED
#define NOTIFIER_H_INCLUDED


#include <string>
#include <memory>
#include <map>


namespace PirateSimulator
{
    class NotifiedElement
    {
    public:
        using const_NameReference = const std::string&;
        using Name = std::string;

    public:
        Name m_name;

    public:
        NotifiedElement(const_NameReference name) :
            m_name{ name }
        {}


    public:
        template<class NotifierType>
        std::shared_ptr<NotifierType> as()
        {
            static_assert(std::is_base_of<NotifiedElement, NotifierType>::value, "Notifier as casting invalid -> Cannot be a Notified Element");

            return std::make_shared<NotifierType>(*static_cast<NotifierType*>(this));
        }
    };
    
    
    using NotifiedRef = std::shared_ptr<NotifiedElement>;


    template<class Notification>
    class INotified : public NotifiedElement
    {
    public:
        INotified(const_NameReference name) :
            NotifiedElement(name)
        {}
        virtual ~INotified() = default;

        virtual void onNotify(const Notification& notifiedElement, NotifiedRef notifier) = 0;
    };


    template<class Notification>
    class Notifier : public INotified<Notification>
    {
    protected:
        std::map<Name, std::shared_ptr<INotified<Notification>>> m_registeredNotice;


    public:
        Notifier(const_NameReference notifierName) :
            INotified<Notification>(notifierName)
        {}


    public:
        void registerNotified(std::shared_ptr<INotified<Notification>> notified)
        {
            m_registeredNotice.emplace(std::pair<Name, std::shared_ptr<INotified<Notification>>>(notified->m_name, notified));
        }

        void unregisterNotified(const_NameReference name)
        {
            m_registeredNotice.erase(name);
        }

        void sendNotification(const Notification& notifiedElement) noexcept
        {
            this->onNotify(notifiedElement, std::make_shared<Notifier<Notification>>(*this));
        }

        template<class NotifierApprouved>
        void sendNotification(const Notification& notifiedElement, const NotifierApprouved& notifier) noexcept
        {
            static_assert(std::is_base_of<NotifiedElement, NotifierApprouved>::value, "Not a derived of a NotifiedElement -> It cannot Notify");
            this->onNotify(notifiedElement, notifier);
        }

        NotifiedRef getNotified(const_NameReference name) const
        {
            return m_registeredNotice.at(name);
        }


    public:
        virtual void onNotify(const Notification& notifiedElement, NotifiedRef notifier) noexcept override
        {
            for (auto iter = m_registeredNotice.begin(); iter != m_registeredNotice.end(); ++iter)
            {
                iter->second->onNotify(notifiedElement, notifier);
            }
        }
    };
}


#endif //!NOTIFIER_H_INCLUDED
//
// Created by duncan on 3/1/16.
//

#ifndef HELLOCLION_SUBJECT_HPP
#define HELLOCLION_SUBJECT_HPP

#include <functional>  // for std::function
#include <utility>     // for std::forward and std::move
#include <map>
#include <vector>

namespace Camax
{
    template <typename Event>
    struct ObserverHandle
    {
        Event event;
        unsigned int vectorIndex;
    };

    template<typename Event>
    class Subject {
    public:

        typedef std::function<void(Event)> EventHandler;

        template<typename Observer>
        const ObserverHandle<Event> RegisterObserver(const Event &event, Observer &&observer)
        {
            observers_[event].push_back(std::forward<Observer>(observer));

            ObserverHandle<Event> handle{event, (unsigned int) (observers_[event].size() - 1)};
            return handle;
        }

        template<typename Observer>
        const ObserverHandle <Event> RegisterObserver(Event &&event, Observer &&observer)
        {
            observers_[std::move(event)].push_back(std::forward<Observer>(observer));

            ObserverHandle<Event> handle{event, (unsigned int) (observers_[event].size() - 1)};
            return handle;
        }

        void Notify(const Event &event) const
        {
            //TODO: Who should throw/rethrow or handle the exceptions? Originator or caller?
            try
            {
                for (const auto &obs : observers_.at(event)) obs(event);
            }
            catch (const std::out_of_range &ex)
            {
#ifdef DEBUG
                std::cout << "\n\nNo observer is registered for this event\n";
                std::cout << "Exception message: " << ex.what() << "\n\n";
#endif // DEBUG
                throw ex;
            }
        }

        bool UnregisterObserver(ObserverHandle<Event> &handle)
        {
            auto it = observers_.find(handle.event);

            if(it != observers_.end())
            {
                // Get the vector associated with the event
                auto eventVector = observers_[handle.event];
                // Get iterator to observer to remove
                auto observerToRemove = eventVector.begin() + handle.vectorIndex;
                // Remove the desired observer
                eventVector.erase(observerToRemove);
                // Re associated the modified event vector to the observers map
                observers_[handle.event] = eventVector;

                return true;
            }

            return false;
        }

//        template<typename Observer>
//        bool Find(Observer &&observer)
        bool FindObserver(ObserverHandle<Event> &handle)
        {
//            return (observers_.count(std::forward<Observer>(observer)) ? true : false);
//            return (observers_.count(event) ? true : false);
            auto it = observers_.find(handle.event);

            if(it != observers_.end())
            {
                // Get the vector associated with the event
                auto eventVector = observers_[handle.event];
                if(eventVector.size() > handle.vectorIndex)
                {
                    // Get iterator to observer to remove
                    auto observerToRemove = eventVector.begin() + handle.vectorIndex;
                    if(observerToRemove != eventVector.end())
                        return true;
                }
            }

            return false;
        }


    private:
        std::map<Event, std::vector<EventHandler>> observers_;
    };
}
#endif //HELLOCLION_SUBJECT_HPP

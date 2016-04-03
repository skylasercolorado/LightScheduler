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

    template<typename Event, typename... HandlerParams>
    class Subject {
    public:

        typedef std::function<void(HandlerParams...)> EventHandler;
        typedef std::map<Event, std::vector<EventHandler>> SubjectContainer;
        typedef std::vector<EventHandler> EventVector;

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

        // Will throw out_of_range exception if no observer is found for the given event. Caller must handle it.
        void Notify(const Event &event, HandlerParams... params) const
        {
            for (const auto &obs : observers_.at(event))
            {
                obs(params...);
            }
        }

        bool UnregisterObserver(ObserverHandle<Event> &handle)
        {
            if(FindObserver(handle))
            {
                eventVector_.erase(observerToRemove_);
                // Re-associate the modified event vector to the observers map
                observers_[handle.event] = eventVector_;

                return true;
            }

            return false;
        }

        bool FindObserver(ObserverHandle<Event> &handle)
        {
            auto it = observers_.find(handle.event);

            if(it != observers_.end())
            {
                // Get the vector associated with the event
                eventVector_ = observers_[handle.event];
                if(eventVector_.size() > handle.vectorIndex)
                {
                    // Get iterator to observer to remove
                    observerToRemove_ = eventVector_.begin() + handle.vectorIndex;
                    if(observerToRemove_ != eventVector_.end())
                        return true;
                }
            }

            return false;
        }

    private:
        SubjectContainer observers_;
        EventVector eventVector_;
        typename EventVector::iterator observerToRemove_;
    };
}
#endif //HELLOCLION_SUBJECT_HPP

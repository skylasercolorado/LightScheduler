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
    template<typename Event>
    class Subject {
    public:

        template<typename Observer>
        void RegisterObserver(const Event &event, Observer &&observer)
        {
            observers_[event].push_back(std::forward<Observer>(observer));
        }

        template<typename Observer>
        void RegisterObserver(Event &&event, Observer &&observer)
        {
            observers_[std::move(event)].push_back(std::forward<Observer>(observer));
        }

        void Notify(const Event &event) const
        {
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

        template<typename Observer>
        bool Find(const Observer &&observer)
        {
            return (observers_.count(std::forward<Observer>(observer)) ? true : false);
        }


    private:
        std::map<Event, std::vector<std::function<void(Event)>>> observers_;
    };
}
#endif //HELLOCLION_SUBJECT_HPP

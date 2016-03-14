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
        void registerObserver(const Event &event, Observer &&observer) {
            observers_[event].push_back(std::forward<Observer>(observer));
        }

        template<typename Observer>
        void registerObserver(Event &&event, Observer &&observer) {
            observers_[std::move(event)].push_back(std::forward<Observer>(observer));
        }

        void notify(const Event &event) const {
//            for (const auto &obs : observers_.at(event)) obs();
            for (const auto &obs : observers_.at(event)) obs(event);
        }

    private:
        //std::map<Event, std::vector<std::function<void()>>> observers_;
        std::map<Event, std::vector<std::function<void(Event)>>> observers_;
    };
}
#endif //HELLOCLION_SUBJECT_HPP

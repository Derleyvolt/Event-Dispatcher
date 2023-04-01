#include "eventDispatcher.h"

void Dispatcher::addListener(std::string eventType, Callback callback, int weigth = 1) {
    this->listeners.insert({eventType, std::make_pair(callback, weigth)});
}

// notifica/trigga todos os listeners de um evento específico
void Dispatcher::notify(Event* e) {
    auto queue = this->orderListeners(e->getType());

    while(!queue.empty()) {
        Callback func = queue.top();
        func(e);
        queue.pop();
    }
}

void Dispatcher::notify(std::string eventType) {
    auto queue = this->orderListeners(eventType);

    while(!queue.empty()) {
        Callback func = queue.top();
        func(nullptr);
        queue.pop();
    }
}

std::priority_queue<int, Callback> Dispatcher::orderListeners(std::string eventType) {
    std::priority_queue<int, Callback> queue;

    for(auto& element : this->listeners) {
        if(element.first == eventType) {
            queue.push(element.second);
        }
    }

    return queue;
}

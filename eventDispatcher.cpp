#include <iostream>
#include <vector>
#include <assert.h>
#include <string>
#include <unordered_map>
#include <functional>
#include <algorithm>

// Essa classe, na prática, é uma interface onde outros eventos implementam ela
// e adicionam novos membros, dependendo do tipo de evento..
class Event {
public:
    Event(std::string eventType) : eventType(eventType) {
    }

    std::string getType() const {
        return this->eventType;
    }

private:
    std::string eventType;
};

class Callback {
public:
    Callback(std::function<void(Event)> f) {
    }
    
    void operator()(Event e) {
        this->f(e);
    }

private:
    std::function<void(Event)> f;
};

class Dispatcher {
public:
    // O parâmetro weigth serve para dá prioridade aos eventos
    void addListener(std::string eventType, Callback callback, double weigth = 1) {
        this->listeners.insert({eventType, std::make_pair(callback, weigth)});
    }

    // notifica/trigga todos os listeners de um evento específico
    void notify(Event e) {
        auto listeners = this->orderListeners(e.getType());

        for(auto func : listeners) {
            func(e);
        }
    }

private:
    using listenerPair = std::pair<Callback, double>;

    std::unordered_map<std::string, listenerPair> listeners;

    // ordena os listeners por peso
    std::vector<Callback> orderListeners(std::string eventType) {
        std::vector<listenerPair> sortedArray;

        for(auto& element : this->listeners) {
            if(element.first == eventType) {
                sortedArray.push_back(element.second);
            }
        }

        sort(sortedArray.begin(), sortedArray.end(), [](listenerPair a, listenerPair b) {
            return a.second > b.second;
        });

        std::vector<Callback> res;

        std::for_each(sortedArray.begin(), sortedArray.end(), [&res](listenerPair element) {
            res.push_back(element.first);
        });

        return res;
    }
};

int main() {

}

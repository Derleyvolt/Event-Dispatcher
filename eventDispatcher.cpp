#include <iostream>
#include <vector>
#include <assert.h>
#include <string>
#include <unordered_map>
#include <functional>
#include <algorithm>

class Event {
public:
    Event() {

    }

    std::string getType() const {
        return this->eventType;
    }

protected:
    std::string eventType;
};

class ClickEvent : public Event {
public:
    ClickEvent(std::string eventType, int x, int y) : x(x), y(y) {
        this->eventType = eventType;
    }

    int getX() const {
        return x;
    }

    int getY() const {
        return y;
    }

    void showPoint() {
        std::cout << "x: " << x << "  y: " << y << std::endl;
    }

private:
    int x;
    int y;
};

class OnChangeEvent : public Event {
public:
    OnChangeEvent(std::string eventType, std::string currentText) : text(currentText) {
        this->eventType = eventType;
    }

    std::string getText() const {
        return this->text;
    }

private:
    std::string text;
};

class Dispatcher {
using Callback = std::function<void(Event*)>;

public:
    void addListener(std::string eventType, Callback callback, double weigth = 0.1) {
        this->listeners.insert({eventType, std::make_pair(callback, weigth)});
    }

    // notifica/trigga todos os listeners de um evento específico
    void notify(Event* e) {
        auto listeners = this->orderListeners(e->getType());

        for(auto func : listeners) {
            func(e);
        }
    }

private:
    using listenerPair = std::pair<Callback, double>;

    std::unordered_multimap<std::string, listenerPair> listeners;

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
    Event* click  = new ClickEvent("click", 1, 2);
    Event* change = new OnChangeEvent("onChange", "Eu juro solenemente nao fazer nada de bom!");
    Dispatcher emitter;

    emitter.addListener("click", [](Event* e) {
        ClickEvent* ce = (ClickEvent*)e;
        std::cout << "Evento 1" << std::endl;
        ce->showPoint();
    }, 5);

    emitter.addListener("click", [](Event* e) {
        ClickEvent* ce = (ClickEvent*)e;
        std::cout << "Evento 2" << std::endl;
        ce->showPoint();
    }, 10);

    emitter.addListener("click", [](Event* e) {
        ClickEvent* ce = (ClickEvent*)e;
        std::cout << "Evento 3" << std::endl;
        ce->showPoint();
    }, 15);

    emitter.addListener("onChange", [](Event* e) {
        OnChangeEvent* oce = (OnChangeEvent*)e;
        std::cout << "text: " << oce->getText() << std::endl;
    });

    // simulando o evento de click e onChange (referente a um button e um input, respectivamente, por exemplo)
    emitter.notify(click);
    emitter.notify(change);
    return 0;
}

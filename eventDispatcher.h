#include <queue>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>
#include <unordered_map>

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

using Callback      = std::function<void(Event*)>;
using listenerPair  = std::pair<Callback, int>;

class Dispatcher {
public:
    void addListener(std::string eventType, Callback callback, int weigth = 1);

    void notify(Event* e);
    void notify(std::string eventType);

private:
    std::unordered_multimap<std::string, listenerPair> listeners;

    // ordena os listeners por peso
    std::priority_queue<int, Callback> orderListeners(std::string eventType);
};

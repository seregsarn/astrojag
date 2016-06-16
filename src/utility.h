// general utility classes and functions.
//===================================================

//---------------------------------------------------
// bresenham's algorithm for line-drawing.
void Line(Point from, Point to, std::function<bool(Point&)> cb);
void Line(int x1, int y1, int x2, int y2, std::function<bool(Point&)> cb);

//---------------------------------------------------
// priority queue implementation, present for pathfinding et al.
// this implementation isn't super efficient, but for most of my pathfinding
// test cases it does fine.
template <typename T>
class PriorityQueue {
protected:
    struct Node {
        int priority;
        T value;
    };
    std::vector<Node> backingStore;
public:
    PriorityQueue();
    ~PriorityQueue();
    void clear();
    int size() { return backingStore.size(); }
    void insert(const int priority, const T value);
    void push(const int priority, const T value) { insert(priority, value); }
    PriorityQueue<T>::Node popNode();
    T pop();
};

template <typename T>
PriorityQueue<T>::PriorityQueue() {
}

template <typename T>
PriorityQueue<T>::~PriorityQueue() {
    backingStore.clear();
}

template <typename T>
void PriorityQueue<T>::clear() {
    backingStore.clear();
}

template <typename T>
void PriorityQueue<T>::insert(const int pri, const T val) {
    PriorityQueue<T>::Node n;
    n.priority = pri;
    n.value = val;
    backingStore.push_back(n);
}

template <typename T>
typename PriorityQueue<T>::Node PriorityQueue<T>::popNode() {
    int bestP = INT_MAX;
    int bestIdx = -1;
    int idx = 0;
    for (auto iter = backingStore.begin(); iter != backingStore.end(); iter++,idx++) {
        if ((*iter).priority < bestP) {
            bestP = (*iter).priority;
            bestIdx = idx;
        }
    }
    auto n = backingStore[bestIdx];
    backingStore.erase(backingStore.begin() + bestIdx);
    return n;
}

template <typename T>
T PriorityQueue<T>::pop() {
    auto n = popNode();
    return n.value;
}


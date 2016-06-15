// topology stuff for libastrojag
//===================================================

template<typename T>
class Grid {
protected:
    int w,h;
    T *data;
public:
    Grid(int w, int h);
    ~Grid();
    T& at(const int& x, const int& y);
    T& at(const Point& p);
    bool inBounds(const int& x, const int& y);
    bool inBounds(const Point& p);
    void clearTo(const T& value);
};

template<typename T>
Grid<T>::Grid(int _w, int _h) {
    w = _w; h = _h;
    data = (T*) malloc(sizeof(T) * w * h);
}

template<typename T>
Grid<T>::~Grid() {
    delete data;
}

template<typename T>
T& Grid<T>::at(const int& x, const int& y) {
    return (data[x + (y*w)]);
}

template<typename T>
T& Grid<T>::at(const Point& p) {
    return at(p.x,p.y);
}

template<typename T>
bool Grid<T>::inBounds(const int& x, const int& y) {
    if (x < 0 || y < 0 || x >= w || y >= h) return false;
    return true;
}

template<typename T>
bool Grid<T>::inBounds(const Point& p) {
    return inBounds(p.x,p.y);
}

template<typename T>
void Grid<T>::clearTo(const T& val) {
    int i;
    for (i = 0; i < (w*h); i++) {
        data[i] = val;
    }
}


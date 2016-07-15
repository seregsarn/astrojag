class TestingMap : public IFieldOfView, IPathable {
public:
    int w, h;
    Grid<int> tiles;
    // 0 = open, 1 = solid, 2 = solid but transparent, 3 = opaque but walkable
    TestingMap(int _w, int _h) : tiles(_w,_h) {
        w = _w; h = _h;
        tiles.clearTo(0);
    }
    virtual Point getDimensions() { return Point(w, h); }
    virtual bool inBounds(int x, int y) { return tiles.inBounds(x,y); }
    virtual bool opaque(FOV *, Point p) { return (tiles.at(p.x,p.y) == 1 || tiles.at(p.x,p.y) == 3); }
    virtual bool blocked(Point p, int) { return (tiles.at(p.x,p.y) == 1 || tiles.at(p.x,p.y) == 2); }
    virtual int heuristic(const Point& src, const Point& dest) { return src.distance(dest); }

    void print() {
        int i, j;
        printf("[");
        for (j = 0; j < h; j++) {
            for (i = 0; i < w; i++) {
                printf("%d%s", tiles.at(i,j), i == w-1 && j == h-1 ? "":"," );
            }
            printf("%s\n%s", j == h-1 ? "]":"", j == h-1 ? "":" ");
        }
    }
    static shared_ptr<TestingMap> makeTestMap() {
        auto m = make_shared<TestingMap>(10,10);
        m->tiles.clearTo(0);
        int i;
        for (i = 0; i < 10; i++) {
            m->tiles.at(i, 0) = 1;
            m->tiles.at(i, 9) = 1;
            m->tiles.at(0, i) = 1;
            m->tiles.at(9, i) = 1;
        }
        m->tiles.at(5,5) = 1;
        // solid
        m->tiles.at(5,2) = 1;
        m->tiles.at(5,3) = 1;
        m->tiles.at(5,4) = 1;
        // floor
        m->tiles.at(5,6) = 0;
        m->tiles.at(5,7) = 0;
        m->tiles.at(5,8) = 0;
        // window
        m->tiles.at(2,5) = 2;
        m->tiles.at(3,5) = 2;
        m->tiles.at(4,5) = 2;
        // fake
        m->tiles.at(6,5) = 3;
        m->tiles.at(7,5) = 3;
        m->tiles.at(8,5) = 3;
        return m;
    }
};

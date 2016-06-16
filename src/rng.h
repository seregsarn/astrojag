// seedable, freezable PRNG.
//===================================================
// this is wrapped up in an object for cleanliness and so that you can have
// multiple RNGs, say one for generating the overworld and another for
// generating dungeon levels, etc.

class RNG {
protected:
    public:
    uint64_t seeds[2];
public:
    // creation/seeding
    RNG();
    RNG(uint64_t s1, uint64_t s2);
    RNG(uint64_t *s);
    RNG& operator=(const RNG& other);
    void seed(uint64_t s1, uint64_t s2);
    void seed(uint64_t *sarray);
    // generating numbers
    uint64_t urand();
    int rand();
    int rand(int max);
    int range(int min, int max);
    float frand();

    // serialization stuff
    void freeze(FILE *fp);
    void thaw(FILE *fp);
    std::string serialize();
    static RNG deserialize(std::string json);
};


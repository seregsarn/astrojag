#include "astrojag_internal.h"

RNG::RNG() {
    seeds[0] = (time(0)<<32) | time(0);//0;
    seeds[1] = (time(0)<<32) | time(0);//0x5555555555555555;
}
RNG::RNG(uint64_t s1, uint64_t s2) {
    seeds[0] = s1;
    seeds[1] = s2;
}
RNG::RNG(uint64_t *s) {
    seeds[0] = s[0];
    seeds[1] = s[1];
}
RNG& RNG::operator=(const RNG& other) {
    seeds[0] = other.seeds[0];
    seeds[1] = other.seeds[1];
    return *this;
}
void RNG::seed(uint64_t s1, uint64_t s2) {
    seeds[0] = s1;
    seeds[1] = s2;
}
void RNG::seed(uint64_t *sarray) {
    seed(sarray[0], sarray[1]);
}
uint64_t RNG::urand() {
    uint64_t x, y;
    x = seeds[0];
    y = seeds[1];
    seeds[0] = y;
    x ^= x << 23;
    seeds[1] = x ^ y ^ (x >> 17) ^ (y >> 26);
    return seeds[1] + y;
}
int RNG::rand() {
    uint64_t x = urand();
    return abs((int)x);
}
int RNG::rand(int max) {
    return urand() % max;
}
/*
int RNG::rn(int sides) {
    return urand() % (sides) + 1;
}
int RNG::rn2(int sides) {
    return urand() % (sides);
}
*/
int RNG::range(int min, int max) {
    return min + rand() % (max - min + 1);
}
float RNG::frand() {
    return (float)(rand()) / INT_MAX;
}

// serialization code
void RNG::freeze(FILE *fp) {
    fwrite(seeds, sizeof(uint64_t), 2, fp);
}
void RNG::thaw(FILE *fp) {
    fread(seeds, sizeof(uint64_t), 2, fp);
}
std::string RNG::serialize() {
    json::Array a;
    a << (signed)((seeds[0] >> 48) & 0xFFFF);
    a << (signed)((seeds[0] >> 32) & 0xFFFF);
    a << (signed)((seeds[0] >> 16) & 0xFFFF);
    a << (signed)(seeds[0] & 0xFFFF);
    a << (signed)((seeds[1] >> 48) & 0xFFFF);
    a << (signed)((seeds[1] >> 32) & 0xFFFF);
    a << (signed)((seeds[1] >> 16) & 0xFFFF);
    a << (signed)(seeds[1] & 0xFFFF);
    return a.json();
}
RNG RNG::deserialize(std::string json_s) {
    json::Array a;
    json::Value v;
    uint64_t s1 = 0, s2 = 0;
    if (!v.parse(json_s)) {
        // throw an error?
        throw "oh noes";
        return RNG();
    }
    if (v.is<json::Array>()) {
        a = v.get<json::Array>();
        s1 =  (unsigned) a.get<json::Number>(0); s1 <<= 16;
        s1 |= (unsigned) a.get<json::Number>(1); s1 <<= 16;
        s1 |= (unsigned) a.get<json::Number>(2); s1 <<= 16;
        s1 |= (unsigned) a.get<json::Number>(3);
        s2 =  (unsigned) a.get<json::Number>(4); s2 <<= 16;
        s2 |= (unsigned) a.get<json::Number>(5); s2 <<= 16;
        s2 |= (unsigned) a.get<json::Number>(6); s2 <<= 16;
        s2 |= (unsigned) a.get<json::Number>(7);
        return RNG(s1, s2);
    }
    throw "oh noes 2";
}

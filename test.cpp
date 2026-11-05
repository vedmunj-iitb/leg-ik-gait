#include <cmath>
#include <cstdio>

#include "leg.hpp"

int failures = 0;
#define CHECK(cond)                                              \
    if (!(cond)) {                                               \
        std::printf("FAIL line %d: %s\n", __LINE__, #cond);      \
        failures++;                                              \
    }

int main() {
    Leg leg{55, 65};
    double hip, knee;

    // fk(ik(p)) must give back p for every reachable point
    for (double x = -60; x <= 60; x += 5) {
        for (double z = 20; z <= 120; z += 5) {
            if (!ik(leg, {x, z}, hip, knee)) continue;
            Point p = fk(leg, hip, knee);
            CHECK(std::fabs(p.x - x) < 1e-9 && std::fabs(p.z - z) < 1e-9);
        }
    }

    // straight leg pointing down
    CHECK(ik(leg, {0, 120}, hip, knee));
    CHECK(std::fabs(hip) < 1e-6 && std::fabs(knee) < 1e-6);

    // out of reach: too far, and closer than thigh - shin
    CHECK(!ik(leg, {0, 121}, hip, knee));
    CHECK(!ik(leg, {0, 5}, hip, knee));

    // stance: the foot stays on the ground
    Gait g;
    for (double p = 0; p < g.duty; p += 0.01) CHECK(foot_at(g, p).z == g.height);

    // no jump where stance turns into swing, or at the end of the cycle
    Point a = foot_at(g, g.duty - 1e-9), b = foot_at(g, g.duty);
    CHECK(std::fabs(a.x - b.x) < 1e-6 && std::fabs(a.z - b.z) < 1e-6);
    Point end = foot_at(g, 1.0 - 1e-9), start = foot_at(g, 0.0);
    CHECK(std::fabs(end.x - start.x) < 1e-6 && std::fabs(end.z - start.z) < 1e-6);

    if (failures > 0) {
        std::printf("%d test(s) failed\n", failures);
        return 1;
    }
    std::printf("all tests passed\n");
    return 0;
}

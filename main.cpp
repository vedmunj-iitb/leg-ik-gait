// Prints the hip and knee angles (degrees) of all four legs over one gait cycle, as CSV.
//   ./gait trot    diagonal pairs move together
//   ./gait walk    one leg at a time, three feet always on the ground
#include <array>
#include <cmath>
#include <cstdio>
#include <string>

#include "leg.hpp"

double deg(double rad) { return rad * 180.0 / PI; }

int main(int argc, char** argv) {
    std::string name = argc > 1 ? argv[1] : "trot";

    Leg leg{55, 65};  // example lengths in mm
    Gait gait;

    // phase offset of each leg, order FL, FR, RL, RR
    std::array<double, 4> offset = {0.0, 0.5, 0.5, 0.0};  // trot: FL+RR, then FR+RL
    if (name == "walk") {
        gait.duty = 0.75;
        // swing order: RL, FL, RR, FR (a leg swings when its phase passes 0.75)
        offset = {0.5, 0.0, 0.75, 0.25};
    } else if (name != "trot") {
        std::fprintf(stderr, "usage: gait [trot|walk]\n");
        return 1;
    }

    const int steps = 20;
    std::printf("t,FL_hip,FL_knee,FR_hip,FR_knee,RL_hip,RL_knee,RR_hip,RR_knee\n");
    for (int i = 0; i < steps; i++) {
        double t = double(i) / steps;
        std::printf("%.2f", t);
        for (int k = 0; k < 4; k++) {
            double p = std::fmod(t + offset[k], 1.0);
            double hip, knee;
            if (!ik(leg, foot_at(gait, p), hip, knee)) {
                std::fprintf(stderr, "foot out of reach at t=%.2f\n", t);
                return 1;
            }
            std::printf(",%.1f,%.1f", deg(hip), deg(knee));
        }
        std::printf("\n");
    }
}

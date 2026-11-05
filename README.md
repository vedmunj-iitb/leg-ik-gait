# leg-ik-gait

Inverse kinematics and a simple gait generator for a quadruped leg with two joints (hip and
knee), in C++17.

My quadruped course project used 8 MG90S servos and hand-tuned keyframes: I set the servo angles
for every pose by hand. This project works the other way round. It says where each foot should be
during a gait cycle and computes the hip and knee angles from that. Written in 2026, separate from
the robot's firmware.

## Inverse kinematics (`leg.hpp`)

Hip at the origin, x forward, z down. For a foot at (x, z), thigh length a and shin length b:

```
d^2 = x^2 + z^2
cos(knee) = (d^2 - a^2 - b^2) / (2ab)                     law of cosines
hip = atan2(x, z) - atan2(b sin(knee), a + b cos(knee))
```

If cos(knee) comes out above 1 or below -1 the foot is out of reach and `ik()` returns false.
`fk()` goes the other way, and the test checks that `fk(ik(p))` gives back `p` at every reachable
point of a 5 mm grid below the hip (x from -60 to 60 mm, z from 20 to 120 mm).

## Gait

Every foot follows the same loop. On the ground (stance) it moves back by one stride, which
pushes the body forward. Then it lifts on a half sine (swing) and moves forward to the start.
The four legs run this loop with different phase offsets:

- **trot**: diagonal pairs move together (front-left with rear-right, front-right with
  rear-left), and each foot is on the ground half the time
- **walk**: one leg swings at a time, in the order rear-left, front-left, rear-right, front-right,
  so three feet are always on the ground

## Build and run

```bash
cmake -B build
cmake --build build
./build/leg_test
./build/gait trot
./build/gait walk > walk.csv
```

```
t,FL_hip,FL_knee,FR_hip,FR_knee,RL_hip,RL_knee,RR_hip,RR_knee
0.00,-31.4,79.9,-56.5,79.9,-56.5,79.9,-31.4,79.9
0.05,-34.5,81.1,-60.4,89.7,-60.4,89.7,-34.5,81.1
...
```

Angles are in degrees. The leg lengths (55 and 65 mm), hip height (90 mm), stride (40 mm) and
lift (20 mm) are example values.

## On real servos

Hip angle 0 means the thigh points straight down and knee angle 0 means a straight leg. A servo
needs `90 + direction * (angle - centre) + trim`, where `centre` is the joint angle that should sit
in the middle of the servo's travel. In these gaits the hip stays between -64 and -31 degrees and
the knee between 80 and 109 degrees, so centres of about -45 and 95 degrees keep both well inside
a 0 to 180 degree servo. The direction (+1 or -1) and the trim are found for each servo on the
robot, because every servo is mounted a little differently.

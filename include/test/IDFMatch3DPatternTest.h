/*
 * Copyright 2016 Andrii Maksai
 */
#ifndef CODE_INCLUDE_TEST_IDFMATCH3DPATTERNTEST_H_
#define CODE_INCLUDE_TEST_IDFMATCH3DPATTERNTEST_H_
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <utility>
#include <vector>
#include "Detection.h"
#include "IDFMatch3DPattern.h"
class IDFMatch3DPatternTest {
 public:
  void testAll() {
    std::cerr << "Testing IDFMatch3DPattern...";
    testScoring();
    testScoringTrajectory();
    std::cerr << "OK\n";
  }
  void testScoring() {
    std::vector<Detection *> trajectory;
    Detection d1, d2, d3, dq;
    d1.x = 0;
    d1.y = 0;
    d1.frame_num = 0;
    d2.x = 1;
    d2.y = 0;
    d2.frame_num = 1;
    d3.x = 2;
    d3.y = 1;
    d3.frame_num = 2;
    trajectory.push_back(&d1);
    trajectory.push_back(&d2);
    trajectory.push_back(&d3);

    Pattern *pattern =
        new IDFMatch3DPattern(Pattern::NormalPattern, trajectory, 1.0);
    dq.x = 0;
    dq.y = 0.5;
    dq.frame_num = 0;
    assert(fabs(pattern->score(&Detection::Source, &dq) - 1.0) < 1e-6);
    dq.x = 0;
    dq.y = 1.5;
    dq.frame_num = 0;
    assert(fabs(pattern->score(&Detection::Source, &dq) - 0.0) < 1e-6);
    dq.x = 0;
    dq.y = 0.5;
    dq.frame_num = -1;
    assert(fabs(pattern->score(&Detection::Source, &dq) - 0.0) < 1e-6);
    dq.x = 0;
    dq.y = 0.5;
    dq.frame_num = 1;
    assert(fabs(pattern->score(&Detection::Source, &dq) - 0.0) < 1e-6);
    dq.x = 0.5;
    dq.y = 0.5;
    dq.frame_num = 1;
    assert(fabs(pattern->score(&Detection::Source, &dq) - 1.0) < 1e-6);
  }
  void testScoringTrajectory() {
    std::vector<Detection *> trajectory, q_trajectory;
    Detection d1, d2, d3;
    d1.x = 0;
    d1.y = 0;
    d1.frame_num = 0;
    d2.x = 1;
    d2.y = 0;
    d2.frame_num = 1;
    d3.x = 2;
    d3.y = 1;
    d3.frame_num = 2;
    trajectory.push_back(&d1);
    trajectory.push_back(&d2);
    trajectory.push_back(&d3);

    Pattern *pattern =
        new IDFMatch3DPattern(Pattern::NormalPattern, trajectory, 1.0);

    Detection t1, t2, t3;
    t1.x = 0;
    t1.y = 0;
    t1.frame_num = -1;
    t2.x = 0;
    t2.y = 0.5;
    t2.frame_num = 0;
    t3.x = 0;
    t3.y = -0.5;
    t3.frame_num = 1;

    q_trajectory.push_back(&t1);
    q_trajectory.push_back(&t2);
    q_trajectory.push_back(&t3);

    assert(fabs(pattern->score_trajectory(q_trajectory) - 1.0) < 1e-6);
    assert(fabs(pattern->score_trajectory(trajectory) - 3.0) < 1e-6);
  }
};
#endif  // CODE_INCLUDE_TEST_IDFMATCH3DPATTERNTEST_H_

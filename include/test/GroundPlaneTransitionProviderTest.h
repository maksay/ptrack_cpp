/*
 * Copyright 2016 Andrii Maksai
 */
#ifndef CODE_INCLUDE_TEST_GROUNDPLANETRANSITIONPROVIDERTEST_H_
#define CODE_INCLUDE_TEST_GROUNDPLANETRANSITIONPROVIDERTEST_H_
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <utility>
#include <vector>
#include "Detection.h"
#include "GroundPlaneTransitionProvider.h"
class GroundPlaneTransitionProviderTest {
 public:
  void testAll() {
    std::cerr << "Testing GroundPlaneTransitionProvider...";
    testEmpty();
    testCondition1();
    testCondition2();
    testCondition3();
    testCondition4();
    testConditions1234();
    std::cerr << "OK\n";
  }

  void testEmpty() {
    float D_1, D_2;
    int D_t;

    D_1 = 100;
    D_2 = 100;
    D_t = 100;
    assert(GroundPlaneTransitionProvider(D_1, D_2, D_t)
           .getTransitions(std::vector<Detection*>(0), NULL).size() == 0);
  }

  void testCondition1() {
    Detection d1, d2, d3;
    float D_1, D_2;
    int D_t;

    d1.x = 0;
    d1.y = 0;
    d1.original_trajectory_id = 0;
    d1.frame_num = 0;

    d2.x = 1;
    d2.y = 1;
    d2.original_trajectory_id = 1;
    d2.frame_num = 1;

    d3.x = 2;
    d3.y = 2;
    d3.original_trajectory_id = 2;
    d3.frame_num = 1;

    std::vector<Detection*> dets;
    dets.push_back(&d1);
    dets.push_back(&d2);
    dets.push_back(&d3);

    std::vector<Transition> transitions;
    std::vector<Transition > expectedTransitions;

    D_1 = 2;
    D_2 = -1;
    D_t = -1;

    transitions =
        GroundPlaneTransitionProvider(D_1, D_2, D_t).getTransitions(dets, NULL);
    expectedTransitions.clear();
    expectedTransitions.push_back(Transition(&d1, &d2, NULL));
    sort(expectedTransitions.begin(), expectedTransitions.end());
    sort(transitions.begin(), transitions.end());
    assert(transitions == expectedTransitions);

    D_1 = 1;
    transitions =
        GroundPlaneTransitionProvider(D_1, D_2, D_t).getTransitions(dets, NULL);
    expectedTransitions.clear();
    assert(transitions == expectedTransitions);

    D_1 = 3;
    transitions =
        GroundPlaneTransitionProvider(D_1, D_2, D_t).getTransitions(dets, NULL);
    expectedTransitions.clear();
    expectedTransitions.push_back(Transition(&d1, &d2, NULL));
    expectedTransitions.push_back(Transition(&d1, &d3, NULL));
    sort(expectedTransitions.begin(), expectedTransitions.end());
    sort(transitions.begin(), transitions.end());
    assert(transitions == expectedTransitions);
  }
  void testCondition2() {
    Detection d1, d2, d3;
    float D_1, D_2;
    int D_t;

    d1.x = 0;
    d1.y = 0;
    d1.original_trajectory_id = 0;
    d1.frame_num = 0;

    d2.x = 1;
    d2.y = 1;
    d2.original_trajectory_id = 0;
    d2.frame_num = 1;

    d3.x = 2;
    d3.y = 2;
    d3.original_trajectory_id = 1;
    d3.frame_num = 2;

    std::vector<Detection*> dets;
    dets.push_back(&d1);
    dets.push_back(&d2);
    dets.push_back(&d3);

    std::vector<Transition> transitions;
    std::vector<Transition> expectedTransitions;

    D_1 = 1;
    D_2 = 1;
    D_t = 1;

    transitions =
        GroundPlaneTransitionProvider(D_1, D_2, D_t).getTransitions(dets, NULL);
    expectedTransitions.clear();
    expectedTransitions.push_back(Transition(&d1, &d2, NULL));
    sort(expectedTransitions.begin(), expectedTransitions.end());
    sort(transitions.begin(), transitions.end());
    assert(transitions == expectedTransitions);

    d3.original_trajectory_id = 0;
    transitions =
        GroundPlaneTransitionProvider(D_1, D_2, D_t).getTransitions(dets, NULL);
    expectedTransitions.clear();
    expectedTransitions.push_back(Transition(&d1, &d2, NULL));
    expectedTransitions.push_back(Transition(&d2, &d3, NULL));
    sort(expectedTransitions.begin(), expectedTransitions.end());
    sort(transitions.begin(), transitions.end());
    assert(transitions == expectedTransitions);
  }
  void testCondition3() {
    Detection d1, d2, d3;
    float D_1, D_2;
    int D_t;

    d1.x = 0;
    d1.y = 0;
    d1.original_trajectory_id = 0;
    d1.frame_num = 0;

    d2.x = 1;
    d2.y = 1;
    d2.original_trajectory_id = 1;
    d2.frame_num = 1;

    d3.x = 2;
    d3.y = 2;
    d3.original_trajectory_id = 2;
    d3.frame_num = 2;

    std::vector<Detection*> dets;
    dets.push_back(&d1);
    dets.push_back(&d2);
    dets.push_back(&d3);

    std::vector<Transition> transitions;
    std::vector<Transition> expectedTransitions;

    D_1 = 1;
    D_2 = 1;
    D_t = 1;

    transitions =
        GroundPlaneTransitionProvider(D_1, D_2, D_t).getTransitions(dets, NULL);
    assert(transitions.size() == 0);

    D_2 = 2;
    transitions =
        GroundPlaneTransitionProvider(D_1, D_2, D_t).getTransitions(dets, NULL);
    expectedTransitions.clear();
    expectedTransitions.push_back(Transition(&d1, &d2, NULL));
    expectedTransitions.push_back(Transition(&d2, &d3, NULL));
    sort(expectedTransitions.begin(), expectedTransitions.end());
    sort(transitions.begin(), transitions.end());
    assert(transitions == expectedTransitions);

    D_t = 2;
    transitions =
        GroundPlaneTransitionProvider(D_1, D_2, D_t).getTransitions(dets, NULL);
    expectedTransitions.clear();
    expectedTransitions.push_back(Transition(&d1, &d2, NULL));
    expectedTransitions.push_back(Transition(&d2, &d3, NULL));
    sort(expectedTransitions.begin(), expectedTransitions.end());
    sort(transitions.begin(), transitions.end());
    assert(transitions == expectedTransitions);

    D_2 = 3;
    transitions =
        GroundPlaneTransitionProvider(D_1, D_2, D_t).getTransitions(dets, NULL);
    expectedTransitions.clear();
    expectedTransitions.push_back(Transition(&d1, &d2, NULL));
    expectedTransitions.push_back(Transition(&d1, &d3, NULL));
    expectedTransitions.push_back(Transition(&d2, &d3, NULL));
    sort(expectedTransitions.begin(), expectedTransitions.end());
    sort(transitions.begin(), transitions.end());
    assert(transitions == expectedTransitions);
  }
  void testCondition4() {
    Detection d1, d2, d3;
    float D_1, D_2;
    int D_t;

    d1.x = 0;
    d1.y = 0;
    d1.original_trajectory_id = 0;
    d1.frame_num = 0;

    d2.x = 1;
    d2.y = 1;
    d2.original_trajectory_id = 1;
    d2.frame_num = 1;

    d3.x = 2;
    d3.y = 2;
    d3.original_trajectory_id = 2;
    d3.frame_num = 2;

    std::vector<Detection*> dets;
    dets.push_back(&d1);
    dets.push_back(&d2);
    dets.push_back(&d3);

    std::vector<Transition> transitions;
    std::vector<Transition> expectedTransitions;

    D_1 = 1;
    D_2 = 1;
    D_t = 1;

    transitions =
        GroundPlaneTransitionProvider(D_1, D_2, D_t).getTransitions(dets, NULL);
    expectedTransitions.clear();
    assert(transitions == expectedTransitions);

    dets.push_back(&Detection::Source);

    transitions =
        GroundPlaneTransitionProvider(D_1, D_2, D_t).getTransitions(dets, NULL);
    expectedTransitions.clear();
    expectedTransitions.push_back(Transition(&Detection::Source, &d1, NULL));
    expectedTransitions.push_back(Transition(&Detection::Source, &d2, NULL));
    expectedTransitions.push_back(Transition(&Detection::Source, &d3, NULL));
    sort(expectedTransitions.begin(), expectedTransitions.end());
    sort(transitions.begin(), transitions.end());
    assert(transitions == expectedTransitions);

    dets.push_back(&Detection::Sink);
    transitions =
        GroundPlaneTransitionProvider(D_1, D_2, D_t).getTransitions(dets, NULL);
    expectedTransitions.push_back(Transition(&d1, &Detection::Sink, NULL));
    expectedTransitions.push_back(Transition(&d2, &Detection::Sink, NULL));
    expectedTransitions.push_back(Transition(&d3, &Detection::Sink, NULL));
    sort(expectedTransitions.begin(), expectedTransitions.end());
    sort(transitions.begin(), transitions.end());
    assert(transitions == expectedTransitions);
  }

  void testConditions1234() {
    Detection d1, d2, d3, d4, d5, d6, d7, d8;
    float D_1, D_2;
    int D_t;

    d1.x = 0;
    d1.y = 0;
    d1.original_trajectory_id = 0;
    d1.frame_num = 0;

    d2.x = 0;
    d2.y = 1;
    d2.original_trajectory_id = 0;
    d2.frame_num = 1;

    d3.x = 1;
    d3.y = 2;
    d3.original_trajectory_id = 0;
    d3.frame_num = 2;

    d4.x = 3;
    d4.y = 3;
    d4.original_trajectory_id = 1;
    d4.frame_num = 4;

    d5.x = 4;
    d5.y = 3;
    d5.original_trajectory_id = 1;
    d5.frame_num = 5;

    d6.x = 2;
    d6.y = 3;
    d6.original_trajectory_id = 2;
    d6.frame_num = 3;

    d7.x = 3;
    d7.y = 4;
    d7.original_trajectory_id = 2;
    d7.frame_num = 4;

    d8.x = 4;
    d8.y = 5;
    d8.original_trajectory_id = 2;
    d8.frame_num = 5;

    std::vector<Detection*> dets;
    dets.push_back(&d1);
    dets.push_back(&d2);
    dets.push_back(&d3);
    dets.push_back(&d4);
    dets.push_back(&d5);
    dets.push_back(&d6);
    dets.push_back(&d7);
    dets.push_back(&d8);
    dets.push_back(&Detection::Source);
    dets.push_back(&Detection::Sink);

    std::vector<Transition> transitions;
    std::vector<Transition> expectedTransitions;

    D_1 = 1;
    D_2 = 2;
    D_t = 2;

    transitions =
        GroundPlaneTransitionProvider(D_1, D_2, D_t).getTransitions(dets, NULL);

    // Condition 4

    expectedTransitions.push_back(Transition(&Detection::Source, &d1, NULL));
    expectedTransitions.push_back(Transition(&Detection::Source, &d2, NULL));
    expectedTransitions.push_back(Transition(&Detection::Source, &d3, NULL));
    expectedTransitions.push_back(Transition(&Detection::Source, &d4, NULL));
    expectedTransitions.push_back(Transition(&Detection::Source, &d5, NULL));
    expectedTransitions.push_back(Transition(&Detection::Source, &d6, NULL));
    expectedTransitions.push_back(Transition(&Detection::Source, &d7, NULL));
    expectedTransitions.push_back(Transition(&Detection::Source, &d8, NULL));

    expectedTransitions.push_back(Transition(&d1, &Detection::Sink, NULL));
    expectedTransitions.push_back(Transition(&d2, &Detection::Sink, NULL));
    expectedTransitions.push_back(Transition(&d3, &Detection::Sink, NULL));
    expectedTransitions.push_back(Transition(&d4, &Detection::Sink, NULL));
    expectedTransitions.push_back(Transition(&d5, &Detection::Sink, NULL));
    expectedTransitions.push_back(Transition(&d6, &Detection::Sink, NULL));
    expectedTransitions.push_back(Transition(&d7, &Detection::Sink, NULL));
    expectedTransitions.push_back(Transition(&d8, &Detection::Sink, NULL));

    // Condition 2
    expectedTransitions.push_back(Transition(&d1, &d2, NULL));
    expectedTransitions.push_back(Transition(&d2, &d3, NULL));
    expectedTransitions.push_back(Transition(&d4, &d5, NULL));
    expectedTransitions.push_back(Transition(&d6, &d7, NULL));
    expectedTransitions.push_back(Transition(&d7, &d8, NULL));

    // Condition 1
    expectedTransitions.push_back(Transition(&d6, &d4, NULL));

    // Condition 3
    expectedTransitions.push_back(Transition(&d3, &d6, NULL));

    sort(expectedTransitions.begin(), expectedTransitions.end());
    sort(transitions.begin(), transitions.end());
    assert(transitions == expectedTransitions);
  }
};
#endif  // CODE_INCLUDE_TEST_GROUNDPLANETRANSITIONPROVIDERTEST_H_

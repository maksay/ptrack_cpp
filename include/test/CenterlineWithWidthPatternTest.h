/*
 * Copyright 2016 Andrii Maksai
 */
#ifndef CODE_INCLUDE_TEST_CENTERLINEWITHWIDTHPATTERNTEST_H_
#define CODE_INCLUDE_TEST_CENTERLINEWITHWIDTHPATTERNTEST_H_
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <utility>
#include <vector>
#include "CenterlineWithWidthPattern.h"
#include "Detection.h"
class CenterlineWithWidthPatternTest {
 public:
  void testAll() {
    std::cerr << "Testing CenterlineWithWidthPattern...";
    testProjectionPointIndexing();
    testPatternLengthBetweenPointsCalculation();
    testNMFunctionEvaluation();
    testScoreFunctionEvaluation();
    testArea();
    testToString();
    testGetters();
    std::cerr << "OK\n";
  }
  void testProjectionPointIndexing() {
    Detection d;
    std::vector<std::pair<float, float> > points;
    float width;
    float alpha, epsilon, epsilon_no_pattern;
    std::pair<size_t, float> ret;

    points.clear();
    points.push_back(std::make_pair(0, 0));
    points.push_back(std::make_pair(1, 1));
    points.push_back(std::make_pair(2, 2));

    d.x = 0;
    d.y = 2;

    ret = CenterlineWithWidthPattern(Pattern::NormalPattern, points, width,
                                     alpha, epsilon, epsilon_no_pattern)
              .projection_point_idx(&d);
    assert(ret.first == 1 && fabs(ret.second - sqrt(2.)) < 1e-6);

    d.x = 0;
    d.y = 0.99;
    ret = CenterlineWithWidthPattern(Pattern::NormalPattern, points, width,
                                     alpha, epsilon, epsilon_no_pattern)
              .projection_point_idx(&d);
    assert(ret.first == 0 && fabs(ret.second - 0.99) < 1e-6);

    d.y = 2;
    d.x = 1.01;
    ret = CenterlineWithWidthPattern(Pattern::NormalPattern, points, width,
                                     alpha, epsilon, epsilon_no_pattern)
              .projection_point_idx(&d);
    assert(ret.first == 2 && fabs(ret.second - 0.99) < 1e-6);

    d.y = -1;
    d.x = -1;
    ret = CenterlineWithWidthPattern(Pattern::NormalPattern, points, width,
                                     alpha, epsilon, epsilon_no_pattern)
              .projection_point_idx(&d);
    assert(ret.first == 0 && fabs(ret.second - sqrt(2.)) < 1e-6);
  }
  void testPatternLengthBetweenPointsCalculation() {
    Detection d;
    std::vector<std::pair<float, float> > points;
    float width;
    float alpha, epsilon, epsilon_no_pattern;
    float ret;

    points.clear();
    points.push_back(std::make_pair(0, 0));
    points.push_back(std::make_pair(2, 0));
    points.push_back(std::make_pair(3, 1));
    points.push_back(std::make_pair(5, 1));

    ret = CenterlineWithWidthPattern(Pattern::NormalPattern, points, width,
                                     alpha, epsilon, epsilon_no_pattern)
              .pattern_length_between(0, 0);
    assert(fabs(ret - 0) < 1e-6);

    ret = CenterlineWithWidthPattern(Pattern::NormalPattern, points, width,
                                     alpha, epsilon, epsilon_no_pattern)
              .pattern_length_between(0, 1);
    assert(fabs(ret - 2) < 1e-6);

    ret = CenterlineWithWidthPattern(Pattern::NormalPattern, points, width,
                                     alpha, epsilon, epsilon_no_pattern)
              .pattern_length_between(0, 2);
    assert(fabs(ret - 2 - sqrt(2.)) < 1e-6);

    ret = CenterlineWithWidthPattern(Pattern::NormalPattern, points, width,
                                     alpha, epsilon, epsilon_no_pattern)
              .pattern_length_between(0, 3);
    assert(fabs(ret - 2 - sqrt(2.) - 2) < 1e-6);

    ret = CenterlineWithWidthPattern(Pattern::NormalPattern, points, width,
                                     alpha, epsilon, epsilon_no_pattern)
              .pattern_length_between(2, 1);
    assert(fabs(ret + sqrt(2.)) < 1e-6);

    ret = CenterlineWithWidthPattern(Pattern::NormalPattern, points, width,
                                     alpha, epsilon, epsilon_no_pattern)
              .pattern_length_between(3, 1);
    assert(fabs(ret + sqrt(2.) + 2) < 1e-6);
  }
  void testNMFunctionEvaluation() {
    Detection d1, d2;
    std::vector<std::pair<float, float> > points;
    float width;
    float alpha, epsilon, epsilon_no_pattern;
    std::pair<float, float> ret;
    double ret_score;

    points.clear();
    points.push_back(std::make_pair(2, 0));
    points.push_back(std::make_pair(1, 1));
    points.push_back(std::make_pair(1, 2));
    points.push_back(std::make_pair(2, 2));
    points.push_back(std::make_pair(5, -1));

    d1.x = 0;
    d1.y = 1;
    d1.frame_num = 0;
    d1.first_batch_frame = false;
    d1.last_batch_frame = false;
    d2.x = 2;
    d2.y = 3;
    d2.frame_num = 1;
    d2.first_batch_frame = false;
    d2.last_batch_frame = false;

    width = 1;

    // Case 1: Normal nodes, normal pattern, positive projection, detections
    // within width of the pattern
    ret = CenterlineWithWidthPattern(Pattern::NormalPattern, points, width,
                                     alpha, epsilon, epsilon_no_pattern)
              .mn_function(&d1, &d2);
    assert(fabs(ret.first - sqrt(2.) - sqrt(2.)) < 1e-6 &&
           fabs(ret.second - 2 - 2 * sqrt(2.)) < 1e-6);

    // Case 1.a: Normal nodes, normal pattern, projection = 0
    ret = CenterlineWithWidthPattern(Pattern::NormalPattern, points, width,
                                     alpha, epsilon, epsilon_no_pattern)
              .mn_function(&d1, &d1);
    assert(fabs(ret.first - 0) < 1e-6 && fabs(ret.second - 0) < 1e-6);

    width = 0.5;
    // Case 2: Normal nodes, normal pattern, positive projection, detections
    // outside of width of the pattern
    ret = CenterlineWithWidthPattern(Pattern::NormalPattern, points, width,
                                     alpha, epsilon, epsilon_no_pattern)
              .mn_function(&d1, &d2);
    assert(fabs(ret.first - 0) < 1e-6 &&
           fabs(ret.second - 2 - 2 * sqrt(2.)) < 1e-6);

    epsilon = 0.5;
    // Case 3: Normal nodes, normal pattern, negative projection
    ret = CenterlineWithWidthPattern(Pattern::NormalPattern, points, width,
                                     alpha, epsilon, epsilon_no_pattern)
              .mn_function(&d2, &d1);
    assert(fabs(ret.first + sqrt(2.) * 1.5) < 1e-6 &&
           fabs(ret.second + 2 - 2 * sqrt(2.)) < 1e-6);

    epsilon_no_pattern = 0.3;
    // Case 4: Normal nodes, no pattern
    ret = CenterlineWithWidthPattern(Pattern::NoPattern, points, width, alpha,
                                     epsilon, epsilon_no_pattern)
              .mn_function(&d1, &d2);
    assert(fabs(ret.first - 2 * sqrt(2.) * 0.3) < 1e-6 &&
           fabs(ret.second - 2 * sqrt(2.)) < 1e-6);

    // Case 5.1: Transition from source to detection NOT in the first
    // frame of the batch, normal pattern
    ret = CenterlineWithWidthPattern(Pattern::NormalPattern, points, width,
                                     alpha, epsilon, epsilon_no_pattern)
              .mn_function(&Detection::Source, &d1);
    assert(fabs(ret.first - 0) < 1e-6 && fabs(ret.second - sqrt(2.)) < 1e-6);

    d1.first_batch_frame = true;
    // Case 6.1: Transition from source to detection IN the first frame
    // of the batch, normal pattern
    ret = CenterlineWithWidthPattern(Pattern::NormalPattern, points, width,
                                     alpha, epsilon, epsilon_no_pattern)
              .mn_function(&Detection::Source, &d1);
    assert(fabs(ret.first - 0) < 1e-6 && fabs(ret.second - 0) < 1e-6);

    // Case 5.2: Transition from detection to sink NOT in the last
    // frame of the batch, normal pattern
    ret = CenterlineWithWidthPattern(Pattern::NormalPattern, points, width,
                                     alpha, epsilon, epsilon_no_pattern)
              .mn_function(&d2, &Detection::Sink);
    assert(fabs(ret.first - 0) < 1e-6 &&
           fabs(ret.second - 3 * sqrt(2.)) < 1e-6);

    d2.last_batch_frame = true;
    // Case 6.2: Transition from detection to sink IN the last frame
    // of the batch, normal pattern
    ret = CenterlineWithWidthPattern(Pattern::NormalPattern, points, width,
                                     alpha, epsilon, epsilon_no_pattern)
              .mn_function(&d2, &Detection::Sink);
    assert(fabs(ret.first - 0) < 1e-6 && fabs(ret.second - 0) < 1e-6);

    d1.first_batch_frame = false;
    d2.last_batch_frame = false;
    // Case 7.1: Transition from source, assigned to no pattern
    epsilon_no_pattern = 0.13;
    ret = CenterlineWithWidthPattern(Pattern::NoPattern, points, width, alpha,
                                     epsilon, epsilon_no_pattern)
              .mn_function(&Detection::Source, &d1);
    assert(fabs(ret.first - 2 * 0.13) < 1e-6 && fabs(ret.second - 2) < 1e-6);

    // Case 7.2: Transition to sink, assigned to no pattern
    ret = CenterlineWithWidthPattern(Pattern::NoPattern, points, width, alpha,
                                     epsilon, epsilon_no_pattern)
              .mn_function(&d2, &Detection::Sink);
    assert(fabs(ret.first - 0) < 1e-6 && fabs(ret.second - 0) < 1e-6);
  }
  void testScoreFunctionEvaluation() {
    Detection d1, d2;
    std::vector<std::pair<float, float> > points;
    float width;
    float alpha, epsilon, epsilon_no_pattern;
    std::pair<float, float> ret;
    double ret_score;

    points.clear();
    points.push_back(std::make_pair(2, 0));
    points.push_back(std::make_pair(1, 1));
    points.push_back(std::make_pair(1, 2));
    points.push_back(std::make_pair(2, 2));
    points.push_back(std::make_pair(5, -1));

    d1.x = 0;
    d1.y = 1;
    d1.frame_num = 0;
    d1.first_batch_frame = true;
    d1.last_batch_frame = false;
    d2.x = 2;
    d2.y = 3;
    d2.frame_num = 1;
    d2.first_batch_frame = false;
    d2.last_batch_frame = true;

    alpha = 0.373;
    width = 1;
    ret_score =
        CenterlineWithWidthPattern(Pattern::NormalPattern, points, width, alpha,
                                   epsilon, epsilon_no_pattern)
            .score(&d1, &d2);
    assert(fabs(ret_score - ((2 * sqrt(2.)) - 0.373 * (2 + 2 * sqrt(2.)))) <
           1e-6);

    width = 0.5;
    ret_score =
        CenterlineWithWidthPattern(Pattern::NormalPattern, points, width, alpha,
                                   epsilon, epsilon_no_pattern)
            .score(&d1, &d2);
    assert(fabs(ret_score - (0 - 0.373 * (2 + 2 * sqrt(2.)))) < 1e-6);

    std::vector<Detection*> trajectory(0);
    trajectory.push_back(&d1);
    trajectory.push_back(&d2);

    float expected_score = 0;
    width = 1;
    expected_score +=
        CenterlineWithWidthPattern(Pattern::NormalPattern, points, width, alpha,
                                   epsilon, epsilon_no_pattern)
            .score(&Detection::Source, &d1);
    expected_score +=
        CenterlineWithWidthPattern(Pattern::NormalPattern, points, width, alpha,
                                   epsilon, epsilon_no_pattern)
            .score(&d1, &d2);
    expected_score +=
        CenterlineWithWidthPattern(Pattern::NormalPattern, points, width, alpha,
                                   epsilon, epsilon_no_pattern)
            .score(&d2, &Detection::Sink);

    ret_score =
        CenterlineWithWidthPattern(Pattern::NormalPattern, points, width, alpha,
                                   epsilon, epsilon_no_pattern)
            .score_trajectory(trajectory);
    assert(fabs(ret_score - expected_score) < 1e-6);
  }
  void testArea() {
    Detection d1, d2;
    std::vector<std::pair<float, float> > points;
    float width;
    float alpha, epsilon, epsilon_no_pattern;
    double ret_score;

    points.clear();
    points.push_back(std::make_pair(2, 0));
    points.push_back(std::make_pair(1, 1));
    points.push_back(std::make_pair(1, 2));
    points.push_back(std::make_pair(2, 2));
    points.push_back(std::make_pair(5, -1));

    // One
    width = 0.373;
    ret_score =
        CenterlineWithWidthPattern(Pattern::NormalPattern, points, width, alpha,
                                   epsilon, epsilon_no_pattern)
            .area();
    assert(fabs(ret_score - 1e4 -
                width * 2 * (sqrt(2.) + 1 + 1 + 3 * sqrt(2.))) < 1);
    // Two
    ret_score = CenterlineWithWidthPattern(Pattern::NoPattern, points, width,
                                           alpha, epsilon, epsilon_no_pattern)
                    .area();
    assert(fabs(ret_score - 0) < 1e-6);
  }
  void testToString() {
    std::vector<std::pair<float, float> > pattern;

    CenterlineWithWidthPattern nop(Pattern::NoPattern, pattern, 0, 0, 0, 0);
    assert(nop.to_string() == "NoPattern");

    pattern.push_back(std::make_pair(0, 0));
    pattern.push_back(std::make_pair(1, 2));

    CenterlineWithWidthPattern pat(Pattern::NormalPattern, pattern, 0.33, 0, 0,
                                   0);
    assert(pat.to_string() == "2 0.33 0 0 1 2");
  }
  void testGetters() {
    CenterlineWithWidthPattern nop(Pattern::NoPattern,
                                   std::vector<std::pair<float, float> >(0), 0,
                                   0.11, 0.12, 0.13);
    assert(fabs(nop.get_alpha() - 0.11) < 1e-6);
    assert(fabs(nop.get_epsilon() - 0.12) < 1e-6);
    assert(fabs(nop.get_epsilon_no_pattern() - 0.13) < 1e-6);
    nop.get_alpha() = 0.14;
    nop.get_epsilon_no_pattern() = 0.15;
    nop.get_epsilon() = 0.22;
    assert(fabs(nop.get_alpha() - 0.14) < 1e-6);
    assert(fabs(nop.get_epsilon_no_pattern() - 0.15) < 1e-6);
    assert(fabs(nop.get_epsilon() - 0.22) < 1e-6);
  }
};
#endif  // CODE_INCLUDE_TEST_CENTERLINEWITHWIDTHPATTERNTEST_H_

/*
 * Copyright 2016 Andrii Maksai
 */
#ifndef CODE_INCLUDE_TEST_PATTERNOPTIMIZERTEST_H_
#define CODE_INCLUDE_TEST_PATTERNOPTIMIZERTEST_H_
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <utility>
#include <vector>
#include "PatternOptimizer.h"

#define mp std::make_pair
class PatternOptimizerTest {
 public:
  void testAll() {
    std::cerr << "Testing PatternOptimizer...";
    testFindingMinimalPatternsWithPositiveScore();
    testFindingBestPatternsBoundByMetric();
    std::cerr << "OK\n";
  }
  void testFindingMinimalPatternsWithPositiveScore() {
    std::vector<std::pair<std::pair<Detection *, Detection *>, float> >
        pattern1_scores(0), pattern2_scores(0), pattern3_scores(0);

    Pattern *pattern1, *pattern2, *pattern3;
    std::vector<Pattern *> possible_patterns, result, expected_result;
    std::vector<std::vector<Detection *> > trajectories;
    std::vector<Detection *> trajectory;
    std::vector<std::pair<std::vector<Detection *>, Pattern *> > assignment,
        expected_assignment;
    Detection d1, d2, d3;

    // One
    pattern1_scores.clear();
    pattern1_scores.push_back(mp(mp(&Detection::Source, &d1), 1));
    pattern1_scores.push_back(mp(mp(&d1, &Detection::Sink), 0));
    pattern1_scores.push_back(mp(mp(&Detection::Source, &d2), -10));
    pattern1_scores.push_back(mp(mp(&d2, &Detection::Sink), 0));
    pattern1_scores.push_back(mp(mp(&Detection::Source, &d3), -10));
    pattern1_scores.push_back(mp(mp(&d3, &Detection::Sink), 0));
    pattern1 = new HardcodedPattern(Pattern::NormalPattern, pattern1_scores, 1);
    pattern2_scores.clear();
    pattern2_scores.push_back(mp(mp(&Detection::Source, &d1), -10));
    pattern2_scores.push_back(mp(mp(&d1, &Detection::Sink), 0));
    pattern2_scores.push_back(mp(mp(&Detection::Source, &d2), 1));
    pattern2_scores.push_back(mp(mp(&d2, &Detection::Sink), 0));
    pattern2_scores.push_back(mp(mp(&Detection::Source, &d3), -10));
    pattern2_scores.push_back(mp(mp(&d3, &Detection::Sink), 0));
    pattern2 = new HardcodedPattern(Pattern::NormalPattern, pattern2_scores, 2);
    pattern3_scores.clear();
    pattern3_scores.push_back(mp(mp(&Detection::Source, &d1), -10));
    pattern3_scores.push_back(mp(mp(&d1, &Detection::Sink), 0));
    pattern3_scores.push_back(mp(mp(&Detection::Source, &d2), -10));
    pattern3_scores.push_back(mp(mp(&d2, &Detection::Sink), 0));
    pattern3_scores.push_back(mp(mp(&Detection::Source, &d3), 1));
    pattern3_scores.push_back(mp(mp(&d3, &Detection::Sink), 0));
    pattern3 = new HardcodedPattern(Pattern::NormalPattern, pattern3_scores, 3);

    possible_patterns.clear();
    possible_patterns.push_back(pattern1);
    possible_patterns.push_back(pattern2);
    possible_patterns.push_back(pattern3);

    trajectories.clear();
    trajectory.clear();
    trajectory.push_back(&d1);
    trajectories.push_back(trajectory);
    trajectory.clear();
    trajectory.push_back(&d2);
    trajectories.push_back(trajectory);
    trajectory.clear();
    trajectory.push_back(&d3);
    trajectories.push_back(trajectory);

    result = PatternOptimizer().findMinimalPatternsWithPositiveScore(
        possible_patterns, trajectories, &assignment);

    expected_result.clear();
    expected_result.push_back(pattern1);
    expected_result.push_back(pattern2);
    expected_result.push_back(pattern3);

    expected_assignment.clear();
    expected_assignment.push_back(mp(trajectories[0], pattern1));
    expected_assignment.push_back(mp(trajectories[1], pattern2));
    expected_assignment.push_back(mp(trajectories[2], pattern3));

    sort(result.begin(), result.end());
    sort(expected_result.begin(), expected_result.end());
    assert(result == expected_result);
    sort(assignment.begin(), assignment.end());
    sort(expected_assignment.begin(), expected_assignment.end());
    assert(assignment == expected_assignment);

    // Two
    pattern1_scores.clear();
    pattern1_scores.push_back(mp(mp(&Detection::Source, &d1), 1));
    pattern1_scores.push_back(mp(mp(&d1, &Detection::Sink), 0));
    pattern1_scores.push_back(mp(mp(&Detection::Source, &d2), -1));
    pattern1_scores.push_back(mp(mp(&d2, &Detection::Sink), 0));
    pattern1_scores.push_back(mp(mp(&Detection::Source, &d3), -10));
    pattern1_scores.push_back(mp(mp(&d3, &Detection::Sink), 0));
    pattern1 = new HardcodedPattern(Pattern::NormalPattern, pattern1_scores, 1);
    pattern2_scores.clear();
    pattern2_scores.push_back(mp(mp(&Detection::Source, &d1), -1));
    pattern2_scores.push_back(mp(mp(&d1, &Detection::Sink), 0));
    pattern2_scores.push_back(mp(mp(&Detection::Source, &d2), 1));
    pattern2_scores.push_back(mp(mp(&d2, &Detection::Sink), 0));
    pattern2_scores.push_back(mp(mp(&Detection::Source, &d3), -10));
    pattern2_scores.push_back(mp(mp(&d3, &Detection::Sink), 0));
    pattern2 = new HardcodedPattern(Pattern::NormalPattern, pattern2_scores, 2);
    pattern3_scores.clear();
    pattern3_scores.push_back(mp(mp(&Detection::Source, &d1), -10));
    pattern3_scores.push_back(mp(mp(&d1, &Detection::Sink), 0));
    pattern3_scores.push_back(mp(mp(&Detection::Source, &d2), -1));
    pattern3_scores.push_back(mp(mp(&d2, &Detection::Sink), 0));
    pattern3_scores.push_back(mp(mp(&Detection::Source, &d3), 1));
    pattern3_scores.push_back(mp(mp(&d3, &Detection::Sink), 0));
    pattern3 = new HardcodedPattern(Pattern::NormalPattern, pattern3_scores, 3);

    possible_patterns.clear();
    possible_patterns.push_back(pattern1);
    possible_patterns.push_back(pattern2);
    possible_patterns.push_back(pattern3);

    trajectories.clear();
    trajectory.clear();
    trajectory.push_back(&d1);
    trajectories.push_back(trajectory);
    trajectory.clear();
    trajectory.push_back(&d2);
    trajectories.push_back(trajectory);
    trajectory.clear();
    trajectory.push_back(&d3);
    trajectories.push_back(trajectory);

    result = PatternOptimizer().findMinimalPatternsWithPositiveScore(
        possible_patterns, trajectories, &assignment);

    expected_result.clear();
    expected_result.push_back(pattern1);
    expected_result.push_back(pattern3);

    expected_assignment.clear();
    expected_assignment.push_back(mp(trajectories[0], pattern1));
    expected_assignment.push_back(mp(trajectories[1], pattern3));
    expected_assignment.push_back(mp(trajectories[2], pattern3));

    sort(result.begin(), result.end());
    sort(expected_result.begin(), expected_result.end());
    assert(result == expected_result);
    sort(assignment.begin(), assignment.end());
    sort(expected_assignment.begin(), expected_assignment.end());
    assert(assignment == expected_assignment);
    // Three
    pattern1_scores.clear();
    pattern1_scores.push_back(mp(mp(&Detection::Source, &d1), 1));
    pattern1_scores.push_back(mp(mp(&d1, &Detection::Sink), 0));
    pattern1_scores.push_back(mp(mp(&Detection::Source, &d2), -10));
    pattern1_scores.push_back(mp(mp(&d2, &Detection::Sink), 0));
    pattern1_scores.push_back(mp(mp(&Detection::Source, &d3), -10));
    pattern1_scores.push_back(mp(mp(&d3, &Detection::Sink), 0));
    pattern1 = new HardcodedPattern(Pattern::NormalPattern, pattern1_scores, 1);
    pattern2_scores.clear();
    pattern2_scores.push_back(mp(mp(&Detection::Source, &d1), -1));
    pattern2_scores.push_back(mp(mp(&d1, &Detection::Sink), 0));
    pattern2_scores.push_back(mp(mp(&Detection::Source, &d2), 1));
    pattern2_scores.push_back(mp(mp(&d2, &Detection::Sink), 0));
    pattern2_scores.push_back(mp(mp(&Detection::Source, &d3), -10));
    pattern2_scores.push_back(mp(mp(&d3, &Detection::Sink), 0));
    pattern2 = new HardcodedPattern(Pattern::NormalPattern, pattern2_scores, 2);
    pattern3_scores.clear();
    pattern3_scores.push_back(mp(mp(&Detection::Source, &d1), -10));
    pattern3_scores.push_back(mp(mp(&d1, &Detection::Sink), 0));
    pattern3_scores.push_back(mp(mp(&Detection::Source, &d2), -10));
    pattern3_scores.push_back(mp(mp(&d2, &Detection::Sink), 0));
    pattern3_scores.push_back(mp(mp(&Detection::Source, &d3), 1));
    pattern3_scores.push_back(mp(mp(&d3, &Detection::Sink), 0));
    pattern3 = new HardcodedPattern(Pattern::NormalPattern, pattern3_scores, 3);

    possible_patterns.clear();
    possible_patterns.push_back(pattern1);
    possible_patterns.push_back(pattern2);
    possible_patterns.push_back(pattern3);

    trajectories.clear();
    trajectory.clear();
    trajectory.push_back(&d1);
    trajectories.push_back(trajectory);
    trajectory.clear();
    trajectory.push_back(&d2);
    trajectories.push_back(trajectory);
    trajectory.clear();
    trajectory.push_back(&d3);
    trajectories.push_back(trajectory);

    result = PatternOptimizer().findMinimalPatternsWithPositiveScore(
        possible_patterns, trajectories, &assignment);

    expected_result.clear();
    expected_result.push_back(pattern2);
    expected_result.push_back(pattern3);

    expected_assignment.clear();
    expected_assignment.push_back(mp(trajectories[0], pattern2));
    expected_assignment.push_back(mp(trajectories[1], pattern2));
    expected_assignment.push_back(mp(trajectories[2], pattern3));

    sort(result.begin(), result.end());
    sort(expected_result.begin(), expected_result.end());
    assert(result == expected_result);
    sort(assignment.begin(), assignment.end());
    sort(expected_assignment.begin(), expected_assignment.end());
    assert(assignment == expected_assignment);
  }
  void testFindingBestPatternsBoundByMetric() {
    std::vector<std::pair<std::pair<Detection *, Detection *>, float> >
        pattern1_scores(0), pattern2_scores(0), pattern3_scores(0);

    Pattern *pattern1, *pattern2, *pattern3;
    std::vector<Pattern *> possible_patterns, result, expected_result;
    std::vector<std::vector<Detection *> > trajectories;
    std::vector<Detection *> trajectory;
    std::vector<std::pair<std::vector<Detection *>, Pattern *> > assignment,
        expected_assignment;
    Detection d1, d2, d3;

    // One
    pattern1_scores.clear();
    pattern1_scores.push_back(mp(mp(&Detection::Source, &d1), 1));
    pattern1_scores.push_back(mp(mp(&d1, &Detection::Sink), 0));
    pattern1_scores.push_back(mp(mp(&Detection::Source, &d2), -10));
    pattern1_scores.push_back(mp(mp(&d2, &Detection::Sink), 0));
    pattern1_scores.push_back(mp(mp(&Detection::Source, &d3), -10));
    pattern1_scores.push_back(mp(mp(&d3, &Detection::Sink), 0));
    pattern1 = new HardcodedPattern(Pattern::NormalPattern, pattern1_scores, 1);
    pattern2_scores.clear();
    pattern2_scores.push_back(mp(mp(&Detection::Source, &d1), -10));
    pattern2_scores.push_back(mp(mp(&d1, &Detection::Sink), 0));
    pattern2_scores.push_back(mp(mp(&Detection::Source, &d2), 1));
    pattern2_scores.push_back(mp(mp(&d2, &Detection::Sink), 0));
    pattern2_scores.push_back(mp(mp(&Detection::Source, &d3), -10));
    pattern2_scores.push_back(mp(mp(&d3, &Detection::Sink), 0));
    pattern2 = new HardcodedPattern(Pattern::NormalPattern, pattern2_scores, 2);
    pattern3_scores.clear();
    pattern3_scores.push_back(mp(mp(&Detection::Source, &d1), -10));
    pattern3_scores.push_back(mp(mp(&d1, &Detection::Sink), 0));
    pattern3_scores.push_back(mp(mp(&Detection::Source, &d2), -10));
    pattern3_scores.push_back(mp(mp(&d2, &Detection::Sink), 0));
    pattern3_scores.push_back(mp(mp(&Detection::Source, &d3), 1));
    pattern3_scores.push_back(mp(mp(&d3, &Detection::Sink), 0));
    pattern3 = new HardcodedPattern(Pattern::NormalPattern, pattern3_scores, 3);

    possible_patterns.clear();
    possible_patterns.push_back(pattern1);
    possible_patterns.push_back(pattern2);
    possible_patterns.push_back(pattern3);

    trajectories.clear();
    trajectory.clear();
    trajectory.push_back(&d1);
    trajectories.push_back(trajectory);
    trajectory.clear();
    trajectory.push_back(&d2);
    trajectories.push_back(trajectory);
    trajectory.clear();
    trajectory.push_back(&d3);
    trajectories.push_back(trajectory);

    result = PatternOptimizer().findBestPatternsBoundByMetric(
        possible_patterns, trajectories, 6, &assignment);

    expected_result.clear();
    expected_result.push_back(pattern1);
    expected_result.push_back(pattern2);
    expected_result.push_back(pattern3);

    expected_assignment.clear();
    expected_assignment.push_back(mp(trajectories[0], pattern1));
    expected_assignment.push_back(mp(trajectories[1], pattern2));
    expected_assignment.push_back(mp(trajectories[2], pattern3));

    sort(result.begin(), result.end());
    sort(expected_result.begin(), expected_result.end());
    assert(result == expected_result);
    sort(assignment.begin(), assignment.end());
    sort(expected_assignment.begin(), expected_assignment.end());
    assert(assignment == expected_assignment);

    // Two
    pattern1_scores.clear();
    pattern1_scores.push_back(mp(mp(&Detection::Source, &d1), 1));
    pattern1_scores.push_back(mp(mp(&d1, &Detection::Sink), 0));
    pattern1_scores.push_back(mp(mp(&Detection::Source, &d2), -10));
    pattern1_scores.push_back(mp(mp(&d2, &Detection::Sink), 0));
    pattern1_scores.push_back(mp(mp(&Detection::Source, &d3), -10));
    pattern1_scores.push_back(mp(mp(&d3, &Detection::Sink), 0));
    pattern1 = new HardcodedPattern(Pattern::NormalPattern, pattern1_scores, 1);
    pattern2_scores.clear();
    pattern2_scores.push_back(mp(mp(&Detection::Source, &d1), 1));
    pattern2_scores.push_back(mp(mp(&d1, &Detection::Sink), 0));
    pattern2_scores.push_back(mp(mp(&Detection::Source, &d2), 1));
    pattern2_scores.push_back(mp(mp(&d2, &Detection::Sink), 0));
    pattern2_scores.push_back(mp(mp(&Detection::Source, &d3), -10));
    pattern2_scores.push_back(mp(mp(&d3, &Detection::Sink), 0));
    pattern2 = new HardcodedPattern(Pattern::NormalPattern, pattern2_scores, 2);
    pattern3_scores.clear();
    pattern3_scores.push_back(mp(mp(&Detection::Source, &d1), -10));
    pattern3_scores.push_back(mp(mp(&d1, &Detection::Sink), 0));
    pattern3_scores.push_back(mp(mp(&Detection::Source, &d2), -1));
    pattern3_scores.push_back(mp(mp(&d2, &Detection::Sink), 0));
    pattern3_scores.push_back(mp(mp(&Detection::Source, &d3), 1));
    pattern3_scores.push_back(mp(mp(&d3, &Detection::Sink), 0));
    pattern3 = new HardcodedPattern(Pattern::NormalPattern, pattern3_scores, 3);

    possible_patterns.clear();
    possible_patterns.push_back(pattern1);
    possible_patterns.push_back(pattern2);
    possible_patterns.push_back(pattern3);

    trajectories.clear();
    trajectory.clear();
    trajectory.push_back(&d1);
    trajectories.push_back(trajectory);
    trajectory.clear();
    trajectory.push_back(&d2);
    trajectories.push_back(trajectory);
    trajectory.clear();
    trajectory.push_back(&d3);
    trajectories.push_back(trajectory);

    result = PatternOptimizer().findBestPatternsBoundByMetric(
        possible_patterns, trajectories, 5, &assignment);

    expected_result.clear();
    expected_result.push_back(pattern2);
    expected_result.push_back(pattern3);

    expected_assignment.clear();
    expected_assignment.push_back(mp(trajectories[0], pattern2));
    expected_assignment.push_back(mp(trajectories[1], pattern2));
    expected_assignment.push_back(mp(trajectories[2], pattern3));

    sort(result.begin(), result.end());
    sort(expected_result.begin(), expected_result.end());
    assert(result == expected_result);
    sort(assignment.begin(), assignment.end());
    sort(expected_assignment.begin(), expected_assignment.end());
    assert(assignment == expected_assignment);
    // Three
    pattern1_scores.clear();
    pattern1_scores.push_back(mp(mp(&Detection::Source, &d1), 1));
    pattern1_scores.push_back(mp(mp(&d1, &Detection::Sink), 0));
    pattern1_scores.push_back(mp(mp(&Detection::Source, &d2), -10));
    pattern1_scores.push_back(mp(mp(&d2, &Detection::Sink), 0));
    pattern1_scores.push_back(mp(mp(&Detection::Source, &d3), -10));
    pattern1_scores.push_back(mp(mp(&d3, &Detection::Sink), 0));
    pattern1 = new HardcodedPattern(Pattern::NormalPattern, pattern1_scores, 1);
    pattern2_scores.clear();
    pattern2_scores.push_back(mp(mp(&Detection::Source, &d1), 1));
    pattern2_scores.push_back(mp(mp(&d1, &Detection::Sink), 0));
    pattern2_scores.push_back(mp(mp(&Detection::Source, &d2), 1));
    pattern2_scores.push_back(mp(mp(&d2, &Detection::Sink), 0));
    pattern2_scores.push_back(mp(mp(&Detection::Source, &d3), -10));
    pattern2_scores.push_back(mp(mp(&d3, &Detection::Sink), 0));
    pattern2 = new HardcodedPattern(Pattern::NormalPattern, pattern2_scores, 2);
    pattern3_scores.clear();
    pattern3_scores.push_back(mp(mp(&Detection::Source, &d1), -10));
    pattern3_scores.push_back(mp(mp(&d1, &Detection::Sink), 0));
    pattern3_scores.push_back(mp(mp(&Detection::Source, &d2), -1));
    pattern3_scores.push_back(mp(mp(&d2, &Detection::Sink), 0));
    pattern3_scores.push_back(mp(mp(&Detection::Source, &d3), 1));
    pattern3_scores.push_back(mp(mp(&d3, &Detection::Sink), 0));
    pattern3 = new HardcodedPattern(Pattern::NormalPattern, pattern3_scores, 3);

    possible_patterns.clear();
    possible_patterns.push_back(pattern1);
    possible_patterns.push_back(pattern2);
    possible_patterns.push_back(pattern3);

    trajectories.clear();
    trajectory.clear();
    trajectory.push_back(&d1);
    trajectories.push_back(trajectory);
    trajectory.clear();
    trajectory.push_back(&d2);
    trajectories.push_back(trajectory);
    trajectory.clear();
    trajectory.push_back(&d3);
    trajectories.push_back(trajectory);

    result = PatternOptimizer().findBestPatternsBoundByMetric(
        possible_patterns, trajectories, 4, &assignment);

    expected_result.clear();
    expected_result.push_back(pattern1);
    expected_result.push_back(pattern3);

    expected_assignment.clear();
    expected_assignment.push_back(mp(trajectories[0], pattern1));
    expected_assignment.push_back(mp(trajectories[1], pattern3));
    expected_assignment.push_back(mp(trajectories[2], pattern3));

    sort(result.begin(), result.end());
    sort(expected_result.begin(), expected_result.end());
    assert(result == expected_result);
    sort(assignment.begin(), assignment.end());
    sort(expected_assignment.begin(), expected_assignment.end());
    assert(assignment == expected_assignment);
  }
};
#endif  // CODE_INCLUDE_TEST_PATTERNOPTIMIZERTEST_H_

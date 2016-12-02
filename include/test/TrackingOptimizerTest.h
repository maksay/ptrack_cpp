/*
 * Copyright 2016 Andrii Maksai
 */
#ifndef CODE_INCLUDE_TEST_TRACKINGOPTIMIZERTEST_H_
#define CODE_INCLUDE_TEST_TRACKINGOPTIMIZERTEST_H_
#include "CenterlineWithWidthPattern.h"
#include "Detection.h"
#include "FlowConservationConstraintProvider.h"
#include "HardcodedConstraintProvider.h"
#include "HardcodedPattern.h"
#include "SpatialExclusionConstraintProvider.h"
#include "TrackingOptimizer.h"

#define mp std::make_pair
class TrackingOptimizerTest {
 public:
  void testAll() {
    std::cerr << "Testing TrackingOptimizer...";
    testTrackingOptimizer();
    testFlowConservation();
    testSpatialExclusion();
    std::cerr << "OK\n";
  }
  void testTrackingOptimizer() {
    Pattern *pattern1, *pattern2;
    Detection d1, d2, d3;
    std::vector<Transition> edges, result, expected_result;
    std::vector<std::pair<std::pair<Detection *, Detection *>, float> >
        pattern1_scores(0), pattern2_scores(0);

    std::vector<TrackingOptimizer::ConstraintProvider *> constraint_providers(
        0);

    // One
    pattern1_scores.clear();
    pattern1_scores.push_back(mp(mp(&Detection::Source, &d1), 1));
    pattern1_scores.push_back(mp(mp(&d1, &Detection::Sink), 1));
    pattern1 = new HardcodedPattern(Pattern::NormalPattern, pattern1_scores);
    edges.clear();
    for (size_t idx = 0; idx < pattern1_scores.size(); ++idx) {
      edges.push_back(Transition(pattern1_scores[idx].first.first,
                                 pattern1_scores[idx].first.second, pattern1));
    }
    result = TrackingOptimizer(constraint_providers).findOptimalTracking(edges);

    expected_result.clear();
    expected_result.push_back(Transition(&Detection::Source, &d1, pattern1));
    expected_result.push_back(Transition(&d1, &Detection::Sink, pattern1));

    sort(result.begin(), result.end());
    sort(expected_result.begin(), expected_result.end());
    assert(result == expected_result);

    // Two
    pattern1_scores.clear();
    pattern1_scores.push_back(mp(mp(&Detection::Source, &d1), -1));
    pattern1_scores.push_back(mp(mp(&d1, &Detection::Sink), 1));
    pattern1 = new HardcodedPattern(Pattern::NormalPattern, pattern1_scores);
    edges.clear();
    for (size_t idx = 0; idx < pattern1_scores.size(); ++idx) {
      edges.push_back(Transition(pattern1_scores[idx].first.first,
                                 pattern1_scores[idx].first.second, pattern1));
    }
    constraint_providers.clear();
    result = TrackingOptimizer(constraint_providers).findOptimalTracking(edges);

    expected_result.clear();
    expected_result.push_back(Transition(&d1, &Detection::Sink, pattern1));

    sort(result.begin(), result.end());
    sort(expected_result.begin(), expected_result.end());
    assert(result == expected_result);

    // Three
    pattern1_scores.clear();
    pattern1_scores.push_back(mp(mp(&Detection::Source, &d1), 1));
    pattern1_scores.push_back(mp(mp(&d1, &d2), 1));
    pattern1_scores.push_back(mp(mp(&d1, &d3), -1));
    pattern1_scores.push_back(mp(mp(&d2, &Detection::Sink), 1));
    pattern1_scores.push_back(mp(mp(&d3, &Detection::Sink), -1));
    pattern1 = new HardcodedPattern(Pattern::NormalPattern, pattern1_scores);
    pattern2_scores.clear();
    pattern2_scores.push_back(mp(mp(&Detection::Source, &d1), 1));
    pattern2_scores.push_back(mp(mp(&d1, &d2), -1));
    pattern2_scores.push_back(mp(mp(&d1, &d3), 1));
    pattern2_scores.push_back(mp(mp(&d2, &Detection::Sink), -1));
    pattern2_scores.push_back(mp(mp(&d3, &Detection::Sink), 1));
    pattern2 = new HardcodedPattern(Pattern::NormalPattern, pattern2_scores);

    edges.clear();
    for (size_t idx = 0; idx < pattern1_scores.size(); ++idx) {
      edges.push_back(Transition(pattern1_scores[idx].first.first,
                                 pattern1_scores[idx].first.second, pattern1));
    }
    for (size_t idx = 0; idx < pattern2_scores.size(); ++idx) {
      edges.push_back(Transition(pattern2_scores[idx].first.first,
                                 pattern2_scores[idx].first.second, pattern2));
    }

    constraint_providers.clear();
    result = TrackingOptimizer(constraint_providers).findOptimalTracking(edges);

    expected_result.clear();
    expected_result.push_back(Transition(&Detection::Source, &d1, pattern1));
    expected_result.push_back(Transition(&d1, &d2, pattern1));
    expected_result.push_back(Transition(&d2, &Detection::Sink, pattern1));
    expected_result.push_back(Transition(&Detection::Source, &d1, pattern2));
    expected_result.push_back(Transition(&d1, &d3, pattern2));
    expected_result.push_back(Transition(&d3, &Detection::Sink, pattern2));

    sort(result.begin(), result.end());
    sort(expected_result.begin(), expected_result.end());
    assert(result == expected_result);

    // Four
    pattern1_scores.clear();
    pattern1_scores.push_back(mp(mp(&Detection::Source, &d1), 1));
    pattern1_scores.push_back(mp(mp(&d1, &d2), 1));
    pattern1_scores.push_back(mp(mp(&d1, &d3), -1));
    pattern1_scores.push_back(mp(mp(&d2, &Detection::Sink), 1));
    pattern1_scores.push_back(mp(mp(&d3, &Detection::Sink), -1));
    pattern1 = new HardcodedPattern(Pattern::NormalPattern, pattern1_scores);
    pattern2_scores.clear();
    pattern2_scores.push_back(mp(mp(&Detection::Source, &d1), 1));
    pattern2_scores.push_back(mp(mp(&d1, &d2), -1));
    pattern2_scores.push_back(mp(mp(&d1, &d3), 1));
    pattern2_scores.push_back(mp(mp(&d2, &Detection::Sink), -1));
    pattern2_scores.push_back(mp(mp(&d3, &Detection::Sink), 1));
    pattern2 = new HardcodedPattern(Pattern::NormalPattern, pattern2_scores);

    edges.clear();
    for (size_t idx = 0; idx < pattern1_scores.size(); ++idx) {
      edges.push_back(Transition(pattern1_scores[idx].first.first,
                                 pattern1_scores[idx].first.second, pattern1));
    }
    for (size_t idx = 0; idx < pattern2_scores.size(); ++idx) {
      edges.push_back(Transition(pattern2_scores[idx].first.first,
                                 pattern2_scores[idx].first.second, pattern2));
    }

    constraint_providers.clear();
    result = TrackingOptimizer(constraint_providers).findOptimalTracking(edges);

    expected_result.clear();
    expected_result.push_back(Transition(&Detection::Source, &d1, pattern1));
    expected_result.push_back(Transition(&d1, &d2, pattern1));
    expected_result.push_back(Transition(&d2, &Detection::Sink, pattern1));
    expected_result.push_back(Transition(&Detection::Source, &d1, pattern2));
    expected_result.push_back(Transition(&d1, &d3, pattern2));
    expected_result.push_back(Transition(&d3, &Detection::Sink, pattern2));

    sort(result.begin(), result.end());
    sort(expected_result.begin(), expected_result.end());
    assert(result == expected_result);
    //
    std::vector<Transition> hardcoded_constraints;
    hardcoded_constraints.push_back(Transition(&Detection::Source, &d1, pattern2));
    hardcoded_constraints.push_back(Transition(&d1, &d2, pattern2));
    hardcoded_constraints.push_back(Transition(&d2, &Detection::Sink, pattern2));
    constraint_providers.push_back(
        new HardcodedConstraintProvider(hardcoded_constraints));

    result = TrackingOptimizer(constraint_providers).findOptimalTracking(edges);

    expected_result.clear();
    expected_result.push_back(Transition(&Detection::Source, &d1, pattern2));
    expected_result.push_back(Transition(&d1, &d2, pattern2));
    expected_result.push_back(Transition(&d2, &Detection::Sink, pattern2));

    sort(result.begin(), result.end());
    sort(expected_result.begin(), expected_result.end());
    assert(result == expected_result);
  }
  void testFlowConservation() {
    Pattern *pattern;

    Detection d1, d2, d3;

    std::vector<Transition> edges, result, expected_result;
    std::vector<std::pair<std::pair<Detection *, Detection *>, float> >
        pattern_scores(0);

    std::vector<TrackingOptimizer::ConstraintProvider *> constraint_providers(
        0);

    constraint_providers.push_back(new FlowConservationConstraintProvider());

    // One
    pattern_scores.clear();
    pattern_scores.push_back(mp(mp(&Detection::Source, &d1), 1));
    pattern_scores.push_back(mp(mp(&d1, &d2), -1));
    pattern_scores.push_back(mp(mp(&d2, &Detection::Sink), 1));
    pattern = new HardcodedPattern(Pattern::NormalPattern, pattern_scores);

    edges.clear();
    for (size_t idx = 0; idx < pattern_scores.size(); ++idx) {
      edges.push_back(Transition(pattern_scores[idx].first.first,
                                 pattern_scores[idx].first.second, pattern));
    }

    result = TrackingOptimizer(constraint_providers).findOptimalTracking(edges);
    expected_result.clear();
    expected_result.push_back(Transition(&Detection::Source, &d1, pattern));
    expected_result.push_back(Transition(&d1, &d2, pattern));
    expected_result.push_back(Transition(&d2, &Detection::Sink, pattern));

    sort(result.begin(), result.end());
    sort(expected_result.begin(), expected_result.end());
    assert(result == expected_result);

    // Two
    pattern_scores.clear();
    pattern_scores.push_back(mp(mp(&Detection::Source, &d1), -1));
    pattern_scores.push_back(mp(mp(&d1, &d2), -1));
    pattern_scores.push_back(mp(mp(&d2, &Detection::Sink), 1));
    pattern = new HardcodedPattern(Pattern::NormalPattern, pattern_scores);

    edges.clear();
    for (size_t idx = 0; idx < pattern_scores.size(); ++idx) {
      edges.push_back(Transition(pattern_scores[idx].first.first,
                                 pattern_scores[idx].first.second, pattern));
    }

    result = TrackingOptimizer(constraint_providers).findOptimalTracking(edges);
    expected_result.clear();

    sort(result.begin(), result.end());
    sort(expected_result.begin(), expected_result.end());
    assert(result == expected_result);

    // Three
    pattern_scores.clear();
    pattern_scores.push_back(mp(mp(&Detection::Source, &d1), -1));
    pattern_scores.push_back(mp(mp(&d1, &d2), 1));
    pattern_scores.push_back(mp(mp(&d2, &Detection::Sink), 1));
    pattern = new HardcodedPattern(Pattern::NormalPattern, pattern_scores);

    edges.clear();
    for (size_t idx = 0; idx < pattern_scores.size(); ++idx) {
      edges.push_back(Transition(pattern_scores[idx].first.first,
                                 pattern_scores[idx].first.second, pattern));
    }

    result = TrackingOptimizer(constraint_providers).findOptimalTracking(edges);
    expected_result.clear();
    expected_result.push_back(Transition(&Detection::Source, &d1, pattern));
    expected_result.push_back(Transition(&d1, &d2, pattern));
    expected_result.push_back(Transition(&d2, &Detection::Sink, pattern));

    sort(result.begin(), result.end());
    sort(expected_result.begin(), expected_result.end());
    assert(result == expected_result);

    // Four
    pattern_scores.clear();
    pattern_scores.push_back(mp(mp(&Detection::Source, &d1), 3));
    pattern_scores.push_back(mp(mp(&Detection::Source, &d3), 1));
    pattern_scores.push_back(mp(mp(&d1, &d2), 1));
    pattern_scores.push_back(mp(mp(&d1, &d3), 0));
    pattern_scores.push_back(mp(mp(&d2, &Detection::Sink), 1));
    pattern_scores.push_back(mp(mp(&d3, &Detection::Sink), 1));
    pattern = new HardcodedPattern(Pattern::NormalPattern, pattern_scores);

    edges.clear();
    for (size_t idx = 0; idx < pattern_scores.size(); ++idx) {
      edges.push_back(Transition(pattern_scores[idx].first.first,
                                 pattern_scores[idx].first.second, pattern));
    }

    result = TrackingOptimizer(constraint_providers).findOptimalTracking(edges);
    expected_result.clear();
    expected_result.push_back(Transition(&Detection::Source, &d1, pattern));
    expected_result.push_back(Transition(&d1, &d2, pattern));
    expected_result.push_back(Transition(&d2, &Detection::Sink, pattern));
    expected_result.push_back(Transition(&Detection::Source, &d3, pattern));
    expected_result.push_back(Transition(&d3, &Detection::Sink, pattern));

    sort(result.begin(), result.end());
    sort(expected_result.begin(), expected_result.end());
    assert(result == expected_result);

    // Five
    pattern_scores.clear();
    pattern_scores.push_back(mp(mp(&Detection::Source, &d1), 3));
    pattern_scores.push_back(mp(mp(&Detection::Source, &d3), 1));
    pattern_scores.push_back(mp(mp(&d1, &d2), -5));
    pattern_scores.push_back(mp(mp(&d1, &d3), 0));
    pattern_scores.push_back(mp(mp(&d2, &Detection::Sink), 1));
    pattern_scores.push_back(mp(mp(&d3, &Detection::Sink), 1));
    pattern = new HardcodedPattern(Pattern::NormalPattern, pattern_scores);

    edges.clear();
    for (size_t idx = 0; idx < pattern_scores.size(); ++idx) {
      edges.push_back(Transition(pattern_scores[idx].first.first,
                                 pattern_scores[idx].first.second, pattern));
    }

    result = TrackingOptimizer(constraint_providers).findOptimalTracking(edges);
    expected_result.clear();
    expected_result.push_back(Transition(&Detection::Source, &d1, pattern));
    expected_result.push_back(Transition(&d1, &d3, pattern));
    expected_result.push_back(Transition(&d3, &Detection::Sink, pattern));

    sort(result.begin(), result.end());
    sort(expected_result.begin(), expected_result.end());
    assert(result == expected_result);
  }
  void testSpatialExclusion() {
    Pattern *pattern1, *pattern2;
    Detection d1, d2, d3;
    std::vector<Transition> edges, result, expected_result;
    std::vector<std::pair<std::pair<Detection *, Detection *>, float> >
        pattern1_scores(0), pattern2_scores(0);

    std::vector<TrackingOptimizer::ConstraintProvider *> constraint_providers(
        0);

    // One
    pattern1_scores.clear();
    pattern1_scores.push_back(mp(mp(&Detection::Source, &d1), 2));
    pattern1_scores.push_back(mp(mp(&d1, &d2), -1));
    pattern1_scores.push_back(mp(mp(&d2, &Detection::Sink), 2));
    pattern1_scores.push_back(mp(mp(&Detection::Source, &d3), -1));
    pattern1_scores.push_back(mp(mp(&d3, &Detection::Sink), -1));
    pattern1 = new HardcodedPattern(Pattern::NormalPattern, pattern1_scores);
    pattern2_scores.clear();
    pattern2_scores.push_back(mp(mp(&Detection::Source, &d1), 1));
    pattern2_scores.push_back(mp(mp(&d1, &d2), -1));
    pattern2_scores.push_back(mp(mp(&d2, &Detection::Sink), 2));
    pattern2_scores.push_back(mp(mp(&Detection::Source, &d3), 0));
    pattern2_scores.push_back(mp(mp(&d3, &Detection::Sink), -1));
    pattern2 = new HardcodedPattern(Pattern::NormalPattern, pattern2_scores);

    edges.clear();
    for (size_t idx = 0; idx < pattern1_scores.size(); ++idx) {
      edges.push_back(Transition(pattern1_scores[idx].first.first,
                                 pattern1_scores[idx].first.second, pattern1));
    }
    for (size_t idx = 0; idx < pattern2_scores.size(); ++idx) {
      edges.push_back(Transition(pattern2_scores[idx].first.first,
                                 pattern2_scores[idx].first.second, pattern2));
    }

    constraint_providers.clear();
    constraint_providers.push_back(
        new SpatialExclusionConstraintProvider(false));
    constraint_providers.push_back(new FlowConservationConstraintProvider());
    result = TrackingOptimizer(constraint_providers).findOptimalTracking(edges);

    expected_result.clear();
    expected_result.push_back(Transition(&Detection::Source, &d1, pattern1));
    expected_result.push_back(Transition(&d1, &d2, pattern1));
    expected_result.push_back(Transition(&d2, &Detection::Sink, pattern1));

    sort(result.begin(), result.end());
    sort(expected_result.begin(), expected_result.end());
    assert(result == expected_result);

    // Two
    pattern1_scores.clear();
    pattern1_scores.push_back(mp(mp(&Detection::Source, &d1), 2));
    pattern1_scores.push_back(mp(mp(&d1, &d2), -1));
    pattern1_scores.push_back(mp(mp(&d2, &Detection::Sink), 2));
    pattern1_scores.push_back(mp(mp(&Detection::Source, &d3), -1));
    pattern1_scores.push_back(mp(mp(&d3, &Detection::Sink), -1));
    pattern1 = new HardcodedPattern(Pattern::NormalPattern, pattern1_scores);
    pattern2_scores.clear();
    pattern2_scores.push_back(mp(mp(&Detection::Source, &d1), 1));
    pattern2_scores.push_back(mp(mp(&d1, &d2), -1));
    pattern2_scores.push_back(mp(mp(&d2, &Detection::Sink), 2));
    pattern2_scores.push_back(mp(mp(&Detection::Source, &d3), 0));
    pattern2_scores.push_back(mp(mp(&d3, &Detection::Sink), -1));
    pattern2 = new HardcodedPattern(Pattern::NormalPattern, pattern2_scores);

    edges.clear();
    for (size_t idx = 0; idx < pattern1_scores.size(); ++idx) {
      edges.push_back(Transition(pattern1_scores[idx].first.first,
                                 pattern1_scores[idx].first.second, pattern1));
    }
    for (size_t idx = 0; idx < pattern2_scores.size(); ++idx) {
      edges.push_back(Transition(pattern2_scores[idx].first.first,
                                 pattern2_scores[idx].first.second, pattern2));
    }

    constraint_providers.clear();
    constraint_providers.push_back(
        new SpatialExclusionConstraintProvider(true));
    constraint_providers.push_back(new FlowConservationConstraintProvider());
    result = TrackingOptimizer(constraint_providers).findOptimalTracking(edges);

    expected_result.clear();
    expected_result.push_back(Transition(&Detection::Source, &d1, pattern1));
    expected_result.push_back(Transition(&d1, &d2, pattern1));
    expected_result.push_back(Transition(&d2, &Detection::Sink, pattern1));
    expected_result.push_back(Transition(&Detection::Source, &d3, pattern2));
    expected_result.push_back(Transition(&d3, &Detection::Sink, pattern2));

    sort(result.begin(), result.end());
    sort(expected_result.begin(), expected_result.end());
    assert(result == expected_result);
  }
};
#endif  // CODE_INCLUDE_TEST_TRACKINGOPTIMIZERTEST_H_

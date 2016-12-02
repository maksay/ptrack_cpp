/*
 * Copyright 2016 Andrii Maksai
 */
#ifndef CODE_INCLUDE_TEST_METRICTEST_H_
#define CODE_INCLUDE_TEST_METRICTEST_H_
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <utility>
#include <vector>
#include "Detection.h"
#include "Metric.h"
#include "Transformation.h"
class MetricTest {
 public:
  void testAll() {
    std::cerr << "Testing Metric...";
    testScoring();
    std::cerr << "OK\n";
  }
  void testScoring() {
    std::vector<std::vector<Detection *> > trajectories, ground_truth;

    trajectories = Transformation::fileToTrajectories(
        "data/towncentre/towncentre_001_150/KSP/KSP.txt");
    ground_truth = Transformation::fileToTrajectories(
        "data/towncentre/towncentre_001_150/gt/gt.txt");
    assert(fabs(Metric::IDF13D(trajectories, ground_truth, 3.0, NULL) - 0.46) <
           0.01);
  }
};
#endif  // CODE_INCLUDE_TEST_METRICTEST_H_

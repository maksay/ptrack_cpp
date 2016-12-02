/*
 * Copyright 2016 Andrii Maksai
 */
#ifndef CODE_INCLUDE_TEST_SOLVERTEST_H
#define CODE_INCLUDE_TEST_SOLVERTEST_H
#include "Detection.h"
#include "Metric.h"
#include "Pattern.h"
#include "Solver.h"
#include "Transformation.h"

class SolverTest {
 public:
  void testAll() {
    std::cerr << "Testing Solver:\n";
    std::cerr << "  finding patterns...";
    testFindingPatterns();
    std::cerr << "OK\n";
    std::cerr << "  finding tracking...";
    testFindingTracking();
    std::cerr << "OK\n";
    std::cerr << "  iterative solver...";
    testSolvingIterative();
    std::cerr << "OK\n";
  }
  void testFindingPatterns() {
    std::vector<std::vector<Detection *> > input_trajectories;
    std::vector<std::vector<Detection *> > trajectories1 =
        Transformation::fileToTrajectories(
            "data/towncentre/towncentre_151_300/gt/gt.txt");
    std::vector<std::vector<Detection *> > trajectories2 =
        Transformation::fileToTrajectories(
            "data/towncentre/towncentre_301_450/gt/gt.txt");

    input_trajectories.insert(input_trajectories.end(), trajectories1.begin(),
                              trajectories1.end());
    input_trajectories.insert(input_trajectories.end(), trajectories2.begin(),
                              trajectories2.end());

    std::vector<std::pair<std::vector<Detection *>, Pattern *> > assignment =
        Solver::findPatterns(
            input_trajectories,
            Parameters::get_float("Training.PatternMetricBound"));
    std::vector<Pattern *> patterns =
        Transformation::assignmentToPatterns(assignment);
    std::vector<Pattern *> expected_patterns =
        Transformation::fileToPatterns("resources/expected.patterns");
    assert(patterns.size() == expected_patterns.size());
    for (size_t idx = 0; idx < patterns.size(); ++idx) {
      CenterlineWithWidthPattern *pat =
          dynamic_cast<CenterlineWithWidthPattern *>(patterns[idx]);
      CenterlineWithWidthPattern *expat =
          dynamic_cast<CenterlineWithWidthPattern *>(expected_patterns[idx]);
      assert(pat->points.size() == expat->points.size());
      for (size_t pos = 0; pos < pat->points.size(); ++pos) {
        assert(fabs(pat->points[pos].first - expat->points[pos].first) < 1e-6);
        assert(fabs(pat->points[pos].second - expat->points[pos].second) <
               1e-6);
      }
    }
  }
  void testFindingTracking() {
    std::vector<std::vector<Detection *> > input_trajectories =
        Transformation::fileToTrajectories(
            "data/towncentre/towncentre_001_150/KSP/KSP.txt");
    std::vector<Pattern *> patterns =
        Transformation::fileToPatterns("resources/expected.patterns");

    std::vector<std::pair<std::vector<Detection *>, Pattern *> > assignment =
        Solver::findTracking(input_trajectories, patterns);
    std::vector<std::vector<Detection *> > trajectories =
        Transformation::assignmentToTrajectories(assignment);

    std::vector<std::vector<Detection *> > gt_trajectories =
        Transformation::fileToTrajectories(
            "data/towncentre/towncentre_001_150/gt/gt.txt");

    std::vector<std::vector<Detection *> > expected_trajectories =
        Transformation::fileToTrajectories("resources/expected.trajectories");

    float metric_val =
        Metric::IDF13D(trajectories, gt_trajectories,
                       Parameters::get_float("Parameters.D_match"), NULL);
    float expected_metric_val =
        Metric::IDF13D(expected_trajectories, gt_trajectories,
                       Parameters::get_float("Parameters.D_match"), NULL);

    assert(fabs(expected_metric_val - metric_val) < 0.01);
  }
  void testSolvingIterative() {
    std::vector<std::vector<Detection *> > input_trajectories =
        Transformation::fileToTrajectories(
            "data/towncentre/towncentre_001_150/KSP/KSP.txt");

    std::vector<std::pair<std::vector<Detection *>, Pattern *> > assignment =
        Solver::solveIterative(input_trajectories);
    std::vector<std::vector<Detection *> > trajectories =
        Transformation::assignmentToTrajectories(assignment);

    std::vector<std::vector<Detection *> > gt_trajectories =
        Transformation::fileToTrajectories(
            "data/towncentre/towncentre_001_150/gt/gt.txt");

    std::vector<std::vector<Detection *> > expected_trajectories =
        Transformation::fileToTrajectories("resources/expected.unsupervised");

    float metric_val =
        Metric::IDF13D(trajectories, gt_trajectories,
                       Parameters::get_float("Parameters.D_match"), NULL);
    float expected_metric_val =
        Metric::IDF13D(expected_trajectories, gt_trajectories,
                       Parameters::get_float("Parameters.D_match"), NULL);

    assert(fabs(expected_metric_val - metric_val) < 0.1);
  }
};
#endif  // CODE_INCLUDE_TEST_SOLVERTEST_H

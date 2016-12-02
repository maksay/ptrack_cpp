/*
 * Copyright 2016 Andrii Maksai
 */
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "CenterlineWithWidthPattern.h"
#include "Detection.h"
#include "FlowConservationConstraintProvider.h"
#include "GroundPlaneTransitionProvider.h"
#include "Metric.h"
#include "Parameters.h"
#include "Pattern.h"
#include "PatternOptimizer.h"
#include "Solver.h"
#include "SpatialExclusionConstraintProvider.h"
#include "TrackingOptimizer.h"
#include "Transformation.h"
#include "Transition.h"
#include "TransitionProvider.h"
int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: ptrack <config_file.cfg>\n";
    exit(0);
  }
  Parameters::read_config(argv[1]);

  if (Parameters::get_string("Mode") == "Training") {
    std::vector<std::string> inputFiles =
        Parameters::get_string_list("GTTrajectories");
    std::vector<std::vector<Detection *> > input_trajectories;
    for (size_t idx = 0; idx < inputFiles.size(); ++idx) {
      std::vector<std::vector<Detection *> > trajectories =
          Transformation::fileToTrajectories(inputFiles[idx]);
      input_trajectories.insert(input_trajectories.end(), trajectories.begin(),
                                trajectories.end());
    }
    std::vector<std::pair<std::vector<Detection *>, Pattern *> > assignment =
        Solver::findPatterns(
            input_trajectories,
            Parameters::get_float("Training.PatternMetricBound"));
    std::vector<Pattern *> patterns =
        Transformation::assignmentToPatterns(assignment);
    if (Parameters::get_string("OutputPatterns").size() > 0) {
      Transformation::patternsToFile(patterns,
                                     Parameters::get_string("OutputPatterns"));
    }
    if (Parameters::get_string("FullResult").size() > 0) {
      Transformation::assignmentToFile(assignment,
                                       Parameters::get_string("FullResult"));
    }
  }

  if (Parameters::get_string("Mode") == "Testing") {
    std::vector<std::vector<Detection *> > input_trajectories =
        Transformation::fileToTrajectories(
            Parameters::get_string("InputTrajectories"));
    std::vector<Pattern *> patterns =
        Transformation::fileToPatterns(Parameters::get_string("InputPatterns"));

    std::vector<std::pair<std::vector<Detection *>, Pattern *> > assignment =
        Solver::findTracking(input_trajectories, patterns);

    std::vector<std::vector<Detection *> > output_trajectories =
        Transformation::assignmentToTrajectories(assignment);

    if (Parameters::get_string("OutputTrajectories").size() > 0) {
      Transformation::trajectoriesToFile(
          output_trajectories, Parameters::get_string("OutputTrajectories"));
    }
    if (Parameters::get_string("FullResult").size() > 0) {
      Transformation::assignmentToFile(assignment,
                                       Parameters::get_string("FullResult"));
    }
    if (Parameters::get_string("GTTrajectories").size() > 0 &&
        Parameters::get_int("Tracking.Stderr")) {
      std::vector<std::vector<Detection *> > gt_trajectories =
          Transformation::fileToTrajectories(
              Parameters::get_string("GTTrajectories"));
      std::cerr << "Original IDF1: "
                << Metric::IDF13D(input_trajectories, gt_trajectories,
                                  Parameters::get_float("Parameters.D_match"),
                                  NULL)
                << std::endl;
      std::cerr << "Our IDF1: "
                << Metric::IDF13D(output_trajectories, gt_trajectories,
                                  Parameters::get_float("Parameters.D_match"),
                                  NULL)
                << std::endl;
    }
  }

  if (Parameters::get_string("Mode") == "Unsupervised") {
    std::vector<std::vector<Detection *> > input_trajectories =
        Transformation::fileToTrajectories(
            Parameters::get_string("InputTrajectories"));

    std::vector<std::pair<std::vector<Detection *>, Pattern *> > assignment =
        Solver::solveIterative(input_trajectories);

    std::vector<std::vector<Detection *> > output_trajectories =
        Transformation::assignmentToTrajectories(assignment);
    std::vector<Pattern *> output_patterns =
        Transformation::assignmentToPatterns(assignment);

    if (Parameters::get_string("OutputTrajectories").size() > 0) {
      Transformation::trajectoriesToFile(
          output_trajectories, Parameters::get_string("OutputTrajectories"));
    }
    if (Parameters::get_string("OutputPatterns").size() > 0) {
      Transformation::patternsToFile(output_patterns,
                                     Parameters::get_string("OutputPatterns"));
    }
    if (Parameters::get_string("FullResult").size() > 0) {
      Transformation::assignmentToFile(assignment,
                                       Parameters::get_string("FullResult"));
    }
    if (Parameters::get_string("GTTrajectories").size() > 0 &&
        Parameters::get_int("Unsupervised.Stderr")) {
      std::vector<std::vector<Detection *> > gt_trajectories =
          Transformation::fileToTrajectories(
              Parameters::get_string("GTTrajectories"));
      std::cerr << "Original IDF1: "
                << Metric::IDF13D(input_trajectories, gt_trajectories,
                                  Parameters::get_float("Parameters.D_match"),
                                  NULL)
                << std::endl;
      std::cerr << "Our IDF1: "
                << Metric::IDF13D(output_trajectories, gt_trajectories,
                                  Parameters::get_float("Parameters.D_match"),
                                  NULL)
                << std::endl;
    }
  }
}

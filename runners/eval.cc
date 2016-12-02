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
int main() {
  Parameters::read_config("resources/eval.cfg");

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
  std::vector<Pattern *> patterns =
      Transformation::assignmentToPatterns(Solver::findPatterns(
          input_trajectories,
          Parameters::get_float("Training.PatternMetricBound")));

  std::vector<std::string> inputs;
  inputs.push_back("CNNTCM");
  inputs.push_back("EAMTT");
  inputs.push_back("JointMC");
  inputs.push_back("KSP");
  inputs.push_back("MDP");
  inputs.push_back("MHT_DAM");
  inputs.push_back("NOMTwSDP");
  inputs.push_back("SCEA");
  inputs.push_back("SORT");
  inputs.push_back("TDAM");
  inputs.push_back("TSML_CDE");
  inputs.push_back("RNN");

  std::vector<std::vector<Detection *> > gt_trajectories =
      Transformation::fileToTrajectories(
          "data/towncentre/towncentre_001_150/gt/gt.txt");

  system("clear");

  std::cerr.width(20);
  std::cerr << "Method";

  std::cerr.width(20);
  std::cerr << "IDF original";

  std::cerr.width(20);
  std::cerr << "IDF our";

  std::cerr.width(20);
  std::cerr << "IDF unsupervised" << std::endl;
  for (size_t idx = 0; idx < inputs.size(); ++idx) {

    std::vector<std::vector<Detection *> > input_trajectories =
        Transformation::fileToTrajectories(
            "data/towncentre/towncentre_001_150/" + inputs[idx] + "/" +
                inputs[idx] + ".txt");

    std::cerr.width(20);
    std::cerr << inputs[idx];
    std::cerr.width(20);
    std::cerr << std::setprecision(2) << std::fixed
              << Metric::IDF13D(input_trajectories, gt_trajectories,
                                Parameters::get_float("Parameters.D_match"),
                                NULL);

    std::vector<std::vector<Detection *> > output_trajectories =
        Transformation::assignmentToTrajectories(
            Solver::findTracking(input_trajectories, patterns));

    Transformation::trajectoriesToFile(
        output_trajectories, "output/" + inputs[idx] + "_our_supervised.txt");

        std::cerr.width(20);
    std::cerr << std::setprecision(2) << std::fixed
              << Metric::IDF13D(output_trajectories, gt_trajectories,
                                Parameters::get_float("Parameters.D_match"),
                                NULL);

    std::vector<std::vector<Detection *> > unsupervised_trajectories =
        Transformation::assignmentToTrajectories(
            Solver::solveIterative(input_trajectories));

    Transformation::trajectoriesToFile(
        unsupervised_trajectories,
        "output/" + inputs[idx] + "_our_unsupervised.txt");

        std::cerr.width(20);
    std::cerr << std::setprecision(2) << std::fixed
              << Metric::IDF13D(unsupervised_trajectories, gt_trajectories,
                                Parameters::get_float("Parameters.D_match"),
                                NULL);

    std::cerr << std::endl;
  }
}

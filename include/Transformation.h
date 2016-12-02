/*
 * Copyright 2016 Andrii Maksai
 */
#ifndef CODE_INCLUDE_TRANSFORMATION_H_
#define CODE_INCLUDE_TRANSFORMATION_H_
#include <string>
#include <utility>
#include <vector>
#include "Detection.h"
#include "Transition.h"
class Transformation {
  Transformation();

 public:
  // Writing to files
  static void trajectoriesToFile(
      std::vector<std::vector<Detection *> > trajectories, std::string fpath);
  static void patternsToFile(std::vector<Pattern *> patterns,
                             std::string fpath);
  static void assignmentToFile(
      std::vector<std::pair<std::vector<Detection *>, Pattern *> > assignment,
      std::string fpath);
  // Reading from files
  static std::vector<std::vector<Detection *> > fileToTrajectories(
      std::string fpath, bool split_discontinuous_trajectories = false);
  static std::vector<Pattern *> fileToPatterns(std::string fpath);
  // Various data transformations
  static std::vector<Pattern *> assignmentToPatterns(
      std::vector<std::pair<std::vector<Detection *>, Pattern *> > assignment);
  static std::vector<std::vector<Detection *> > assignmentToTrajectories(
      std::vector<std::pair<std::vector<Detection *>, Pattern *> > assignment);
  static std::vector<Detection *> trajectoriesToDetections(
      std::vector<std::vector<Detection *> > trajectories);
  static std::vector<std::vector<Detection *> > transitionsToTrajectories(
      std::vector<Transition> transitions, bool ignore_no_pattern,
      std::vector<std::pair<std::vector<Detection *>, Pattern *> > *assignment);
  static std::vector<std::vector<Detection *> >
  trajectoriesToContinuousTrajectories(
      std::vector<std::vector<Detection *> > trajectories);
  static void splitTrajectoriesInTwo(
      std::vector<std::vector<Detection *> > trajectories,
      std::vector<std::vector<Detection *> > *left,
      std::vector<std::vector<Detection *> > *right);
};
#endif  // CODE_INCLUDE_TRANSFORMATION_H_

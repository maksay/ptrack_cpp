/*
 * Copyright 2016 Andrii Maksai
 */
#ifndef CODE_INCLUDE_SOLVER_H_
#define CODE_INCLUDE_SOLVER_H_
#include <string>
#include <utility>
#include <vector>
#include "Detection.h"
#include "Pattern.h"
class Solver {
  Solver();

 public:
  static std::vector<std::pair<std::vector<Detection *>, Pattern *> >
  findTracking(std::vector<std::vector<Detection *> > trajectories,
               std::vector<Pattern *> patterns);
  static std::vector<std::pair<std::vector<Detection *>, Pattern *> >
  findPatterns(std::vector<std::vector<Detection *> > trajectories,
               float pattern_metric_bound);
  static std::vector<std::pair<std::vector<Detection *>, Pattern *> >
  solveIterative(std::vector<std::vector<Detection *> > trajectories);
};
#endif  // CODE_INCLUDE_SOLVER_H_

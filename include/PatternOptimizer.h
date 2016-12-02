/*
 * Copyright 2016 Andrii Maksai
 */
#ifndef CODE_INCLUDE_PATTERNOPTIMIZER_H_
#define CODE_INCLUDE_PATTERNOPTIMIZER_H_
#include <utility>
#include <vector>
#include "Pattern.h"
#include "Detection.h"

class PatternOptimizer {
 public:
  PatternOptimizer();
  std::vector<Pattern *> findMinimalPatternsWithPositiveScore(
      std::vector<Pattern *> possible_patterns,
      std::vector<std::vector<Detection *> > trajectories,
      std::vector <
          std::pair<std::vector<Detection *>, Pattern *> > * assignment);
  std::vector<Pattern *> findBestPatternsBoundByMetric(
      std::vector<Pattern *> possible_patterns,
      std::vector<std::vector<Detection *> > trajectories,
      float metric_bound,
      std::vector <
          std::pair<std::vector<Detection *>, Pattern *> > * assignment);
};
#endif  // CODE_INCLUDE_PATTERNOPTIMIZER_H_

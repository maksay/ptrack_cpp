/*
 * Copyright 2016 Andrii Maksai
 */
#ifndef CODE_INCLUDE_METRIC_H
#define CODE_INCLUDE_METRIC_H
#include <vector>
#include <utility>
#include "Detection.h"
class Metric {
  Metric();
  public:
  static float IDF13D(std::vector<std::vector<Detection *> > trajectories,
      std::vector<std::vector<Detection * > > ground_truth,
      float D, std::vector<std::pair<int, int> > *assignment);
};
#endif  // CODE_INCLUDE_METRIC_H

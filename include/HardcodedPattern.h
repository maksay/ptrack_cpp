/*
 * Copyright 2016 Andrii Maksai
 */
#ifndef CODE_INCLUDE_HARDCODEDPATTERN_H_
#define CODE_INCLUDE_HARDCODEDPATTERN_H_
#include <utility>
#include <vector>
#include "Detection.h"
#include "Pattern.h"
class HardcodedPattern : public Pattern {
  std::vector<std::pair<std::pair<Detection*, Detection*>, float> >
      hardcoded_scores;
  float hardcoded_area;

 public:
  HardcodedPattern(
      Pattern::PatternType _pattern_type,
      std::vector<std::pair<std::pair<Detection*, Detection*>, float> >
          _hardcoded_scores,
      float _hardcoded_area = 0);
  virtual float score(Detection* node_start, Detection* node_end);
  virtual float area();
};
#endif  // CODE_INCLUDE_HARDCODEDPATTERN_H_

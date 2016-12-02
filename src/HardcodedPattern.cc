/*
 * Copyright 2016 Andrii Maksai
 */
#include "HardcodedPattern.h"
#include <cassert>
HardcodedPattern::HardcodedPattern(
    Pattern::PatternType _pattern_type,
    std::vector<std::pair<std::pair<Detection*, Detection*>, float> >
        _hardcoded_scores,
    float _hardcoded_area)
    : Pattern(_pattern_type),
      hardcoded_scores(_hardcoded_scores),
      hardcoded_area(_hardcoded_area) {}

float HardcodedPattern::score(Detection* node_start, Detection* node_end) {
  for (size_t idx = 0; idx < hardcoded_scores.size(); ++idx) {
    if (hardcoded_scores[idx].first.first == node_start &&
        hardcoded_scores[idx].first.second == node_end) {
      return hardcoded_scores[idx].second;
    }
  }
  assert(1 > 2);
}

float HardcodedPattern::area() { return hardcoded_area; }

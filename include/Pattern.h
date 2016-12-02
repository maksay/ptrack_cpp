/*
 * Copyright 2016 Andrii Maksai
 */
#ifndef CODE_INCLUDE_PATTERN_H_
#define CODE_INCLUDE_PATTERN_H_
#include <vector>
#include <string>
#include "Detection.h"
#include "Parameters.h"
class Pattern {
 public:
  enum PatternType { NormalPattern, NoPattern };
  PatternType pattern_type;
  Pattern(PatternType _pattern_type) : pattern_type(_pattern_type) {}
  virtual float score(Detection* node_start, Detection* node_end) = 0;
  virtual float area() {return 0;}
  virtual std::string to_string() {return "";}
  virtual float score_trajectory(std::vector<Detection*> trajectory) {
    if (trajectory.size() == 0) { return 0.; }
    float ret = 0.;
    ret += score(&Detection::Source, trajectory[0]);
    for (size_t idx = 0; idx < trajectory.size() - 1; ++idx) {
      ret += score(trajectory[idx], trajectory[idx + 1]);
    }
    ret += score(trajectory.back(), &Detection::Sink);
    return ret;
  }
};
#endif  // CODE_INCLUDE_PATTERN_H_

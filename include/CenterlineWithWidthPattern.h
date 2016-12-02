/*
 * Copyright 2016 Andrii Maksai
 */
#ifndef CODE_INCLUDE_CENTERLINEWITHWIDTHPATTERN_H_
#define CODE_INCLUDE_CENTERLINEWITHWIDTHPATTERN_H_
#include <utility>
#include <vector>
#include <string>
#include "Detection.h"
#include "Pattern.h"
class CenterlineWithWidthPattern : public Pattern {
  friend class CenterlineWithWidthPatternTest;
  friend class SolverTest;
  friend class TransformationTest;

  std::pair<size_t, float> projection_point_idx(Detection* node);
  float pattern_length_between(size_t start_idx, size_t end_idx);
  std::pair<float, float> mn_function(Detection* node_start,
                                      Detection* node_end);

  std::vector<std::pair<float, float> > points;
  float width;
  float alpha;
  float epsilon;
  float epsilon_no_pattern;
 public:
  virtual float area();
  virtual std::string to_string();
  CenterlineWithWidthPattern(std::string pattern_string);
  CenterlineWithWidthPattern(PatternType _pattern_type,
                             std::vector<std::pair<float, float> > _points,
                             float _width, float _alpha, float _epsilon,
                             float _epsilon_no_pattern);
  virtual float score(Detection* node_start, Detection* node_end);
  float& get_alpha();
  float& get_epsilon_no_pattern();
  float& get_epsilon();
};
#endif  // CODE_INCLUDE_CENTERLINEWITHWIDTHPATTERN_H_

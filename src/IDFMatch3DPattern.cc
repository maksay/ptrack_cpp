/*
 * Copyright 2016 Andrii Maksai
 */
#include "IDFMatch3DPattern.h"
#include <cassert>
#include <cmath>
IDFMatch3DPattern::IDFMatch3DPattern(PatternType _pattern_type,
                                     std::vector<Detection*> _trajectory,
                                     float _D)
    : Pattern(_pattern_type), trajectory(_trajectory), D(_D) {}
float IDFMatch3DPattern::score(Detection* node_start, Detection* node_end) {
  if (pattern_type == Pattern::NoPattern) return 0;
  if (node_end->node_type != Detection::NormalNode) return 0;
  assert(node_start->node_type == Detection::SourceNode ||
         node_start->frame_num + 1 == node_end->frame_num);
  for (size_t idx = 0; idx < trajectory.size(); ++idx) {
    if (trajectory[idx]->frame_num == node_end->frame_num) {
      if (hypot(node_end->x - trajectory[idx]->x,
                node_end->y - trajectory[idx]->y) <= D) {
        return 1;
      } else {
        return 0;
      }
    }
  }
  return 0;
}

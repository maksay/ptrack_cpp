/* * Copyright 2016 Andrii Maksai
 */
#ifndef CODE_INCLUDE_IDFMATCH3DPATTERN_H_
#define CODE_INCLUDE_IDFMATCH3DPATTERN_H_
#include <utility>
#include <vector>
#include "Detection.h"
#include "Pattern.h"
class IDFMatch3DPattern : public Pattern {
 std::vector<Detection*> trajectory;
 float D;
 public:
  IDFMatch3DPattern(PatternType _pattern_type,
                    std::vector<Detection*> _trajectory, float D);
  virtual float score(Detection* node_start, Detection* node_end);
};
#endif  // CODE_INCLUDE_IDFMATCH3DPATTERN_H_

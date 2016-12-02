/*
 * Copyright 2016 Andrii Maksai
 */
#ifndef CODE_INCLUDE_TRANSITION_H_
#define CODE_INCLUDE_TRANSITION_H_
#include "Detection.h"
#include "Pattern.h"
struct Transition {
  Detection *start;
  Detection *end;
  Pattern *pattern;
  Transition() : start(NULL), end(NULL), pattern(NULL) {}
  Transition(Detection *_start, Detection *_end, Pattern *_pattern)
      : start(_start), end(_end), pattern(_pattern) {}
  bool operator<(const Transition &other) const {
    if (start != other.start) return start < other.start;
    if (end != other.end) return end < other.end;
    return pattern < other.pattern;
  }
  bool operator==(const Transition &other) const {
    return start == other.start && end == other.end && pattern == other.pattern;
  }
};
#endif  // CODE_INCLUDE_TRANSITION_H_

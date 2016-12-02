/*
 * Copyright 2016 Andrii Maksai
 */
#ifndef CODE_INCLUDE_TRANSITIONPROVIDER_H_
#define CODE_INCLUDE_TRANSITIONPROVIDER_H_
#include <utility>
#include <vector>
#include "Detection.h"
#include "Pattern.h"
#include "Transition.h"
class TransitionProvider {
 public:
  TransitionProvider() {}
  virtual std::vector<Transition> getTransitions(
      std::vector<Detection *> detections, Pattern *pattern) = 0;
};
#endif  // CODE_INCLUDE_TRANSITIONPROVIDER_H_

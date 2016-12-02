/*
 * Copyright 2016 Andrii Maksai
 */
#ifndef CODE_INCLUDE_GROUNDPLANETRANSITIONPROVIDER_H_
#define CODE_INCLUDE_GROUNDPLANETRANSITIONPROVIDER_H_
#include <utility>
#include <vector>
#include "TransitionProvider.h"
class GroundPlaneTransitionProvider : public TransitionProvider {
 private:
  float D_1;
  float D_2;
  float D_t;

 public:
  GroundPlaneTransitionProvider(float _D_1, float _D_2, float _D_t);
  virtual std::vector<Transition> getTransitions(
      std::vector<Detection *> detections, Pattern *pattern);
};
#endif  // CODE_INCLUDE_GROUNDPLANETRANSITIONPROVIDER_H_

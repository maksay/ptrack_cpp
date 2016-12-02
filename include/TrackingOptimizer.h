/*
 * Copyright 2016 Andrii Maksai
 */
#ifndef CODE_INCLUDE_TRACKINGOPTIMIZER_H_
#define CODE_INCLUDE_TRACKINGOPTIMIZER_H_
#include <utility>
#include <vector>
#include "Transition.h"
#include "gurobi_c++.h"

class TrackingOptimizer {
 public:

  class ConstraintProvider {
   public:
    ConstraintProvider();
    virtual void provideConstraints(
        std::vector<std::pair<Transition, GRBVar*> > tracking_variables,
        GRBModel* model) = 0;
  };

 private:
  std::vector<ConstraintProvider*> constraint_providers;

 public:
  explicit TrackingOptimizer(
      std::vector<ConstraintProvider*> _constraint_providers);
  std::vector<Transition> findOptimalTracking(
      std::vector<Transition> transitions);
};
#endif  // CODE_INCLUDE_TRACKINGOPTIMIZER_H_

/*
 * Copyright 2016 Andrii Maksai
 */
#ifndef CODE_INCLUDE_HARDCODEDCONSTRAINTPROVIDER_H_
#define CODE_INCLUDE_HARDCODEDCONSTRAINTPROVIDER_H_
#include <utility>
#include <vector>
#include "Detection.h"
#include "Parameters.h"
#include "Pattern.h"
#include "TrackingOptimizer.h"
class HardcodedConstraintProvider
    : public TrackingOptimizer::ConstraintProvider {
  std::vector<Transition> hardcoded_constraints;

 public:
  explicit HardcodedConstraintProvider(
      std::vector<Transition> _hardcoded_constraints);
  virtual void provideConstraints(
      std::vector<std::pair<Transition, GRBVar*> > tracking_variables,
      GRBModel* model);
};
#endif  // CODE_INCLUDE_HARDCODEDCONSTRAINTPROVIDER_H_

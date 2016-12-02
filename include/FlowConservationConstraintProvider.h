/*
 * Copyright 2016 Andrii Maksai
 */
#ifndef CODE_INCLUDE_FLOWCONSERVATIONCONSTRAINTPROVIDER_H_
#define CODE_INCLUDE_FLOWCONSERVATIONCONSTRAINTPROVIDER_H_
#include <utility>
#include <vector>
#include "Detection.h"
#include "Parameters.h"
#include "Pattern.h"
#include "TrackingOptimizer.h"
class FlowConservationConstraintProvider
    : public TrackingOptimizer::ConstraintProvider {
 public:
  FlowConservationConstraintProvider();
  virtual void provideConstraints(
      std::vector<std::pair<Transition, GRBVar *> > tracking_variables,
      GRBModel *model);
};
#endif  // CODE_INCLUDE_FLOWCONSERVATIONCONSTRAINTPROVIDER_H_

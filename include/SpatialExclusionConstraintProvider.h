/*
 * Copyright 2016 Andrii Maksai
 */
#ifndef CODE_INCLUDE_SPATIALEXCLUSIONCONSTRAINTPROVIDER_H_
#define CODE_INCLUDE_SPATIALEXCLUSIONCONSTRAINTPROVIDER_H_
#include <utility>
#include <vector>
#include "Detection.h"
#include "Parameters.h"
#include "Pattern.h"
#include "TrackingOptimizer.h"
class SpatialExclusionConstraintProvider
    : public TrackingOptimizer::ConstraintProvider {
 public:
  bool always_choose_every_node;
  explicit SpatialExclusionConstraintProvider(bool _always_choose_every_node);
  virtual void provideConstraints(
      std::vector<std::pair<Transition, GRBVar * > >
          tracking_variables,
      GRBModel *model);
};
#endif  // CODE_INCLUDE_SPATIALEXCLUSIONCONSTRAINTPROVIDER_H_

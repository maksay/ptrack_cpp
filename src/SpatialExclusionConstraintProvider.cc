/*
 * Copyright 2016 Andrii Maksai
 */
#include "SpatialExclusionConstraintProvider.h"
#include <algorithm>
SpatialExclusionConstraintProvider::SpatialExclusionConstraintProvider(
    bool _always_choose_every_node)
    : always_choose_every_node(_always_choose_every_node) {}
void SpatialExclusionConstraintProvider::provideConstraints(
    std::vector<std::pair<Transition, GRBVar *> > tracking_variables,
    GRBModel *model) {
  struct sort_comparator {
    static bool sort_by_first_node(std::pair<Transition, GRBVar *> one,
                                   std::pair<Transition, GRBVar *> two) {
      return one.first.start < two.first.start;
    }
  };

  std::sort(tracking_variables.begin(), tracking_variables.end(),
            sort_comparator::sort_by_first_node);

  for (size_t variable_idx = 0; variable_idx < tracking_variables.size();) {
    if (tracking_variables[variable_idx].first.start->node_type ==
        Detection::SourceNode) {
      ++variable_idx;
      continue;
    }
    size_t index_sorted_by_first_node = variable_idx;
    GRBLinExpr lhs;
    while (variable_idx < tracking_variables.size() &&
           !sort_comparator::sort_by_first_node(
               tracking_variables[index_sorted_by_first_node],
               tracking_variables[variable_idx])) {
      lhs += *(tracking_variables[variable_idx].second);
      ++variable_idx;
    }
    if (always_choose_every_node) {
      model->addConstr(lhs, GRB_EQUAL, 1);
    } else {
      model->addConstr(lhs, GRB_LESS_EQUAL, 1);
    }
  }
}

/*
 * Copyright 2016 Andrii Maksai
 */
#include "FlowConservationConstraintProvider.h"
#include <algorithm>
FlowConservationConstraintProvider::FlowConservationConstraintProvider() {}

void FlowConservationConstraintProvider::provideConstraints(
    std::vector<std::pair<Transition, GRBVar *> > tracking_variables,
    GRBModel *model) {
  struct sort_comparator {
    static bool sort_by_pattern_by_first_node(
        std::pair<Transition, GRBVar *> one,
        std::pair<Transition, GRBVar *> two) {
      if (one.first.pattern != two.first.pattern)
        return one.first.pattern < two.first.pattern;
      return one.first.start < two.first.start;
    }
    static bool sort_by_pattern_by_second_node(
        std::pair<Transition, GRBVar *> one,
        std::pair<Transition, GRBVar *> two) {
      if (one.first.pattern != two.first.pattern)
        return one.first.pattern < two.first.pattern;
      return one.first.end < two.first.end;
    }
  };

  std::vector<std::pair<Transition, GRBVar *> > tracking_variables_copy =
      tracking_variables;

  std::sort(tracking_variables.begin(), tracking_variables.end(),
            sort_comparator::sort_by_pattern_by_first_node);
  std::sort(tracking_variables_copy.begin(), tracking_variables_copy.end(),
            sort_comparator::sort_by_pattern_by_second_node);

  for (size_t variable_idx = 0; variable_idx < tracking_variables.size();) {
    if (tracking_variables[variable_idx].first.start->node_type ==
        Detection::SourceNode) {
      ++variable_idx;
      continue;
    }

    size_t index_sorted_by_first_node = variable_idx;
    std::pair<Transition, GRBVar *> tmp = std::make_pair(
        Transition(NULL, tracking_variables[variable_idx].first.start,
                   tracking_variables[variable_idx].first.pattern),
        tracking_variables[variable_idx].second);
    size_t variable_idx_in_copy =
        lower_bound(tracking_variables_copy.begin(),
                    tracking_variables_copy.end(), tmp,
                    sort_comparator::sort_by_pattern_by_second_node) -
        tracking_variables_copy.begin();
    assert(variable_idx_in_copy >= 0 &&
           variable_idx_in_copy < tracking_variables_copy.size());
    assert(tracking_variables_copy[variable_idx_in_copy].first.end ==
           tracking_variables[variable_idx].first.start);
    assert(tracking_variables_copy[variable_idx_in_copy].first.pattern =
               tracking_variables[variable_idx].first.pattern);
    size_t index_sorted_by_second_node = variable_idx_in_copy;

    GRBLinExpr lhs, rhs;
    while (variable_idx < tracking_variables.size() &&
           !sort_comparator::sort_by_pattern_by_first_node(
               tracking_variables[index_sorted_by_first_node],
               tracking_variables[variable_idx])) {
      lhs += *(tracking_variables[variable_idx].second);
      ++variable_idx;
    }
    while (variable_idx_in_copy < tracking_variables_copy.size() &&
           !sort_comparator::sort_by_pattern_by_second_node(
               tracking_variables_copy[index_sorted_by_second_node],
               tracking_variables_copy[variable_idx_in_copy])) {
      rhs += *(tracking_variables_copy[variable_idx_in_copy].second);
      ++variable_idx_in_copy;
    }
    model->addConstr(lhs, GRB_EQUAL, rhs);
  }
}

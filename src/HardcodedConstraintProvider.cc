/*
 * Copyright 2016 Andrii Maksai
 */
#include "HardcodedConstraintProvider.h"
HardcodedConstraintProvider::HardcodedConstraintProvider(
    std::vector<Transition> _hardcoded_constraints)
    : hardcoded_constraints(_hardcoded_constraints) {}

void HardcodedConstraintProvider::provideConstraints(
    std::vector<std::pair<Transition, GRBVar *> > tracking_variables,
    GRBModel *model) {
  for (size_t variable_idx = 0; variable_idx < tracking_variables.size();
       ++variable_idx) {
    bool present = false;
    for (size_t constr_idx = 0; constr_idx < hardcoded_constraints.size();
         ++constr_idx) {
      if (tracking_variables[variable_idx].first.start ==
              hardcoded_constraints[constr_idx].start &&
          tracking_variables[variable_idx].first.end ==
              hardcoded_constraints[constr_idx].end &&
          tracking_variables[variable_idx].first.pattern ==
              hardcoded_constraints[constr_idx].pattern) {
        present = true;
        break;
      }
    }
    model->addConstr(*(tracking_variables[variable_idx].second), GRB_EQUAL,
                     ((present) ? (1) : (0)));
  }
}

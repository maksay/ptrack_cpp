/*
 * Copyright 2016 Andrii Maksai
 */
#include "TrackingOptimizer.h"
#include <algorithm>
#include <cassert>
#include "Parameters.h"
#include "gurobi_c++.h"
TrackingOptimizer::ConstraintProvider::ConstraintProvider() {}

TrackingOptimizer::TrackingOptimizer(
    std::vector<ConstraintProvider *> _constraint_providers)
    : constraint_providers(_constraint_providers) {}

std::vector<Transition> TrackingOptimizer::findOptimalTracking(
    std::vector<Transition> edges) {
  GRBEnv *env = new GRBEnv();
  GRBModel model = GRBModel(*env);

  vector<std::pair<Transition, GRBVar *> > tracking_variables(0);

  char *type = new char[edges.size()];
  for (size_t idx = 0; idx < edges.size(); ++idx) {
    type[idx] = GRB_BINARY;
  }
  GRBVar *vars = model.addVars(NULL, NULL, NULL, type, NULL, edges.size());

  for (size_t idx = 0; idx < edges.size(); ++idx) {
    tracking_variables.push_back(std::make_pair(edges[idx], vars + idx));
  }
  model.update();

  for (size_t idx = 0; idx < constraint_providers.size(); ++idx) {
    constraint_providers[idx]->provideConstraints(tracking_variables, &model);
  }
  model.update();

  double *objcoefs = new double[tracking_variables.size()];
  for (size_t idx = 0; idx < edges.size(); ++idx) {
    objcoefs[idx] = edges[idx].pattern->score(edges[idx].start, edges[idx].end);
  }
  GRBLinExpr objfun;
  objfun.addTerms(objcoefs, vars, edges.size());
  model.setObjective(objfun, GRB_MAXIMIZE);
  model.update();

  if (Parameters::get_int("Tracking.Optimizer.SaveModel")) {
    model.write(Parameters::get_string("Tracking.Optimizer.ModelPath"));
  }
  model.getEnv().set(GRB_IntParam_OutputFlag,
                     Parameters::get_int("Tracking.Optimizer.OutputFlag"));

  model.optimize();
  delete[] objcoefs;

  std::vector<Transition> result(0);
  for (size_t idx = 0; idx < tracking_variables.size(); ++idx) {
    if (tracking_variables[idx].second->get(GRB_DoubleAttr_X) > 0.5) {
      result.push_back(edges[idx]);
    }
  }
  return result;
}

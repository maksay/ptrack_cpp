/*
 * Copyright 2016 Andrii Maksai
 */
#include "PatternOptimizer.h"
#include "gurobi_c++.h"
PatternOptimizer::PatternOptimizer() {}

std::vector<Pattern *> PatternOptimizer::findMinimalPatternsWithPositiveScore(
    std::vector<Pattern *> possible_patterns,
    std::vector<std::vector<Detection *> > trajectories,
    std::vector<std::pair<std::vector<Detection *>, Pattern *> > *assignment) {
  GRBEnv *env = new GRBEnv();
  GRBModel model = GRBModel(*env);

  char *assignment_type =
      new char[possible_patterns.size() * trajectories.size()];
  for (size_t idx = 0; idx < possible_patterns.size() * trajectories.size();
       ++idx) {
    assignment_type[idx] = GRB_BINARY;
  }

  char *usage_type = new char[possible_patterns.size()];
  for (size_t idx = 0; idx < possible_patterns.size(); ++idx) {
    usage_type[idx] = GRB_BINARY;
  }
  GRBVar *assignment_vars =
      model.addVars(NULL, NULL, NULL, assignment_type, NULL,
                    possible_patterns.size() * trajectories.size());
  GRBVar *usage_vars = model.addVars(NULL, NULL, NULL, usage_type, NULL,
                                     possible_patterns.size());

  model.update();

  {
    // Assignment unique for each trajectory
    double *assignment_coefs = new double[possible_patterns.size()];
    GRBVar *assignment_terms = new GRBVar[possible_patterns.size()];
    for (size_t idx = 0; idx < possible_patterns.size(); ++idx) {
      assignment_coefs[idx] = 1;
    }
    for (size_t jdx = 0; jdx < trajectories.size(); ++jdx) {
      GRBLinExpr lhs;
      for (size_t idx = 0; idx < possible_patterns.size(); ++idx) {
        assignment_terms[idx] =
            assignment_vars[idx * trajectories.size() + jdx];
      }
      lhs.addTerms(assignment_coefs, assignment_terms,
                   possible_patterns.size());
      model.addConstr(lhs, GRB_EQUAL, 1);
      model.update();
    }
    // Usage >= assignment
    for (size_t idx = 0; idx < possible_patterns.size(); ++idx) {
      for (size_t jdx = 0; jdx < trajectories.size(); ++jdx) {
        model.addConstr(assignment_vars[idx * trajectories.size() + jdx],
                        GRB_LESS_EQUAL, usage_vars[idx]);
      }
    }
    delete assignment_coefs;
    delete assignment_terms;
  }

  {
    // Positive score
    double *assignment_coefs =
        new double[possible_patterns.size() * trajectories.size()];
    for (size_t idx = 0; idx < possible_patterns.size(); ++idx) {
      for (size_t jdx = 0; jdx < trajectories.size(); ++jdx) {
        assignment_coefs[idx * trajectories.size() + jdx] =
            possible_patterns[idx]->score_trajectory(trajectories[jdx]);
      }
    }
    GRBLinExpr lhs;
    lhs.addTerms(assignment_coefs, assignment_vars,
                 possible_patterns.size() * trajectories.size());
    model.addConstr(lhs, GRB_GREATER_EQUAL, 0);
    model.update();
    delete assignment_coefs;
  }

  {
    // Minimize the metric
    double *assignment_coefs = new double[possible_patterns.size()];
    for (size_t idx = 0; idx < possible_patterns.size(); ++idx) {
      assignment_coefs[idx] = possible_patterns[idx]->area();
    }
    GRBLinExpr lhs;
    lhs.addTerms(assignment_coefs, usage_vars, possible_patterns.size());
    model.setObjective(lhs, GRB_MINIMIZE);
    model.update();
    delete assignment_coefs;
  }

  if (Parameters::get_int("Pattern.Optimizer.SaveModel")) {
    model.write(Parameters::get_string("Pattern.Optimizer.ModelPath"));
  }
  model.getEnv().set(GRB_IntParam_OutputFlag,
                     Parameters::get_int("Pattern.Optimizer.OutputFlag"));

  model.optimize();
  std::vector<Pattern *> answer;
  for (size_t idx = 0; idx < possible_patterns.size(); ++idx) {
    if (usage_vars[idx].get(GRB_DoubleAttr_X) > 0.5) {
      answer.push_back(possible_patterns[idx]);
    }
  }

  if (assignment != NULL) {
    assignment->clear();
    for (size_t idx = 0; idx < possible_patterns.size(); ++idx) {
      for (size_t jdx = 0; jdx < trajectories.size(); ++jdx) {
        if (assignment_vars[idx * trajectories.size() + jdx].get(
                GRB_DoubleAttr_X) > 0.5) {
          assignment->push_back(
              std::make_pair(trajectories[jdx], possible_patterns[idx]));
        }
      }
    }
  }

  return answer;
}

std::vector<Pattern *> PatternOptimizer::findBestPatternsBoundByMetric(
    std::vector<Pattern *> possible_patterns,
    std::vector<std::vector<Detection *> > trajectories,
    float metric_bound,
    std::vector<std::pair<std::vector<Detection *>, Pattern *> > *assignment) {
  GRBEnv *env = new GRBEnv();
  GRBModel model = GRBModel(*env);

  char *assignment_type =
      new char[possible_patterns.size() * trajectories.size()];
  for (size_t idx = 0; idx < possible_patterns.size() * trajectories.size();
       ++idx) {
    assignment_type[idx] = GRB_BINARY;
  }

  char *usage_type = new char[possible_patterns.size()];
  for (size_t idx = 0; idx < possible_patterns.size(); ++idx) {
    usage_type[idx] = GRB_BINARY;
  }
  GRBVar *assignment_vars =
      model.addVars(NULL, NULL, NULL, assignment_type, NULL,
                    possible_patterns.size() * trajectories.size());
  GRBVar *usage_vars = model.addVars(NULL, NULL, NULL, usage_type, NULL,
                                     possible_patterns.size());

  model.update();

  {
    // Assignment unique for each trajectory
    double *assignment_coefs = new double[possible_patterns.size()];
    GRBVar *assignment_terms = new GRBVar[possible_patterns.size()];
    for (size_t idx = 0; idx < possible_patterns.size(); ++idx) {
      assignment_coefs[idx] = 1;
    }
    for (size_t jdx = 0; jdx < trajectories.size(); ++jdx) {
      GRBLinExpr lhs;
      for (size_t idx = 0; idx < possible_patterns.size(); ++idx) {
        assignment_terms[idx] =
            assignment_vars[idx * trajectories.size() + jdx];
      }
      lhs.addTerms(assignment_coefs, assignment_terms,
                   possible_patterns.size());
      model.addConstr(lhs, GRB_EQUAL, 1);
      model.update();
    }
    // Usage >= assignment
    for (size_t idx = 0; idx < possible_patterns.size(); ++idx) {
      for (size_t jdx = 0; jdx < trajectories.size(); ++jdx) {
        model.addConstr(assignment_vars[idx * trajectories.size() + jdx],
                        GRB_LESS_EQUAL, usage_vars[idx]);
      }
    }
    delete assignment_coefs;
    delete assignment_terms;
  }

  {
    // Bound by metric
    double *assignment_coefs = new double[possible_patterns.size()];
    for (size_t idx = 0; idx < possible_patterns.size(); ++idx) {
      assignment_coefs[idx] = possible_patterns[idx]->area();
    }
    GRBLinExpr lhs;
    lhs.addTerms(assignment_coefs, usage_vars, possible_patterns.size());
    model.addConstr(lhs, GRB_LESS_EQUAL, metric_bound);
    model.update();
    delete assignment_coefs;
  }

  {
    // Maximize score
    double *assignment_coefs =
        new double[possible_patterns.size() * trajectories.size()];
    for (size_t idx = 0; idx < possible_patterns.size(); ++idx) {
      for (size_t jdx = 0; jdx < trajectories.size(); ++jdx) {
        assignment_coefs[idx * trajectories.size() + jdx] =
            possible_patterns[idx]->score_trajectory(trajectories[jdx]);
      }
    }
    GRBLinExpr lhs;
    lhs.addTerms(assignment_coefs, assignment_vars,
                 possible_patterns.size() * trajectories.size());
    model.setObjective(lhs, GRB_MAXIMIZE);
    model.update();
    delete assignment_coefs;
  }


  if (Parameters::get_int("Pattern.Optimizer.SaveModel")) {
    model.write(Parameters::get_string("Pattern.Optimizer.ModelPath"));
  }
  model.getEnv().set(GRB_IntParam_OutputFlag,
                     Parameters::get_int("Pattern.Optimizer.OutputFlag"));

  model.optimize();
  std::vector<Pattern *> answer;
  for (size_t idx = 0; idx < possible_patterns.size(); ++idx) {
    if (usage_vars[idx].get(GRB_DoubleAttr_X) > 0.5) {
      answer.push_back(possible_patterns[idx]);
    }
  }

  if (assignment != NULL) {
    assignment->clear();
    for (size_t idx = 0; idx < possible_patterns.size(); ++idx) {
      for (size_t jdx = 0; jdx < trajectories.size(); ++jdx) {
        if (assignment_vars[idx * trajectories.size() + jdx].get(
                GRB_DoubleAttr_X) > 0.5) {
          assignment->push_back(
              std::make_pair(trajectories[jdx], possible_patterns[idx]));
        }
      }
    }
  }

  return answer;
}

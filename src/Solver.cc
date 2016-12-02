/*
 * Copyright 2016 Andrii Maksai
 */
#include "Solver.h"
#include <iostream>
#include <climits>
#include "CenterlineWithWidthPattern.h"
#include "FlowConservationConstraintProvider.h"
#include "GroundPlaneTransitionProvider.h"
#include "Parameters.h"
#include "PatternOptimizer.h"
#include "SpatialExclusionConstraintProvider.h"
#include "TrackingOptimizer.h"
#include "Transformation.h"
std::vector<std::pair<std::vector<Detection *>, Pattern *> >
Solver::findTracking(std::vector<std::vector<Detection *> > trajectories,
                     std::vector<Pattern *> patterns) {
  std::vector<Detection *> detections =
      Transformation::trajectoriesToDetections(trajectories);
  float epsilon = Parameters::get_float("Parameters.epsilon");
  float epsilon_no_pattern =
      Parameters::get_float("Parameters.Testing.epsilon_no_pattern");
  float D_1 = Parameters::get_float("Parameters.D_1");
  float D_2 = Parameters::get_float("Parameters.D_2");
  int D_t = Parameters::get_int("Parameters.D_t");

  for (size_t idx = 0; idx < patterns.size(); ++idx) {
    (dynamic_cast<CenterlineWithWidthPattern *>(patterns[idx]))
        ->get_epsilon_no_pattern() = epsilon_no_pattern;
    (dynamic_cast<CenterlineWithWidthPattern *>(patterns[idx]))->get_epsilon() =
        epsilon;
  }

  std::vector<TrackingOptimizer::ConstraintProvider *> constraint_providers;
  constraint_providers.push_back(new FlowConservationConstraintProvider());
  constraint_providers.push_back(new SpatialExclusionConstraintProvider(true));
  TrackingOptimizer tracking_optimizer(constraint_providers);

  std::vector<Transition> selected_transitions;
  float alpha_low = 0.;
  float alpha_high = 1.;
  if (Parameters::get_int("Tracking.Stderr")) {
    std::cerr << "Searching for optimal alpha:";
  }
  while (alpha_high - alpha_low > 0.01) {
    float alpha_mid = (alpha_high + alpha_low) / 2.;
    if (Parameters::get_int("Tracking.Stderr")) {
      std::cerr << "->" << alpha_mid;
    }

    for (size_t idx = 0; idx < patterns.size(); ++idx) {
      (dynamic_cast<CenterlineWithWidthPattern *>(patterns[idx]))->get_alpha() =
          alpha_mid;
    }

    std::vector<Transition> tracking_graph;
    TransitionProvider *transition_provider =
        new GroundPlaneTransitionProvider(D_1, D_2, D_t);
    for (size_t idx = 0; idx < patterns.size(); ++idx) {
      std::vector<Transition> transitions =
          transition_provider->getTransitions(detections, patterns[idx]);
      for (size_t jdx = 0; jdx < transitions.size(); ++jdx) {
        tracking_graph.push_back(transitions[jdx]);
      }
    }

    selected_transitions =
        tracking_optimizer.findOptimalTracking(tracking_graph);
    float total_score = 0.;
    for (size_t idx = 0; idx < selected_transitions.size(); ++idx) {
      total_score += selected_transitions[idx].pattern->score(
          selected_transitions[idx].start, selected_transitions[idx].end);
    }

    if (total_score < 0) {
      alpha_high = alpha_mid;
    } else {
      alpha_low = alpha_mid;
    }
  }
  if (Parameters::get_int("Tracking.Stderr")) {
    std::cerr << std::endl;
  }

  std::vector<std::pair<std::vector<Detection *>, Pattern *> > assignment;
  std::vector<std::vector<Detection *> > our_trajectories =
      Transformation::transitionsToTrajectories(
          selected_transitions,
          Parameters::get_int("Parameters.ignore_no_pattern"), &assignment);

  // Filling in the missing frames
  our_trajectories =
      Transformation::trajectoriesToContinuousTrajectories(our_trajectories);
  for (size_t idx = 0; idx < our_trajectories.size(); ++idx) {
    assignment[idx].first = our_trajectories[idx];
  }
  return assignment;
}

std::vector<std::pair<std::vector<Detection *>, Pattern *> >
Solver::findPatterns(std::vector<std::vector<Detection *> > trajectories,
                     float pattern_metric_bound) {
  std::vector<std::vector<Detection *> > full_trajectories;
  for (size_t idx = 0; idx < trajectories.size(); ++idx) {
    if (trajectories[idx][0]->first_batch_frame) continue;
    if (trajectories[idx].back()->last_batch_frame) continue;
    full_trajectories.push_back(trajectories[idx]);
  }

  std::vector<float> widths =
      Parameters::get_float_list("Training.PatternWidths");
  float epsilon = Parameters::get_float("Parameters.epsilon");
  float epsilon_no_pattern =
      Parameters::get_float("Parameters.Training.epsilon_no_pattern");
  float alpha = 0.;

  std::vector<Pattern *> possible_patterns;
  for (size_t idx = 0; idx < full_trajectories.size(); ++idx) {
    std::vector<std::pair<float, float> > points(0);
    for (size_t kdx = 0; kdx < full_trajectories[idx].size(); ++kdx) {
      points.push_back(std::make_pair(full_trajectories[idx][kdx]->x,
                                      full_trajectories[idx][kdx]->y));
    }
    for (size_t jdx = 0; jdx < widths.size(); ++jdx) {
      possible_patterns.push_back(new CenterlineWithWidthPattern(
          Pattern::NormalPattern, points, widths[jdx], alpha, epsilon,
          epsilon_no_pattern));
    }
  }
  possible_patterns.push_back(new CenterlineWithWidthPattern(
      Pattern::NoPattern, std::vector<std::pair<float, float> >(0), 0, alpha,
      epsilon, epsilon_no_pattern));

  if (Parameters::get_int("Pattern.Stderr")) {
    std::cerr << "Possible patterns: " << possible_patterns.size() << std::endl;
    std::cerr << "Trajectories to assign: " << trajectories.size() << std::endl;
  }

  std::vector<Pattern *> patterns;
  std::vector<std::pair<std::vector<Detection *>, Pattern *> > assignment;

  float patt_alpha_low = 0.;
  float patt_alpha_high = 1.;
  if (Parameters::get_int("Pattern.Stderr")) {
    std::cerr << "Binary searching for optimal alpha for patterns:";
  }
  while (patt_alpha_high - patt_alpha_low > 0.01) {
    float alpha_mid = (patt_alpha_high + patt_alpha_low) / 2.;

    if (Parameters::get_int("Pattern.Stderr")) {
      std::cerr << "->" << alpha_mid;
    }

    for (size_t idx = 0; idx < possible_patterns.size(); ++idx) {
      (dynamic_cast<CenterlineWithWidthPattern *>(possible_patterns[idx]))
          ->get_alpha() = alpha_mid;
    }

    patterns = PatternOptimizer().findBestPatternsBoundByMetric(
        possible_patterns, trajectories, pattern_metric_bound, &assignment);

    float total_score = 0.;
    for (size_t idx = 0; idx < assignment.size(); ++idx) {
      total_score +=
          assignment[idx].second->score_trajectory(assignment[idx].first);
    }

    if (total_score < 0) {
      patt_alpha_high = alpha_mid;
    } else {
      patt_alpha_low = alpha_mid;
    }
  }
  if (Parameters::get_int("Pattern.Stderr")) {
    std::cerr << std::endl;
  }

  int normal_patterns = patterns.size();
  for (size_t idx = 0; idx < patterns.size(); ++idx) {
    if (patterns[idx]->pattern_type == Pattern::NoPattern) {
      --normal_patterns;
      break;
    }
  }

  if (Parameters::get_int("Pattern.Stderr")) {
    std::cerr << "Found patterns: " << normal_patterns;
    if (normal_patterns < patterns.size()) {
      std::cerr << " + No Pattern";
    }
    std::cerr << std::endl;
  }

  return assignment;
}
std::vector<std::pair<std::vector<Detection *>, Pattern *> >
Solver::solveIterative(std::vector<std::vector<Detection *> > trajectories) {
  std::vector<std::vector<Detection *> > current_trajectories = trajectories;
  std::vector<std::pair<std::vector<Detection *>, Pattern *> > best_assignment;
  float best_IDF_hat = -1;

  if (Parameters::get_int("Unsupervised.Stderr")) {
    std::cerr << "Unsupervised search for best trajectories and patterns\n";
  }
  int global_iter = 0;
  for (int pattern_size = Parameters::get_int("Unsupervised.StartPatternCount");
       pattern_size <= Parameters::get_int("Unsupervised.EndPatternCount");
       ++pattern_size) {
    for (int iter = 0;
         iter < Parameters::get_int("Unsupervised.PatternCountIncreaseStep");
         ++iter) {
      float pattern_metric_bound = pattern_size * 10000 + 9999;
      std::vector<Pattern *> patterns = Transformation::assignmentToPatterns(
          Solver::findPatterns(current_trajectories, pattern_metric_bound));

      std::vector<std::pair<std::vector<Detection *>, Pattern *> >
          current_assignment =
              Solver::findTracking(current_trajectories, patterns);

      current_trajectories =
          Transformation::assignmentToTrajectories(current_assignment);

      std::vector<std::vector<Detection *> > traj_left, traj_right;
      Transformation::splitTrajectoriesInTwo(current_trajectories, &traj_left,
                                             &traj_right);

      std::vector<std::pair<std::vector<Detection *>, Pattern *> >
          assignment_left =
              Solver::findPatterns(traj_left, pattern_metric_bound);
      std::vector<std::pair<std::vector<Detection *>, Pattern *> >
          assignment_right =
              Solver::findPatterns(traj_right, pattern_metric_bound);

      float current_IDF_hat = 0.;
      for (int swaps = 0; swaps < 2; ++swaps) {
        float alpha_low = 0, alpha_high = 1.;
        while (alpha_high - alpha_low > 0.01) {
          float alpha_mid = (alpha_high + alpha_low) / 2.;
          for (size_t idx = 0; idx < assignment_left.size(); ++idx) {
            (dynamic_cast<CenterlineWithWidthPattern *>(
                 assignment_left[idx].second))
                ->get_alpha() = alpha_mid;
          }
          float cur_score = 0;
          for (size_t idx = 0; idx < assignment_right.size(); ++idx) {
            float best_match_score = INT_MIN;
            for (size_t jdx = 0; jdx < assignment_left.size(); ++jdx) {
              best_match_score =
                  std::max(best_match_score,
                           assignment_left[jdx].second->score_trajectory(
                               assignment_right[idx].first));
            }
            cur_score += best_match_score;
          }
          if (cur_score > 0) {
            alpha_low = alpha_mid;
          } else {
            alpha_high = alpha_mid;
          }
        }
        current_IDF_hat += ((alpha_high + alpha_low) / 2.) / 2.;
        swap(assignment_left, assignment_right);
      }

      if (Parameters::get_int("Unsupervised.Stderr")) {
        std::cerr << "Iter #" << global_iter / 100 << (global_iter / 10) % 10
                  << global_iter % 10 << " (" << pattern_size
                  << " patterns) : IDF_hat = " << current_IDF_hat << std::endl;
      }
      ++global_iter;
      if (current_IDF_hat > best_IDF_hat) {
        best_IDF_hat = current_IDF_hat;
        best_assignment = current_assignment;
      }
    }
  }
  return best_assignment;
}

/*
 * Copyright 2016 Andrii Maksai
 */
#include "Transformation.h"
#include <cassert>
#include <climits>
#include <algorithm>
#include <cstdio>
#include <iostream>
#include "CenterlineWithWidthPattern.h"
Transformation::Transformation() {}

void Transformation::trajectoriesToFile(
    std::vector<std::vector<Detection *> > trajectories, std::string fpath) {
  if (Parameters::get_int("IO.Stderr")) {
    std::cerr << "Writing trajectories to " << fpath << std::endl;
  }
  FILE *fp = fopen(fpath.c_str(), "w");
  for (size_t idx = 0; idx < trajectories.size(); ++idx) {
    for (size_t jdx = 0; jdx < trajectories[idx].size(); ++jdx) {
      Detection *det = trajectories[idx][jdx];
      assert(det->node_type == Detection::NormalNode);
      fprintf(fp, "%d,%d,%d,%d,%d,%d,%f,%f,%f,%f\n", det->frame_num, (int)idx,
              det->bbox_lft, det->bbox_top, det->bbox_rgt - det->bbox_lft,
              det->bbox_bot - det->bbox_top, det->confidence, det->x, det->y,
              det->z);
    }
  }
  fclose(fp);
}

void Transformation::patternsToFile(std::vector<Pattern *> patterns,
                                    std::string fpath) {
  if (Parameters::get_int("IO.Stderr")) {
    std::cerr << "Writing patterns to " << fpath << std::endl;
  }
 FILE *fp = fopen(fpath.c_str(), "w");
  for (size_t idx = 0; idx < patterns.size(); ++idx) {
    fprintf(fp, "%s\n", patterns[idx]->to_string().c_str());
  }
  fclose(fp);
}

void Transformation::assignmentToFile(
    std::vector<std::pair<std::vector<Detection *>, Pattern *> > assignment,
    std::string fpath) {
  if (Parameters::get_int("IO.Stderr")) {
    std::cerr << "Writing full assignment to " << fpath << std::endl;
  }
  FILE *fp = fopen(fpath.c_str(), "w");
  std::vector<Pattern *> patterns =
      Transformation::assignmentToPatterns(assignment);
  fprintf(fp, "%d %d\n", (int)patterns.size(), (int)assignment.size());
  for (size_t idx = 0; idx < patterns.size(); ++idx) {
    fprintf(fp, "%s\n", patterns[idx]->to_string().c_str());
  }
  for (size_t idx = 0; idx < assignment.size(); ++idx) {
    size_t pattern_pos = 0;
    while (patterns[pattern_pos] != assignment[idx].second) ++pattern_pos;
    fprintf(fp, "%d %d", (int)assignment[idx].first.size(), (int)pattern_pos);
    for (size_t jdx = 0; jdx < assignment[idx].first.size(); ++jdx) {
      fprintf(fp, " %f %f", assignment[idx].first[jdx]->x,
              assignment[idx].first[jdx]->y);
    }
    fprintf(fp, "\n");
  }
  fclose(fp);
}

std::vector<std::vector<Detection *> > Transformation::fileToTrajectories(
    std::string fpath, bool split_discontinuous_trajectories) {
  if (Parameters::get_int("IO.Stderr")) {
    std::cerr << "Reading trajectories from " << fpath << std::endl;
  }
  FILE *fp = fopen(fpath.c_str(), "r");
  std::vector<std::vector<Detection *> > trajectories;
  std::map<int, std::vector<Detection *> > ongoing_trajectories;
  float bbox_top, bbox_lft, bbox_hgt, bbox_wid, confidence, ground_x, ground_y,
      ground_z;
  int original_trajectory_id, frame_num;
  int frame_min = INT_MAX;
  int frame_max = INT_MIN;
  while (fscanf(fp, "%d,%d,%f,%f,%f,%f,%f,%f,%f,%f", &frame_num,
                &original_trajectory_id, &bbox_lft, &bbox_top, &bbox_wid,
                &bbox_hgt, &confidence, &ground_x, &ground_y, &ground_z) > 0) {
    frame_min = std::min(frame_min, frame_num);
    frame_max = std::max(frame_max, frame_num);

    Detection *det = new Detection(Detection::NormalNode);
    det->x = ground_x;
    det->y = ground_y;
    det->z = ground_z;
    det->original_trajectory_id = original_trajectory_id;
    det->frame_num = frame_num;
    det->bbox_top = static_cast<int>(bbox_top);
    det->bbox_lft = static_cast<int>(bbox_lft);
    det->bbox_bot = static_cast<int>(bbox_top + bbox_hgt);
    det->bbox_rgt = static_cast<int>(bbox_lft + bbox_wid);
    det->confidence = confidence;

    if (ongoing_trajectories.count(original_trajectory_id)) {
      if (ongoing_trajectories[original_trajectory_id].back()->frame_num !=
              frame_num - 1 &&
          split_discontinuous_trajectories == true) {
        trajectories.push_back(ongoing_trajectories[original_trajectory_id]);
        ongoing_trajectories[original_trajectory_id].clear();
      }
    }
    ongoing_trajectories[original_trajectory_id].push_back(det);
  }
  fclose(fp);
  for (std::map<int, std::vector<Detection *> >::iterator it =
           ongoing_trajectories.begin();
       it != ongoing_trajectories.end(); ++it) {
    trajectories.push_back(it->second);
  }
  if (!split_discontinuous_trajectories) {
    trajectories =
        Transformation::trajectoriesToContinuousTrajectories(trajectories);
  }
  for (size_t idx = 0; idx < trajectories.size(); ++idx) {
    for (size_t jdx = 0; jdx < trajectories[idx].size(); ++jdx) {
      Detection *det = trajectories[idx][jdx];
      det->first_batch_frame = (det->frame_num == frame_min);
      det->last_batch_frame = (det->frame_num == frame_max);
    }
  }
  return trajectories;
}

std::vector<Pattern *> Transformation::fileToPatterns(std::string fpath) {
  if (Parameters::get_int("IO.Stderr")) {
    std::cerr << "Reading patterns from " << fpath << std::endl;
  }
  FILE *fp = fopen(fpath.c_str(), "r");
  char buff[111111];
  std::vector<Pattern *> patterns;
  while (fgets(buff, 111111, fp)) {
    patterns.push_back(new CenterlineWithWidthPattern(buff));
  }
  return patterns;
}

std::vector<Pattern *> Transformation::assignmentToPatterns(
    std::vector<std::pair<std::vector<Detection *>, Pattern *> > assignment) {
  std::vector<Pattern *> patterns;
  for (size_t idx = 0; idx < assignment.size(); ++idx) {
    bool already_present = false;
    for (size_t jdx = 0; jdx < patterns.size(); ++jdx) {
      if (patterns[jdx] == assignment[idx].second) {
        already_present = true;
        break;
      }
    }
    if (!already_present) {
      patterns.push_back(assignment[idx].second);
    }
  }
  return patterns;
}

std::vector<std::vector<Detection *> > Transformation::assignmentToTrajectories(
    std::vector<std::pair<std::vector<Detection *>, Pattern *> > assignment) {
  std::vector<std::vector<Detection *> > trajectories;
  for (size_t idx = 0; idx < assignment.size(); ++idx) {
    trajectories.push_back(assignment[idx].first);
  }
  return trajectories;
}

std::vector<Detection *> Transformation::trajectoriesToDetections(
    std::vector<std::vector<Detection *> > trajectories) {
  std::vector<Detection *> detections;
  for (size_t idx = 0; idx < trajectories.size(); ++idx) {
    for (size_t jdx = 0; jdx < trajectories[idx].size(); ++jdx) {
      detections.push_back(trajectories[idx][jdx]);
    }
  }
  detections.push_back(&Detection::Source);
  detections.push_back(&Detection::Sink);
  return detections;
}

std::vector<std::vector<Detection *> >
Transformation::transitionsToTrajectories(
    std::vector<Transition> transitions, bool ignore_no_pattern,
    std::vector<std::pair<std::vector<Detection *>, Pattern *> > *assignment) {
  struct sort_comparator {
    static bool sort_by_pattern_by_first_node(Transition one, Transition two) {
      if (one.pattern != two.pattern) return one.pattern < two.pattern;
      return one.start < two.start;
    }
  };

  std::vector<std::vector<Detection *> > answer;
  std::sort(transitions.begin(), transitions.end(),
            sort_comparator::sort_by_pattern_by_first_node);

  if (assignment != NULL) assignment->clear();
  for (size_t idx = 0; idx < transitions.size(); ++idx) {
    if (transitions[idx].start->node_type == Detection::SourceNode) {
      assert(transitions[idx].end->node_type == Detection::NormalNode);
      int current_edge_idx = idx;
      std::vector<Detection *> trajectory(0);
      while (true) {
        trajectory.push_back(transitions[current_edge_idx].end);
        Transition tmp(transitions[current_edge_idx].end, NULL,
                       transitions[current_edge_idx].pattern);
        int next_edge_idx =
            lower_bound(transitions.begin(), transitions.end(), tmp,
                        sort_comparator::sort_by_pattern_by_first_node) -
            transitions.begin();
        int next_edge_idx_copy = next_edge_idx;
        assert(next_edge_idx >= 0);
        assert(next_edge_idx < transitions.size());
        assert(transitions[next_edge_idx].start ==
               transitions[current_edge_idx].end);
        assert(transitions[next_edge_idx].pattern ==
               transitions[current_edge_idx].pattern);
        current_edge_idx = next_edge_idx;
        if (transitions[current_edge_idx].end->node_type ==
            Detection::SinkNode) {
          break;
        }
      }
      if (transitions[idx].pattern->pattern_type == Pattern::NoPattern &&
          ignore_no_pattern) {
        continue;
      } else {
        answer.push_back(trajectory);
        if (assignment != NULL) {
          assignment->push_back(
              std::make_pair(trajectory, transitions[idx].pattern));
        }
      }
    }
  }
  return answer;
}

std::vector<std::vector<Detection *> >
Transformation::trajectoriesToContinuousTrajectories(
    std::vector<std::vector<Detection *> > trajectories) {
  std::vector<std::vector<Detection *> > answer;
  for (size_t idx = 0; idx < trajectories.size(); ++idx) {
    std::vector<Detection *> current_trajectory;
    current_trajectory.push_back(trajectories[idx][0]);
    for (size_t jdx = 1; jdx < trajectories[idx].size(); ++jdx) {
      for (int now = current_trajectory.back()->frame_num + 1;
           now < trajectories[idx][jdx]->frame_num; ++now) {
        Detection *prev = current_trajectory.back();
        Detection *next = trajectories[idx][jdx];
        int distance_to_prev = now - prev->frame_num;
        int distance_to_next = next->frame_num - now;
        int total_distance = distance_to_prev + distance_to_next;
        float ratio_of_prev = 1. * distance_to_next / total_distance;
        float ratio_of_next = 1. * distance_to_prev / total_distance;

        Detection *det = new Detection(Detection::NormalNode);
        det->x = prev->x * ratio_of_prev + next->x * ratio_of_next;
        det->y = prev->y * ratio_of_prev + next->y * ratio_of_next;
        det->z = prev->z * ratio_of_prev + next->z * ratio_of_next;
        det->original_trajectory_id = prev->original_trajectory_id;
        det->frame_num = now;
        det->first_batch_frame = false;
        det->last_batch_frame = false;
        det->bbox_top = static_cast<int>(prev->bbox_top * ratio_of_prev +
                                         next->bbox_top * ratio_of_next + 1e-9);
        det->bbox_lft = static_cast<int>(prev->bbox_lft * ratio_of_prev +
                                         next->bbox_lft * ratio_of_next + 1e-9);
        det->bbox_bot = static_cast<int>(prev->bbox_bot * ratio_of_prev +
                                         next->bbox_bot * ratio_of_next + 1e-9);
        det->bbox_rgt = static_cast<int>(prev->bbox_rgt * ratio_of_prev +
                                         next->bbox_rgt * ratio_of_next + 1e-9);
        det->confidence =
            prev->confidence * ratio_of_prev + next->confidence * ratio_of_next;

        current_trajectory.push_back(det);
      }
      current_trajectory.push_back(trajectories[idx][jdx]);
    }
    answer.push_back(current_trajectory);
  }
  return answer;
}

void Transformation::splitTrajectoriesInTwo(
    std::vector<std::vector<Detection *> > trajectories,
    std::vector<std::vector<Detection *> > *left,
    std::vector<std::vector<Detection *> > *right) {
  left->clear();
  right->clear();
  int time_min = INT_MAX;
  int time_max = INT_MIN;
  for(size_t idx = 0; idx < trajectories.size(); ++idx) {
    time_min = std::min(time_min, trajectories[idx][0]->frame_num);
    time_max = std::max(time_max, trajectories[idx].back()->frame_num);
  }
  int time_mid = (time_min + time_max) / 2;
  for(size_t idx = 0; idx < trajectories.size(); ++idx) {
    std::vector<Detection *> cur_left(0), cur_right(0);
    for(size_t jdx = 0; jdx < trajectories[idx].size(); ++jdx) {
      if (trajectories[idx][jdx]->frame_num < time_mid) {
        cur_left.push_back(trajectories[idx][jdx]);
      } else {
        cur_right.push_back(trajectories[idx][jdx]);
      }
    }
    if (cur_left.size() > 0) {
      left->push_back(cur_left);
    }
    if (cur_right.size() > 0) {
      right->push_back(cur_right);
    }
  }
}

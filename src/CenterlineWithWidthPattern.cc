/*
 * Copyright 2016 Andrii Maksai
 */
#include "CenterlineWithWidthPattern.h"
#include <cassert>
#include <cstdlib>
#include <cmath>
#include <sstream>

CenterlineWithWidthPattern::CenterlineWithWidthPattern(
    PatternType _pattern_type, std::vector<std::pair<float, float> > _points,
    float _width, float _alpha, float _epsilon, float _epsilon_no_pattern)
    : Pattern(_pattern_type),
      points(_points),
      width(_width),
      alpha(_alpha),
      epsilon(_epsilon),
      epsilon_no_pattern(_epsilon_no_pattern) {}

CenterlineWithWidthPattern::CenterlineWithWidthPattern(
    std::string pattern_string)
    : Pattern(Pattern::NormalPattern) {
  std::istringstream iss(pattern_string);
  std::string first_token;
  iss >> first_token;
  if (first_token == "NoPattern") {
    pattern_type = Pattern::NoPattern;
    return;
  }
  int points_size = atoi(first_token.c_str());
  points.clear();
  iss >> width;
  while (points_size--) {
    float x, y;
    iss >> x >> y;
    points.push_back(std::make_pair(x, y));
  }
}

float CenterlineWithWidthPattern::score(Detection* node_start,
                                        Detection* node_end) {
  std::pair<float, float> mn = mn_function(node_start, node_end);
  return mn.first - alpha * mn.second;
}

std::pair<size_t, float> CenterlineWithWidthPattern::projection_point_idx(
    Detection* node) {
  assert(node->node_type == Detection::NormalNode);
  assert(this->pattern_type == Pattern::NormalPattern);
  assert(points.size() > 0);

  size_t best_idx = 0;
  float best_dist =
      hypot(points[0].first - node->x, points[0].second - node->y);
  for (size_t idx = 1; idx < points.size(); ++idx) {
    float cur_dist =
        hypot(points[idx].first - node->x, points[idx].second - node->y);
    if (cur_dist < best_dist) {
      best_dist = cur_dist;
      best_idx = idx;
    }
  }
  return std::make_pair(best_idx, best_dist);
}

float CenterlineWithWidthPattern::pattern_length_between(size_t start_idx,
                                                         size_t end_idx) {
  assert(this->pattern_type == Pattern::NormalPattern);
  assert(start_idx >= 0 && start_idx < points.size());
  assert(end_idx >= 0 && end_idx < points.size());

  float pattern_length = 0.;
  for (size_t idx = start_idx; idx < end_idx; ++idx) {
    pattern_length += hypot(points[idx].first - points[idx + 1].first,
                            points[idx].second - points[idx + 1].second);
  }
  for (size_t idx = end_idx; idx < start_idx; ++idx) {
    pattern_length -= hypot(points[idx].first - points[idx + 1].first,
                            points[idx].second - points[idx + 1].second);
  }
  return pattern_length;
}

std::pair<float, float> CenterlineWithWidthPattern::mn_function(
    Detection* node_start, Detection* node_end) {
  if (node_start->node_type == Detection::NormalNode &&
      node_end->node_type == Detection::NormalNode) {
    float transition_line_length =
        hypot(node_start->x - node_end->x, node_start->y - node_end->y);
    if (this->pattern_type == Pattern::NormalPattern) {
      std::pair<size_t, float> projection_start =
          projection_point_idx(node_start);
      std::pair<size_t, float> projection_end = projection_point_idx(node_end);

      float pattern_curve_length =
          pattern_length_between(projection_start.first, projection_end.first);
      // Case 1.a: Normal nodes, normal pattern, projection = 0
      if (fabs(pattern_curve_length) < 1e-6) {
        return std::make_pair(0, transition_line_length);
      }
      float pattern_line_length =
          hypot(points[projection_start.first].first -
                    points[projection_end.first].first,
                points[projection_start.first].second -
                    points[projection_end.first].second);
      // Case 1: Normal nodes, normal pattern, positive projection, detections
      // within width of the pattern
      if (projection_start.second <= width && projection_end.second <= width &&
          projection_start.first <= projection_end.first) {
        float scalar_product = (node_start->x - node_end->x) *
                                   (points[projection_start.first].first -
                                    points[projection_end.first].first) +
                               (node_start->y - node_end->y) *
                                   (points[projection_start.first].second -
                                    points[projection_end.first].second);
        float reprojection_onto_edge = scalar_product / transition_line_length;
        return std::make_pair(reprojection_onto_edge + pattern_line_length,
                              transition_line_length + pattern_curve_length);
      }
      // Case 2: Normal nodes, normal pattern, positive projection, detections
      // outside of width of the pattern
      if ((projection_start.second > width || projection_end.second > width) &&
          projection_start.first <= projection_end.first) {
        return std::make_pair(0, transition_line_length + pattern_curve_length);
      }
      // Case 3: Normal nodes, normal pattern, negative projection
      if (projection_start.first > projection_end.first) {
        return std::make_pair(-pattern_line_length * (1 + epsilon),
                              transition_line_length + pattern_curve_length);
      }
    } else {
      // Case 4: Normal nodes, no pattern
      return std::make_pair(transition_line_length * epsilon_no_pattern,
                            transition_line_length);
    }
  } else {
    if (this->pattern_type == Pattern::NormalPattern) {
      if (node_start->node_type == Detection::SourceNode) {
        assert(node_end->node_type == Detection::NormalNode);
        if (!node_end->first_batch_frame) {
          // Case 5.1: Transition from source to detection NOT in the first
          // frame of the batch, normal pattern
          std::pair<size_t, float> projection_end =
              projection_point_idx(node_end);
          return std::make_pair(
              0, pattern_length_between(0, projection_end.first));
        } else {
          // Case 6.1: Transition from source to detection IN the first frame
          // of the batch, normal pattern
          return std::make_pair(0, 0);
        }
      }
      if (node_end->node_type == Detection::SinkNode) {
        assert(node_start->node_type == Detection::NormalNode);
        if (!node_start->last_batch_frame) {
          // Case 5.2: Transition from detection to sink NOT in the last
          // frame of the batch, normal pattern
          std::pair<size_t, float> projection_start =
              projection_point_idx(node_start);
          return std::make_pair(0,
                                pattern_length_between(projection_start.first,
                                                       points.size() - 1));
        } else {
          // Case 6.2: Transition from detection to sink IN the last frame
          // of the batch, normal pattern
          return std::make_pair(0, 0);
        }
      }
    } else {
      // Case 7.1: Transition from source  assigned to no pattern
      if (node_start->node_type == Detection::SourceNode) {
        return std::make_pair(2 * epsilon_no_pattern, 2);
      } else {
        // Case 7.2: Transition to sink assigned to no pattern
        return std::make_pair(0, 0);
      }
    }
  }
  assert(1 > 2);
}

float CenterlineWithWidthPattern::area() {
  if (pattern_type == Pattern::NoPattern) return 0.;
  float answer = 0;
  for (size_t idx = 0; idx < points.size() - 1; ++idx) {
    answer += hypot(points[idx + 1].first - points[idx].first,
                    points[idx + 1].second - points[idx].second) *
              width * 2;
  }
  answer += 1e4;
  return answer;
}

std::string CenterlineWithWidthPattern::to_string() {
  if (pattern_type == Pattern::NoPattern) return "NoPattern";
  std::ostringstream oss;
  oss << points.size() << " " << width;
  for (size_t idx = 0; idx < points.size(); ++idx) {
    oss << " " << points[idx].first << " " << points[idx].second;
  }
  return oss.str();
}

float& CenterlineWithWidthPattern::get_alpha() { return alpha; }
float& CenterlineWithWidthPattern::get_epsilon_no_pattern() {
  return epsilon_no_pattern;
}
float& CenterlineWithWidthPattern::get_epsilon() { return epsilon; }

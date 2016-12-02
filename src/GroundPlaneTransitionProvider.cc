/*
 * Copyright 2016 Andrii Maksai
 */
#include "GroundPlaneTransitionProvider.h"
#include <algorithm>
#include <iostream>
#include "Detection.h"
GroundPlaneTransitionProvider::GroundPlaneTransitionProvider(float _D_1,
                                                             float _D_2,
                                                             float _D_t)
    : D_1(_D_1), D_2(_D_2), D_t(_D_t) {}

std::vector<Transition>
GroundPlaneTransitionProvider::getTransitions(
    std::vector<Detection *> detections, Pattern *pattern) {
  std::vector<Transition> transitions;
  Detection *source = NULL, *sink = NULL;

  float D_1_sq = D_1 * D_1;
  float D_2_sq = D_2 * D_2;

  if (detections.size() == 0) {
    return transitions;
  }

  int first_frame_num = detections[0]->frame_num;
  int last_frame_num = detections[0]->frame_num;
  for (size_t idx = 1; idx < detections.size(); ++idx) {
    first_frame_num = std::min(first_frame_num, detections[idx]->frame_num);
    last_frame_num = std::max(last_frame_num, detections[idx]->frame_num);
  }

  std::vector<std::vector<Detection *> > detectionsByFrame(
      last_frame_num - first_frame_num + 1, std::vector<Detection *>(0));

  std::map<int, std::pair<Detection *, Detection *> > trajectoryBounds;

  for (size_t idx = 0; idx < detections.size(); ++idx) {
    if (detections[idx]->node_type == Detection::SourceNode) {
      source = detections[idx];
    } else if (detections[idx]->node_type == Detection::SinkNode) {
      sink = detections[idx];
    } else {
      detectionsByFrame[detections[idx]->frame_num - first_frame_num].push_back(
          detections[idx]);
      int trajectory_id = detections[idx]->original_trajectory_id;
      if (!trajectoryBounds.count(trajectory_id)) {
        trajectoryBounds[trajectory_id] =
            std::make_pair(detections[idx], detections[idx]);
      } else {
        if (detections[idx]->frame_num <
            trajectoryBounds[trajectory_id].first->frame_num) {
          trajectoryBounds[trajectory_id].first = detections[idx];
        }
        if (detections[idx]->frame_num >
            trajectoryBounds[trajectory_id].second->frame_num) {
          trajectoryBounds[trajectory_id].second = detections[idx];
        }
      }
    }
  }

  for (int time_moment = 0; time_moment < last_frame_num - first_frame_num;
       ++time_moment) {
    for (size_t now_idx = 0; now_idx < detectionsByFrame[time_moment].size();
         ++now_idx) {
      for (size_t nxt_idx = 0;
           nxt_idx < detectionsByFrame[time_moment + 1].size(); ++nxt_idx) {
        Detection *det_now = detectionsByFrame[time_moment][now_idx];
        Detection *det_nxt = detectionsByFrame[time_moment + 1][nxt_idx];
        float dist_sq = (det_now->x - det_nxt->x) * (det_now->x - det_nxt->x) +
                        (det_now->y - det_nxt->y) * (det_now->y - det_nxt->y);
        // Condition 1: distance D_1 within ground plane
        if (dist_sq <= D_1_sq) {
          transitions.push_back(Transition(det_now, det_nxt, pattern));
        }
        // Condition 2: connected in original results
        if (det_now->original_trajectory_id ==
            det_nxt->original_trajectory_id) {
          transitions.push_back(Transition(det_now, det_nxt, pattern));
        }
      }
    }
  }

  for (std::map<int, std::pair<Detection *, Detection *> >::iterator
           endPointer = trajectoryBounds.begin();
       endPointer != trajectoryBounds.end(); ++endPointer) {
    for (std::map<int, std::pair<Detection *, Detection *> >::iterator
             startPointer = trajectoryBounds.begin();
         startPointer != trajectoryBounds.end(); ++startPointer) {
      Detection *det_end = endPointer->second.second;
      Detection *det_beg = startPointer->second.first;
      float dist_sq = (det_end->x - det_beg->x) * (det_end->x - det_beg->x) +
                      (det_end->y - det_beg->y) * (det_end->y - det_beg->y);
      int dist_dt = det_beg->frame_num - det_end->frame_num;
      // Condition 3: distance withing D_2 and time within D_t
      if (dist_dt > 0 && dist_dt <= D_t && dist_sq <= D_2_sq) {
        transitions.push_back(Transition(det_end, det_beg, pattern));
      }
    }
  }

  for (size_t idx = 0; idx < detections.size(); ++idx) {
    // Condition 4: Everything connected to the source and the sink
    if (detections[idx]->node_type == Detection::NormalNode) {
      if (source != NULL)
        transitions.push_back(Transition(source, detections[idx], pattern));
      if (sink != NULL)
        transitions.push_back(Transition(detections[idx], sink, pattern));
    }
  }

  // Removing duplicates
  sort(transitions.begin(), transitions.end());
  transitions.resize(unique(transitions.begin(), transitions.end()) -
                     transitions.begin());

  return transitions;
}

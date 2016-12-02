/*
 * Copyright 2016 Andrii Maksai
 */
#include "Metric.h"
#include "IDFMatch3DPattern.h"
#include <cstdio>
#include <cstdlib>
Metric::Metric() {}
float Metric::IDF13D(std::vector<std::vector<Detection *> > trajectories,
                     std::vector<std::vector<Detection *> > ground_truth,
                     float D, std::vector<std::pair<int, int> > *assignment) {
  std::vector<IDFMatch3DPattern> patterns;
  for (size_t idx = 0; idx < ground_truth.size(); ++idx) {
    patterns.push_back(
        IDFMatch3DPattern(Pattern::NormalPattern, ground_truth[idx], D));
  }
  FILE *fp = fopen("input.txt", "w");
  for (size_t idx = 0; idx < trajectories.size(); ++idx) {
    for (size_t jdx = 0; jdx < patterns.size(); ++jdx) {
      if (jdx > 0) fprintf(fp, " ");
      fprintf(fp, "%0.3f", patterns[jdx].score_trajectory(trajectories[idx]));
    }
    fprintf(fp, "\n");
  }
  fclose(fp);

  char cmd[1111];
  sprintf(cmd,
          "external/hungarian_assignment/bin/hungarian_solver %d %d < "
          "input.txt > output.txt",
          (int)trajectories.size(), (int)patterns.size());
  system(cmd);

  fp = fopen("output.txt", "r");
  if (assignment != NULL) assignment->clear();
  int trajectory_idx, pattern_idx;
  float total_matches = 0;
  while (fscanf(fp, "%d %d", &trajectory_idx, &pattern_idx) > 0) {
    if (assignment != NULL)
      assignment->push_back(std::make_pair(trajectory_idx, pattern_idx));
    total_matches +=
        2 *
        patterns[pattern_idx].score_trajectory(trajectories[trajectory_idx]);
  }
  fclose(fp);
  system("rm input.txt output.txt");

  int total_lengths = 0;
  for (size_t idx = 0; idx < trajectories.size(); ++idx) {
    total_lengths += trajectories[idx].size();
  }
  for (size_t idx = 0; idx < ground_truth.size(); ++idx) {
    total_lengths += ground_truth[idx].size();
  }

  return total_matches / total_lengths;
}

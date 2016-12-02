/*
 * Copyright 2016 Andrii Maksai
 */
#ifndef CODE_INCLUDE_TEST_TRANSFORMATIONTEST_H_
#define CODE_INCLUDE_TEST_TRANSFORMATIONTEST_H_
#include <iostream>
#include <string>
#include <vector>
#include "Transformation.h"
class TransformationTest {
 public:
  void testAll() {
    std::cerr << "Testing Transformation...";
    testTrajectoriesToFile();
    testPatternsToFile();
    testAssignmentToFile();
    testFileToTrajectories();
    testFileToPatterns();
    testTrajectoriesToContinuousTrajectories();
    testTransitionsToTrajectories();
    testAssignmentToPatternsAssignmentToTrajectories();
    testTrajectoriesToDetections();
    testSplitTrajectoriesInTwo();
    std::cerr << "OK\n";
  }
  void testTrajectoriesToDetections() {
    std::vector<std::vector<Detection *> > trajectories;
    std::vector<Detection *> trajectory;
    Detection *d1 = new Detection(Detection::NormalNode);
    Detection *d2 = new Detection(Detection::NormalNode);
    Detection *d3 = new Detection(Detection::NormalNode);
    trajectory.clear();
    trajectory.push_back(d1);
    trajectory.push_back(d2);
    trajectories.push_back(trajectory);
    trajectory.clear();
    trajectory.push_back(d3);
    trajectories.push_back(trajectory);

    std::vector<Detection *> detections =
        Transformation::trajectoriesToDetections(trajectories);
    std::vector<Detection *> expected_detections;
    expected_detections.push_back(d1);
    expected_detections.push_back(d2);
    expected_detections.push_back(d3);
    expected_detections.push_back(&Detection::Source);
    expected_detections.push_back(&Detection::Sink);
    sort(detections.begin(), detections.end());
    sort(expected_detections.begin(), expected_detections.end());
    assert(detections == expected_detections);
  }
  void testSplitTrajectoriesInTwo() {
    std::vector<std::vector<Detection *> > trajectories;
    std::vector<Detection *> trajectory;
    Detection *d1, *d2, *d3, *d4;

    d1 = new Detection(Detection::NormalNode);
    d1->frame_num = 0;
    d2 = new Detection(Detection::NormalNode);
    d2->frame_num = 1;
    d3 = new Detection(Detection::NormalNode);
    d3->frame_num = 2;
    d4 = new Detection(Detection::NormalNode);
    d4->frame_num = 3;

    trajectory.clear();
    trajectory.push_back(d1);
    trajectory.push_back(d2);
    trajectory.push_back(d3);
    trajectories.push_back(trajectory);
    trajectory.clear();
    trajectory.push_back(d1);
    trajectory.push_back(d2);
    trajectories.push_back(trajectory);
    trajectory.clear();
    trajectory.push_back(d2);
    trajectory.push_back(d3);
    trajectory.push_back(d4);
    trajectories.push_back(trajectory);
    trajectory.clear();
    trajectory.push_back(d3);
    trajectory.push_back(d4);
    trajectories.push_back(trajectory);

    std::vector<std::vector<Detection *> > left, right;
    std::vector<std::vector<Detection *> > expected_left;
    std::vector<std::vector<Detection *> > expected_right;

    Transformation::splitTrajectoriesInTwo(trajectories, &left, &right);

    trajectory.clear();
    trajectory.push_back(d1);
    expected_left.push_back(trajectory);
    trajectory.clear();
    trajectory.push_back(d1);
    expected_left.push_back(trajectory);

    trajectory.clear();
    trajectory.push_back(d3);
    trajectory.push_back(d4);
    expected_right.push_back(trajectory);
    trajectory.clear();
    trajectory.push_back(d2);
    trajectory.push_back(d3);
    trajectory.push_back(d4);
    expected_right.push_back(trajectory);
    trajectory.clear();
    trajectory.push_back(d2);
    trajectory.push_back(d3);
    expected_right.push_back(trajectory);
    trajectory.clear();
    trajectory.push_back(d2);
    expected_right.push_back(trajectory);

    sort(left.begin(), left.end());
    sort(expected_left.begin(), expected_left.end());

    sort(right.begin(), right.end());
    sort(expected_right.begin(), expected_right.end());

    assert(left == expected_left);
    assert(right == expected_right);
  }
  void testTrajectoriesToFile() {
    std::vector<std::vector<Detection *> > trajectories;
    std::vector<Detection *> trajectory;
    Detection *d1, *d2, *d3;

    d1 = new Detection(Detection::NormalNode);
    d1->x = 0;
    d1->y = 0;
    d1->z = 0;
    d1->original_trajectory_id = 0;
    d1->frame_num = 0;
    d1->first_batch_frame = true;
    d1->last_batch_frame = false;
    d1->bbox_top = 10;
    d1->bbox_lft = 10;
    d1->bbox_rgt = 20;
    d1->bbox_bot = 20;
    d1->confidence = 0.3;

    d2 = new Detection(Detection::NormalNode);
    d2->x = 2;
    d2->y = 4;
    d2->z = 5;
    d2->original_trajectory_id = 0;
    d2->frame_num = 1;
    d2->first_batch_frame = false;
    d2->last_batch_frame = false;
    d2->bbox_top = 14;
    d2->bbox_lft = 16;
    d2->bbox_rgt = 24;
    d2->bbox_bot = 26;
    d2->confidence = 0.5;

    d3 = new Detection(Detection::NormalNode);
    d3->x = 3;
    d3->y = 6;
    d3->z = 7.5;
    d3->original_trajectory_id = 0;
    d3->frame_num = 1;
    d3->first_batch_frame = false;
    d3->last_batch_frame = true;
    d3->bbox_top = 16;
    d3->bbox_lft = 19;
    d3->bbox_rgt = 17;
    d3->bbox_bot = 29;
    d3->confidence = 0.6;
    trajectory.clear();
    trajectory.push_back(d1);
    trajectory.push_back(d2);
    trajectories.push_back(trajectory);
    trajectory.clear();
    trajectory.push_back(d3);
    trajectories.push_back(trajectory);

    Transformation::trajectoriesToFile(trajectories, "input.txt");
    FILE *fp = fopen("input.txt", "r");
    char buff[1111];
    string buffs, expected;
    fgets(buff, 1111, fp);
    buffs = std::string(buff);
    expected = "0,0,10,10,10,10,0.300000,0.000000,0.000000,0.000000";
    assert(buffs.size() >= expected.size() &&
           buffs.substr(0, expected.size()) == expected);
    fgets(buff, 1111, fp);
    buffs = std::string(buff);
    expected = "1,0,16,14,8,12,0.500000,2.000000,4.000000,5.000000";
    assert(buffs.size() >= expected.size() &&
           buffs.substr(0, expected.size()) == expected);
    fgets(buff, 1111, fp);
    buffs = std::string(buff);
    expected = "1,1,19,16,-2,13,0.600000,3.000000,6.000000,7.500000";
    assert(buffs.size() >= expected.size() &&
           buffs.substr(0, expected.size()) == expected);
    assert(!fgets(buff, 1111, fp));
    fclose(fp);
    system("rm input.txt");
  }
  void testPatternsToFile() {
    std::vector<std::pair<float, float> > points;
    points.push_back(std::make_pair(0., 0.));
    points.push_back(std::make_pair(1., 2.));
    CenterlineWithWidthPattern p1(Pattern::NormalPattern, points, 0.3, 0, 0, 0);
    CenterlineWithWidthPattern p2(Pattern::NoPattern, points, 0.3, 0, 0, 0);
    std::vector<Pattern *> patterns;
    patterns.push_back(&p1);
    patterns.push_back(&p2);
    Transformation::patternsToFile(patterns, "input.txt");
    FILE *fp = fopen("input.txt", "r");
    char buff[1111];
    string buffs, expected;
    fgets(buff, 1111, fp);
    buffs = std::string(buff);
    expected = "2 0.3 0 0 1 2";
    assert(buffs.size() >= expected.size() &&
           buffs.substr(0, expected.size()) == expected);
    fgets(buff, 1111, fp);
    buffs = std::string(buff);
    expected = "NoPattern";
    assert(buffs.size() >= expected.size() &&
           buffs.substr(0, expected.size()) == expected);
    assert(!fgets(buff, 1111, fp));
    fclose(fp);
    system("rm input.txt");
  }

  void testAssignmentToPatternsAssignmentToTrajectories() {
    std::vector<std::pair<std::vector<Detection *>, Pattern *> > assignment;
    std::vector<Detection *> trajectory;
    Detection *d1, *d2, *d3;

    d1 = new Detection(Detection::NormalNode);
    d1->x = 0;
    d1->y = 0;

    d2 = new Detection(Detection::NormalNode);
    d2->x = 2;
    d2->y = 4;

    d3 = new Detection(Detection::NormalNode);
    d3->x = 3;
    d3->y = 6;

    std::vector<std::pair<float, float> > points;
    points.push_back(std::make_pair(0., 0.));
    points.push_back(std::make_pair(1., 2.));
    CenterlineWithWidthPattern p1(Pattern::NormalPattern, points, 0.3, 0, 0, 0);
    CenterlineWithWidthPattern p2(Pattern::NoPattern, points, 0.3, 0, 0, 0);

    std::vector<std::vector<Detection *> > expected_trajectories;

    trajectory.clear();
    trajectory.push_back(d1);
    trajectory.push_back(d2);
    assignment.push_back(std::make_pair(trajectory, &p1));
    expected_trajectories.push_back(trajectory);
    trajectory.clear();
    trajectory.push_back(d3);
    assignment.push_back(std::make_pair(trajectory, &p2));
    expected_trajectories.push_back(trajectory);
    trajectory.clear();
    trajectory.push_back(d1);
    assignment.push_back(std::make_pair(trajectory, &p2));
    expected_trajectories.push_back(trajectory);

    std::vector<std::vector<Detection *> > obtained_trajectories =
        Transformation::assignmentToTrajectories(assignment);
    std::vector<Pattern *> obtained_patterns =
        Transformation::assignmentToPatterns(assignment);

    std::vector<Pattern *> expected_patterns;
    expected_patterns.push_back(&p1);
    expected_patterns.push_back(&p2);
    assert(obtained_patterns == expected_patterns);
    assert(obtained_trajectories == expected_trajectories);
  }
  void testAssignmentToFile() {
    std::vector<std::pair<std::vector<Detection *>, Pattern *> > assignment;
    std::vector<Detection *> trajectory;
    Detection *d1, *d2, *d3;

    d1 = new Detection(Detection::NormalNode);
    d1->x = 0;
    d1->y = 0;

    d2 = new Detection(Detection::NormalNode);
    d2->x = 2;
    d2->y = 4;

    d3 = new Detection(Detection::NormalNode);
    d3->x = 3;
    d3->y = 6;

    std::vector<std::pair<float, float> > points;
    points.push_back(std::make_pair(0., 0.));
    points.push_back(std::make_pair(1., 2.));
    CenterlineWithWidthPattern p1(Pattern::NormalPattern, points, 0.3, 0, 0, 0);
    CenterlineWithWidthPattern p2(Pattern::NoPattern, points, 0.3, 0, 0, 0);

    trajectory.clear();
    trajectory.push_back(d1);
    trajectory.push_back(d2);
    assignment.push_back(std::make_pair(trajectory, &p1));
    trajectory.clear();
    trajectory.push_back(d3);
    assignment.push_back(std::make_pair(trajectory, &p2));
    Transformation::assignmentToFile(assignment, "input.txt");

    FILE *fp = fopen("input.txt", "r");
    char buff[1111];
    string buffs, expected;
    fgets(buff, 1111, fp);
    buffs = std::string(buff);
    expected = "2 2";
    assert(buffs.size() >= expected.size() &&
           buffs.substr(0, expected.size()) == expected);
    fgets(buff, 1111, fp);
    buffs = std::string(buff);
    expected = "2 0.3 0 0 1 2";
    assert(buffs.size() >= expected.size() &&
           buffs.substr(0, expected.size()) == expected);
    fgets(buff, 1111, fp);
    buffs = std::string(buff);
    expected = "NoPattern";
    assert(buffs.size() >= expected.size() &&
           buffs.substr(0, expected.size()) == expected);
    fgets(buff, 1111, fp);
    buffs = std::string(buff);
    expected = "2 0 0.000000 0.000000 2.000000 4.000000";
    assert(buffs.size() >= expected.size() &&
           buffs.substr(0, expected.size()) == expected);
    fgets(buff, 1111, fp);
    buffs = std::string(buff);
    expected = "1 1 3.000000 6.000000";
    assert(buffs.size() >= expected.size() &&
           buffs.substr(0, expected.size()) == expected);
    assert(!fgets(buff, 1111, fp));
    fclose(fp);
    system("rm input.txt");
  }

  void testFileToTrajectories() {
    FILE *fp = fopen("input.txt", "w");
    fprintf(fp, "1,1,10,10,20,20,-1,0,0,0\n");
    fprintf(fp, "2,1,11,11,19,19,-1,0.5,0.3,0\n");
    fprintf(fp, "4,1,13,13,16,16,-1,0.8,0.2,0\n");
    fprintf(fp, "4,2,14,15,16,17,-1,0.1,0.2,0\n");
    fprintf(fp, "5,2,18,19,20,21,-1,0.3,0.4,0\n");
    fclose(fp);
    std::vector<std::vector<Detection *> > result;

    // One
    result = Transformation::fileToTrajectories("input.txt");
    assert(result.size() == 2);
    assert(result[0].size() == 4);
    assert(fabs(result[0][0]->x - 0) < 1e-6 &&
           fabs(result[0][0]->y - 0) < 1e-6 &&
           fabs(result[0][0]->z - 0) < 1e-6 &&
           result[0][0]->original_trajectory_id == 1 &&
           result[0][0]->frame_num == 1 && result[0][0]->bbox_lft == 10 &&
           result[0][0]->bbox_top == 10 && result[0][0]->bbox_rgt == 30 &&
           result[0][0]->bbox_bot == 30 &&
           fabs(result[0][0]->confidence - -1) < 1e-6 &&
           result[0][0]->first_batch_frame == true &&
           result[0][0]->last_batch_frame == false);
    assert(fabs(result[0][1]->x - 0.5) < 1e-6 &&
           fabs(result[0][1]->y - 0.3) < 1e-6 &&
           fabs(result[0][1]->z - 0) < 1e-6 &&
           result[0][1]->original_trajectory_id == 1 &&
           result[0][1]->frame_num == 2 && result[0][1]->bbox_lft == 11 &&
           result[0][1]->bbox_top == 11 && result[0][1]->bbox_rgt == 30 &&
           result[0][1]->bbox_bot == 30 &&
           fabs(result[0][1]->confidence - -1) < 1e-6 &&
           result[0][1]->first_batch_frame == false &&
           result[0][1]->last_batch_frame == false);
    assert(fabs(result[0][2]->x - 0.65) < 1e-6 &&
           fabs(result[0][2]->y - 0.25) < 1e-6 &&
           fabs(result[0][2]->z - 0) < 1e-6 &&
           result[0][2]->original_trajectory_id == 1 &&
           result[0][2]->frame_num == 3 && result[0][2]->bbox_lft == 12 &&
           result[0][2]->bbox_top == 12 && result[0][2]->bbox_rgt == 29 &&
           result[0][2]->bbox_bot == 29 &&
           fabs(result[0][2]->confidence - -1) < 1e-6 &&
           result[0][2]->first_batch_frame == false &&
           result[0][2]->last_batch_frame == false);
    assert(fabs(result[0][3]->x - 0.8) < 1e-6 &&
           fabs(result[0][3]->y - 0.2) < 1e-6 &&
           fabs(result[0][3]->z - 0) < 1e-6 &&
           result[0][3]->original_trajectory_id == 1 &&
           result[0][3]->frame_num == 4 && result[0][3]->bbox_lft == 13 &&
           result[0][3]->bbox_top == 13 && result[0][3]->bbox_rgt == 29 &&
           result[0][3]->bbox_bot == 29 &&
           fabs(result[0][3]->confidence - -1) < 1e-6 &&
           result[0][3]->first_batch_frame == false &&
           result[0][3]->last_batch_frame == false);
    assert(result[1].size() == 2);
    assert(fabs(result[1][0]->x - 0.1) < 1e-6 &&
           fabs(result[1][0]->y - 0.2) < 1e-6 &&
           fabs(result[1][0]->z - 0) < 1e-6 &&
           result[1][0]->original_trajectory_id == 2 &&
           result[1][0]->frame_num == 4 && result[1][0]->bbox_lft == 14 &&
           result[1][0]->bbox_top == 15 && result[1][0]->bbox_rgt == 30 &&
           result[1][0]->bbox_bot == 32 &&
           fabs(result[1][0]->confidence - -1) < 1e-6 &&
           result[1][0]->first_batch_frame == false &&
           result[1][0]->last_batch_frame == false);
    assert(fabs(result[1][1]->x - 0.3) < 1e-6 &&
           fabs(result[1][1]->y - 0.4) < 1e-6 &&
           fabs(result[1][1]->z - 0) < 1e-6 &&
           result[1][1]->original_trajectory_id == 2 &&
           result[1][1]->frame_num == 5 && result[1][1]->bbox_lft == 18 &&
           result[1][1]->bbox_top == 19 && result[1][1]->bbox_rgt == 38 &&
           result[1][1]->bbox_bot == 40 &&
           fabs(result[1][1]->confidence - -1) < 1e-6 &&
           result[1][1]->first_batch_frame == false &&
           result[1][1]->last_batch_frame == true);
    system("rm input.txt");
    // Four
    result = Transformation::fileToTrajectories(
        "data/towncentre/towncentre_001_150/gt/gt.txt");
    assert(result.size() == 85);
  }
  void testTransitionsToTrajectories() {
    std::vector<Transition> transitions;
    Pattern *pattern1, *pattern2;
    Detection d1, d2, d3, d4;
    pattern1 = new HardcodedPattern(
        Pattern::NormalPattern,
        std::vector<std::pair<std::pair<Detection *, Detection *>, float> >(0),
        0);
    pattern2 = new HardcodedPattern(
        Pattern::NoPattern,
        std::vector<std::pair<std::pair<Detection *, Detection *>, float> >(0),
        0);
    transitions.push_back(Transition(&Detection::Source, &d1, pattern1));
    transitions.push_back(Transition(&d1, &d2, pattern1));
    transitions.push_back(Transition(&d2, &Detection::Sink, pattern1));
    transitions.push_back(Transition(&Detection::Source, &d3, pattern1));
    transitions.push_back(Transition(&d3, &d4, pattern1));
    transitions.push_back(Transition(&d4, &Detection::Sink, pattern1));
    transitions.push_back(Transition(&Detection::Source, &d1, pattern2));
    transitions.push_back(Transition(&d1, &d4, pattern2));
    transitions.push_back(Transition(&d4, &Detection::Sink, pattern2));
    transitions.push_back(Transition(&Detection::Source, &d3, pattern2));
    transitions.push_back(Transition(&d3, &d2, pattern2));
    transitions.push_back(Transition(&d2, &Detection::Sink, pattern2));

    std::vector<std::vector<Detection *> > result, expected_result;
    std::vector<std::pair<std::vector<Detection *>, Pattern *> > assignment,
        expected_assignment;
    std::vector<Detection *> trajectory;
    // One
    result = Transformation::transitionsToTrajectories(transitions, false,
                                                       &assignment);
    expected_result.clear();
    expected_assignment.clear();
    trajectory.clear();
    trajectory.push_back(&d1);
    trajectory.push_back(&d2);
    expected_result.push_back(trajectory);
    expected_assignment.push_back(std::make_pair(trajectory, pattern1));
    trajectory.clear();
    trajectory.push_back(&d3);
    trajectory.push_back(&d4);
    expected_result.push_back(trajectory);
    expected_assignment.push_back(std::make_pair(trajectory, pattern1));
    trajectory.clear();
    trajectory.push_back(&d1);
    trajectory.push_back(&d4);
    expected_result.push_back(trajectory);
    expected_assignment.push_back(std::make_pair(trajectory, pattern2));
    trajectory.clear();
    trajectory.push_back(&d3);
    trajectory.push_back(&d2);
    expected_result.push_back(trajectory);
    expected_assignment.push_back(std::make_pair(trajectory, pattern2));

    sort(result.begin(), result.end());
    sort(expected_result.begin(), expected_result.end());
    assert(result == expected_result);
    sort(assignment.begin(), assignment.end());
    sort(expected_assignment.begin(), expected_assignment.end());
    assert(assignment == expected_assignment);
    // Two
    result = Transformation::transitionsToTrajectories(transitions, true,
                                                       &assignment);
    expected_result.clear();
    expected_assignment.clear();
    trajectory.clear();
    trajectory.push_back(&d1);
    trajectory.push_back(&d2);
    expected_result.push_back(trajectory);
    expected_assignment.push_back(std::make_pair(trajectory, pattern1));
    trajectory.clear();
    trajectory.push_back(&d3);
    trajectory.push_back(&d4);
    expected_result.push_back(trajectory);
    expected_assignment.push_back(std::make_pair(trajectory, pattern1));

    sort(result.begin(), result.end());
    sort(expected_result.begin(), expected_result.end());
    assert(result == expected_result);
    sort(assignment.begin(), assignment.end());
    sort(expected_assignment.begin(), expected_assignment.end());
    assert(assignment == expected_assignment);
    // Three
    result = Transformation::transitionsToTrajectories(transitions, true, NULL);
    expected_result.clear();
    trajectory.clear();
    trajectory.push_back(&d1);
    trajectory.push_back(&d2);
    expected_result.push_back(trajectory);
    trajectory.clear();
    trajectory.push_back(&d3);
    trajectory.push_back(&d4);
    expected_result.push_back(trajectory);

    sort(result.begin(), result.end());
    sort(expected_result.begin(), expected_result.end());
    assert(result == expected_result);
  }

  void testFileToPatterns() {
    FILE *fp = fopen("input.txt", "w");
    fprintf(fp, "2 0.3 0 0 1 2\n");
    fprintf(fp, "NoPattern\n");
    fclose(fp);
    std::vector<Pattern *> patterns =
        Transformation::fileToPatterns("input.txt");
    assert(patterns.size() == 2);
    CenterlineWithWidthPattern *p0 =
        dynamic_cast<CenterlineWithWidthPattern *>(patterns[0]);
    CenterlineWithWidthPattern *p1 =
        dynamic_cast<CenterlineWithWidthPattern *>(patterns[1]);

    assert(p0->pattern_type == Pattern::NormalPattern);
    assert(fabs(p0->width - 0.3) < 1e-6);
    assert(p0->points.size() == 2);
    assert(fabs(p0->points[0].first - 0) < 1e-6);
    assert(fabs(p0->points[0].second - 0) < 1e-6);
    assert(fabs(p0->points[1].first - 1) < 1e-6);
    assert(fabs(p0->points[1].second - 2) < 1e-6);
    assert(p1->pattern_type == Pattern::NoPattern);
  }

  void testTrajectoriesToContinuousTrajectories() {
    std::vector<std::vector<Detection *> > trajectories;
    std::vector<Detection *> trajectory;
    Detection *d1, *d2, *d3;

    d1 = new Detection(Detection::NormalNode);
    d1->x = 0;
    d1->y = 0;
    d1->z = 0;
    d1->original_trajectory_id = 0;
    d1->frame_num = 0;
    d1->first_batch_frame = true;
    d1->last_batch_frame = false;
    d1->bbox_top = 10;
    d1->bbox_lft = 10;
    d1->bbox_rgt = 20;
    d1->bbox_bot = 20;
    d1->confidence = 0.3;

    d2 = new Detection(Detection::NormalNode);
    d2->x = 2;
    d2->y = 4;
    d2->z = 5;
    d2->original_trajectory_id = 0;
    d2->frame_num = 2;
    d2->first_batch_frame = false;
    d2->last_batch_frame = false;
    d2->bbox_top = 14;
    d2->bbox_lft = 16;
    d2->bbox_rgt = 24;
    d2->bbox_bot = 26;
    d2->confidence = 0.5;

    d3 = new Detection(Detection::NormalNode);
    d3->x = 3;
    d3->y = 6;
    d3->z = 7.5;
    d3->original_trajectory_id = 0;
    d3->frame_num = 3;
    d3->first_batch_frame = false;
    d3->last_batch_frame = true;
    d3->bbox_top = 16;
    d3->bbox_lft = 19;
    d3->bbox_rgt = 17;
    d3->bbox_bot = 29;
    d3->confidence = 0.6;

    trajectory.clear();
    trajectory.push_back(d1);
    trajectory.push_back(d2);
    trajectory.push_back(d3);
    trajectories.push_back(trajectory);
    trajectory.clear();
    trajectory.push_back(d1);
    trajectory.push_back(d3);
    trajectories.push_back(trajectory);

    std::vector<std::vector<Detection *> > result =
        Transformation::trajectoriesToContinuousTrajectories(trajectories);

    assert(result.size() == 2);
    assert(result[0].size() == 4);
    assert(result[0][0] == d1);
    assert(fabs(result[0][1]->x - 1) < 1e-6 &&
           fabs(result[0][1]->y - 2) < 1e-6 &&
           fabs(result[0][1]->z - 2.5) < 1e-6 &&
           result[0][1]->original_trajectory_id == 0 &&
           result[0][1]->frame_num == 1 && result[0][1]->bbox_top == 12 &&
           result[0][1]->bbox_lft == 13 && result[0][1]->bbox_rgt == 22 &&
           result[0][1]->bbox_bot == 23 &&
           fabs(result[0][1]->confidence - 0.4) < 1e-6 &&
           result[0][1]->first_batch_frame == false &&
           result[0][1]->last_batch_frame == false);
    assert(result[0][2] == d2);
    assert(result[0][3] == d3);

    assert(result[1].size() == 4);
    assert(result[1][0] == d1);

    assert(fabs(result[1][1]->x - 1) < 1e-6 &&
           fabs(result[1][1]->y - 2) < 1e-6 &&
           fabs(result[1][1]->z - 2.5) < 1e-6 &&
           result[1][1]->original_trajectory_id == 0 &&
           result[1][1]->frame_num == 1 && result[1][1]->bbox_top == 12 &&
           result[1][1]->bbox_lft == 13 && result[1][1]->bbox_rgt == 19 &&
           result[1][1]->bbox_bot == 23 &&
           fabs(result[1][1]->confidence - 0.4) < 1e-6 &&
           result[1][1]->first_batch_frame == false &&
           result[1][1]->last_batch_frame == false);

    assert(fabs(result[1][2]->x - 2) < 1e-6 &&
           fabs(result[1][2]->y - 4) < 1e-6 &&
           fabs(result[1][2]->z - 5) < 1e-6 &&
           result[1][2]->original_trajectory_id == 0 &&
           result[1][2]->frame_num == 2 && result[1][2]->bbox_top == 14 &&
           result[1][2]->bbox_lft == 16 && result[1][2]->bbox_rgt == 18 &&
           result[1][2]->bbox_bot == 26 &&
           fabs(result[1][2]->confidence - 0.5) < 1e-6 &&
           result[1][2]->first_batch_frame == false &&
           result[1][2]->last_batch_frame == false);
    assert(result[1][3] == d3);
  }
};
#endif  // CODE_INCLUDE_TEST_TRANSFORMATIONTEST_H_

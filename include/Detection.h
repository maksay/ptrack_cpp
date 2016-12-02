/*
 * Copyright 2016 Andrii Maksai
 */
#ifndef CODE_INCLUDE_DETECTION_H_
#define CODE_INCLUDE_DETECTION_H_
struct Detection {
 public:
  enum NodeTypes { NormalNode, SourceNode, SinkNode };
  NodeTypes node_type;
  Detection() : node_type(NormalNode) {}
  explicit Detection(NodeTypes _node_type) : node_type(_node_type) {}

  float x, y, z;
  int original_trajectory_id;
  int frame_num;
  bool first_batch_frame;
  bool last_batch_frame;

  int bbox_top;
  int bbox_lft;
  int bbox_bot;
  int bbox_rgt;

  float confidence;

  static Detection Source;
  static Detection Sink;
};
#endif  // CODE_INCLUDE_DETECTION_H_

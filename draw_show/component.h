// Copyright 2018 Shanghai Fangling Software Co., Ltd. All Rights Reserved.
// Author: Yalong Ye

#ifndef COMPONENT_H__
#define COMPONENT_H__

#include "opencv2/opencv.hpp"

#include <iostream>

#include "gcodeparse/GCommand.h"
#include "gcodeparse/GCodeParse.h"
#include "obase/GCodeTypeDef.h"

using namespace std;
using namespace cv;
using namespace gcode;

//Analyze component drawing.
class Component {
 public:
  Component();
  Component(const Component &other);
  //Analysis gcode and draw.
  bool LoadComponent(std::string path);
  //Set rotate angle.
  int SetR(double R);
  //Get offset.
  int SetOffset(double x_offset, double y_offset);
  //Set User units.
  int SetUnit(double unit_1mm_to_pixs);
  //Get original mat
  Mat &GetMatOriginal() {return m_img_original_;}
  //Get rotate mat
  Mat &GetMatRotate() {return m_img_rotate_;}

  //Rotate angle.
  double R_;
  //x offset.
  double x_offset_;
  //y offset.
  double y_offset_;
  //User units.
  double unit_1mm_to_pixs_;
  //Mat height mm.
  double height_;
  //Mat height mm.
  double width_;
  //Rotate center for x mm.
  double x_center_;
  //Rotate center for y mm.
  double y_center_;
  Mat m_img_, m_img_original_, m_img_rotate_;
  GCodeParse gcode_parse_;

 private:
  inline void ComparePoint(double x, double y);
  inline void UpdateMatInfo();
  inline void DrawLine(GCommand &cmdline);
  inline void DrawCycle(GCommand &cmdcycle);
  inline void DrawLineRotate(GCommand &cmdline);
  inline void DrawCycleRotate(GCommand &cmdcycle);
  inline void CoordinateLine(GCommand &cmdline);
  inline void CoordinateCycle(GCommand &cmdcycle);
  inline void Draw();
  inline void DrawRotate();


  double x_min_;
  double x_max_;
  double y_min_;
  double y_max_;
  Point2d point_start_;
  Point2d point_end_;
  Point2d centerpoint_;
  Size ellsize_;

  std::vector<GCommand> gcode_cmds_;
};

#endif // COMPONENT_H__
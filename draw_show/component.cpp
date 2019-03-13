// Copyright 2018 Shanghai Fangling Software Co., Ltd. All Rights Reserved.
// Author: Yalong Ye

#include "Component.h"

#include <iostream>

using namespace std;
using namespace gcode;

Component::Component()
  : R_(0.0),
    x_offset_(0.0),
    y_offset_(0.0),
    unit_1mm_to_pixs_(1.0),
    m_img_(3000, 3000, CV_32FC3){}

Component::Component(const Component &other) {
  R_ = other.R_;
  x_offset_ = other.x_offset_;
  y_offset_ = other.y_offset_;
  unit_1mm_to_pixs_ = other.unit_1mm_to_pixs_;
}

void Component::ComparePoint(double x, double y) {
  x_min_ = (x < x_min_) ? x : x_min_;
  x_max_ = (x > x_max_) ? x : x_max_;
  y_min_ = (y < y_min_) ? y : y_min_;
  y_max_ = (y > y_max_) ? y : y_max_;
}

void Component::UpdateMatInfo() {
  x_min_ = 0.;
  x_max_ = 0.;
  y_min_ = 0.;
  y_max_ = 0.;
  for (int i = 0; i < gcode_cmds_.size(); i++) {
    if (gcode_cmds_[i].name_ == G01 || gcode_cmds_[i].name_ == G02 || gcode_cmds_[i].name_ == G03) {
      ComparePoint(gcode_cmds_[i].x_, gcode_cmds_[i].y_);
      ComparePoint(gcode_cmds_[i].x0_, gcode_cmds_[i].y0_);
      ComparePoint(gcode_cmds_[i].i_ + gcode_cmds_[i].r_, gcode_cmds_[i].j_ + gcode_cmds_[i].r_);
      ComparePoint(gcode_cmds_[i].i_ - gcode_cmds_[i].r_, gcode_cmds_[i].j_ - gcode_cmds_[i].r_);
    }
  }
  height_ = x_max_ - x_min_;
  width_ = y_max_ - y_min_;
  x_center_ = height_ / 2.0;
  y_center_ = width_ / 2.0;
}

bool Component::LoadComponent(std::string path) {
  //Original gcode.
  gcode_parse_.ToStdGCode(path);
  gcode_cmds_ = gcode_parse_.GetCommandVec(0);
  // Modify center position.
  UpdateMatInfo();
  gcode_parse_.ChangeOrigin(x_center_, y_center_);
  //Gcode after adjusting the center position.
  gcode_cmds_ = gcode_parse_.GetCommandVec(0);
  // Draw.
  std::cout << "Original center (mm) is:  " << "(" << x_center_ << "," << y_center_ << ")" << std::endl;
  std::cout << "Original height_ and width_ (mm) is:  " << height_ << ", " << width_ << std::endl;
  Draw();
  return true;
}

int Component::SetR(double R){
  R_ = R;
  GCodeParse gcode_parse = gcode_parse_;
  // Set r
  gcode_parse.RotateGCommandVec(R);
  // Update info
  gcode_cmds_ = gcode_parse.GetCommandVec(0);
  UpdateMatInfo();
  // Draw
  std::cout << "Rotation center (mm) is:  " << "(" << x_center_ << "," << y_center_ << ")" << std::endl;
  std::cout << "Rotation height_ and width_ (mm) is:  " << height_ << ", " << width_ << std::endl;
  DrawRotate();
  return 0;
}

int Component::SetOffset(double x_offset, double y_offset) {
  x_offset_ = x_offset;
  y_offset_ = y_offset;
  return 0;
}

int Component::SetUnit(double unit_1mm_to_pixs) {
  unit_1mm_to_pixs_ = unit_1mm_to_pixs;
  return 0;
}

void Component::Draw() {
  resize(m_img_, m_img_original_, Size((height_ + 1) * unit_1mm_to_pixs_, (width_ + 1) * unit_1mm_to_pixs_), 0, 0, INTER_LINEAR);
  for (int i = 0; i < gcode_cmds_.size(); i++) {
    if (gcode_cmds_[i].name_ == G01) {
      this->DrawLine(gcode_cmds_[i]);
    }else if (gcode_cmds_[i].name_ == G02 || gcode_cmds_[i].name_ == G03) {
      this->DrawCycle(gcode_cmds_[i]);
    }
  }
  std::cout << "Original center (pix) is:  " << "(" << (int)m_img_original_.cols / 2 << "," << (int)m_img_original_.rows / 2 << ")" << std::endl;
  std::cout << "Original height_ and width_ (pix) is:  " << m_img_original_.cols << ", " << m_img_original_.rows << std::endl;
  //namedWindow("ShowImage", WINDOW_NORMAL);
  imshow("ShowImage", m_img_original_);
  waitKey(0);
}

void Component::DrawRotate() {
  resize(m_img_, m_img_rotate_, Size((height_ + 1) * unit_1mm_to_pixs_, (width_ + 1) * unit_1mm_to_pixs_), 0, 0, INTER_LINEAR);
  for (int i = 0; i < gcode_cmds_.size(); i++) {
    if (gcode_cmds_[i].name_ == G01) {
      this->DrawLineRotate(gcode_cmds_[i]);
    }else if (gcode_cmds_[i].name_ == G02 || gcode_cmds_[i].name_ == G03) {
      this->DrawCycleRotate(gcode_cmds_[i]);
    }
  }
  std::cout << "Rotation center (pix) is:  " << "(" << (int)m_img_rotate_.cols / 2 << "," << (int)m_img_rotate_.rows / 2 << ")" << std::endl;
  std::cout << "Rotation height_ and width_ (pix) is:  " << m_img_rotate_.cols << "," << m_img_rotate_.rows << std::endl;
  //namedWindow("ShowImage", WINDOW_NORMAL);
  imshow("ShowImage", m_img_rotate_);
  waitKey(0);
}

void Component::CoordinateLine(GCommand &cmdline){
  point_start_.x = (cmdline.x0_ + x_center_) * unit_1mm_to_pixs_;
  point_start_.y = (cmdline.y0_ + y_center_) * unit_1mm_to_pixs_;
  point_end_.x = (cmdline.x_ + x_center_) * unit_1mm_to_pixs_;
  point_end_.y = (cmdline.y_ + y_center_) * unit_1mm_to_pixs_;
}

void Component::CoordinateCycle(GCommand &cmdcycle){
  centerpoint_.x = (cmdcycle.i_ + x_center_) * unit_1mm_to_pixs_;
  centerpoint_.y = (cmdcycle.j_ + y_center_) * unit_1mm_to_pixs_;
}

void Component::DrawLine(GCommand &cmdline) {
  CoordinateLine(cmdline);
  line(m_img_original_, point_start_, point_end_, Scalar(0, 255, 255), 4, 8, 0);
}

void Component::DrawCycle(GCommand &cmdcycle) {
  CoordinateCycle(cmdcycle);
  ellsize_.height = cmdcycle.r_ * unit_1mm_to_pixs_;
  ellsize_.width = cmdcycle.r_ * unit_1mm_to_pixs_;
  cmdcycle.r_ = cmdcycle.r_ * unit_1mm_to_pixs_;
  double start_angle = atan2(cmdcycle.y0_ - cmdcycle.j_, cmdcycle.x0_ - cmdcycle.i_) * 180 / PI;
  double angle_fir = atan2(cmdcycle.y_ - cmdcycle.j_, cmdcycle.x_ - cmdcycle.i_);
  double angle_sec = atan2(cmdcycle.y0_ - cmdcycle.j_, cmdcycle.x0_ - cmdcycle.i_);
  double number_angle = (angle_fir-angle_sec) * 180 / PI;
  if ((cmdcycle.x_ == cmdcycle.x0_) && (cmdcycle.y_ == cmdcycle.y0_)){
    circle(m_img_original_, centerpoint_, cmdcycle.r_, Scalar(0, 255, 0), 4, 8, 0);
  }else if(cmdcycle.name_ == G02){
    ellipse(m_img_original_, centerpoint_, ellsize_, 0, start_angle, start_angle + number_angle, Scalar(0, 255, 0), 4, 8, 0);
  }else if(cmdcycle.name_ == G03){
    ellipse(m_img_original_, centerpoint_, ellsize_, 180, start_angle, start_angle + number_angle, Scalar(0, 255, 0), 4, 8, 0);
  }
}

void Component::DrawLineRotate(GCommand &cmdline) {
  CoordinateLine(cmdline);
  line(m_img_rotate_, point_start_, point_end_, Scalar(0, 255, 255), 4, 8, 0);
}

void Component::DrawCycleRotate(GCommand &cmdcycle) {
  CoordinateCycle(cmdcycle);
  ellsize_.height = cmdcycle.r_ * unit_1mm_to_pixs_;
  ellsize_.width = cmdcycle.r_ * unit_1mm_to_pixs_;
  cmdcycle.r_ = cmdcycle.r_ * unit_1mm_to_pixs_;
  double start_angle = atan2(cmdcycle.y0_ - cmdcycle.j_, cmdcycle.x0_ - cmdcycle.i_) * 180 / PI;
  double angle_fir = atan2(cmdcycle.y_ - cmdcycle.j_, cmdcycle.x_ - cmdcycle.i_);
  double angle_sec = atan2(cmdcycle.y0_ - cmdcycle.j_, cmdcycle.x0_ - cmdcycle.i_);
  double number_angle = (angle_fir-angle_sec) * 180 / PI;
  if ((cmdcycle.x_ == cmdcycle.x0_) && (cmdcycle.y_ == cmdcycle.y0_)){
    circle(m_img_rotate_, centerpoint_, cmdcycle.r_, Scalar(0, 255, 0), 4, 8, 0);
  }else if(cmdcycle.name_ == G02){
    ellipse(m_img_rotate_, centerpoint_, ellsize_, 0, start_angle, start_angle + number_angle, Scalar(0, 255, 0), 4, 8, 0);
  }else if(cmdcycle.name_ == G03){
    ellipse(m_img_rotate_, centerpoint_, ellsize_, 180, start_angle, start_angle + number_angle, Scalar(0, 255, 0), 4, 8, 0);
  }
}

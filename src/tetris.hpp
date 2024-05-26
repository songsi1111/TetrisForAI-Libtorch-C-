#pragma once

#include <memory>
#include <cstdint>
#include <cmath>
#include <string>
#include <iostream>
#include <vector>
//宏命名冲突
#ifdef slots
#undef slots
#endif

#ifdef TORCH_CHECK
#undef TORCH_CHECK
#endif

struct Shape{
  Shape()=default;
  Shape(const char& name, const std::vector<uint32_t>& form) : _name(name), _form(form) {}
  char getName() const { return _name; }
  std::vector<u_int32_t> getForm() const { return _form; }
private:
  char _name;             // 形状名称
  std::vector<uint32_t> _form;      // 形状元组数据
};
using Shapes=std::vector<Shape>;
struct Coords {
    std::uint32_t x, y;
};
class Tetris{
public:
  static const std::uint32_t lines   {20};
  static const std::uint32_t cols    {10};
  static const std::uint32_t squares {4};
  static const std::uint32_t scores  {100};
  const Shapes shapes={
    {'I',{1,5,3,7}}, // I
    {'Z',{2,5,4,7}}, // Z
    {'S',{3,5,4,6}}, // S
    {'T',{3,5,4,7}}, // T
    {'L',{2,5,3,7}}, // L
    {'J',{3,5,7,6}}, // J
    {'O',{2,3,4,5}}, // O
  };

  std::vector<std::vector<std::uint32_t>> area;

  Coords z[squares], k[squares];
  Shape curShape; //当前下落的形状


  int dirx, color, score;
  bool rotate, gameover;
  float timercount, delay;
  int cleared_lines;//消除总行数
  int shape_num;//出现形状的总数

  public:
    void events();
    // void draw(QPainter& painter, const QVector<QVector<int>>& all_blocks, const QVector<QPair<int, int>>& moving_blocks, int moving_color, 
    //       const QVector<QImage>& pics, int scores, int pieces, int lines);
    void moveToDown();
    void setRotate();
    void resetValues();
    void changePosition();
    bool inLimit();
    void UpdateNewShape();
    // void load_pics(QVector<QImage>& images);

  public:
    Tetris();
    ~Tetris();
    void run();
    virtual void reset() ;
    int setScore(); //执行消除，设置游戏结束；返回被消除的行数
    
};





#include <iostream>
#include <vector>
#include <deque>
#include <random>
#include <algorithm>
#include <cstdlib>
#include <tuple>
#include <string>

#include "tetris.hpp"
#include "DQN.hpp"
#include "torch/torch.h"

static const float learning_rate = 1e-3;
static const int replaybuffer_maxsize=30000;
static const int batch_size=512;
static const int epochs=5000;
static const int decay_epochs=4000;
static const float initial_epsilon = 1;
static const float final_epsilon = 1e-3;
static const float _gamma=0.99;
static const char* save_path = "trained_models";
static const int save_interval = 1000;

static std::default_random_engine ram(time(NULL));

using State=torch::Tensor;
using Action=std::pair<int,int>;//可能的动作：旋转次数、X坐标
using Mapping=std::map<Action,State>;
struct Exp {
  public:
    State state;
    int reward;
    State next_state;
    bool done;
  public:
    Exp(State s,int r,State n_s,bool d){state=s;reward=r;next_state=n_s;done=d;}
};
using ReplayBuffer=std::vector<Exp>;//经验池：state, reward, next_state, done

class TetrisForAI:public Tetris{
  public:
  TetrisForAI(TetrisForAI& t);
  TetrisForAI& operator=(const TetrisForAI& t);
  TetrisForAI();
  ~TetrisForAI() {
      // // 释放area中的内存
      // area.clear();
      // // 释放图形资源
      // window.reset();
      // sprite.reset();
      // background.reset();
      // // 关闭窗口（如果适用）
      // if (window && window->isOpen()) {
      //   window->close();
      // }
    }
  public:
    int get_holes(); //返回空洞数量
    std::pair<int,int> get_bumpiness_and_height(); //返回凸起度和高度 

    State get_state_prperties();
    void setRotate() ;
    void moveToBottom();
    Mapping get_next_states();
    
    std::pair<int, int> get_shape_width();
    // 单步进行游戏
    std::pair<int,bool> step(Action& action);
    void reset();
   
};
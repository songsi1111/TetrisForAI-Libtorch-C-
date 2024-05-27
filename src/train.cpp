#include "train.hpp"
// TetrisForAI类
TetrisForAI::TetrisForAI(){
  area.resize(lines);
  for (std::size_t i {}; i < area.size(); ++i) {
    area[i].resize(cols); 
  }

  dirx = score = {0};
  rotate = gameover = {false};
  timercount = {0.f};
  delay = {0.3f};
  color = {1};
  
  UpdateNewShape();

}
TetrisForAI::TetrisForAI(TetrisForAI& t){
 for(int i=0;i<t.area.size();++i){
    for(int j=0;j<t.area[0].size();++j){
        area[i][j]=t.area[i][j];
    }
  }
  curShape=t.curShape;
  color=t.color;
  for(int i=0;i<t.squares;++i){
    z[i]=t.z[i];
  }
  for(int i=0;i<t.squares;++i){
    k[i]=t.k[i];
  }
  gameover=t.gameover;
  dirx=t.dirx;
  color=t.color;
  score=t.score; 
  shape_num=t.shape_num;
  cleared_lines=t.cleared_lines;
}
//游戏的赋值
TetrisForAI& TetrisForAI::operator=(const TetrisForAI& t){
  for(int i=0;i<t.area.size();++i){
    for(int j=0;j<t.area[0].size();++j){
        area[i][j]=t.area[i][j];
    }
  }
  curShape=t.curShape;
  for(int i=0;i<t.squares;++i){
    z[i]=t.z[i];
  }
  for(int i=0;i<t.squares;++i){
    k[i]=t.k[i];
  }
  gameover=t.gameover;
  dirx=t.dirx;
  color=t.color;
  score=t.score; 
  shape_num=t.shape_num;
  cleared_lines=t.cleared_lines;
  
  return *this;
}
//找到第一个非零元素所在的行，并从该行的下一行开始统计空洞的数量
int TetrisForAI::get_holes(){
  int num_holes = 0;
  for(uint32_t col=0;col<cols;++col){
    int line=1;
    while(line<area.size() && area[line][col]==0){
      line++;
    }
    for(int i=line+1;i<area.size();i++){
      if(area[i][col]==0){
        num_holes++;
      }
    }
  }
  return num_holes;
}
//返回凸起度和高度 
std::pair<int,int> TetrisForAI::get_bumpiness_and_height(){
  int bumpiness = 0,height = 0;
  std::vector<int> heights(cols, 0);
  for(uint32_t col=0;col<cols;++col){
      for (std::size_t i = 1; i < lines; ++i) {
        if(area[i][col]!=0){
          heights[col]=lines-i;
		  break;
        }
      }
      height += heights[col];

  }
  for (uint32_t i = 0; i + 1 < cols; ++i) {
        bumpiness += std::abs(heights[i] - heights[i + 1]);
    }
  return std::make_pair(bumpiness, height);
}
//获取状态数据
State TetrisForAI::get_state_prperties(){
    int lines_cleared=setScore();
    int holes=get_holes();
    auto h_pair=get_bumpiness_and_height();
    int bumpiness=h_pair.first;
    int height=h_pair.second;
    // Normalize the features
    float normalized_lines_cleared = lines_cleared / static_cast<float>(lines);
    float normalized_holes = holes / static_cast<float>(cols * lines);
    float normalized_bumpiness = bumpiness / static_cast<float>(cols * lines);
    float normalized_height = height / static_cast<float>(cols*lines);
    State state = torch::tensor({lines_cleared,holes,bumpiness,height},torch::kFloat32);
    return state;
}
//获得当前下落形状的横向宽度
std::pair<int,int> TetrisForAI::get_shape_width(){
    int core=z[1].x;
    int min=z[0].x;
    int max=z[0].x;
    for(int i=1;i<squares;i++){
        max=z[i].x>max?z[i].x:max;
        min=z[i].x<min?z[i].x:min;
    }
    std::pair<int,int> pair = std::make_pair(core-min,max-core);
    return pair;
}
//获得所有行动将导致的下一状态，存放在MAP中
Mapping TetrisForAI::get_next_states(){
  Mapping states_map;
  int rotations;
  switch(curShape.getName()){
    case 'O':
        rotations=1;break;
    case 'Z':
    case 'S':
    case 'I':
        rotations=2;break;
    case 'T':
    case 'L':
    case 'J':
        rotations=4;break;
    default:
        rotations=1;break;
  }
  //auto Game=TetrisForAI(*this);
  //Problem Here
  TetrisForAI Game(*this);
  //外层根据旋转次数迭代
  for(int r=0;r<rotations;++r){
    bool CanMoveUp=true;
    while(CanMoveUp){
        for(int i=0;i<squares;++i){
            if(Game.z[i].y<1){
                CanMoveUp=false;
                break;
            }
        }
        if(CanMoveUp){
            for(int i=0;i<squares;++i){
                Game.z[i].y-=1;
        }
        }
    }
    auto shape_width=Game.get_shape_width();
    int left=shape_width.first;
    int right=shape_width.second;
    //Problem
    //内层根据x轴坐标范围迭代
    for(int x=left;x<cols-right;++x){
        TetrisForAI Save=Game;
        //根据x重新设置形状位置
        int dias=Game.z[1].x-x;
        for(int i=0;i<squares;++i){
            Game.z[i].x-=dias;
        }
        //当前形状直接到底部
        Game.moveToBottom();
        states_map.insert(std::make_pair(std::make_pair(r, x), Game.get_state_prperties()));
        
      //撤销以上动作
      Game=Save;
    }
    Game.setRotate();
  }
  //*this=Game;
  if(states_map.empty()){
    std::cout<<"wrong!!";
  }
//   for(auto&s:states_map){
//     std::cout<<s.first;
//     std::cout<<s.second;
//   }
  return states_map;
}
void TetrisForAI::setRotate(){
  if(curShape.getName()=='O'){
    return;
  }
  Coords coords = z[1];
  for (std::size_t i {}; i < squares; ++i) {
    int x = z[i].y - coords.y; 
    int y = z[i].x - coords.x;

    z[i].x = coords.x - x;
    z[i].y = coords.y + y;
  }
  if( inLimit() ){
    for (std::size_t i {}; i < squares; ++i) {
      z[i] = k[i]; 
    }
    
  }
}
void TetrisForAI::moveToBottom(){
  if(gameover){ return; }
  while(!inLimit()){
    //不断下移
    for (std::size_t i =0; i < squares; ++i) {
      k[i] = z[i];
      ++z[i].y; 
    }
  } 
  //撞底：给区域赋值
  if( inLimit() ){
    for (std::size_t i {}; i < squares; ++i) {
      if(k[i].y==1){gameover=true;}
      area[ k[i].y ][ k[i].x ] = color; 
    }
  }
  UpdateNewShape();
}
std::pair<int,bool> TetrisForAI::step(Action& action){
	auto rotations=action.first;
	auto dirx=action.second;
	//旋转
	for(int i=0;i<rotations;i++){
		setRotate();
	}
    //上移
    bool CanMoveUp=true;
    while(CanMoveUp){
        for(int i=0;i<squares;++i){
            if(z[i].y<1){
                CanMoveUp=false;
                break;
            }
        }
        if(CanMoveUp){
            for(int i=0;i<squares;++i){
                z[i].y-=1;
        }
        }
    }
	//偏移
	int dias=z[1].x-dirx;
	for(int i=0;i<squares;++i){
        k[i]=z[i];
		z[i].x-=dias;
	}
    if(inLimit()){
    for(int i=0;i<squares;++i){
        z[i]=k[i];
	}      
    }
	moveToBottom();
	auto old_height=get_bumpiness_and_height().second;
	auto lines_cleared=setScore();//setScore()设置了游戏结束状态
	int reward=1+pow(lines_cleared,2)*cols;
    // int reward=1+ lines_cleared * cols;
	return std::make_pair(reward,gameover);
}
void TetrisForAI::reset() {
  dirx = score = shape_num=cleared_lines={0};
  rotate = gameover = {false};
  timercount = {0.f};
  delay = {0.3f};
  UpdateNewShape();
  //清零area
  for(auto&sub:area){
    std::fill(sub.begin(), sub.end(), 0);
  }
}
void train(){
    //设置随机种子
    torch::manual_seed(123);
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    std::shared_ptr<DQN> model = std::make_shared<DQN>();
    torch::optim::Adam optimizer(model->parameters(), torch::optim::AdamOptions(learning_rate));
    torch::nn::MSELoss criterion;
    TetrisForAI env;
    env.reset();
    torch::Tensor state = env.get_state_prperties();
    ReplayBuffer replaybuffer;
    //循环训练
    int epoch=0;//注意epoch变为null
    while(epoch < epochs){
	//	env.draw();
      auto next_steps=env.get_next_states();
      
      //探索和利用
      float epsilon=final_epsilon+(std::max(decay_epochs-epoch,0)*(initial_epsilon-final_epsilon)/decay_epochs);
      //取0到1之间的随机数
    //   std::uniform_real_distribution<double> uniform(0.0,1.0);
    //   auto u=uniform(ram);
    //   bool random_action = (u<=epsilon);
	  std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.0, 1.0);
        bool random_action = dis(gen) <= epsilon;
      //下一步的行动
      std::vector<Action> next_actions;
      std::vector<State> next_states;
      for(const auto&S:next_steps){
        next_actions.emplace_back(S.first);
        next_states.emplace_back(S.second);
      }
      //张量堆叠
      if (next_states.empty()) {
            std::cerr << "Error: next_states is empty." << std::endl;
            break;
      }
      auto next_states_tensor=torch::stack(next_states);
		//模型预测
      model->eval();
    //   torch::Tensor predictions = model->forward(next_states_tensor);
	  torch::Tensor predictions;
        {
            torch::NoGradGuard no_grad; // 禁用梯度计算
            predictions = model->forward(next_states_tensor).squeeze(1);
        }
      model->train();
      int index;
      if(random_action){
        std::uniform_int_distribution<int> uniform(0,next_steps.size()-1);
        index=uniform(gen);
      }
      else{
        index=predictions.argmax().item<int>();
      }
	  //根据指数选择状态和动作
      State next_state=next_states_tensor[index];
	    Action action=next_actions[index];
	  //进行单步游戏
	  auto game_result=env.step(action);
	  auto reward=game_result.first;
	  auto done=game_result.second;
      
	  //if(replaybuffer.size()<replaybuffer_maxsize)
	    replaybuffer.push_back(Exp(state, reward, next_state, done));
	  int final_score,final_shapes,final_lines;
    //初始化经验回放区
	  if(done){
        final_score=env.score;
        final_shapes=env.shape_num;
        final_lines=env.cleared_lines;
        env.reset();
        state=env.get_state_prperties();
	  }
	  else{
        state=next_state;
        continue;
	  }
	  if(replaybuffer.size()<replaybuffer_maxsize/10){
		  continue;
	  }
      if(replaybuffer.size()>=replaybuffer_maxsize){
        replaybuffer.erase(replaybuffer.begin());
      }
	  epoch+=1;
	  ReplayBuffer batch;//对经验的采样
	  //随机采样
	  std::sample(replaybuffer.begin(), replaybuffer.end(), std::back_inserter(batch), batch_size, gen);
	  //将批次数据进行处理
        std::vector<State> state_batch;
        std::vector<torch::Tensor> reward_batch;
        std::vector<State> next_state_batch;
        std::vector<bool> done_batch;
        for(const auto&e:batch){
            state_batch.emplace_back(e.state);
            reward_batch.emplace_back(torch::tensor(e.reward));
            next_state_batch.emplace_back(e.next_state);
            done_batch.emplace_back(e.done);
        }
	  auto state_batch_tensor=torch::stack(state_batch);
	  //转换为列向量
	  auto reward_batch_tensor=torch::stack(reward_batch).unsqueeze(1);
	  auto next_state_batch_tensor=torch::stack(next_state_batch);
	  //auto done_batch_tensor = torch::from_blob(done_batch.data(), {static_cast<long>(done_batch.size()), 1}, torch::kBool).clone();

	  //Q值预测
	  auto q_values=model->forward(state_batch_tensor);
	  model->eval();
	//   auto next_prediction_batch = model->forward(next_state_batch_tensor);
	  torch::Tensor next_prediction_batch;
        {
            torch::NoGradGuard no_grad; // 禁用梯度计算
            next_prediction_batch = model->forward(next_state_batch_tensor);
        }
	  model->train();

	  //torch::Tensor y_batch=torch::cat({reward_batch_tensor * (1-torch::tensor(done_batch).unsqueeze(1)),reward_batch_tensor+_gamma*next_prediction_batch},0);
	  std::vector<torch::Tensor> y_batch_data;
        for (int i = 0; i < reward_batch.size(); i++) {
			if (done_batch[i]) {
                y_batch_data.push_back(reward_batch_tensor[i]);
            } else {
                y_batch_data.push_back(reward_batch_tensor[i] + (_gamma * next_prediction_batch[i].max()));
            }
        }
    
        torch::Tensor y_batch = torch::stack(y_batch_data);//.unsqueeze(1);
      y_batch = y_batch.view_as(q_values);
	  optimizer.zero_grad();
	  torch::Tensor loss=criterion(q_values,y_batch);
	  loss.backward();
	  optimizer.step();
	  // 输出训练信息
     std::cout << "Epoch: " << epoch << "/" << epochs << ", Action: " << action << ", Score: " << final_score
           << ", Tetrominoes: " << final_shapes << ", Cleared lines: " << final_lines <<", Loss: " << loss.item<float>()<< std::endl;// 
        // 保存模型
		if (epoch > 0 && epoch % save_interval == 0) {
       model->save(save_path + std::string("/tetris_")+ std::to_string(epoch));
		}
  } 
  model->save(save_path + std::string("/tetris"));
}
int main(int argc , char **argv){
    train();
}
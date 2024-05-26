#include "tetris.hpp"

#include <iostream>

//torch
Tetris::Tetris(){
  area.resize(lines);
  for (std::size_t i {}; i < area.size(); ++i) {
    area[i].resize(cols); 
  }

  dirx = score = shape_num = cleared_lines= {0};
  rotate = gameover = {false};
  timercount = {0.f};
  delay = {0.3f};
  color = {1};
  UpdateNewShape();


}
Tetris::~Tetris() {

}

//处理键盘事件
// void Tetris::events(){
//   float time = clock.getElapsedTime().asSeconds();
//   clock.restart();
//   timercount += time;

//   auto e = std::make_shared<sf::Event>();
//   while( window->pollEvent(*e)){
//     if( e->type == sf::Event::Closed ){
//       window->close();
//     }

//     if( e->type == sf::Event::KeyPressed ){
//       if( e->key.code == sf::Keyboard::Up ){
//         rotate = true;
//       }else if( e->key.code == sf::Keyboard::Right ){
//         ++dirx;
//       }else if( e->key.code == sf::Keyboard::Left ){
//         --dirx;
//       }else if(e->key.code == sf::Keyboard::Down){
//         //moveToDown();
//       } 
//     }
//   }
//   if( sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
//     delay = 0.05f;
//   }
  
//   if(e->type == sf::Event::MouseButtonPressed){
//     if(e->mouseButton.button == sf::Mouse::Left){
//       sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
//       sf::Vector2f circlePos = aiButton.getPosition();
//       float radius = aiButton.getRadius();

//       float distance = std::sqrt(std::pow(mousePos.x - circlePos.x - radius, 2) +  std::pow(mousePos.y - circlePos.y - radius, 2));
//       if(distance<=radius){
//         reset();
//       }
//     }
//   }
// }

// void Tetris::draw(){
//   // window->clear(sf::Color::Black);
//   // window->draw(*background);

//   // for (std::size_t i {}; i < lines; ++i) {
//   //  for (std::size_t j {}; j < cols; ++j) {
//   //    if( area[i][j] != 0 ){
//   //      sprite->setTextureRect(sf::IntRect( area[i][j] * 36, 0, 36, 36 ));
//   //      sprite->setPosition(j * 36, i * 36);
//   //      window->draw(*sprite);

//   //    }
//   //  } 
//   // }

//   // for (std::size_t i {}; i < squares; ++i) {
//   //   sprite->setTextureRect(sf::IntRect( color * 36, 0, 36, 36 ));
//   //   sprite->setPosition( z[i].x * 36, z[i].y * 36);
//   //   window->draw(*sprite);
//   // }
//   // window->draw(aiButton);
//   // window->draw(txtScore);
  
  
//   // if( gameover ){
//   //   window->draw(txtGameOver);
//   // }
//   // window->display();
//   for(int i=0;i<area.size();++i){
//     for(int j=0;j<area[0].size();++j){
//       std::cout<<area[i][j];
//     }
//     std::cout<<"\n";
//   }
// }


void Tetris::run(){
  // while( window->isOpen() ){
  //   events();
  //   if(!gameover){
  //     changePosition();
  //     setRotate();
  //     moveToDown();
  //     setScore();
  //     resetValues();
  //   }
  //   else{
      
  //   }
  //   draw();
  // }
  //draw();
}
//获取新的形状
void Tetris::UpdateNewShape(){
  color = std::rand() % shapes.size() + 1;
  std::uint32_t number = std::rand() % shapes.size();
  curShape = shapes[number];
  //z[]确定了形状中每个方块的下一时刻位置
  for (std::size_t i {}; i < squares; ++i) {
    z[i].x = curShape.getForm()[i] % 2 ; 
    z[i].y = curShape.getForm()[i] / 2; 
  }
  shape_num++;
  score++;
  
}
void Tetris::moveToDown(){
  if(gameover){ return; }
  if( timercount > delay ){
    for (std::size_t i {}; i < squares; ++i) {
      k[i] = z[i];
      ++z[i].y; 
    }
    //绘制区域
    if( inLimit() ){
      //给区域赋值为color
      for (std::size_t i {}; i < squares; ++i) {
       area[ k[i].y ][ k[i].x ] = color; 
      }
      if(!gameover){
        //更新当前形状
		UpdateNewShape();
      } 
    }

    timercount = 0;
  }

}

void Tetris::setRotate(){
  if(curShape.getName()=='O'){
    return;
  }
  if( rotate ){
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
}

void Tetris::resetValues(){
  dirx = 0;
  rotate = false;
  delay = 0.3f;
}

void Tetris::changePosition(){
  for (std::size_t i {}; i < squares; ++i) {
   k[i] = z[i];
   z[i].x += dirx; 
  }

  if( inLimit() ){
    for (std::size_t i {}; i < squares; ++i) {
     z[i] = k[i]; 
    }
  }
}
//检查边界和碰撞
bool Tetris::inLimit(){
  for (std::size_t i {}; i < squares; ++i) {
   if( z[i].x < 0 || 
       z[i].x >= cols || 
       z[i].y >= lines || 
       area[ z[i].y ][ z[i].x ]){
      //碰撞
     return true;
   }
  }
  return false;
}

int Tetris::setScore(){
  std::uint32_t match = lines - 1;
  for (std::size_t i = match; i >= 1; --i) {
    std::uint32_t sum {};
   for (std::size_t j {}; j < cols; ++j) {
    if( area[i][j] ){
      if( i == 1 ){
        gameover = true;
		    score-=2;
      }
      ++sum;
    }
    area[match][j] = area[i][j];
   } 
   //不匹配时全部下移
   if( sum < cols ){
      --match;
   }
  }
  cleared_lines+=match;
  score+=1+pow(match,2)*cols;
  //txtScore.setString("SCORE: " + std::to_string(score));
  return match;
}
void Tetris::reset(){
  dirx = score = shape_num =cleared_lines= {0};
  rotate = gameover = {false};
  timercount = {0.f};
  delay = {0.3f};
  UpdateNewShape();
  //清零area
  for(auto&sub:area){
    std::fill(sub.begin(), sub.end(), 0);
  }
  // run();
}

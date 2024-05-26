#include <torch/torch.h>
#include <torch/script.h>
#include <iostream>
#include <vector>
#include <QApplication>
#include <QTimer>
#include <unistd.h>
#include "train.hpp"
#include "qtDraw.hpp"

// 全局变量
TetrisForAI env;
std::shared_ptr<DQN> model;
TetrisWidget* window;

void updateGame() {
    std::cout << std::endl;
    std::cout << "score: " << env.score << std::endl;
    std::cout << "lines: " << env.cleared_lines << std::endl;

    auto next_steps = env.get_next_states();
    std::vector<Action> next_actions;
    std::vector<State> next_states;
    for (const auto& pair : next_steps) {
        next_actions.push_back(pair.first);
        next_states.push_back(pair.second);
    }
    if (next_states.empty()) {
        std::cerr << "Error: next_states is empty." << std::endl;
        return;
    }
    torch::Tensor next_states_tensor = torch::stack(next_states);
    auto outputs = model->forward(next_states_tensor);

    if (outputs.dim() == 0) {
        std::cerr << "Error: outputs has incorrect dimensions." << std::endl;
        return;
    }

    int index = torch::argmax(outputs).item<int>();
    Action action = next_actions[index];

    bool done;
    std::tie(std::ignore, done) = env.step(action);
    std::vector<std::pair<int, int>> move;
    for (auto& p : env.z) {
        move.emplace_back(std::make_pair(p.x, p.y));
    }
    window->updateState(env.area, move, env.color, env.score, env.shape_num, env.cleared_lines);

    if (done) {
        QCoreApplication::quit();
    }
}

int main(int argc, char* argv[]) {
    torch::manual_seed(111);
    QApplication app(argc, argv);
    TetrisWidget tetrisWindow;
    window = &tetrisWindow;
    window->show();
    window->startGame();

    const char* saved_path = "trained_models/tetris";
    torch::serialize::InputArchive loaded_archive;
    torch::jit::script::Module module = torch::jit::load(saved_path);
    model = std::make_shared<DQN>();
    model->load(saved_path);
    model->eval();

    env.reset();
    srand(time(NULL));

    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, updateGame);
    timer.start(100); // 每100毫秒更新一次

    return app.exec();
}
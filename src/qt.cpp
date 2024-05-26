#include "qtDraw.hpp"
#include <QPainter>
#include <QString>
#include <QDebug>
TetrisWidget::TetrisWidget(QWidget *parent)
    : QWidget(parent), moving_color(0), scores(0), pieces(0), lines(0), pics(PIC_NUMS) {
    setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    load_pics();

    // 初始化游戏状态
    // all_blocks.resize(BLOCK_HEIGHT_NUM);
    // for (int i = 0; i < BLOCK_HEIGHT_NUM; ++i) {
    //     all_blocks[i].resize(BLOCK_WIDTH_NUM);
    //     for (int j = 0; j < BLOCK_WIDTH_NUM; ++j) {
    //         all_blocks[i][j] = 7; // 使用黑色块初始化
    //     }
    // }

    moving_blocks.resize(4);
    // for(int i=0;i<moving_blocks.size();++i){
    //     moving_blocks[i].first=0;
    //     moving_blocks[i].second=7;
    // }

    // 设置定时器，用于刷新游戏状态
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &TetrisWidget::updateGame);
}
void TetrisWidget::load_pics() {
    if (!pics[7].load(":/pics/blue.png")) qDebug() << "Failed to load blue.png";
    if (!pics[1].load(":/pics/lightblue.png")) qDebug() << "Failed to load lightblue.png";
    if (!pics[2].load(":/pics/green.png")) qDebug() << "Failed to load green.png";
    if (!pics[3].load(":/pics/purple.png")) qDebug() << "Failed to load purple.png";
    if (!pics[4].load(":/pics/orange.png")) qDebug() << "Failed to load orange.png";
    if (!pics[5].load(":/pics/red.png")) qDebug() << "Failed to load red.png";
    if (!pics[6].load(":/pics/yellow.png")) qDebug() << "Failed to load yellow.png";
    if (!pics[0].load(":/pics/black.png")) qDebug() << "Failed to load black.png";
}

void TetrisWidget::draw(QPainter& painter) {
     painter.fillRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, Qt::black);
    // // 设置绘制颜色和边框线条
     painter.setPen(Qt::black);
     // painter.drawLine(BLOCK_SIZE * BLOCK_WIDTH_NUM, 0, BLOCK_SIZE * BLOCK_WIDTH_NUM, WINDOW_HEIGHT);
     painter.drawLine(BLOCK_SIZE * BLOCK_WIDTH_NUM, 0, BLOCK_SIZE * BLOCK_WIDTH_NUM, WINDOW_HEIGHT);
     //painter.fillRect(BLOCK_SIZE * BLOCK_WIDTH_NUM, 0, WINDOW_WIDTH - BLOCK_SIZE * BLOCK_WIDTH_NUM, WINDOW_HEIGHT, Qt::white);

    for (int i = 0; i < all_blocks.size(); ++i) {
        for (int j = 0; j < all_blocks[0].size(); ++j) {
            if(all_blocks[i][j]==0)continue;
            //painter.drawImage(j * BLOCK_SIZE, i * BLOCK_SIZE, pics[all_blocks[i][j]]);
            QImage scaledImage = pics[all_blocks[i][j]].scaled(BLOCK_SIZE, BLOCK_SIZE);
            painter.drawImage(j * BLOCK_SIZE, i * BLOCK_SIZE +100, scaledImage);
        }
    }
    
    // // 绘制移动中的块
    // for (const auto& block : moving_blocks) {
    //     painter.drawImage(block.first * BLOCK_SIZE, block.second * BLOCK_SIZE, pics[moving_color]);
    // }
    // 绘制文本信息
    painter.setPen(Qt::red); // 确保文本颜色为黑色
    painter.drawText(BLOCK_SIZE * BLOCK_WIDTH_NUM + 30, 110, "Score:");
    painter.drawText(BLOCK_SIZE * BLOCK_WIDTH_NUM + 30, 200, "Pieces:");
    painter.drawText(BLOCK_SIZE * BLOCK_WIDTH_NUM + 30, 300, "Lines:");
    painter.drawText(BLOCK_SIZE * BLOCK_WIDTH_NUM + 30, 140, QString::number(scores));
    painter.drawText(BLOCK_SIZE * BLOCK_WIDTH_NUM + 30, 230, QString::number(pieces));
    painter.drawText(BLOCK_SIZE * BLOCK_WIDTH_NUM + 30, 330, QString::number(lines));
}


void TetrisWidget::paintEvent(QPaintEvent *event) {
    qDebug() << "Paint Event Called";
    QPainter painter(this);
    draw(painter);
}

void TetrisWidget::updateGame() {
    // 更新游戏状态（逻辑省略）
    // 更新界面
    update();
}
void TetrisWidget::startGame() {
    timer->start(1000 / 60); // 每秒刷新60次
}
void TetrisWidget::updateState(const std::vector<std::vector<std::uint32_t>>& blocks,const std::vector<std::pair<int,int>> moving, int color, int score, int pieces, int lines) {
    // 初始化游戏状态
    for (int i = 0; i < BLOCK_HEIGHT_NUM; ++i) {
        all_blocks.resize(BLOCK_HEIGHT_NUM);
        for (int j = 0; j < BLOCK_WIDTH_NUM; ++j) {
            all_blocks[i].resize(BLOCK_WIDTH_NUM);
            all_blocks[i][j] = blocks[i][j]; // 使用黑色块初始化
        }
    }
    for(int i=0;i<moving_blocks.size();++i){
        moving_blocks[i].first=moving[i].first;
        moving_blocks[i].second=moving[i].second;
    }
    moving_color = color;
    scores = score;
    this->pieces = pieces;
    this->lines = lines;
    update();
}
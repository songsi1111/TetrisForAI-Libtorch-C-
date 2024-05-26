#ifndef TETRISWIDGET_HPP
#define TETRISWIDGET_HPP
#include <QWidget>
#include <QImage>
#include <QVector>
#include <QPair>
#include <QTimer>

#define WINDOW_WIDTH 350
#define WINDOW_HEIGHT 500
#define BLOCK_WIDTH_NUM 10 
#define BLOCK_HEIGHT_NUM 20
#define BLOCK_SIZE 20
#define PIC_NUMS 8

class TetrisWidget : public QWidget {
    Q_OBJECT
public:
    explicit TetrisWidget(QWidget *parent = nullptr);

    void startGame();
    void pauseGame();
    void resetGame();
    void updateState(const std::vector<std::vector<std::uint32_t>>& blocks,const std::vector<std::pair<int,int>> moving, int color, int score, int pieces, int lines);

protected:
    void paintEvent(QPaintEvent *event) override;

private:// slots:
    void updateGame();

private:
    void load_pics();
    void draw(QPainter& painter);

    QVector<QVector<int>> all_blocks;
    QVector<QPair<int, int>> moving_blocks;
    int moving_color;
    QVector<QImage> pics;
    int scores;
    int pieces;
    int lines;
    QTimer *timer;
};

#endif // TETRISWIDGET_HPP

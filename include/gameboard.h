#ifndef GAMEBOARD_H
#define GAMEBOARD_H
#include <QAbstractListModel>
#include <QString>
#include <vector>
#include <QDebug>
#include <algorithm>
#include <random>
#include <iostream>
#include <QQmlEngine>
#include <QPropertyAnimation>
#include <QQmlComponent>
#include <QQmlProperty>
#include <QQuickView>
#include <QObject>

#define BoardPosition std::pair<int,int>

#define PINK "#603E95"
#define BLUE "blue"
#define YELLOW "#D7255D"
#define PURPURE "#FAC22B"



class GameBoard : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int col READ col CONSTANT);
    Q_PROPERTY(int raw READ raw CONSTANT);
    Q_PROPERTY(int score READ getScore CONSTANT);

public:
    static constexpr size_t MINIMUN_CONNECTED_BALLS{3};
    static constexpr size_t DEFAULT_BOARD_SIZE{24};
    GameBoard(const size_t dimensionSize = DEFAULT_BOARD_SIZE, QObject* parent = nullptr);
    static void registerMe(const std::string& moduleName);
    bool move(int first, int second);
    Q_INVOKABLE void changeMovePhase(int index);

    size_t getBoardSize() const;
    size_t raw() const;
    size_t col() const;
    int getScore() const;

    int rowCount(const QModelIndex &parent = QModelIndex{}) const override;
    QHash<int, QByteArray> roleNames() const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    void qSleep(int ms);

    struct Ball{
        const std::vector<QString> Colors = {
            PINK,
            BLUE,
            PURPURE,
            YELLOW
        };
        Ball()
            :color{Colors[std::rand() % 4]}
        {
        }

        QString color;
        bool operator==(const QString other){
            return other == color;
        }
        Ball& operator=(const QString newColor)
        {
            color = newColor;
            return *this;
        }
        Ball& operator=(const Ball newColor)
        {
            color = newColor.color;
            return *this;
        }
    };



private:
    void shuffle(std::vector<Ball> &ballVec);
    void fallDownBalls(std::vector<Ball> &ballVec);
    bool isNear(BoardPosition fr, BoardPosition sc) const;
    bool isIndexValid( const size_t position) const;
    size_t amountOfConnectedBalls(QString type, std::vector<Ball>& boardCopy, BoardPosition ballPos) const;   
    int getIndexFromPosition(const BoardPosition pos)const;
     BoardPosition getRowCol(const size_t index) const;
    Ball createNewBall()const;


    bool  m_isMovePhase;
    int m_movingBallIndex;
    int m_score;
    const size_t m_col;
    const size_t m_row;
    const size_t m_boardSize;
    std::vector<Ball> m_rowColBoard;

    enum GameRoles{
        ColorRole = Qt::UserRole + 1
    };


};

#endif // GAMEBOARD_H

#pragma once
#include <QAbstractListModel>
#include <QString>
#include <QDebug>

#include <QQmlEngine>
#include <QFile>

#include <QJsonDocument>
#include <QJsonObject>

#include <vector>
#include <algorithm>
#include <random>


class GameBoard : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int col READ col CONSTANT);
    Q_PROPERTY(int row READ row CONSTANT);
    Q_PROPERTY(int score READ getScore NOTIFY scoreChanged);


// [Constructors:]
public:
    GameBoard(const int dimensionSize = DEFAULT_BOARD_SIZE, QObject* parent = nullptr);

// [Properties:]
public:
    using BoardPosition = std::pair<int,int>;
    static constexpr int MINIMUN_CONNECTED_BALLS {3};
    static constexpr int DEFAULT_BOARD_SIZE {12};

    struct Ball{
        const std::vector<QString> Colors = {
            "hotpink",
            "dodgerblue",
            "limegreen",
            "gold"
        };
        Ball()
            :color{""}
        {

        };
        Ball(const Ball& newBall) = default;

        bool operator==(const QString other){
            return other == color;
        }
        Ball& operator=(const QString newColor)
        {
            color = newColor;
            return *this;
        }
        Ball& operator=(const Ball& newBall)
        {
            color = newBall.color;
            return *this;
        };

        QString color {};
    };

// [Methods:]
public:
    Q_INVOKABLE void changeMovePhase(int index);
    static void registerMe(const std::string& moduleName);



    int readJson(const QJsonObject &json, const QString name);
    int loadConstants(const QString name);

    int getBoardSize() const;
    int row() const;
    int col() const;
    int getScore() const;

    int rowCount(const QModelIndex &parent = QModelIndex{}) const override;
    QHash<int, QByteArray> roleNames() const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

signals:
    void scoreChanged();
    void ballAdded();

private:
    void createTable(std::vector<Ball> &ballVec);
    void fallDownBalls(std::vector<Ball> &ballVec);

    bool isNear(const BoardPosition fr,const BoardPosition sc) const;
    bool isIndexValid( const int position) const;

    int getConnectedBallsCount(const QString type, std::vector<Ball>& boardCopy,const BoardPosition ballPos) const;
    int getIndexFromPosition(const BoardPosition pos)const;

    BoardPosition getRowCol(const int index) const;

    Ball createNewBall()const;

// [Variables:]
private:
    bool  m_isMovePhase;
    int m_movingBallIndex;
    int m_score;
    const int m_col;
    const int m_row;
    const int m_boardSize;
    std::vector<Ball> m_rowColBoard;

    enum GameRoles{
        ColorRole = Qt::UserRole + 1
    };

    // QAbstractItemModel interface
public:

     bool moveRows(const QModelIndex &sourceParent, int sourceRow, int count,
                   const QModelIndex &destinationParent, int destinationChild) override;
     bool insertRows(int row, int count, const QModelIndex &parent) override;
     bool removeRows(int row, int count, const QModelIndex &parent) override;
};


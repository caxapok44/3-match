#include "include/gameboard.h"

GameBoard::GameBoard(const int dimensionSize,QObject* parent)
    :QAbstractListModel{parent},
      m_isMovePhase{false},
      m_movingBallIndex{0},
      m_col{dimensionSize},
      m_row{m_col / 2},
      m_boardSize{m_col * m_row}
{
    m_score = 0;
    m_rowColBoard.resize(m_boardSize);
    createTable(m_rowColBoard);
}

void GameBoard::registerMe(const std::string &moduleName)
{
    qmlRegisterType<GameBoard>(moduleName.c_str(),1,0,"GameBoardModel");
}

void GameBoard::createTable(std::vector<Ball> &ballVec)
{
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    static std::mt19937 generator(seed);
    for(int i = 0;i < m_boardSize; i++)
    {
        Ball newBall{};
        ballVec[i] = newBall.color;
    }
    std::shuffle(m_rowColBoard.begin(), m_rowColBoard.end(), generator);


}

void GameBoard::fallDownBalls(std::vector<GameBoard::Ball> &ballVec)
{
    for (int col = 0; col < m_col; col++)
    {
        for(int row = m_row-1; row >=0; row--)
        {
            if(ballVec[getIndexFromPosition(std::make_pair(row,col))] == nullptr)
            {
                Ball moveBall;
                moveBall = nullptr;
                for(int moveRow = row-1; moveRow >= 0; moveRow--)
                {
                     moveBall = ballVec[getIndexFromPosition(std::make_pair(moveRow,col))];
                     if(!moveBall.color.isEmpty())
                     {
                       ballVec[getIndexFromPosition(std::make_pair(moveRow,col))] = nullptr;
                       ballVec[getIndexFromPosition(std::make_pair(row,col))] = moveBall;
                       break;
                     }
                }
                if(moveBall.color.isEmpty())
                {
                   for(int newRaw = row; newRaw >= 0; newRaw--)
                   {
                      ballVec[getIndexFromPosition(std::make_pair(newRaw,col))] = createNewBall();
                   }
                   break;
                }
            }
        }
    }
}

bool GameBoard::isNear(const BoardPosition fr,const BoardPosition sc) const
{
    if  (fr == sc)
    {
        return false;
    }
    int distance = 0;
    if (fr.first == sc.first)
    {
        distance = std::abs(static_cast<int>(fr.second - sc.second) );
    }
    else if (fr.second == sc.second)
    {
        distance = std::abs(static_cast<int>(fr.first - sc.first) );
    }

    return  distance == 1 ? true : false;
}

int GameBoard::getConnectedBallsCount(const QString type,
                                         std::vector<GameBoard::Ball>& boardCopy,
                                         const BoardPosition pos) const
{
    int ballIndex = getIndexFromPosition(pos);
    if  (ballIndex >= m_boardSize || ballIndex < 0)
    {
        return 0;
    }
    Ball ball;
    ball = boardCopy[ballIndex];

    if (ball == nullptr)
    {
        return 0;
    }

    if  (ball.color != type)
    {
        return 0;
    }


    int count {1};

    boardCopy[ballIndex] = nullptr;


    count += getConnectedBallsCount(type, boardCopy, std::make_pair(pos.first+1,pos.second));
    count += getConnectedBallsCount(type, boardCopy, std::make_pair(pos.first-1,pos.second));
    count += getConnectedBallsCount(type, boardCopy, std::make_pair(pos.first,pos.second+1));
    count += getConnectedBallsCount(type, boardCopy, std::make_pair(pos.first,pos.second-1));
    return count;
}

int GameBoard::getBoardSize() const
{
    return m_boardSize;
}

QHash<int, QByteArray> GameBoard::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[GameRoles::ColorRole] = "colorBall";
    return roles;
}

int GameBoard::col() const
{
    return m_col;
}

int GameBoard::row() const
{
    return m_row;
}

int GameBoard::rowCount(const QModelIndex &parent)const
 {
     Q_UNUSED(parent);
     return m_rowColBoard.size();
 }

QVariant GameBoard::data(const QModelIndex &index, int role) const
{
  if(!index.isValid() || index.row() > rowCount(index))
  {
      return QVariant{};
  }

  const Ball& ball{m_rowColBoard.at(index.row())};

  switch (role)
  {
    case GameRoles::ColorRole:
    {
      return  QVariant::fromValue(ball.color);
    }
    default:
    {
       return QVariant{};
    }
  }

  return QVariant{};
}
void GameBoard::changeMovePhase(int index)
{
    m_isMovePhase = !m_isMovePhase;

    if(m_isMovePhase)
    {
        m_movingBallIndex = index;
    }
    else
    {
        if(move(m_movingBallIndex, index))
        {
            //direct checking of connected Balls
            std::vector<Ball> copygameBoardFirst{m_rowColBoard};
            int connectedBlocksFromFirst = getConnectedBallsCount(copygameBoardFirst[index].color,
                                                  copygameBoardFirst,
                                                  getRowCol(index));

            if  (connectedBlocksFromFirst >= MINIMUN_CONNECTED_BALLS)
            {
                fallDownBalls(copygameBoardFirst);
                m_rowColBoard.assign(copygameBoardFirst.begin(),copygameBoardFirst.end());
                m_score += connectedBlocksFromFirst * (connectedBlocksFromFirst + 1) / 2;
            }
            if  (connectedBlocksFromFirst < MINIMUN_CONNECTED_BALLS)
            {
                qDebug() << "Here is back move!!!";
                move(m_movingBallIndex, index);
            }

        }
        emit scoreChanged();
        emit dataChanged(createIndex(0,0), createIndex(m_boardSize,0));
    }

}

bool GameBoard::move(const int first,const int second)
{

    if (!isIndexValid(first) && !isIndexValid(second))
    {
        return false;
    }
    if  (m_rowColBoard[first].color == m_rowColBoard[second].color)
    {
        return false;
    }

    const BoardPosition firstElementPosition {getRowCol(first)};
    const BoardPosition secondElementPosition {getRowCol(second)};
    if  (!isNear(firstElementPosition, secondElementPosition))
    {
        return false;
    }
    std::swap(m_rowColBoard[first], m_rowColBoard[second]);
    emit dataChanged(createIndex(0,0), createIndex(m_boardSize,0));

    return true;
}

bool GameBoard::isIndexValid(const int position) const
{
    return position < m_boardSize;
}

GameBoard::BoardPosition GameBoard::getRowCol(const int index) const
{
    int row = (index / m_col);
    int coulmn = (index % m_col);

    return std::make_pair(row,coulmn);
}

int GameBoard::getIndexFromPosition(const BoardPosition pos) const
{
    if  (pos.first < 0 || pos.first >= m_row ||
        pos.second < 0 || pos.second >= m_col)
    {
        return -1;                              //return -1 if invalid ball position
    }

    int ind = pos.first * m_col + pos.second;
    return ind;
}

GameBoard::Ball GameBoard::createNewBall() const
{
    Ball ball{};
    return ball;
}

int GameBoard::getScore() const
{
    return m_score;
}

int GameBoard::readJson(const QJsonObject &json, const QString name)
{
    if (json.contains(name) && json[name].isDouble())
    {
        return json[name].isDouble();
    }
    return 0;
}

int GameBoard::loadConstants(const QString name)
{
    QFile loadFile(QStringLiteral("constants.json"));
    if (!loadFile.open(QIODevice::ReadOnly))
    {
        qWarning("Couldn't open save file.");
        return false;
    }
    QByteArray saveData = loadFile.readAll();

    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

    return readJson(loadDoc.object(), name);

}

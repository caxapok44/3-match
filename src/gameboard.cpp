#include "include/gameboard.h"


void GameBoard::qSleep(int ms)
{
    struct timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };
    nanosleep(&ts, NULL);
}


GameBoard::GameBoard(const size_t dimensionSize,QObject* parent)
    :QAbstractListModel{parent},
      m_isMovePhase{false},
      m_col{dimensionSize},
      m_row{m_col / 2},
      m_boardSize{m_col * m_row}
{
    m_score = 0;
    m_rowColBoard.resize(m_boardSize);
    shuffle(m_rowColBoard);
}

void GameBoard::registerMe(const std::string &moduleName)
{
    qmlRegisterType<GameBoard>(moduleName.c_str(),1,0,"GameBoardModel");
}

void GameBoard::shuffle(std::vector<Ball> &ballVec)
{

    for(size_t i = 0;i < m_boardSize; i++)
    {
        Ball newBall{};
        ballVec[i] = newBall.color;
    }


}

void GameBoard::fallDownBalls(std::vector<GameBoard::Ball> &ballVec)
{
    for (size_t col = 0; col < m_col; col++)
        for(int raw = m_row-1; raw >=0; raw--)
            if(ballVec[getIndexFromPosition(std::make_pair(raw,col))] == nullptr)
            {
                Ball moveBall;
                moveBall = nullptr;
                for(int moveRow = raw-1; moveRow >= 0; moveRow--) {
                     moveBall = ballVec[getIndexFromPosition(std::make_pair(moveRow,col))];

                     if(moveBall.color != nullptr) {
                       ballVec[getIndexFromPosition(std::make_pair(moveRow,col))] = nullptr;
                       ballVec[getIndexFromPosition(std::make_pair(raw,col))] = moveBall;
                       break;
                     }
                }
                if(moveBall.color == nullptr)
                {
                   for(int newRaw = raw; newRaw >= 0; newRaw--) {
                       ballVec[getIndexFromPosition(std::make_pair(newRaw,col))] = createNewBall();;
                   }
                   break;
                }

            }

}

bool GameBoard::isNear(BoardPosition fr, BoardPosition sc) const
{
    if  (fr == sc){
        return false;
    }
    int temp = 0;
    if (fr.first == sc.first)
        temp = std::abs(static_cast<int>(fr.second - sc.second) );
    else if (fr.second == sc.second)
        temp = std::abs(static_cast<int>(fr.first - sc.first) );

    return  temp == 1 ? true : false;
}

size_t GameBoard::amountOfConnectedBalls(QString type,
                                         std::vector<GameBoard::Ball>& boardCopy,
                                         BoardPosition pos) const
{
    int ballIndex = getIndexFromPosition(pos);
    if  (ballIndex >= static_cast<int>(m_boardSize) || ballIndex < 0)
        return 0;

    Ball ball = boardCopy[ballIndex];

    if  (ball.color != type)
        return 0;

    if  (ball == nullptr)
        return 0;

    size_t count {1};

    boardCopy[ballIndex] = nullptr;


    count += amountOfConnectedBalls(type, boardCopy, std::make_pair(pos.first+1,pos.second));
    count += amountOfConnectedBalls(type, boardCopy, std::make_pair(pos.first-1,pos.second));
    count += amountOfConnectedBalls(type, boardCopy, std::make_pair(pos.first,pos.second+1));
    count += amountOfConnectedBalls(type, boardCopy, std::make_pair(pos.first,pos.second-1));
    return count;
}

size_t GameBoard::getBoardSize() const
{
    return m_boardSize;
}

QHash<int, QByteArray> GameBoard::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[GameRoles::ColorRole] = "colorBall";
    return roles;
}

size_t GameBoard::col() const
{
    return m_col;
}

size_t GameBoard::raw() const
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
      return {};
  }

  const Ball& ball{m_rowColBoard.at(index.row())};

  switch (role) {
    case GameRoles::ColorRole:
      return  QVariant::fromValue(ball.color);
    default:{
       return true;
    }
  }
  return QVariant();
}
void GameBoard::changeMovePhase(int index)
{
    m_isMovePhase = !m_isMovePhase;

    if(m_isMovePhase)
        m_movingBallIndex = index;
    else
    {
        if(move(m_movingBallIndex, index))
        {
            //direct checking of connected Balls
            std::vector<Ball> copygameBoardFirst{m_rowColBoard};
            size_t connectedBlocksFromFirst = amountOfConnectedBalls(copygameBoardFirst[index].color,
                                                  copygameBoardFirst,
                                                  getRowCol(index));

            if  (connectedBlocksFromFirst >= MINIMUN_CONNECTED_BALLS)
            {
                fallDownBalls(copygameBoardFirst);
                m_rowColBoard.assign(copygameBoardFirst.begin(),copygameBoardFirst.end());
                m_score += connectedBlocksFromFirst * (connectedBlocksFromFirst + 1) / 2;
            }
            //reverse check of move
            std::vector<Ball> copygameBoardSecond{m_rowColBoard};
            size_t connectedBlocksFromSecond = amountOfConnectedBalls(copygameBoardSecond[m_movingBallIndex].color,
                                                  copygameBoardSecond,
                                                  getRowCol(m_movingBallIndex));
            if  (connectedBlocksFromSecond >= MINIMUN_CONNECTED_BALLS)
            {
                fallDownBalls(copygameBoardSecond);
                m_rowColBoard.assign(copygameBoardSecond.begin(),copygameBoardSecond.end());
                m_score += connectedBlocksFromSecond * (connectedBlocksFromSecond + 1) / 2;
            }

            if  (connectedBlocksFromSecond < MINIMUN_CONNECTED_BALLS &&
                 connectedBlocksFromFirst < MINIMUN_CONNECTED_BALLS)
            {
                qDebug() << "Here is back move!!!";
                move(m_movingBallIndex, index);
            }

        }
        emit dataChanged(createIndex(0,0), createIndex(m_boardSize,0));
    }

}

bool GameBoard::move(int first, int second)
{

    if (!isIndexValid(static_cast<size_t>(first)) && !isIndexValid(static_cast<size_t>(second)))
        return false;

    if  (m_rowColBoard[first].color == m_rowColBoard[second].color)
        return false;

    const BoardPosition firstElementPosition {getRowCol(first)};
    const BoardPosition secondElementPosition {getRowCol(second)};
    if  (!isNear(firstElementPosition, secondElementPosition))
    {
        return false;
    }
    std::swap(m_rowColBoard[first].color, m_rowColBoard[second].color);
    emit dataChanged(createIndex(0,0), createIndex(m_boardSize,0));

    return true;
}

bool GameBoard::isIndexValid(const size_t position) const
{
    return position < m_boardSize;
}

BoardPosition GameBoard::getRowCol(const size_t index) const
{
    size_t row = (index / m_col);
    size_t coulmn = (index % m_col);

    return std::make_pair(row,coulmn);
}

int GameBoard::getIndexFromPosition(const BoardPosition pos) const
{
    if  (pos.first < 0 || pos.first >= m_row || pos.second < 0 || pos.second >=m_col)
        return -1; //return -1 if invalid ball position
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

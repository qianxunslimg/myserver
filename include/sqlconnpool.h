#ifndef SQLCONNPOOL_H
#define SQLCONNPOOL_H

#include "log.h"
#include <mutex>
#include <mysql/mysql.h>
#include <queue>
#include <semaphore.h>
#include <string>
#include <thread>

class SqlConnPool {
public:
  static SqlConnPool *Instance();

  MYSQL *GetConn();           //获取一个连接
  void FreeConn(MYSQL *conn); //归还一个连接
  int GetFreeConnCount();

  void Init(const char *host, int port, const char *user, const char *pwd,
            const char *dbName, int connSize);
  void ClosePool();

private:
  SqlConnPool();
  ~SqlConnPool();

  int MAX_CONN_;
  int useCount_;
  int freeCount_;

  std::queue<MYSQL *> connQue_;
  std::mutex mtx_;
  sem_t semId_;
};

#endif

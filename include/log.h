#ifndef LOG_H
#define LOG_H

#include "buffer.h"
#include <assert.h>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/stat.h>
#include <sys/time.h>
#include <thread>

using namespace std;

// 日志等级
enum LOG_LEVEL { V_INFO = 0, V_DEBUG, V_WARN, V_ERROR };

class Log {
public:
  // log初始化
  void init(const char *path = "./../log", const char *suffix = ".log");
  // 单例化模式
  static Log *Instance();
  // 单例模式异步写日志
  static void flushLogThreadRun();
  // 异步日志
  void asyncWriteLog();
  // 添加日志
  void logAdd(LOG_LEVEL level, const char *format, ...); //添加一条日志
  // 添加日志级别
  void AppendLogLevelTitle_(LOG_LEVEL level);
  // 是否打开
  bool IsOpen();

private:
  Log();
  virtual ~Log();

  int lineCnt_;
  bool isAsync_;
  bool isOpen_;

  Buffer buff_;
  int today_;

  FILE *fileptr_;
  mutex logmtx_;
  condition_variable que_not_empty;

  unique_ptr<thread> workThread_;
  queue<string> logQue_;

  const char *path_;
  const char *suffix_;

  static const int LOG_NAME_LEN = 100;
  static const int MAX_LINES = 5000;
};
#define LOG_BASE(level, format, ...)                                           \
  do {                                                                         \
    Log *log = Log::Instance();                                                \
    if (log->IsOpen()) {                                                       \
      log->logAdd(level, format, ##__VA_ARGS__);                               \
    }                                                                          \
  } while (0);

#define LOG_DEBUG(format, ...)                                                 \
  do {                                                                         \
    LOG_BASE(V_DEBUG, format, ##__VA_ARGS__)                                   \
  } while (0);
#define LOG_INFO(format, ...)                                                  \
  do {                                                                         \
    LOG_BASE(V_INFO, format, ##__VA_ARGS__)                                    \
  } while (0);
#define LOG_WARN(format, ...)                                                  \
  do {                                                                         \
    LOG_BASE(V_WARN, format, ##__VA_ARGS__)                                    \
  } while (0);
#define LOG_ERROR(format, ...)                                                 \
  do {                                                                         \
    LOG_BASE(V_ERROR, format, ##__VA_ARGS__)                                   \
  } while (0);

#endif

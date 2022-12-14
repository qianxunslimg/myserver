#ifndef HEAP_TIMER_H
#define HEAP_TIMER_H

#include "log.h"
#include <algorithm>
#include <arpa/inet.h>
#include <assert.h>
#include <chrono>
#include <functional>
#include <queue>
#include <time.h>
#include <unordered_map>

struct TimerNode {
  int fd_;
  timeval endTime_;          //时间戳
  std::function<void()> cb_; //超时回调
  TimerNode *next;
  TimerNode *pre;
  TimerNode() : fd_(-1), next(nullptr), pre(nullptr) {}
  bool operator<(const TimerNode &t) {
    return endTime_.tv_sec < t.endTime_.tv_sec;
  }
};

class HeapTimer {
public:
  HeapTimer();
  ~HeapTimer();

  void updateNode(int fd, int timeout);
  void addNode(int fd, int timeout, const std::function<void()> cb);
  int nextNodeClock();

private:
  void removeNode(int fd);
  void removeEnding(); //清除超时结点
  void nodeToBack(TimerNode *node);

  TimerNode *head_; //头节点
  TimerNode *back_;
  std::unordered_map<int, TimerNode *> node_map_;
};

#endif // HEAP_TIMER_H

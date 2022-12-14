#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <errno.h>
#include <mysql/mysql.h>
#include <regex>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "buffer.h"
#include "log.h"
#include "sqlconnRAII.h"
#include "sqlconnpool.h"

enum sqlVerifyState {
  loginOK = 0,
  registerOK,
  loginFail,
  registerFail,
  otherError
};

class HttpRequest {
public:
  enum PARSE_STATE { //主状态机
    REQUEST_LINE,
    HEADERS,
    BODY,
    FINISH,
  };

  enum HTTP_CODE { //从状态机
    NO_REQUEST = 0,
    GET_REQUEST,
    BAD_REQUEST,
    NO_RESOURSE,
    FORBIDDENT_REQUEST,
    FILE_REQUEST,
    INTERNAL_ERROR,
    CLOSED_CONNECTION,
  };

  HttpRequest() { Init(); }
  ~HttpRequest() = default;

  void Init();
  bool parseData(Buffer &buff);

  std::string path() const;
  std::string &path();
  std::string method() const;
  std::string version() const;
  std::string GetPost(const std::string &key) const;
  std::string GetPost(const char *key) const;

  bool IsKeepAlive() const;

private:
  std::string userName;

  bool ParseRequestLine_(const std::string &line);
  void ParseHeader_(const std::string &line);
  void ParseBody_();

  void ParsePath_();
  void ParsePost_();
  bool ParseBodyForUserInfo();
  bool ParseBodyForFile();
  bool SaveFile(char filename[]);

  static sqlVerifyState UserVerify(const std::string &name,
                                   const std::string &pwd, bool isLogin);

  PARSE_STATE master_state_;
  std::string method_, path_, version_;
  Buffer *body_;
  std::string fileSaveDir_;
  std::unordered_map<std::string, std::string> header_;
  std::unordered_map<std::string, std::string> post_;

  static const std::unordered_set<std::string> DEFAULT_HTML_REQUEST;
  static const std::unordered_map<std::string, int> DEFAULT_HTML_POST_REQUEST;
  static uint32_t ConverHex(char ch);
  void decode_str(char *from);
  void decode_str(std::string &from);
};

#endif

#ifndef _logger_h_
#define _logger_h_

#include <Arduino.h>
#include <ezTime.h>

// 124 bytes - message
#define LOG_MSG_LENGTH (128-3-1) 

// 8 lines a 128 = 1k, 40 lines => 5k
#define MAX_LOGLINES (8 * 5)

class Logger {
  public:
    typedef enum {
      ERROR = 0,
      WARN,
      INFO,
      DEBUG
    } LogLevel_t;

    Logger(Timezone aTimezone);

    void debug(const char *msg);
    void info(const char *msg);
    void warn(const char *msg);
    void error(const char *msg);

    void debug(const __FlashStringHelper *msg);
    void info(const __FlashStringHelper *msg);
    void warn(const __FlashStringHelper *msg);
    void error(const __FlashStringHelper *msg);

     void debugf(const char *format, ...);
     void infof(const char *format, ...);
     void warnf(const char *format, ...);
     void errorf(const char *format, ...);

  private:
    typedef struct {
      uint8_t timestamp[3]; // hh|mm|ss
      LogLevel_t level; 
      char message[LOG_MSG_LENGTH];
    } LogLine_t;

    LogLine_t logLines[MAX_LOGLINES];
    uint8_t logLineIdx = MAX_LOGLINES - 1;
    uint32_t logMsgId = 0 - 1; // init with 

    Timezone tz;

    void nextLogLine(LogLevel_t level);
    void writePrefix(LogLevel_t level);

    void write(LogLevel_t level, const char *msg);
    void write(LogLevel_t level, const __FlashStringHelper *msg);

    void writef(LogLevel_t level, const char *format, va_list arg);
};

#endif
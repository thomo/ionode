#include "logger.h"
#include <HardwareSerial.h>

Logger::Logger(Timezone aTimezone) {
  tz = aTimezone;
}

void Logger::info(const char *msg) {
  write(Logger::INFO, msg);
}

void Logger::info(const __FlashStringHelper *msg) {
  write(Logger::INFO, msg);
}

void Logger::debugf(const char *format, ...) {
    va_list arg;
    va_start(arg, format);
    writef(Logger::DEBUG, format, arg);
    va_end(arg);
}

void Logger::infof(const char *format, ...) {
    va_list arg;
    va_start(arg, format);
    writef(Logger::INFO, format, arg);
    va_end(arg);
}

void Logger::warnf(const char *format, ...) {
    va_list arg;
    va_start(arg, format);
    writef(Logger::WARN, format, arg);
    va_end(arg);
}

void Logger::errorf(const char *format, ...) {
    va_list arg;
    va_start(arg, format);
    writef(Logger::ERROR, format, arg);
    va_end(arg);
}

void Logger::write(LogLevel_t level, const char *msg) {
  nextLogLine(level);

  strlcpy(logLines[logLineIdx].message, msg, LOG_MSG_LENGTH); 

  Serial.println(logLines[logLineIdx].message);
}

void Logger::write(LogLevel_t level, const __FlashStringHelper *msg) {
  nextLogLine(level);

  strncpy_P(logLines[logLineIdx].message, (const char *) msg, LOG_MSG_LENGTH);
  logLines[logLineIdx].message[LOG_MSG_LENGTH-1] = '\0';

  Serial.println(logLines[logLineIdx].message);
}

void Logger::writef(LogLevel_t level, const char *format, va_list arg) {
  nextLogLine(level);

  vsnprintf(logLines[logLineIdx].message, LOG_MSG_LENGTH, format, arg); 
  
  Serial.println(logLines[logLineIdx].message);
}

void Logger::nextLogLine(LogLevel_t level) {
  ++logLineIdx;
  ++logMsgId;
  if (logLineIdx >= MAX_LOGLINES) logLineIdx = 0;

  writePrefix(level);  
}

void Logger::writePrefix(LogLevel_t level) {
  logLines[logLineIdx].timestamp[0] = tz.hour();
  logLines[logLineIdx].timestamp[1] = tz.minute();
  logLines[logLineIdx].timestamp[2] = tz.second();
  logLines[logLineIdx].level = level;

  Serial.printf("%02d:%02d:%02d ", logLines[logLineIdx].timestamp[0], logLines[logLineIdx].timestamp[1], logLines[logLineIdx].timestamp[2]);
  switch (level) {
    case Logger::ERROR : Serial.print("E"); break;
    case Logger::WARN  : Serial.print("W"); break;
    case Logger::INFO  : Serial.print("I"); break;
    case Logger::DEBUG : Serial.print("D"); break;
    default            : Serial.print("X"); break;
  }
  Serial.print(" ");
}

#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>

#define VERSION_MAJOR 0
#define VERSION_MINOR 1
#define VERSION_BUILD 17

#undef INCUBUS_LOG_DEBUG_DISPATCHER
//#define INCUBUS_LOG_DEBUG_DISPATCHER

#define MAX_STRING_SIZE 1024
#define log_error(...) do{if (m_log->GetLogLevel() >= llError) {char log_str[MAX_STRING_SIZE]; snprintf(static_cast<char*>(log_str), sizeof(log_str), __VA_ARGS__); m_log->LogMsgError(log_str);}}while(0)
#define log_warning(...) do{if (m_log->GetLogLevel() >= llWarning) {char log_str[MAX_STRING_SIZE]; snprintf(static_cast<char*>(log_str), sizeof(log_str), __VA_ARGS__); m_log->LogMsgWarning(log_str);}}while(0)
#define log_debug1(...) do{if (m_log->GetLogLevel() >= llDebug1) {char log_str[MAX_STRING_SIZE]; snprintf(static_cast<char*>(log_str), sizeof(log_str), __VA_ARGS__); m_log->LogMsgDebug1(log_str);}}while(0)
#define log_debug2(...) do{if (m_log->GetLogLevel() >= llDebug2) {char log_str[MAX_STRING_SIZE]; snprintf(static_cast<char*>(log_str), sizeof(log_str), __VA_ARGS__); m_log->LogMsgDebug2(log_str);}}while(0)
#define log_debug3(...) do{if (m_log->GetLogLevel() >= llDebug3) {char log_str[MAX_STRING_SIZE]; snprintf(static_cast<char*>(log_str), sizeof(log_str), __VA_ARGS__); m_log->LogMsgDebug3(log_str);}}while(0)
#define logobj_error(...) do{if (m_logobj->GetLogLevel() >= llError) {char log_str[MAX_STRING_SIZE]; snprintf(static_cast<char*>(log_str), sizeof(log_str), __VA_ARGS__); m_logobj->LogMsgError(log_str);}}while(0)
#define logobj_warning(...) do{if (m_logobj->GetLogLevel() >= llWarning) {char log_str[MAX_STRING_SIZE]; snprintf(static_cast<char*>(log_str), sizeof(log_str), __VA_ARGS__); m_logobj->LogMsgWarning(log_str);}}while(0)
#define logobj_debug1(...) do{if (m_logobj->GetLogLevel() >= llDebug1) {char log_str[MAX_STRING_SIZE]; snprintf(static_cast<char*>(log_str), sizeof(log_str), __VA_ARGS__); m_logobj->LogMsgDebug1(log_str);}}while(0)
#define logobj_debug2(...) do{if (m_logobj->GetLogLevel() >= llDebug2) {char log_str[MAX_STRING_SIZE]; snprintf(static_cast<char*>(log_str), sizeof(log_str), __VA_ARGS__); m_logobj->LogMsgDebug2(log_str);}}while(0)
#define logobj_debug3(...) do{if (m_logobj->GetLogLevel() >= llDebug3) {char log_str[MAX_STRING_SIZE]; snprintf(static_cast<char*>(log_str), sizeof(log_str), __VA_ARGS__); m_logobj->LogMsgDebug3(log_str);}}while(0)

using int_8 = int8_t;
using uint_8 = uint8_t;
using int_16 = int16_t;
using uint_16 = uint16_t;
using int_32 = int32_t;
using uint_32 = uint32_t;
using int_64 = int64_t;
using uint_64 = uint64_t;

#endif // COMMON_H

/*
 * Drizzle Client & Protocol Library
 *
 * Copyright (C) 2011 Brian Aker (brian@tangent.org)
 * Copyright (C) 2008 Eric Day (eday@oddments.org)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *
 *     * The names of its contributors may not be used to endorse or
 * promote products derived from this software without specific prior
 * written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#pragma once

/**
 * @file
 * @brief Defines, typedefs, enums, and macros
 */

#include <libdrizzle/return.h>
#include <libdrizzle/verbose.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup drizzle_constants Constants
 * @ingroup drizzle_client_interface
 * @ingroup drizzle_server_interface
 * @{
 */

/* Defines. */
#define DRIZZLE_DEFAULT_TCP_HOST         "localhost"
#define DRIZZLE_DEFAULT_TCP_PORT         3306
#define DRIZZLE_DEFAULT_UDS              "/tmp/mysql.sock"
#define DRIZZLE_DEFAULT_BACKLOG          64
#define DRIZZLE_MAX_ERROR_SIZE           2048
#define DRIZZLE_MAX_USER_SIZE            64
#define DRIZZLE_MAX_PASSWORD_SIZE        32
#define DRIZZLE_MAX_DB_SIZE              64
#define DRIZZLE_MAX_INFO_SIZE            2048
#define DRIZZLE_MAX_SQLSTATE_SIZE        5
#define DRIZZLE_MAX_CATALOG_SIZE         128
#define DRIZZLE_MAX_TABLE_SIZE           128
#define DRIZZLE_MAX_COLUMN_NAME_SIZE     2048
#define DRIZZLE_MAX_DEFAULT_VALUE_SIZE   2048
#define DRIZZLE_MAX_PACKET_SIZE          UINT32_MAX
#define DRIZZLE_MAX_BUFFER_SIZE          32768
#define DRIZZLE_BUFFER_COPY_THRESHOLD    8192
#define DRIZZLE_MAX_SERVER_VERSION_SIZE  32
#define DRIZZLE_MAX_SERVER_EXTRA_SIZE    32
#define DRIZZLE_MAX_SCRAMBLE_SIZE        20
#define DRIZZLE_STATE_STACK_SIZE         8
#define DRIZZLE_ROW_GROW_SIZE            8192
#define DRIZZLE_DEFAULT_SOCKET_TIMEOUT   10
#define DRIZZLE_DEFAULT_SOCKET_SEND_SIZE 32768
#define DRIZZLE_DEFAULT_SOCKET_RECV_SIZE 32768
#define DRIZZLE_MYSQL_PASSWORD_HASH      41

#define DRIZZLE_BINLOG_MAGIC             "\xFE\x62\x69\x6E"

/** @} */

/**
 * @ingroup drizzle
 * Options for drizzle_st.
 */
enum drizzle_options_t
{
  DRIZZLE_NONE=            0,
  DRIZZLE_NON_BLOCKING=    (1 << 1),
  DRIZZLE_FREE_OBJECTS=    (1 << 2),
  DRIZZLE_ASSERT_DANGLING= (1 << 3)
};

#ifndef __cplusplus
typedef enum drizzle_options_t drizzle_options_t;
#endif

/**
 * @ingroup drizzle_con
 * Options for drizzle_con_st.
 */
enum drizzle_con_options_t
{
  DRIZZLE_CON_NONE=             0,
  DRIZZLE_CON_RAW_PACKET=       (1 << 2),
  DRIZZLE_CON_RAW_SCRAMBLE=     (1 << 3),
  DRIZZLE_CON_READY=            (1 << 4),
  DRIZZLE_CON_NO_RESULT_READ=   (1 << 5),
  DRIZZLE_CON_IO_READY=         (1 << 6),
  DRIZZLE_CON_LISTEN=           (1 << 7),
  DRIZZLE_CON_FOUND_ROWS=       (1 << 9),
  DRIZZLE_CON_INTERACTIVE=      (1 << 11),
  DRIZZLE_CON_MULTI_STATEMENTS= (1 << 12),
  DRIZZLE_CON_AUTH_PLUGIN=      (1 << 13)
};

#ifndef __cplusplus
typedef enum drizzle_con_options_t drizzle_con_options_t;
#endif

/**
 * @ingroup drizzle_con
 * Socket types for drizzle_con_st.
 */
enum drizzle_con_socket_t
{
  DRIZZLE_CON_SOCKET_TCP= 0,
  DRIZZLE_CON_SOCKET_UDS= (1 << 0)
};

#ifndef __cplusplus
typedef enum drizzle_con_socket_t drizzle_con_socket_t;
#endif

/**
 * @ingroup drizzle_con
 * Status flags for drizle_con_st.
 */
enum drizzle_con_status_t
{
  DRIZZLE_CON_STATUS_NONE=                     0,
  DRIZZLE_CON_STATUS_IN_TRANS=                 (1 << 0),
  DRIZZLE_CON_STATUS_AUTOCOMMIT=               (1 << 1),
  DRIZZLE_CON_STATUS_MORE_RESULTS_EXISTS=      (1 << 3),
  DRIZZLE_CON_STATUS_QUERY_NO_GOOD_INDEX_USED= (1 << 4),
  DRIZZLE_CON_STATUS_QUERY_NO_INDEX_USED=      (1 << 5),
  DRIZZLE_CON_STATUS_CURSOR_EXISTS=            (1 << 6),
  DRIZZLE_CON_STATUS_LAST_ROW_SENT=            (1 << 7),
  DRIZZLE_CON_STATUS_DB_DROPPED=               (1 << 8),
  DRIZZLE_CON_STATUS_NO_BACKSLASH_ESCAPES=     (1 << 9),
  DRIZZLE_CON_STATUS_QUERY_WAS_SLOW=           (1 << 10)
};

#ifndef __cplusplus
typedef enum drizzle_con_status_t drizzle_con_status_t;
#endif

/**
 * @ingroup drizzle_con
 * Capabilities for drizzle_con_st.
 */
enum drizzle_capabilities_t
{
  DRIZZLE_CAPABILITIES_NONE=                   0,
  DRIZZLE_CAPABILITIES_LONG_PASSWORD=          (1 << 0),
  DRIZZLE_CAPABILITIES_FOUND_ROWS=             (1 << 1),
  DRIZZLE_CAPABILITIES_LONG_FLAG=              (1 << 2),
  DRIZZLE_CAPABILITIES_CONNECT_WITH_DB=        (1 << 3),
  DRIZZLE_CAPABILITIES_NO_SCHEMA=              (1 << 4),
  DRIZZLE_CAPABILITIES_COMPRESS=               (1 << 5),
  DRIZZLE_CAPABILITIES_ODBC=                   (1 << 6),
  DRIZZLE_CAPABILITIES_LOCAL_FILES=            (1 << 7),
  DRIZZLE_CAPABILITIES_IGNORE_SPACE=           (1 << 8),
  DRIZZLE_CAPABILITIES_PROTOCOL_41=            (1 << 9),
  DRIZZLE_CAPABILITIES_INTERACTIVE=            (1 << 10),
  DRIZZLE_CAPABILITIES_SSL=                    (1 << 11),
  DRIZZLE_CAPABILITIES_IGNORE_SIGPIPE=         (1 << 12),
  DRIZZLE_CAPABILITIES_TRANSACTIONS=           (1 << 13),
  DRIZZLE_CAPABILITIES_RESERVED=               (1 << 14),
  DRIZZLE_CAPABILITIES_SECURE_CONNECTION=      (1 << 15),
  DRIZZLE_CAPABILITIES_MULTI_STATEMENTS=       (1 << 16),
  DRIZZLE_CAPABILITIES_MULTI_RESULTS=          (1 << 17),
  DRIZZLE_CAPABILITIES_PS_MULTI_RESULTS=       (1 << 18),
  DRIZZLE_CAPABILITIES_PLUGIN_AUTH=            (1 << 19),
  DRIZZLE_CAPABILITIES_SSL_VERIFY_SERVER_CERT= (1 << 30),
  DRIZZLE_CAPABILITIES_REMEMBER_OPTIONS=       (1 << 31),
  DRIZZLE_CAPABILITIES_CLIENT= (DRIZZLE_CAPABILITIES_LONG_PASSWORD |
                                DRIZZLE_CAPABILITIES_FOUND_ROWS |
                                DRIZZLE_CAPABILITIES_LONG_FLAG |
                                DRIZZLE_CAPABILITIES_CONNECT_WITH_DB |
                                DRIZZLE_CAPABILITIES_PLUGIN_AUTH |
                                DRIZZLE_CAPABILITIES_TRANSACTIONS |
                                DRIZZLE_CAPABILITIES_PROTOCOL_41 |
                                DRIZZLE_CAPABILITIES_SECURE_CONNECTION)
};

#ifndef __cplusplus
typedef enum drizzle_capabilities_t drizzle_capabilities_t;
#endif

/**
 * @ingroup drizzle_command 
 * Commands for drizzle_command functions.
 */
enum drizzle_command_t
{
  DRIZZLE_COMMAND_SLEEP,               /* Not used currently. */
  DRIZZLE_COMMAND_QUIT,
  DRIZZLE_COMMAND_INIT_DB,
  DRIZZLE_COMMAND_QUERY,
  DRIZZLE_COMMAND_FIELD_LIST,          /* Deprecated. */
  DRIZZLE_COMMAND_CREATE_DB,           /* Deprecated. */
  DRIZZLE_COMMAND_DROP_DB,             /* Deprecated. */
  DRIZZLE_COMMAND_REFRESH,
  DRIZZLE_COMMAND_SHUTDOWN,
  DRIZZLE_COMMAND_STATISTICS,
  DRIZZLE_COMMAND_PROCESS_INFO,        /* Deprecated. */
  DRIZZLE_COMMAND_CONNECT,             /* Not used currently. */
  DRIZZLE_COMMAND_PROCESS_KILL,        /* Deprecated. */
  DRIZZLE_COMMAND_DEBUG,
  DRIZZLE_COMMAND_PING,
  DRIZZLE_COMMAND_TIME,                /* Not used currently. */
  DRIZZLE_COMMAND_DELAYED_INSERT,      /* Not used currently. */
  DRIZZLE_COMMAND_CHANGE_USER,
  DRIZZLE_COMMAND_BINLOG_DUMP,         /* Not used currently. */
  DRIZZLE_COMMAND_TABLE_DUMP,          /* Not used currently. */
  DRIZZLE_COMMAND_CONNECT_OUT,         /* Not used currently. */
  DRIZZLE_COMMAND_REGISTER_SLAVE,      /* Not used currently. */
  DRIZZLE_COMMAND_STMT_PREPARE,        /* Not used currently. */
  DRIZZLE_COMMAND_STMT_EXECUTE,        /* Not used currently. */
  DRIZZLE_COMMAND_STMT_SEND_LONG_DATA, /* Not used currently. */
  DRIZZLE_COMMAND_STMT_CLOSE,          /* Not used currently. */
  DRIZZLE_COMMAND_STMT_RESET,          /* Not used currently. */
  DRIZZLE_COMMAND_SET_OPTION,          /* Not used currently. */
  DRIZZLE_COMMAND_STMT_FETCH,          /* Not used currently. */
  DRIZZLE_COMMAND_DAEMON,              /* Not used currently. */
  DRIZZLE_COMMAND_END                  /* Not used currently. */
};

#ifndef __cplusplus
typedef enum drizzle_command_t drizzle_command_t;
#endif

/**
 * @ingroup drizzle_query
 * States for drizle_query_st.
 */
enum drizzle_query_state_t
{
  DRIZZLE_QUERY_STATE_INIT,
  DRIZZLE_QUERY_STATE_QUERY,
  DRIZZLE_QUERY_STATE_RESULT,
  DRIZZLE_QUERY_STATE_DONE
};

#ifndef __cplusplus
typedef enum drizzle_query_state_t drizzle_query_state_t;
#endif

/**
 * @ingroup drizzle_result
 * Options for drizzle_result_st.
 */
enum drizzle_result_options_t
{
  DRIZZLE_RESULT_NONE=          0,
  DRIZZLE_RESULT_SKIP_COLUMN=   (1 << 1),
  DRIZZLE_RESULT_BUFFER_COLUMN= (1 << 2),
  DRIZZLE_RESULT_BUFFER_ROW=    (1 << 3),
  DRIZZLE_RESULT_EOF_PACKET=    (1 << 4),
  DRIZZLE_RESULT_ROW_BREAK=     (1 << 5)
};

#ifndef __cplusplus
typedef enum drizzle_result_options_t drizzle_result_options_t;
#endif

/**
 * @ingroup drizzle_column
 * Options for drizzle_column_st, currently unused.
 */
enum drizzle_column_options_t
{
  DRIZZLE_COLUMN_UNUSED= (1 << 0)
};

#ifndef __cplusplus
typedef enum drizzle_column_options_t drizzle_column_options_t;
#endif

/**
 * @ingroup drizzle_column
 * Types for drizzle_column_st.
 */
enum drizzle_column_type_t
{
  DRIZZLE_COLUMN_TYPE_DECIMAL,
  DRIZZLE_COLUMN_TYPE_TINY,
  DRIZZLE_COLUMN_TYPE_SHORT,
  DRIZZLE_COLUMN_TYPE_LONG,
  DRIZZLE_COLUMN_TYPE_FLOAT,
  DRIZZLE_COLUMN_TYPE_DOUBLE,
  DRIZZLE_COLUMN_TYPE_NULL,
  DRIZZLE_COLUMN_TYPE_TIMESTAMP,
  DRIZZLE_COLUMN_TYPE_LONGLONG,
  DRIZZLE_COLUMN_TYPE_INT24,
  DRIZZLE_COLUMN_TYPE_DATE,
  DRIZZLE_COLUMN_TYPE_TIME,
  DRIZZLE_COLUMN_TYPE_DATETIME,
  DRIZZLE_COLUMN_TYPE_YEAR,
  DRIZZLE_COLUMN_TYPE_NEWDATE,
  DRIZZLE_COLUMN_TYPE_VARCHAR,
  DRIZZLE_COLUMN_TYPE_BIT,
  DRIZZLE_COLUMN_TYPE_NEWDECIMAL=  246,
  DRIZZLE_COLUMN_TYPE_ENUM=        247,
  DRIZZLE_COLUMN_TYPE_SET=         248,
  DRIZZLE_COLUMN_TYPE_TINY_BLOB=   249,
  DRIZZLE_COLUMN_TYPE_MEDIUM_BLOB= 250,
  DRIZZLE_COLUMN_TYPE_LONG_BLOB=   251,
  DRIZZLE_COLUMN_TYPE_BLOB=        252,
  DRIZZLE_COLUMN_TYPE_VAR_STRING=  253,
  DRIZZLE_COLUMN_TYPE_STRING=      254,
  DRIZZLE_COLUMN_TYPE_GEOMETRY=    255
};

#ifndef __cplusplus
typedef enum drizzle_column_type_t drizzle_column_type_t;
#endif

/**
 * @ingroup drizzle_column
 * Flags for drizzle_column_st.
 */
enum drizzle_column_flags_t
{
  DRIZZLE_COLUMN_FLAGS_NONE=             0,
  DRIZZLE_COLUMN_FLAGS_NOT_NULL=         (1 << 0),
  DRIZZLE_COLUMN_FLAGS_PRI_KEY=          (1 << 1),
  DRIZZLE_COLUMN_FLAGS_UNIQUE_KEY=       (1 << 2),
  DRIZZLE_COLUMN_FLAGS_MULTIPLE_KEY=     (1 << 3),
  DRIZZLE_COLUMN_FLAGS_BLOB=             (1 << 4),
  DRIZZLE_COLUMN_FLAGS_UNSIGNED=         (1 << 5),
  DRIZZLE_COLUMN_FLAGS_ZEROFILL=         (1 << 6),
  DRIZZLE_COLUMN_FLAGS_BINARY=           (1 << 7),
  DRIZZLE_COLUMN_FLAGS_ENUM=             (1 << 8),
  DRIZZLE_COLUMN_FLAGS_AUTO_INCREMENT=   (1 << 9),
  DRIZZLE_COLUMN_FLAGS_TIMESTAMP=        (1 << 10),
  DRIZZLE_COLUMN_FLAGS_SET=              (1 << 11),
  DRIZZLE_COLUMN_FLAGS_NO_DEFAULT_VALUE= (1 << 12),
  DRIZZLE_COLUMN_FLAGS_ON_UPDATE_NOW=    (1 << 13),
  DRIZZLE_COLUMN_FLAGS_PART_KEY=         (1 << 14),
  DRIZZLE_COLUMN_FLAGS_NUM=              (1 << 15),
  DRIZZLE_COLUMN_FLAGS_GROUP=            (1 << 15), /* NUM & GROUP the same. */
  DRIZZLE_COLUMN_FLAGS_UNIQUE=           (1 << 16),
  DRIZZLE_COLUMN_FLAGS_BINCMP=           (1 << 17),
  DRIZZLE_COLUMN_FLAGS_GET_FIXED_FIELDS= (1 << 18),
  DRIZZLE_COLUMN_FLAGS_IN_PART_FUNC=     (1 << 19),
  DRIZZLE_COLUMN_FLAGS_IN_ADD_INDEX=     (1 << 20),
  DRIZZLE_COLUMN_FLAGS_RENAMED=          (1 << 21)
};

typedef enum
{
  DRIZZLE_SSL_STATE_NONE= 0,
  DRIZZLE_SSL_STATE_HANDSHAKE_COMPLETE
} drizzle_ssl_state_t;

typedef enum
{
  DRIZZLE_EVENT_POSITION_TIMESTAMP= 0,
  DRIZZLE_EVENT_POSITION_TYPE= 4,
  DRIZZLE_EVENT_POSITION_SERVERID= 5,
  DRIZZLE_EVENT_POSITION_LENGTH= 9,
  DRIZZLE_EVENT_POSITION_NEXT= 13,
  DRIZZLE_EVENT_POSITION_FLAGS= 17,
  DRIZZLE_EVENT_POSITION_EXTRA_FLAGS= 19
} drizzle_binlog_event_positions_t;

typedef enum
{
  DRIZZLE_EVENT_TYPE_UNKNOWN= 0,
  DRIZZLE_EVENT_TYPE_START= 1,
  DRIZZLE_EVENT_TYPE_QUERY= 2,
  DRIZZLE_EVENT_TYPE_STOP= 3,
  DRIZZLE_EVENT_TYPE_ROTATE= 4,
  DRIZZLE_EVENT_TYPE_INTVAR= 5,
  DRIZZLE_EVENT_TYPE_LOAD= 6,
  DRIZZLE_EVENT_TYPE_SLAVE= 7,
  DRIZZLE_EVENT_TYPE_CREATE_FILE= 8,
  DRIZZLE_EVENT_TYPE_APPEND_BLOCK= 9,
  DRIZZLE_EVENT_TYPE_EXEC_LOAD= 10,
  DRIZZLE_EVENT_TYPE_DELETE_FILE= 11,
  DRIZZLE_EVENT_TYPE_NEW_LOAD= 12,
  DRIZZLE_EVENT_TYPE_RAND= 13,
  DRIZZLE_EVENT_TYPE_USER_VAR= 14,
  DRIZZLE_EVENT_TYPE_FORMAT_DESCRIPTION= 15,
  DRIZZLE_EVENT_TYPE_XID= 16,
  DRIZZLE_EVENT_TYPE_BEGIN_LOAD_QUERY= 17,
  DRIZZLE_EVENT_TYPE_EXECUTE_LOAD_QUERY= 18,
  DRIZZLE_EVENT_TYPE_TABLE_MAP= 19,
  // Next 3 are used in 5.1.0 -> 5.1.15 only
  DRIZZLE_EVENT_TYPE_OBSOLETE_WRITE_ROWS= 20,
  DRIZZLE_EVENT_TYPE_OBSOLETE_UPDATE_ROWS= 21,
  DRIZZLE_EVENT_TYPE_OBSOLETE_DELETE_ROWS= 22,
  DRIZZLE_EVENT_TYPE_WRITE_ROWS= 23,
  DRIZZLE_EVENT_TYPE_UPDATE_ROWS= 24,
  DRIZZLE_EVENT_TYPE_DELETE_ROWS= 25,
  DRIZZLE_EVENT_TYPE_INCIDENT= 26,
  DRIZZLE_EVENT_TYPE_HEARTBEAT= 27,
  DRIZZLE_EVENT_TYPE_END
} drizzle_binlog_event_types_t;

#ifndef __cplusplus
typedef enum drizzle_column_flags_t drizzle_column_flags_t;
#endif

/**
 * @addtogroup drizzle_types Types
 * @ingroup drizzle_client_interface
 * @ingroup drizzle_server_interface
 * @{
 */

/* Types. */
typedef struct drizzle_st drizzle_st;
typedef struct drizzle_con_tcp_st drizzle_con_tcp_st;
typedef struct drizzle_con_uds_st drizzle_con_uds_st;
typedef struct drizzle_con_st drizzle_con_st;
typedef struct drizzle_result_st drizzle_result_st;
typedef struct drizzle_column_st drizzle_column_st;
typedef struct drizzle_binlog_st drizzle_binlog_st;
typedef char *drizzle_field_t;
typedef drizzle_field_t *drizzle_row_t;
typedef uint8_t drizzle_charset_t;

/* Function types. */
typedef void (drizzle_context_free_fn)(drizzle_st *drizzle,
                                       void *context);
typedef void (drizzle_log_fn)(const char *line, drizzle_verbose_t verbose,
                              void *context);
typedef drizzle_return_t (drizzle_state_fn)(drizzle_con_st *con);
typedef void (drizzle_con_context_free_fn)(drizzle_con_st *con,
                                           void *context);
/**
 * Custom function to register or deregister interest in file descriptor
 * events. See drizzle_set_event_watch_fn().
 *
 * @param[in] con Connection that has changed the events it is interested in.
 *  Use drizzle_con_fd() to get the file descriptor.
 * @param[in] events A bit mask of POLLIN | POLLOUT, specifying if the
 *  connection is waiting for read or write events.
 * @param[in] context Application context pointer registered with
 *  drizzle_set_event_watch_fn().
 * @return DRIZZLE_RETURN_OK if successful.
 */
typedef drizzle_return_t (drizzle_event_watch_fn)(drizzle_con_st *con,
                                                  short events,
                                                  void *context);

/** @} */

/**
 * @addtogroup drizzle_macros Macros
 * @ingroup drizzle_client_interface
 * @ingroup drizzle_server_interface
 * @{
 */

/* Protocol unpacking macros. */
#define drizzle_get_byte2(__buffer) \
  (uint16_t)((__buffer)[0] | \
            ((__buffer)[1] << 8))
#define drizzle_get_byte3(__buffer) \
  (uint32_t)((__buffer)[0] | \
            ((__buffer)[1] << 8) | \
            ((__buffer)[2] << 16))
#define drizzle_get_byte4(__buffer) \
  (uint32_t)((__buffer)[0] | \
            ((__buffer)[1] << 8) | \
            ((__buffer)[2] << 16) | \
            ((__buffer)[3] << 24))
#define drizzle_get_byte8(__buffer) \
  ((uint64_t)(__buffer)[0] | \
  ((uint64_t)(__buffer)[1] << 8) | \
  ((uint64_t)(__buffer)[2] << 16) | \
  ((uint64_t)(__buffer)[3] << 24) | \
  ((uint64_t)(__buffer)[4] << 32) | \
  ((uint64_t)(__buffer)[5] << 40) | \
  ((uint64_t)(__buffer)[6] << 48) | \
  ((uint64_t)(__buffer)[7] << 56))

/* Protocol packing macros. */
#define drizzle_set_byte2(__buffer, __int) do { \
  (__buffer)[0]= (uint8_t)((__int) & 0xFF); \
  (__buffer)[1]= (uint8_t)(((__int) >> 8) & 0xFF); } while (0)
#define drizzle_set_byte3(__buffer, __int) do { \
  (__buffer)[0]= (uint8_t)((__int) & 0xFF); \
  (__buffer)[1]= (uint8_t)(((__int) >> 8) & 0xFF); \
  (__buffer)[2]= (uint8_t)(((__int) >> 16) & 0xFF); } while (0)
#define drizzle_set_byte4(__buffer, __int) do { \
  (__buffer)[0]= (uint8_t)((__int) & 0xFF); \
  (__buffer)[1]= (uint8_t)(((__int) >> 8) & 0xFF); \
  (__buffer)[2]= (uint8_t)(((__int) >> 16) & 0xFF); \
  (__buffer)[3]= (uint8_t)(((__int) >> 24) & 0xFF); } while (0)
#define drizzle_set_byte8(__buffer, __int) do { \
  (__buffer)[0]= (uint8_t)((__int) & 0xFF); \
  (__buffer)[1]= (uint8_t)(((__int) >> 8) & 0xFF); \
  (__buffer)[2]= (uint8_t)(((__int) >> 16) & 0xFF); \
  (__buffer)[3]= (uint8_t)(((__int) >> 24) & 0xFF); \
  (__buffer)[4]= (uint8_t)(((__int) >> 32) & 0xFF); \
  (__buffer)[5]= (uint8_t)(((__int) >> 40) & 0xFF); \
  (__buffer)[6]= (uint8_t)(((__int) >> 48) & 0xFF); \
  (__buffer)[7]= (uint8_t)(((__int) >> 56) & 0xFF); } while (0)

/* Multi-byte character macros. */
#define drizzle_mb_char(__c) (((__c) & 0x80) != 0)
#define drizzle_mb_length(__c) \
  ((uint32_t)(__c) <= 0x7f ? 1 : \
  ((uint32_t)(__c) <= 0x7ff ? 2 : \
  ((uint32_t)(__c) <= 0xd7ff ? 3 : \
  ((uint32_t)(__c) <= 0xdfff || (uint32_t)(__c) > 0x10ffff ? 0 : \
  ((uint32_t)(__c) <= 0xffff ? 3 : 4)))))

/** @} */

#ifdef __cplusplus
}
#endif

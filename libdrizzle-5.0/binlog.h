/* vim:expandtab:shiftwidth=2:tabstop=2:smarttab:
 *
 * Drizzle Client & Protocol Library
 *
 * Copyright (C) 2012 Andrew Hutchings (andrew@linuxjedi.co.uk)
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

#ifdef __cplusplus
extern "C" {
#endif

DRIZZLE_API
drizzle_result_st *drizzle_start_binlog(drizzle_con_st *con,
                                        uint32_t server_id,
                                        const char *file,
                                        uint32_t start_position,
                                        drizzle_return_t *ret_ptr);

DRIZZLE_API
drizzle_return_t drizzle_binlog_get_next_event(drizzle_result_st *result);

DRIZZLE_API
uint32_t drizzle_binlog_event_timestamp(drizzle_result_st *result);

DRIZZLE_API
drizzle_binlog_event_types_t drizzle_binlog_event_type(drizzle_result_st *result);

DRIZZLE_API
uint32_t drizzle_binlog_event_server_id(drizzle_result_st *result);

DRIZZLE_API
uint32_t drizzle_binlog_event_length(drizzle_result_st *result);

DRIZZLE_API
uint32_t drizzle_binlog_event_next_pos(drizzle_result_st *result);

DRIZZLE_API
uint16_t drizzle_binlog_event_flags(drizzle_result_st *result);

DRIZZLE_API
uint16_t drizzle_binlog_event_extra_flags(drizzle_result_st *result);

DRIZZLE_API
const uint8_t *drizzle_binlog_event_data(drizzle_result_st *result);

DRIZZLE_API
const uint8_t *drizzle_binlog_event_raw_data(drizzle_result_st *result);

DRIZZLE_API
uint32_t drizzle_binlog_event_raw_length(drizzle_result_st *result);

DRIZZLE_LOCAL
drizzle_return_t drizzle_state_binlog_read(drizzle_con_st *con);

#ifdef __cplusplus
}
#endif

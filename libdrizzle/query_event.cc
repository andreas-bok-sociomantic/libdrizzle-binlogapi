/* 
 * Copyright (C) 2013 Drizzle Developer Group
 * Copyright (C) 2013 Kuldeep Porwal
 * All rights reserved.
 *
 * Use and distribution licensed under the BSD license.  See
 * the COPYING file in the parent directory for full text.
 *
 * summary: parse query event
 *
 */
#include "config.h"
#include<iostream>
#include "libdrizzle/common.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <inttypes.h>
#include<string.h>

#ifndef write_map
#define write_map

#include<libdrizzle-5.1/query_event.h>

#endif
//#include"helper.h"

using namespace std;
using namespace binlogevent;



void QueryEvent::initWithData(const unsigned char* data)
{
	uint64_t tmp;
	int start_pos = header.setHeader(data);
	if(start_pos==-1)
		return;
	
	tmp = getByte4(start_pos,data); 
	if(tmp==UINT_MAX)
		return;
	setProxyId((uint32_t)tmp); 
	start_pos+=4;// 4 byte for proxy id.

	tmp = getByte4(start_pos,data); 
	if(tmp==UINT_MAX)
		return;
	setExecutionTime((uint32_t)tmp);
	start_pos+=4;// 4 byte for execution time.

	if(sizeof(data)-start_pos<1)
		return;
	setSchemaLength((uint8_t)data[start_pos]);
	start_pos+=1;// 1 byte for schema length.

	tmp = getByte2(start_pos,data); 
	if(tmp==USHRT_MAX)
		return;
	setErrorCode((uint16_t)tmp);
	start_pos+=2;// 2 byte for error code.
	
}

// getters

uint32_t QueryEvent::getTimestamp()
{
	return  header.timestamp;
}
uint8_t QueryEvent::getType()
{
	return header.type; 
}
uint32_t QueryEvent::getServerId()
{
	return header.server_id;
}
uint32_t QueryEvent::getEventSize()
{
	return header.event_size;
}
uint32_t QueryEvent::getLogPos()
{
	return header.log_pos;
}
uint16_t QueryEvent::getFlagH()
{
	return header.flag; 
}
uint32_t QueryEvent::getProxyId()
{
	return proxy_id;
}
uint32_t QueryEvent::getExecutionTime()
{
	return execution_time;
}
uint8_t QueryEvent::getSchemaLength()
{
	return schema_length;
}
uint16_t QueryEvent::getErrorCode()
{
	return error_code;
}

//setters

void QueryEvent::setProxyId(uint32_t value)
{
	proxy_id = value;
}
void QueryEvent::setExecutionTime(uint32_t value)
{
	execution_time = value;
}
void QueryEvent::setSchemaLength(uint8_t value)
{
	schema_length = value;
}
void QueryEvent::setErrorCode(uint16_t value)
{
	error_code = value;
}

/* 
 * Copyright (C) 2013 Drizzle Developer Group
 * Copyright (C) 2013 Kuldeep Porwal
 * All rights reserved.
 *
 * Use and distribution licensed under the BSD license.  See
 * the COPYING file in the parent directory for full text.
 *
 * summary: parse xid event
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

#include<libdrizzle-5.1/xid_event.h>


#ifndef HELPER
#define HELPER

#include<libdrizzle-5.1/helper.h>

#endif

using namespace std;
using namespace binlogevent;



void XidEvent::initWithData(const unsigned char* data)
{

	int start_pos = header.setHeader(data);
	if(start_pos==-1)
		return;
	uint64_t tmp;

	tmp=getByte8(start_pos,data);
	if(tmp==UINT_MAX)
		return;
	setXid((uint64_t)tmp);
}

// getters

uint32_t XidEvent::getTimestamp()
{
	return  header.timestamp;
}
enum_event_type XidEvent::getType()
{
	return (enum_event_type)header.type; 
}
uint32_t XidEvent::getServerId()
{
	return header.server_id;
}
uint32_t XidEvent::getLogPos()
{
	return header.log_pos;
}
uint16_t XidEvent::getFlagH()
{
	return header.flag; 
}
uint64_t XidEvent::getXid()
{
	return Xid;
}

//setters

void XidEvent::setXid(uint64_t value)
{
	Xid = value;
}

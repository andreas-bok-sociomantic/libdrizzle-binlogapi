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
#include<cstring>
#include<inttypes.h>

#ifndef event_interface
#define event_interface
#include"event_interface.h"
#endif


using namespace std;

#ifndef QUERY_EVENT
#define QUERY_EVENT
//using namespace binlogevent;
namespace binlogevent
{
	class QueryEvent : public Events 
	{
		public:

//			QueryEvent(TableMapEvent& tableMap) : table_id(0),
			QueryEvent() : proxy_id(-1),
					execution_time(-1),
					schema_length(-1),
					error_code(-1)
			{
			}	

			~QueryEvent()
			{
			}


			/** Get the raw data and call all the setters with
			  * appropriate value
			  *
			  * @param[in] data Raw data from binglog.
			  */
			DRIZZLE_API
			void initWithData(const unsigned char * data);

			//getters

			/**
			  * @retval timestamp of event.
			  */
			
			DRIZZLE_API
			uint32_t getTimestamp();
			/**
			  * @retval type of event.
			  */
			
			DRIZZLE_API
			uint8_t getType();
			
			/**
			  * @retval server-id of the originating mysql-server. Used to filter out events in circular replication.
			  */
			DRIZZLE_API
			uint32_t getServerId();
			
			/**
			  * @retval of size of event.
			  */
			DRIZZLE_API
			uint32_t getEventSize();
			/**
			  * @retval of position of the next event .
			  */
			DRIZZLE_API
			uint32_t getLogPos();
			
			/**
			  * @retval Binlog Event Flag from header
			  */
			DRIZZLE_API
			uint16_t getFlagH();

			/**
			  * @retval proxy id. 
			  */
		
			DRIZZLE_API
			uint32_t getProxyId();
			
			/**
			  * @retval execution time.
			  */
			DRIZZLE_API
			uint32_t getExecutionTime();
			
			/**
			  *@retval schema length.
			  */
			DRIZZLE_API
			uint8_t getSchemaLength();
			
			/**
			  *@retval error code.
			  */
			DRIZZLE_API
			uint16_t getErrorCode();
			
			

			//setters
			/** set master proxy Id
			  *
			  *@param[in] value Proxy Id of master
			  */

			void setProxyId(uint32_t value);
			
			/** set execution time.
			  *
			  * @param[in] value Execution time of query.
			  */
			void setExecutionTime(uint32_t value);
			
			/** set schema length
			  *
			  * @param[in] value Schema length of query.
			  */
			void setSchemaLength(uint8_t value);
			
			/** set error code.
			  *
			  * @param[in] value Error code if any error occurred.
			  */
			void setErrorCode(uint16_t value);




		private:
			uint32_t proxy_id;  
			uint32_t execution_time;
			uint8_t schema_length; 
			uint16_t error_code;

	}; // query event
} /*namespace binlogevent*/
#endif


Binlog Retrieval
================

.. code-block:: c

/**
  * Basic Example to show the use of api
  */

#include<iostream>
#include <libdrizzle-5.1/libdrizzle.h>
#include <libdrizzle-5.1/binlogapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <inttypes.h>
#include<string.h>



using namespace binlogevent;

TableMapEvent tableMap;
XidEvent xevent;

void binlog_error(drizzle_return_t ret, drizzle_st *con, void *context)
{
	
	(void) context;
	if (ret != DRIZZLE_RETURN_EOF)
	{
		printf("Error retrieving binlog: %s\n", drizzle_error(con));
	}

}

void binlog_event(drizzle_binlog_event_st *event, void *context)
{

	(void) context;
	uint32_t length;
	uint8_t type;
	int i;
	const unsigned char* data;
	uint32_t ts = drizzle_binlog_event_timestamp(event);
	length= drizzle_binlog_event_length(event);
	type= drizzle_binlog_event_type(event);
	cout<<(int)type<<endl;

	if(type==16)
	{
		data= drizzle_binlog_event_raw_data(event);
		EventData event_raw_data(data);
		event_raw_data.getXidEvent(xevent);
		printf("AS->%lu\n",xevent.getTimestamp());


	}
	
	if((int)type==19) //TABLE_MAP_EVENT)
	{
		data= drizzle_binlog_event_raw_data(event);
		EventData event_raw_data(data);
		event_raw_data.getTableMap(tableMap);
		cout << "Timestamp by data and raw_data :" << tableMap.getTableName() << ":  " << ts << endl;
		uint8_t *val=  tableMap.getColumnTypeDef();
		for(int i=0;i< tableMap.getColumnCount();i++)
		{
			cout<< (int)val[i]<<endl;
		}
	}
	if(type==23) //WRITE_ROW_EVENTv1
	{
		data= drizzle_binlog_event_raw_data(event);
		EventData event_raw_data(data);
		RowEvent writeEvent(tableMap.getColumnTypeDef());
		event_raw_data.getWriteEvent(writeEvent);
		cout << "col count by data and raw_data :" << writeEvent.getTimestamp() << ":  " <<  writeEvent.getColumnCount() << endl;
		cout << "QUIT event type: 23" << endl;
	}

}

int main(void)
{
	drizzle_st *con;
	drizzle_binlog_st *binlog;
	drizzle_return_t ret;

	con= drizzle_create("localhost", 3306, "root", "iiit", "", NULL);
	ret= drizzle_connect(con);
	if (ret != DRIZZLE_RETURN_OK)
	{
		printf("Could not connect to server: %s\n", drizzle_error(con));
		return EXIT_FAILURE;
	}

	binlog= drizzle_binlog_init(con, binlog_event, binlog_error, NULL, true);

	//ret= drizzle_binlog_start(binlog, 0, "mysql-bin.000150", 0);
	ret= drizzle_binlog_start(binlog, 0, "", 0);
	
	if (ret != DRIZZLE_RETURN_OK)
	{
		return EXIT_FAILURE;  
	}
	drizzle_quit(con);
	return EXIT_SUCCESS;
}

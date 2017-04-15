#include <stdlib.h>

#include "types.h"
#include "user_interface.h"
#include "socket_interface.h"
#include "reporting_engine.h"
#include "report_sender.h"

void send_report_data(uint64_t ieeeaddr, uint8_t commandid)
{
	UI_PRINT_LOG("Begin sending report to reporting daemon! addr: 0x%016X, cmdid: %u", ieeeaddr, commandid);

	pkt_buf_t * pkt = NULL;
	uint8_t len = 9;
	pkt = malloc(sizeof(pkt_buf_hdr_t) + len);
	if(pkt)
	{
		pkt->header.len = len;
		pkt->header.subsystem = REPORT_SENDER;
		pkt->header.cmd_id = CMD_SEND_REPORT;
		//uint8_t * buffer = malloc(sizeof(uint8_t));
		//uint8_t buffer[] = {7,8};
		pkt->packed_protobuf_packet[0] = (ieeeaddr>>(8*7))&0xFF;
		pkt->packed_protobuf_packet[1] = (ieeeaddr>>(8*6))&0xFF;
		pkt->packed_protobuf_packet[2] = (ieeeaddr>>(8*5))&0xFF;
		pkt->packed_protobuf_packet[3] = (ieeeaddr>>(8*4))&0xFF;
		pkt->packed_protobuf_packet[4] = (ieeeaddr>>(8*3))&0xFF;
		pkt->packed_protobuf_packet[5] = (ieeeaddr>>(8*2))&0xFF;
		pkt->packed_protobuf_packet[6] = (ieeeaddr>>(8*1))&0xFF;
		pkt->packed_protobuf_packet[7] = (ieeeaddr>>(8*0))&0xFF;
		pkt->packed_protobuf_packet[8] = commandid;

		if( si_send_packet(pkt, NULL, NULL) != 0)
		{
			UI_PRINT_LOG("send_report_data: Error: could not send msg.");
			ui_print_packet_to_log(pkt, "not sent: ", BOLD);
		}

		free(pkt);
	}
	UI_PRINT_LOG("Finished sender report to reporting daemon!!!");
}	


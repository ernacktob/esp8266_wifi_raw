#ifndef WIFI_RAW
#define WIFI_RAW

#include "c_types.h"

struct RxControl {
	signed rssi:8;
	unsigned rate:4;
	unsigned is_group:1;
	unsigned:1;
	unsigned sig_mode:2;
	unsigned legacy_length:12;
	unsigned damatch0:1;
	unsigned damatch1:1;
	unsigned bssidmatch0:1;
	unsigned bssidmatch1:1;
	unsigned MCS:7;
	unsigned CWB:1;
	unsigned HT_length:16;
	unsigned Smoothing:1;
	unsigned Not_Sounding:1;
	unsigned:1;
	unsigned Aggregation:1;
	unsigned STBC:2;
	unsigned FEC_CODING:1;
	unsigned SGI:1;
	unsigned rxend_state:8;
	unsigned ampdu_cnt:8;
	unsigned channel:4;
	unsigned:12;
};

struct RxPacket {
	struct RxControl rx_ctl;
	uint8 data[];
};

typedef void (*wifi_raw_recv_cb_fn)(struct RxPacket *);

void ICACHE_FLASH_ATTR wifi_set_raw_recv_cb(wifi_raw_recv_cb_fn rx_fn);
void ICACHE_FLASH_ATTR wifi_send_raw_packet(void *buf, uint16 len);

#endif

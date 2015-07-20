struct ieee80211_frame {
	uint8 i_fc[2];
	uint8 i_dur[2];
	uint8 i_addr1[6];
	uint8 i_addr2[6];
	uint8 i_addr3[6];
	uint8 i_seq[2];
	uint8 i_data[];
};

struct _ebuf_sub1 {
	uint8 data[24];
	/* byte 10 increases by 8 for a 1 byte increase in the length. */
	/* bytes 16 to 19 appear to be a timestamp in microseconds */
};

struct esf_buf {
	struct pbuf *pb1;                       /*  0 */
	struct pbuf *pb2;                       /*  4 */
	struct pbuf *pb3;                       /*  8 */
	uint16 cnt1;                            /* 12 */
	uint8 flg;                              /* 14 */
	uint8 pad1[1];
	struct ieee80211_frame *e_data;         /* 16 */
	uint16 len1;				/* 20 */
	uint16 len2;                            /* 22 */
	uint8 pad2[4];
	uint32 type1;                           /* 28 */
	struct esf_buf *next;                   /* 32 */
	struct _ebuf_sub1 *ep;                  /* 36 */
};

static void print_esf_buf(struct esf_buf *ebuf)
{
	int i;

	ets_uart_printf("ebuf = %p {\n", ebuf);
	ets_uart_printf("\tpb1 = %p\n", ebuf->pb1);
	ets_uart_printf("\tpb2 = %p\n", ebuf->pb2);
	ets_uart_printf("\tpb3 = %p\n", ebuf->pb3);
	ets_uart_printf("\tcnt1 = %d\n", ebuf->cnt1);
	ets_uart_printf("\tflg = 0x%02x\n", ebuf->flg);
	ets_uart_printf("\tpad1 = 0x%02x\n", ebuf->pad1[0]);
	ets_uart_printf("\te_data = %p {", ebuf->e_data);

	for (i = 0; i < 32; i++) {
		ets_uart_printf("0x%02x, ", ((uint8 *)(ebuf->e_data))[i]);
	}

	ets_uart_printf("0x%02x}\n", ((uint8 *)(ebuf->e_data))[32]);
	ets_uart_printf("\tlen1 = %d\n", ebuf->len1);
	ets_uart_printf("\tlen2 = %d\n", ebuf->len2);
	ets_uart_printf("\tpad2 = %02x %02x %02x %02x\n", ebuf->pad2[0], ebuf->pad2[1], ebuf->pad2[2], ebuf->pad2[3]);
	ets_uart_printf("\ttype1 = %p\n", (void *)(ebuf->type1));
	ets_uart_printf("\tep = %p {", ebuf->ep);

	for (i = 0; i < 23; i++) {
		ets_uart_printf("0x%02x, ", ebuf->ep->data[i]);
	}

	ets_uart_printf("0x%02x}\n");
	ets_uart_printf("}\n");
}

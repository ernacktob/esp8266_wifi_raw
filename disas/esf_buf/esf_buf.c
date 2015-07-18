struct ieee80211_frame {
	uint8 i_fc[2];
	uint8 i_dur[2];
	uint8 i_addr1[6];
	uint8 i_addr2[6];
	uint8 i_addr3[6];
	uint8 i_seq[2];
	uint8 i_data[];
};

struct esf_buf {
	struct pbuf *pb1;			/*  0 */
	struct pbuf *pb2;			/*  4 */
	uint8 pad1[8];
	struct ieee80211_frame *e_data;		/* 16 */
	uint8 pad2[6];
	uint16 len;				/* 22 */
	uint8 pad3[10];
	struct esf_buf *next;			/* 32 */
	struct _ebuf_sub1 *ep;			/* 36 */
};

struct esf_buf *esf_buf_alloc(struct pbuf *pb, uint32 flags)
{
	struct pbuf *a1_8;
	struct esf_buf *ebuf;

	$a5 = pb;
	$a2 = 0;
	$a0 = 0x3ffee780;

	if (flags == 1 && pb != NULL) {
		a1_8 = pb;

		if (*(struct esf_buf **)((uint8 *)0x3ffee780 + 0) == NULL)
			return NULL;

		ets_intr_lock();
		ebuf = *(struct esf_buf **)((uint8 *)0x3ffee780 + 0);
		*(struct esf_buf **)((uint8 *)0x3ffee780 + 0) = ebuf->next;
		ebuf->next = NULL;
		ets_intr_unlock();

		$a5 = 0x00002000;
		$a0 = ebuf;
		ebuf->pb1 = pb;
		ebuf->pb2->payload = pb->payload;
		$a3 = ebuf->ep;
		ebuf->len = pb->len;
		$a2 = *(uint32 *)((uint8 *)(ebuf->ep) + 0);
		ebuf->e_data = pb->payload - 36;
	} else if ($a3 == 4) {
		$a4 = *(uint32 *)((uint8 *)$a0 + 4);

		if ($a4 == 0)
			return NULL;

		/* do stuff... */
	} else if ($a3 == 5) {
		$a11 = *(uin32 *)((uint8 *)$a0 + 8);

		if ($a11 == 0)
			return NULL;

		/* do stuff... */
	} else if ($a3 == 6) {
		$a6 = *(uint32 *)((uint8 *)$a0 + 12);

		if ($a6 == 0)
			return NULL;

		/* do stuff */
	} else {
		return NULL;
	}

	$a4 = $a2 & 0x3f;
	$a6 = $a2 >> 6;
	$a6 |= $a5;
	$a6 <<= 6;
	$a4 |= $a6;
	*(uint32 *)((uint8 *)$a3 + 0) = $a4;
	$a2 = $a0;
	return $a2;
}

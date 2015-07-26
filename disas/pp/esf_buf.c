struct ieee80211_frame {
	uint16 i_fc;
	uint16 i_dur;
	uint8 i_addr1[6];
	uint8 i_addr2[6];
	uint8 i_addr3[6];
	uint16 i_seq;
};

struct _ebuf_sub1 {
	uint8 data[24];
	/* bytes 10 and 11 are set by RC_GetCtsTime, seem to increase by 8 per unit length */
	/* bytes 16 to 19 appear to be a timestamp in microseconds */
};

/* Might be a wdev_ctrl_sub1 */
struct not_pbuf {
	unsigned len1:12;	/*  0b */
	unsigned len2:12;	/* 12b */
	unsigned pad1:8;	/* 24b */
	void *data;		/*   4 */
};

struct esf_buf {
	struct pbuf *pb1;			/*  0 */
	struct not_pbuf *np1; /* Not a pbuf! */	/*  4 */
	struct not_pbuf *np2; /* Not a pbuf! */	/*  8 */
	uint16 cnt1;				/* 12 */
	uint8 flg;				/* 14 */
	uint8 pad1[1];
	struct ieee80211_frame *e_data;		/* 16 */
	uint16 len1;				/* 20 */
	uint16 len2;				/* 22 */
	uint8 pad3[4];
	void *type1;				/* 28 */
	struct esf_buf *next;			/* 32 */
	struct _ebuf_sub1 *ep;			/* 36 */
};

/* 0x40104b04 */
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

/* 0x40104c28 */
void esf_buf_recycle(struct esf_buf *ebuf, uint8 type)
{
	struct esf_buf *ebuf;

	$a4 = type - 1;
	a1_0 = ebuf;

	if ($a4 == 0)
		goto _0x40104ce2;

_0x40104c34:
	/* do stuff... */

_0x40104cdb:
	return;

_0x40104ce2:
	$a2 = *(uint32 *)((uint8 *)a1_0 + 36);
	ets_memset($a2, 0, 24);

	ets_intr_lock();
	$a4 = 0x3ffee780;
	$a3 = a1_0;
	$a5 = *(uint32 *)((uint8 *)$a4 + 0);
	*(uint32 *)((uint8 *)$a3 + 32) = $a5;
	*((uint8 *)$a4 + 0) = $a3;
	ets_intr_unlock();
	goto _0x40104cdb;
}

/* 0x40247d68 */
void ICACHE_FLASH_ATTR esf_buf_setup()
{
	uint32 a1_20;
	uint32 a1_24;

	$a13 = 0x3ffef760;
	$a12 = 0x3ffee7a0;

	for ($a15 = 60; $a15 >= 1; $a15 -= 12) {
		$a4 = wDevCtrl.f_36.f_4;
		$a5 = (uint8 *)$a4 + $a15;
		*(uint32 *)((uint8 *)$a12 + 4) = $a5;
		*(uint32 *)((uint8 *)$a12 + 8) = $a5;
		$a4 = *(uint32 *)((uint8 *)$a4 + 4);
		*(uint32 *)((uint8 *)$a12 + 16) = $a4;
		*(uint16 *)((uint8 *)$a12 + 12) = 1;
		*(uint32 *)((uint8 *)$a12 + 36) = $a13;

		esf_buf_recycle($a12, 1);
		$a12 += 40;	/* sizeof (struct esf_buf) */
		$a13 += 24;	/* sizeof (struct ieee80211_frame) */
	}

	a1_20 = 0x3ffeeca0;

	for ($a14 = 8; $a14 >= 1; $a14--) {
		$a8 = a1_20;
		$a10 = *(volatile uint16 *)((uint8 *)$a8 + 0);
		$a10 &= 0xf000;
		$a10 |= 0x0100;
		*(volatile uint16 *)((uint8 *)$a8 + 0) = $a10;
		*(uint32 *)((uint8 *)$a12 + 36) = $a13;
		*(uint32 *)((uint8 *)$a12 + 4) = $a8;
		*(uint32 *)((uint8 *)$a12 + 8) = $a8;
		*(uint16 *)((uint8 *)$a12 + 12) = 1;
		$a8 += 12;
		*(uint16 *)((uint8 *)$a12 + 16) = $a8;
		esf_buf_recycle($a12, 4);
		$a12 += 40;
		$a13 += 24;
		a1_20 += 0x10c;
	}

	a1_24 = 0x3ffef500;

	for ($a14 = 8; $a14 >= 1; $a14--) {
		$a4 = a1_24;
		$a6 = *(volatile uint16 *)((uint8 *)$a4 + 0);
		$a6 &= 0xf000;
		$a6 |= 0x0040;
		*(volatile uint16 *)((uint8 *)$a4 + 0) = $a6;
		*(uint32 *)((uint8 *)$a12 + 36) = $a13;
		*(uint32 *)((uint8 *)$a12 + 4) = $a4;
		*(uint32 *)((uint8 *)$a12 + 8) = $a4;
		*(uint16 *)((uint8 *)$a12 + 12) = 1;
		$a4 += 12;
		*(uint32 *)((uint8 *)$a12 + 16) = $a4;
		esf_buf_recycle($a12, 5);
		$a12 += 40;
		$a13 += 24;
		a1_24 += 76;
	}

	for ($a14 = 4; $a14 >= 1; $a14--) {
		*(uint32 *)((uint8 *)$a12 + 36) = $a13;
		esf_buf_recycle($a12, 6);
		$a12 += 40;
		$a13 += 24;
	}

	$a13 = 0x3ffef9c0;

	for ($a14 = 7; $a14 >= 1; $a14--) {
		*(uint32 *)((uint8 *)$a12 + 36) = $a13;
		esf_buf_recycle($a12, 7);
		$a12 += 40;
		$a13 += 12;
	}
}

struct ieee80211_frame {
	uint8 i_fc[2];
	uint8 i_dur[2];
	uint8 i_addr1[6];
	uint8 i_addr2[6];
	uint8 i_addr3[6];
	uint8 i_seq[2];
};

struct esf_buf {
	struct pbuf *pb;			/*  4 */
	struct ieee80211_frame *e_data;		/* 16 */
	uint8 pad1[20];
	struct _ebuf_sub1 *sub1;		/* 36 */
};

struct esf_buf *esf_buf_alloc(struct pbuf *pb, uint32 flags, uint32 len)
{
	struct pbuf *a1_8;

	$a5 = pb;
	$a2 = 0;
	$a0 = 0x3ffee780;

	if (flags != 1)
		goto _0x40104b1f;

	if (pb == NULL)
		goto _0x40104b1f;

	$a3 = *(uint32 *)((uint8 *)0x3ffee780 + 0);
	a1_8 = pb;

	if ($a3 == 0)
		goto _0x40104bac;

	goto _0x40104bb3;

_0x40104b1f:

_0x40104bac:
	return NULL;

_0x40104bb3:

}

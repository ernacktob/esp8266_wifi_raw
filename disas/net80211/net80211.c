struct ieee80211vap {
	uint8 pad1[43];
	uint8 iv_myaddr[6];		/*  43 */
};

struct ieee80211_xxx {
	uint8 macaddr[6];
	uint8 pad1[300];
};

struct ieee80211_node {
	struct ieee80211vap *ni_vap;	/*   0 */
	uint8 pad1[116];
	struct ieee80211xxx *ni_x1;	/* 120 */
	struct ieee80211xxx *ni_x2;	/* 124 */
	uint8 pad2[48];
	uint32 opmode;			/* 176 */
}

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
};

#define MCLBYTES	(1 << 8) /* size of an mbuf cluster */
#define MINCLSIZE	65

struct esf_buf *ieee80211_getmgtframe(uint8 **frm, uint32 headroom, uint32 pktlen)
{
	struct esf_buf *ebuf;
	uint32 len;

	len = roundup2(headroom + pktlen, 4);

	if (len > MCLBYTES)
		return NULL;

	if (len < MINCLSIZE)
		ebuf = esf_buf_alloc(NULL, 5, len);
	else
		ebuf = esf_buf_alloc(NULL, 4, len);

	if (ebuf != NULL)
		*frm = ebuf->pb->payload + headroom;

	return ebuf;
}

#define IEEE80211_FC0_VERSION_0		0x00
#define IEEE80211_FC0_TYPE_MASK		0x0c
#define IEEE80211_FC0_TYPE_DATA		0x08
#define IEEE80211_FC1_DIR_NODS		0x00
#define IEEE80211_FC1_DIR_TODS		0x01
#define IEEE80211_FC1_DIR_FROMDS	0x02

#define IEEE80211_SEQ_SEQ_SHIFT		4

#define htole16(x)			((uint16)(x))

#define IEEE80211_ADDR_LEN		6
#define IEEE80211_IS_MULTICAST(_a)	(*(_a) & 0x01)

enum ieee80211_opmode {
	IEEE80211_M_STA		= 1,	/* infrastructure station */
	IEEE80211_M_IBSS	= 0,	/* IBSS (adhoc) station */
	IEEE80211_M_AHDEMO	= 3,	/* Old lucent compatible adhoc demo */
	IEEE80211_M_HOSTAP	= 6,	/* Software Access Point */
	IEEE80211_M_MONITOR	= 8,	/* Monitor mode */
	IEEE80211_M_WDS		= 2,	/* WDS link */
};

typedef uint16 ieee80211_seq;

void ieee80211_send_setup(struct ieee80211_node *ni, struct esf_buf *ebuf, int type, int tid,
				const uint8 sa[6], const uint8 da[6], const uint8 bssid[6])
{
	$a9 = 1;
	int a1_4 = tid;
	struct ieee80211_frame *wh = ebuf->e_data;
	ieee80211_seq seqno;
	struct esf_buf *a1_32 = ebuf;

	uint32 a1_28;
	uint8 a1_0[6];

	$a14 = da;
	$a13 = ni;
	$a3 = IEEE80211_FC0_TYPE_MASK & type;
	$a0 = *(uint8 *)((uint8 *)ni + 176);
	$a15 = esf_buf;
	$a2 = sa;
	a1_28 = *(uint32 *)((uint8 *)ni + 120);
	a1_0 = $a2;
	wh->i_fc[0] = IEEE80211_FC0_VERSION_0 | type;
	$a4 = (signed int8)$a0;

	if ((type & IEEE80211_FC0_TYPE_MASK) == IEEE80211_FC0_TYPE_DATA) {
		switch ($a0) {
			case 0:	/* STATION MODE? */
				if ($a0 == 0)
					a1_28 = *(uint32 *)((uint8 *)ni + 124);

				wh->i_fc[1] = IEEE80211_DIR_TODS;
				ets_memcpy(wh->i_addr1, bssid, 6);
				ets_memcpy(wh->i_addr2, sa, 6);
				ets_memcpy(wh->i_addr3, da, 6);
				break;

			case 1:	/* HOSTAP MODE? */
				wh->i_fc[1] = IEEE80211_FC1_DIR_FROMDS;
				ets_memcpy(wh->i_addr1, da, 6);	/* IEEE80211_ADDR_COPY */
				ets_memcpy(wh->i_addr2, bssid, 6);
				ets_memcpy(wh->i_addr3, sa, 6);
				break;
		}
	} else {
		if ($a0 == 0)
			a1_28 = *(uint32 *)((uint8 *)ni + 124);
		
		wh->i_fc[1] = IEEE80211_FC1_DIR_NODS;
		ets_memcpy(wh->i_addr1, da, 6);
		ets_memcpy(wh->i_addr2, sa, 6);
		ets_memcpy(wh->i_addr3, bssid, 6);	/* IEEE80211_SUPPORT_MESH? */
	}

	*(uint16 *)&wh->i_dur[0] = 0;	/* duration id */

	if ($a13 != NULL) {	/* how??? */
		$a7 = &((uint16 *)a1_28)[tid];	/* ni->tx_seqs[tid]*/
		seqno = *(uint16 *)((uint8 *)$a7 + 124)++;
		*(uint16 *)&wh->i_seq[0] = htole16(seqno << IEEE80211_SEQ_SEQ_SHIFT);
	}

	$a0 = *(uint32 *)((uint8 *)esf_buf + 36);

	if (IEEE80211_IS_MULTICAST(da)) {
		$a12 = 0x400;
		$a13 = *(uint8 *)((uint8 *)$a0 + 0);
		$a14 = 2;
		$a11 = $a13 & 0x3f;
		$a13 >>= 6;
		$a13 |= $a14;
		$a13 <<= 6;
		$a11 |= $a13;
		$a10 = $a11 & 0x3f;
		$a11 >>= 6;
		$a11 |= $a12;
		$a11 <<= 6;
		$a10 |= $a11;
		*(uint32 *)((uint8 *)$a0 + 0) = $a10;
	}

	$a14 = ((uint8 *)&g_ic + 125);
	*(uint8 *)((uint8 *)$a0 + 8) = $a14;
}

int ieee80211_mgmt_output(struct ieee80211_node *ni, struct esf_buf *ebuf, int type)
{
	int ret;
	uint32 a1_16;
	uint32 a1_0;

	$a6 = *(uint32 *)((uint8 *)ni + 0);	/* ni->ni_vap */
	$a7 = *(uint32 *)((uint8 *)ni + 120);	/* ni->ni_macaddr */

	$a12 = ebuf;
	$a3 = *(uint16 *)((uint8 *)ebuf + 22);
	$a0 = *(uint16 *)((uint8 *)ebuf + 20);
	$a0 += $a3;

	$a11 = *(uint32 *)((uint8 *)ebuf + 4);
	$a5 = *(volatile uint8 *)((uint8 *)$a11 + 3);
	$a5 &= 0x7f;
	$a5 |= 0x80;
	*(volatile uint8 *)((uint8 *)$a11 + 3) = $a5;

	$a8 = *(volatile uint8 *)((uint8 *)$a11 + 3);
	$a8 &= 0xbf;
	$a8 |= 0x40;
	*(volatile uint8 *)((uint8 *)$a11 + 3) = $a8;

	*(volatile uint8 *)((uint8 *)$a11 + 3) &= 0xdf;

	$a0 &= 0x0fff;
	$a3 = 0xff000fff;
	$a0 <<= 12;
	$a8 = *(volatile uint16 *)((uint8 *)$a11 + 2);
	$a8 <<= 16;
	$a5 = *(volatile uint16 *)((uint8 *)$a11 + 0);
	$a8 |= $a5;
	$a8 &= 0xff000fff;
	$a8 |= $a0;
	*(volatile uint16 *)((uint8 *)$a11 + 0) = $a8;
	$a8 = ($a8 >> 16) & 0xffff;
	*(volatile uint16 *)((uint8 *)$a11 + 2) = $a8;

	$a6 += 43;	/* vap->iv_myaddr ?*/
	$a0 = *(uint32 *)((uint8 *)ni + 176);	/* ni-> opmode ? */
	$a3 = *(uint32 *)((uint8 *)ebuf + 16);
	a1_16 = $a3;

	if ($a0 == 0) {
		a1_0 = $a7;
		$a3 = ebuf;
		$a5 = 16;
		ieee80211_send_setup(ni, ebuf, type, 16, $a6, $a7, $a7);
	} else {
		$a8 = 0xffff3fff;
		$a5 = *(uint16 *)($a7 + 26);

		if (!($a5 & 0xffff3fff))
			$a7 = (uint8 *)ni + 48;

		a1_0 = $a6;
		$a3 = ebuf;
		$a5 = 16;
		ieee80211_send_setup(ni, ebuf, type, 16, $a6, $a7, $a6);
	}

	$a9 = *(uint32 *)((uint8 *)ebuf + 36);
	$a11 = a1_16;
	$a9 = *(uint32 *)((uint8 *)$a9 + 0);
	$a0 = 64;

	if ($a9 & (1 << 6))
		*(uint8 *)((uint8 *)$a11 + 1) |= 0x40;

	if (chm_check_same_channel()) {
		ret = ppTxPkt(ebuf);
		return ret;
	}

	$a2 = 0;
	$a3 = (uint8 *)ebuf + 32;
	$a4 = 0x3ffefddc;
	$a6 = 0;
	$a5 = *(uint32 *)((uint8 *)0x3ffefddc + 0x1d4);
	*(uint32 *)((uint8 *)ebuf + 32) = 0;
	*(uint32 *)((uint8 *)$a5 + 0) = ebuf;
	*(uint32 *)((uint8 *)0x3ffefddc + 0x1d4) = $a3;
	return 0;
}

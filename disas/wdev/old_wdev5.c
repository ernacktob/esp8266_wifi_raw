struct _wdev_ctrl_sub1 {
	unsigned:12		 f_b0;	/* length of the sniffer_buf ? */
	unsigned:12		 f_b12;	
	unsigned:8		 f_b24;	/* last packet in queue? */
	struct sniffer_buf	*f_4;	/* the sniffer buf data */
	struct _wdev_ctrl_sub1	*f_8;	/* pointer to next element in queue? */
};

/* total size of this struct should be 4 bytes */
struct _wdev_ctrl_sub4 {
	unsigned:8		 f_b0;	/* Thing that should be 0 or between 224 and 252 */
	unsigned:12		 f_b8;	/* length field? */
	unsigned:12		 f_b20;
};

/* This might actually be pretty much the same as struct _wdev_ctrl_sub1 */
struct _wdev_ctrl_sub2 {
	unsigned:12		 f_b0;
	unsigned:12		 f_b12;	/* length field ?*/
	unsigned:8		 f_b24;
	struct _wdev_ctrl_sub4	*f_4;	/* Points to an array of these structs */
	struct _wdev_ctrl_sub2	*f_8;	/* pointer to next element in queue? */
};

/* Don't know much about this struct yet... */
struct _wdev_ctrl_sub3 {
	void			*f_8;
};

struct _wdev_ctrl {
	/* stub... most of the fields I have no idea what they mean */
	uint16			 f_0;	/* some counter ? */
	uint16			 f_2;	/* other counter ? */
	uint8			 f_4;	/* counter or flag? */
	uint8			 f_5;	/* whether in promiscuous mode or not */
	struct _wdev_ctrl_sub1	*f_8;	/* possibly a queue for sniffer bufs */
	struct _wdev_ctrl_sub3	*f_12;
	struct _wdev_ctrl_sub1	*f_16;	/* also a queue, move from here to f_8 */
	struct _wdev_ctrl_sub3	*f_20;
	void			*f_24;	/* Might be a struct (not a pointer), not sure yet... */
	struct _wdev_ctrl_sub2	*f_48;
	uint32			 f_356;	/* counts number of full packets? */
	// other fields...
};

struct _wdev_ctrl wDevCtrl;	/* XXX Don't know the size yet... */

/* <wDev_MacTim1Arm+0x68> */
static void _0x40103e64(uint32 arg1)
{
	/* arg1 can be 0 or 1 */
	if (((void **)&tcb + 159)[arg1] != NULL)
		((void (*)())(((void **)&tcb + 159)[arg1]))();
}

/* <trc_NeedRTS+0xc4> */
static void _0x40103a14(uint16 arg1, struct _wdev_ctrl_sub1 *arg2)
{
	wDevCtrl.f_0 -= arg1;

	if (wDevCtrl.f_0 < 2) {
		if (wDevCtrl.f_0 != 1) {
			wDevCtrl.f_0 = wDevCtrl.f_2;
			wDevCtrl.f_2 = 0;

			wDevCtrl.f_8 = wDevCtrl.f_16;
			wDevCtrl.f_16 = NULL;

			wDevCtrl.f_12 = wDevCtrl.f_20;
			wDevCtrl.f_20 = NULL;

			if (wDevCtrl.f_0 == 1)
				wDevCtrl.f_12->f_8 = &(wDevCtrl.f_24);

			arg2->f_8 = NULL;
			((volatile struct _wdev_ctrl_sub1 **)0x3ff1fe00)[130] = wDevCtrl.f_8;
		} else {
			wDevCtrl.f_8 = arg2->f_8;
			arg2->f_8 = NULL;
			wDevCtrl.f_12->f_8 = &(wDevCtrl.f_24);
		}
	} else {
		wDevCtrl.f_8 = arg2->f_8;
		arg2->f_8 = NULL;
	}

	if (wDevCtrl.f_0 + wDevCtrl.f_2 < 2) {
		((volatile uint32 *)0x3ff20a00)[170] &= 0x0fffffff;
		((volatile uint32 *)0x3ff20a00)[171] &= 0x0fffffff;
	}
}

/* <trc_NeedRTS+0x1d4> */
static void _0x40103b24(struct _wdev_ctrl_sub1 *arg1, uint16 arg2)
{
	_0x40103a14(arg2, arg1);	/* <trc_NeedRTS+0xc4> */
	wDev_AppendRxBlocks(wDevCtrl.f_8, arg1, arg2);
}

/* <trc_NeedRTS+0x204> */
static void _0x40103b54(struct _wdev_ctrl_sub2 *arg1, uint8 arg2)
{
	wDevCtrl.f_48 = arg1->f_8;
	arg1->f_8 = NULL;
	wDevCtrl.f_4 -= arg2;
	wDev_AppendRxAmpduLensBlocks(wDevCtrl.f_48, arg1);
}

/* <wDevDisableRx+0x34> */
static void _0x4010462c()
{
	struct RxControl *rx_ctl_p;
	struct RxControl *rx_ctl_new_p;

	if (wDevCtrl.f_0 < 2)
		return;

	$a2 = ((volatile uint32 *)0x3ff1fe00)[142] & 0xff;

	if ($a2 > 65 && !($a2 & (1 << 7)))
		return;

	rx_ctl_p = (struct RxControl *)(wDevCtrl.f_8->f_4);

	if (rx_ctl_p->sig_mode == 0)
		return;

	if ((rx_ctl_p->CWB == 0) && (rx_ctl_p->MCS < 8) && (rx_ctl_p->FEC_CODING == 0))
		return;

	rx_ctl_new_p = pvPortMalloc(sizeof *rx_ctl_new_p); /* 12 bytes */

	if (rx_ctl_new_p == NULL)
		return;

	ets_memcpy(rx_ctl_new_p, rx_ctl_p, sizeof *rx_ctl_p);

	if (($a2 = chm_get_current_channel()) == NULL) {
		ets_printf("%s %u\n", "wdev.c", 1862);
		while (1);
	}

	rx_ctl_new_p->channel = (rx_ctl_new_p->channel & 0xf0) | (((uint8 *)$a2)[6] & 0x0f);

	if (rx_ctl_new_p->sig_mode == 0)
		goto _0x401046a1;

	if ((rx_ctl_new_p->CWB == 0) && (rx_ctl_new_p->MCS < 8) && (rx_ctl_new_p->FEC_CODING == 0))
		goto _0x401046a1;

	if (rx_ctl_new_p->MCS >= 32)
		goto _0x401046a1;

	if (pp_post2(32, 9, rx_ctl_new_p) != 1)
		return;	/* memory leak? Did not free rx_ctl_new_p. Maybe freed in pp_post2. */

	vPortFree(rx_ctl_new_p);
	return;

_0x401046a1:
	vPortFree(rx_ctl_new_p);
	return;
}

/* <wDevDisableRx+0x108> */
static void _0x40104700()
{
	struct RxControl *rx_ctl_p;
	struct RxControl *rx_ctl_new_p;

	$a0 = ((volatile uint32 *)0x3ff20e00)[191];
	$a3 = 29;	/* from machine instructions. Not sure... */

	/* XXX $a3 is not even initialized here, wtf...
		The calling function (wDev_ProcessFiq) sets $a3 to 4 at this point. */
	while ($a3 >= ((volatile uint32 *)0x3ff20a00)[128] - $a0) {
		if (((volatile uint32 *)0x3ff20a00)[135] & (1 << 2))
			return;
	}

	if (((((volatile uint32 *)0x3ff1fe00)[245] >> 8) & 0xff) < 8)
		return;

	((volatile uint32 *)0x3ff20a00)[156] &= 0xfffffffd;	/* -3 in 32-bit unsigned hex */
	
	while ((((volatile uint32 *)0x3ff20a00)[135] & (1 << 2)) != 0);

	if (wDevCtrl.f_0 < 2)
		goto _0x40104794;

	((volatile uint32 *)0x3ff20a00)[137] = 4;

	$a2 = ((volatile uint32 *)0x3ff1fe00)[142] & 0xff;

	if (($a2 > 65) && !($a2 & (1 << 7)))
		goto _0x40104794;

	rx_ctl_p = (struct RxControl *)(wDevCtrl.f_8->f_4);
	rx_ctl_new_p = pvPortMalloc(sizeof *rx_ctl_new_p);

	if (rx_ctl_new_p == NULL)
		goto _0x40104794;

	ets_memcpy(rx_ctl_new_p, rx_ctl_p, sizeof *rx_ctl_p);
	
	if (($a2 = chm_get_current_channel()) == NULL) {
		ets_printf("%s %u\n", "wdev.c", 1922);
		while (1);
	}

	rx_ctl_new_p->channel = (rx_ctl_new_p->channel & 0xf0) | (((uint8 *)$a2)[6] & 0x0f);

	if (rx_ctl_new_p->sig_mode == 0)
		goto _0x40104829;

	if ((rx_ctl_new_p->CWB == 0) && (rx_ctl_new_p->MCS < 8))
		goto _0x40104829;

	if (rx_ctl_new_p->MCS >= 32)
		goto _0x40104829;

	if (pp_post2(32, 9, rx_ctl_new_p) != 1)
		goto _0x40104794;	/* possible memory leak if rx_ctl_new_p not freed */

	vPortFree(rx_ctl_new_p);
	goto _0x40104794;

_0x40104829:
	vPortFree(rx_ctl_new_p);

_0x40104794:
	((volatile uint32 *)0x3ff20a00)[156] |= (1 << 1);
}

/* <wDevDisableRx+0x240> */
static void _0x40104838()
{
	struct sniffer_buf *sniff_buf;	/* $a12 */
	struct sniffer_buf *new_buf;	/* $a13 */
	struct _wdev_ctrl_sub1 *a15;	/* $a15 */
	uint8 *data;			/* $a1 + 16 */
	uint16 counter;
	struct _wdev_ctrl_sub4 *a1_0;	/* $a1 + 0 */
	struct _wdev_ctrl_sub2 *a1_4;	/* $a1 + 4 */
	uint32 a1_8;			/* $a1 + 8 */
	struct Ampdu_Info *a1_12;	/* $a1 + 12 */

	$a2 = wDevCtrl.f_8;
	sniff_buf = wDevCtrl.f_8->f_4;	/* looks like this is a sniffer_buf struct */

	data = sniff_buf->buf;

	if (sniff_buf->rx_ctrl.Aggregation) {
		/* AMPDU packets (A stands for Aggregation?) */
		/* Seems that in an AMPDU packet, there is one RxControl header, followed by
		   a bunch of (36-byte data buf || 2-byte cnt || 10-byte struct Ampdu_Info), ampdu_info.cnt times. */
		/* They fill the new sniffer_buf with the first frame, and all other frames are skipped,
		   only placing the Ampdu_Info structs concatenated in the sniffer_buf.ampdu_info array. */
		a15 = wDevCtrl.f_8;
		counter = 1;

		while (((*(volatile uint8 *)(&(a15->f_b24)) & 0x7f) >> 6) == 0) {
			a15 = a15->f_8;
			++counter;
		}

		a1_4 = wDevCtrl.f_48;

		/* sizeof (struct Ampdu_Info) == 10
		   sizeof (struct sniffer_buf) (minus the Ampdu_Info) == 50.
		   Thus in total, they allocate memory for a sniffer_buf struct
		   with ampdu_cnt Ampdu_Info structs at the tail. */
		new_buf = (struct sniffer_buf *)pvPortMalloc((sniff_buf->rx_ctrl.ampdu_cnt) * 10 + 50);

		if (new_buf == NULL) {
			_0x40103b24(a15, 1);		/* <trc_NeedRTS+0x1d4> */
			_0x40103b54(wDevCtrl.f_48, 1);	/* <trc_NeedRTS+0x204> */
			return;
		}

		/* 0x4010491a */
		/* This happens in Aggregation == 1. Packet has length ampdu_cnt * 10 + 50 */
		ets_memcpy(new_buf, sniff_buf, 60);	/* Copy the sniffer_buf and first ampdu_info packet */

		a1_12 = new_buf->ampdu_info; /* Points to the struct Ampdu_Info array */
		new_buf->cnt = 0;

		/* a1_0 iterates over an array of struct _wdev_ctrl_sub4.
		   The array has total size a1_4->f_12 bytes, and each struct has size 4 */
		for (a1_0 = a1_4->f_4; a1_0 - a1_4->f_4 < a1_4->f_b12; a1_0++) {
			if (new_buf->cnt >= sniff_buf->rx_ctl.ampdu_cnt)
				break;

			$a2 = a1_0->f_b0;

			/* No idea what this does yet... */
			if ($a2 != 0 && (($a2 <= 224) || ($a2 > 252)))
				continue;

			/* f_8->f_b0 is the total size of the sniffer buf (12 bits)?
			 * So they check if the sniffer buf can reach to byte 24 of data.
			 * In an IEEE-802.11 frame, this points to the Addr3 field.
			 * I suppose that if the buffer is too short, they just stop. */
			if ((uint8 *)(wDevCtrl.f_8->f_4) + wDevCtrl.f_8->f_b0 <= data + 24)
				break;

			new_buf->cnt += 1; /* Increment new_buf->cnt */

			a1_12->length = a1_0->f_b8;
			a1_12->seq = *((uint16 *)(data + 22));		/* Sequence Control field */

			ets_memcpy(a1_12->address3, data + 16, 6);	/* Copy the Addr3 MAC Address to a1_12->address3 */

			$a2 = 34; /* IEEE frame up to Addr3 (included) (22 bytes) + sniffer_buf.cnt (2 bytes) + Ampdu_Info (10 bytes) */
			a1_12++;  /* a1_12 now points to next new ampdu_info struct. (size is 10) */

			/* Looks at byte 1 of Frame Control field, bits To DS and From DS */
			if ((*(uint8 *)(data + 1)) & 0x3 != 3)
				$a2 = 28;	/*  Only include Addr4 (6 bytes) if To DS and From DS are both set */

			$a2 += 2;		/* Plus 2 bytes for Sequence Control field */
			$a2 &= 0xffff;		/* Possibly the length is stored in a uint16. */

			if ((*(uint8 *)(data + 1)) & (1 << 7)) { /* Order bit */
				$a2 += 4;	/* Add the HT Control field (4 bytes) */
				$a2 &= 0xffff;
			}

			/* What about the QoS field (2 bytes)? Maybe I missed the code that checks this... */

			roundup2($a2, 4);	/* So $a2 is an offset in data, and they round up to multiples of 4? */
			data += $a2 & 0xffff;	/* So now data points to the next data buffer */
		}

		_0x40103b24(a15, counter);	/* <trc_NeedRTS+0x1d4> */
		_0x40103b54(wDevCtrl.f_48, 1);
	} else {
		/* Not an AMPDU packet.*/
		/* Look at the Frame Control type field (proto is always 0) */
		a1_8 = (((sniff_buf->buf[0]) & 0x0f) == 0 ? 1 : 0);	/* 1 if type == 0 (Management frame) */

		/* Management frames will be given 128 bytes, otherwise 60 bytes */
		if (a1_8 != 0)
			new_buf = (struct sniffer_buf2 *)pvPortMalloc(128);
		else
			new_buf = (struct sniffer_buf *)pvPortMalloc(60);

		if (new_buf == NULL) {
			_0x40103b24(wDevCtrl.f_8, 1); /* <trc_NeedRTS+0x1d4> */
			return;
		}

		if (a1_8 == 0) {
			/* Not a Management frame. --> Data or Control frame */
			/* In this case, buffer will be size 60. */
			ets_memcpy(new_buf, sniff_buf, 60);
			new_buf->ampdu_info.cnt = 1;

			if (sniff_buf->rx_ctrl.sig_mode != 0)
				$a0 = sniff_buf->rx_ctrl.HT_length;
			else
				$a0 = sniff_buf->rx_ctrl.legacy_length;

			new_buf->ampdu_info.length = $a0;
			new_buf->ampdu_info.seq = *(uint16 *)(sniff_buf->buf + 22);	/* Sequence Control field */
			ets_memcpy(new_buf->ampdu_info.address3, sniff_buf->buf + 16, 6); /* Copy the Addr3 field in address3 */
		} else {
			/* Management frame */
			/* In this case, it's a struct sniffer_buf2 with 112 bytes of data buf. */
			/* The format is: 12 bytes rx_ctrl, 112 bytes buf, 2 bytes cnt, 2 bytes len */
			ets_memcpy(new_buf, sniff_buf, 128);
			(struct sniffer_buf2 *)new_buf->cnt = 1;

			/* sig_mode = 0 means it's an 11n packet: use HT_length field */
			if (sniff_buf->rx_ctrl.sig_mode != 0)
				$a0 = sniff_buf->rx_ctrl.HT_length;
			else
				$a0 = sniff_buf->rx_ctrl.legacy_length; /* length if not 11n packet */

			(struct sniffer_buf2 *)new_buf->len = $a0;
		}

		_0x40103b24(wDevCtrl.f_8, 1); /* <trc_NeedRTS+0x1d4> */
	}
	
	if (pp_post2(32, 9, new_buf) != 1)
		return;	/* Again check for memory leaks... */

	vPortFree(new_buf);
	return;
}

/* <trc_NeedRTS+0x240> */
static void _0x40103b90(struct _wdev_ctrl_sub1 *arg1, uint32 arg2, uint32 arg3, uint32 arg4)
{
	// stub
	struct sniffer_buf *sniff_buf;	/* $a13 */
	uint32 a1_28;	/* $a1 + 28 */
	uint32 a1_24;	/* $a1 + 24 */
	uint32 a1_0;	/* $a1 + 0 */

	a1_24 = arg2;
	a1_0 = arg3;
	a1_28 = arg4;

	sniff_buf = wDevCtrl.f_8->f_4;
	$a14 = sniff_buf->rx_ctrl.Aggregation;
	$a12 = arg1;

	if (($a15 = chm_get_current_channel()) == NULL) {
		ets_printf("%s %u\n", "wdev.c", 371);
		while (1);
	}

	$a2 = *(uint16 *)(sniff_buf->rx_ctrl.buf); /* Frame Control field in IEEE-802.11 frames */
	$a0 = ((uint8 *)$a15)[6];
	$a3 = sniff_buf->rx_ctrl.channel;
	$a11 = 240;
	$a3 &= 240;
	$a0 &= 0x0f;
	$a0 |= $a3;
	sniff_buf->rx_ctrl.channel = $a0;

	if ($a14 != 0)
		goto _0x40103c16;

	$a0 = &wDevCtrl;
	$a10 = sniff_buf->rx_ctrl.damatch0;	/* $a9 = sniff_buf->rx_ctrl .byte 3 */

	if (sniff_buf->rx_ctrl.damatch0)
		goto _0x40103bfc;

	$a3 = sniff_buf->rx_ctrl.damatch1;

	if ($a3 == 0)
		goto _0x40103c99;

	goto _0x40103bfc;

_0x40103bfc:
	$a7 = $a2 & 0x0f;	/* subtype in frame control? */

	if ($a7 == 0)
		goto _0x40103cd0;

	$a4 = $a7 - 4;

	if ($a4 == 0)
		goto _0x40103ce0;

	$a5 = $a7 - 8;

	if ($a5 == 0)
		goto _0x40103d2d;

	((uint32 *)&wDevCtrl)[99] += 1;
	goto _0x40103c16;

_0x40103c16:
	$a7 = 0;
	$a2 = 0;
	$a13 = 1;
	goto _0x40103c2d;

_0x40103c1f:
	$a7 = 0;
	$a13 = 1;
	goto _0x40103c23;

_0x40103c23:
	((uint32 *)&wDevCtrl)[95] += 1;
	$a2 = 1;
	goto _0x40103c2d;
	
_0x40103c2d:
	$a9 = &wDevCtrl;
	$a10 = wDevCtrl.f_2;
	$a9 = wDevCtrl.f_0;
	$a9 += $a10;

	if ($a9 >= 2)
		goto _0x40103c42;

	$a10 = 1;

	if ($a2 == 0)
		$a13 = $a10;

	goto _0x40103c42;

_0x40103c42:
	if ($a13 == 0)
		goto _0x40103c4e;

	$a2 = $a12;
	$a3 = a1_24;
	_0x40103b24($a12, a1_24);
	return;

_0x40103c4e:
	$a13 = 0x00ffffff;
	$a11 = arg1->f_b12;	/* volatile? */

	if ($a11 == 0) {
		ets_printf("%s %u\n", "wdev.c", 557);
		while (1);
	}

	$a4 = $a12;
	$a3 = $a14;
	$a2 = $a15;
	$a5 = a1_24;
	$a6 = a1_0;
	_0x40103aa4($a15, $a14, $a12, a1_24, a1_0);
	return;

_0x40103c99:
	if (sniff_buf->rx_ctrl.bssidmatch0)
		goto _0x40103ca2;

	$a13 = sniff_buf->rx_ctrl.bssidmatch1;

	if ($a13 == 0)
		goto _0x40103d49;

	goto _0x40103ca2;

_0x40103ca2:
	$a7 = $a2 & 0x0f;

	/* this is probably a switch statement */
	if ($a7 == 0)
		goto _0x40103d0c;

	if ($a7 == 4)
		goto _0x40103cb6;

	if ($a7 == 8)
		goto _0x40103d24;

	/* default */
	((uint32 *)&wDevCtrl)[99] += 1;
	goto _0x40103cb6;

_0x40103cb6:
	$a7 = 0;
	$a2 = 0;
	$a13 = 1;
	goto _0x40103cc5;

_0x40103cbf:
	$a7 = 0;
	$a13 = 0;
	goto _0x40103cc3;

_0x40103cc3:
	$a2 = 1;
	goto _0x40103cc5;

_0x40103cc5:
	((uint32 *)&wDevCtrl)[97] += 1;
	goto _0x40103c2d;

_0x40103cd0:
	$a4 = $a2 & $a11;

	if ($a4 != 128)
		goto _0x40103cd9;

	goto _0x40103d5d;

_0x40103cd9:
	$a7 = 0;
	$a13 = 0;
	goto _0x40103c23;

_0x40103ce0:
	$a2 &= $a11;
	$a5 = $a2 - 128;

	if ($a5 == 0)
		goto _0x40103d84;

	$a6 = 144;
	$a6 = $a2 - $a6;

	if ($a6 == 0)
		goto _0x40103d9c;

	$a7 = 160;
	$a7 = $a2 - $a7;

	if ($a7 == 0)
		goto _0x40103d8f;

	((uint32 *)&wDevCtrl)[94] += 1;
	goto _0x40103d03;

_0x40103d03:
	$a13 = 1;
	goto _0x40103d05;

_0x40103d05:
	$a7 = 0;
	$a2 = 0;
	goto _0x40103c2d;

_0x40103d0c:
	$a2 &= 0xff;

	if ($a2 == 64)
		goto _0x40103cbf;

	$a9 = 80;

	if ($a2 == $a9)
		goto _0x40103d1d;

	$a10 = $a2 - 128;

	if ($a10 == 0)
		goto _0x40103da7;

	goto _0x40103d1d;

_0x40103d1d:
	$a7 = 0;
	$a13 = 1;
	goto _0x40103cc3;

_0x40103d24:
	$a7 = 0;
	$a2 = 0;
	$a13 = 0;
	goto _0x40103cc5;

_0x40103d2d:
	$a0 = &wDevCtrl;
	((uint32 *)&wDevCtrl)[96] += 1;
	$a2 = $a13;
	rcUpdateDataRxDone($a2);

	$a7 = 0;
	$a2 = 0;
	$a13 = 0;
	goto _0x40103c2d;

_0x40103d49:
	$a7 = 0;
	$a2 = 0;
	$a13 = ((uint8 *)0x3ffeff5c)[100];
	$a0 = 1;
	$a13 -= 1;

	if ($a13 != 0)
		$a13 = $a0;

	goto _0x40103c2d;

_0x40103d5d:
	if ($a10 == 0)
		goto _0x40103c1f;

	$a4 = a1_28;
	$a5 = 1;
	$a13 = 0;

	if ($a4 == 0)
		$a13 = $a5;

	if ($a4 == 0)
		goto _0x40103dd6;

	$a2 = phy_get_bb_freqoffset();
	$a0 = &wDevCtrl;
	$a7 = $a2 << 16;
	(signed)$a7 >>= 16;	/* srai 16 instead of srli 16 */
	goto _0x40103c23;

_0x40103d84:
	((uint32 *)&wDevCtrl)[91] += 1;
	goto _0x40103d03;

_0x40103d8f:
	$a13 = 0;
	((uint32 *)&wDevCtrl)[93] += 1;
	goto _0x40103d05;

_0x40103d9c:
	((uint32 *)&wDevCtrl)[92] += 1;
	goto _0x40103d03;

_0x40103da7:
	$a11 = a1_28;
	$a3 = 1;
	$a13 = 0;

	if ($a11 == 0)
		$a13 = $a3;

	if ($a11 == 0)
		goto _0x40103dc4;

	$a2 = phy_get_bb_freqoffset();
	$a0 = &wDevCtrl;
	$a7 = $a2 << 16;
	(signed)$a7 >>= 16;	/* srai 16 instead of srli 16 */
	goto _0x40103dc6;

_0x40103dc4:
	$a7 = 0;
	goto _0x40103dc6;

_0x40103dc6:
	$a8 = a1_28;

	if ($a8 == 0)
		goto _0x40103cc3;

	((uint32 *)&wDevCtrl)[98] += 1;
	goto _0x40103cc3;

_0x40103dd6:
	$a7 = 0;
	goto _0x40103c23;

}

void wDev_ProcessFiq()
{
	uint32 a1_0;	/* $a1 + 0 */
	uint32 a1_4;	/* $a1 + 4 */
	void *a1_8;	/* $a1 + 8 */
	uint32 a1_32;	/* $a1 + 32 */

	while ($a12 = ((volatile uint32 *)0x3ff20a00)[136]) {
		$a15 = ((volatile uint32 *)0x3ff20a00)[161];
		((volatile uint32 *)0x3ff20a00)[137] = $a12;

		if ($a12 & (1 << 28)) {
			ets_printf("%s %u\n", "wdev.c", 1064);
			while (1);
		}

		if ($a12 & (1 << 27))
			_0x40103e64(0);	/* <wDev_MacTim1Arm+0x68> */

		if ($a12 & (1 << 26))
			_0x40103e64(1); /* <wDev_MacTim1Arm+0x68> */

		if ($a12 & (1 << 3)) {
			if (!($a12 & (1 << 2)))
				_0x40104700();	/* <wDevDisableRx+0x108> */
		} else {
			if ($a12 & (1 << 2)) {
				if (!($a12 & (1 << 8)))
					_0x4010462c();	/* <wDevDisableRx+0x34> */
			}
		}

		if ($a12 & (1 << 8)) {
			$a0 = *(volatile uint8 *)(&(wDevCtrl.f_5));
			$a3 = ((volatile uint32 *)0x3ff1fe00)[135];

			if ($a0) {
				/* in sniffer mode */

				/* Possibly, pointer to last fragment of last packet */
				$a14 = ((volatile struct _wdev_ctrl_sub1 **)0x3ff1fe00)[133];
				$a4 = ((volatile uint32 *)0x3ff20e00)[191];
				$a3 = ((volatile uint32 *)0x3ff1fe00)[134];	

				if ($a14 == NULL) {
					ets_printf("%s %u\n", "wdev.c", 1097);
					while (1);
				}

				if ((struct _wdev_ctrl_sub1 *)$a14->f_8 == NULL) {
					ets_printf("%s %u\n", "wdev.c", 1098);
					while (1);
				}

				/* Possibly, check that last packet is denoted with the 'last' flag. */
				if (!((struct _wdev_ctrl_sub1 *)$a14->f_b24 & (1 << 6))) {
					ets_printf("%s %u\n", "wdev.c", 1099);
					while (1);
				}

				$a2 = wDevCtrl.f_8;

				/* Possibly what happens is that there are multiple packets in a queue
				   which belong to the same bigger packet (fragmented?), and the last such
				   packet is noted by a 1 in the f_3 field of _wdev_ctrl_sub1.
				   So the nested while loop finds the last fragment of the packet.
				   They check if this last fragment is also the last packet in total ($a14).
				   If it is, they stop, otherwise they continue over the next fragmented packet.
				   They're probably counting the number of full packets. */
				while ($a2 != NULL) {
					$a13 = $a2;

					/* Find last fragment in packet */
					while (!((struct _wdev_ctrl_sub1 *)$a13->f_b24 & (1 << 6)))
						$a13 = (struct _wdev_ctrl_sub1 *)$a13->f_8;

					_0x40104838();	/* <wDevDisableRx+0x240> */
					wDevCtrl.f_356 += 1;	/* Increment count of full packets */

					if ($a13 == $a14)	/* if last fragment and packet */
						break;

					$a2 = (struct _wdev_ctrl_sub1 *)$a13->f_8;	/* Go to next packet */
				}
			} else {
				/* Not in sniffer mode */

				$a0 = ((volatile uint32 *)0x3ff20e00)[191];
				/* Pointer to last fragment of last packet? */
				$a14 = ((volatile struct _wdev_ctrl_sub1 **)0x3ff1fe00)[133];
				$a2 = ((volatile uint32 *)0x3ff20e00)[134];

				if ($a14 == NULL) {
					ets_printf("%s %u\n", "wdev.c", 1130);
					while (1);	
				}
				
				if ((struct _wdev_ctrl_sub1 *)$a14->f_8 == NULL) {
					ets_printf("%s %u\n", "wdev.c", 1131);
					while (1);		
				}

				/* Check if 'last' flag is set for last packet */
				if (!((struct _wdev_ctrl_sub1 *)$a14->f_b24 & (1 << 6))) {
					ets_printf("%s %u\n", "wdev.c", 1132);
					while (1);
				}

				a1_4 = $a0;
				$a13 = wDevCtrl.f_8;

				do {
					a1_8 = (struct _wdev_ctrl_sub1 *)$a13->f_8;
					_0x40103b90($a13, 1, a1_4, ($a13 == $a14 ? 1 : 0)); /* <trc_NeedRTS+0x240> */
					$a3 = $a14 - $a13;
					wDevCtrl.f_356 += 1;	/* Increment packet count? */
					$a13 = a1_8;
				} while ($a3 != 0);	/* XXX Can this loop look better? */
			}
		}

		if ($a12 & 0x00030000)
			lmacProcessRtsStart(($a15 >> 12) & 0xf);

		if ($a12 & (1 << 19)) {
			$a14 = ((volatile uint32 *)0x3ffe0e00)[1];

			if ($a14 & (1 << 24))
				$a14 = ($a14 >> 16) & 0xff;
			else
				$a14 = 127;	/* 0x0000007f */

			$a13 = ($a15 >> 12) & 0xf;
			if (($a13 >= 8) && ($a13 != 10)) {
				ets_printf("%s %u\n", "wdev.c", 1175);
				while (1);
			}

			a1_0 = $a12 & (1 << 18);

			if ((!lmacIsActive()) && ($a12 & (1 << 18))) {
				lmacProcessTXStartData($a13); /* $a13 would be result from the loop (while, $a13 == $a14) */
				$a12 &= ~(1 << 18);
				a1_0 = $a12 & (1 << 18);
			}

			switch (($a15 >> 28) & 0xf) {
				case 0: /* 0x4010416a */
					lmacProcessTxSuccess($a13, $a14 & 0xff);
					a1_32 = $a13;
					break;

				case 1: /* 0x4010417a */
					lmacProcessTxRtsError(($a15 >> 16) & 0xff, $a13);
					a1_32 = $a13;
					break;

				case 2: /* _0x4010418a */
					lmacProcessCtsTimeout($a13);
					a1_32 = $a13;
					break;

				case 4: /* _0x40104197 */
					lmacProcessTxError(($a15 >> 16) & 0xff);
					a1_32 = $a13;
					break;

				case 5: /* 0x4010414f */
					a1_32 = $a13;
					lmacProcessAckTimeout();
					break;

				default: ets_printf("%s %u\n", "wdev.c", 1202);
					 while (1);
			}

			((uint32 **)((uint8 *)&wDevCtrl + 0x40))[a1_32][12] += 1;
			$a0 = a1_0;
		}

		if ($a12 & (1 << 17)) {
			if (((($a15 >> 12) & 0xf) >= 8) && ((($a15 >> 12) & 0xf) != 10)) {
				ets_printf("%s %u\n", "wdev.c", 1209);
				while (1);
			}

			lmacProcessTXStartData();
		}

		if ($a12 & (1 << 20))
			lmacProcessCollisions();

		if ($a12 & (1 << 9)) {
			((uint32 *)&wDevCtrl)[101] += 1;

			if ($a4 & (1 << 23))
				((uint32 *)&wDevCtrl)[102] += 1;
		}
	}
}

void wDevDisableRx()
{
	((volatile uint32 *)0x3ff1fe00)[129] &= 0x7fffffff;
}

void wDevEnableRx()
{
	((volatile uint32 *)0x3ff1fe00)[129] |= 0x80000000;
}

void wDev_SetMacAddress(uint32 arg1, uint8 *arg2)
{
	$a5 = (arg2[3] << 24) | (arg2[2] << 16) | (arg2[1] << 8) | arg2[0];

	if (arg1 == 0) {
		((volatile uint32 *)0x3ff20a00)[146] = $a5;
		$a11 = (arg2[5] << 8) | arg2[4];
		((volatile uint32 *)0x3ff20a00)[147] = $a11;
		((volatile uint32 *)0x3ff20a00)[150] = 0xffffffff;
		((volatile uint32 *)0x3ff20a00)[151] = 0x0000ffff;
		((volatile uint32 *)0x3ff20a00)[151] |= 0x00010000;
	} else {
		((volatile uint32 *)0x3ff20a00)[148] = $a5;
		$a10 = (arg2[5] << 8) | arg2[4];
		((volatile uint32 *)0x3ff20a00)[149] = $a10;
		((volatile uint32 *)0x3ff20a00)[152] = 0xffffffff;
		((volatile uint32 *)0x3ff20a00)[153] = 0x0000ffff;
		((volatile uint32 *)0x3ff20a00)[153] |= 0x00010000;
	}
}

void wdev_go_sniffer()
{
	wDevCtrl.f_5 = 1;

	((volatile void **)0x3ff1fe00)[131] = wDevCtrl.f_48;
	((volatile uint32 *)0x3ff20a00)[162] |= 0x00040000;

	((volatile uint32 *)0x3ff20600)[128] |= 0x03000000;
	((volatile uint32 *)0x3ff20600)[128] &= 0x3ffeffff;
	((volatile uint32 *)0x3ff20600)[129] |= 0x03000000;
	((volatile uint32 *)0x3ff20600)[129] &= 0x3ffeffff;

	((volatile uint32 *)0x3ff20a00)[150] = 0;
	((volatile uint32 *)0x3ff20a00)[151] = 0x00010000;
	((volatile uint32 *)0x3ff20a00)[142] = 0;
	((volatile uint32 *)0x3ff20a00)[143] = 0x00010000;

	((volatile uint32 *)0x3ff20a00)[134] |= 0x0000000c;
	((volatile uint32 *)0x60009a00)[209] &= 0xdbffffff;

	ets_delay_us(15000);
	
	((volatile uint32 *)0x3ff20a00)[165] &= 0xfffffffe;
}

void wdev_set_sniffer_addr(uint8 *arg1)
{
	((volatile uint32 *)0x3ff1fe00)[155] |= 1;
	((volatile uint32 *)0x3ff1fe00)[155] |= 2;
	((volatile uint32 *)0x3ff1fe00)[155] |= 4;
	wDev_SetMacAddress(0, arg1);
}

void wdev_exit_sniffer()
{
	if (wDevCtrl.f_5 == 0)
		return;

	((volatile uint32 *)0x3ff20a00)[165] |= 1;

	((volatile uint32 *)0x60009a00)[209] |= 0x24000000;
	((volatile uint32 *)0x3ff20a00)[134] &= 0xfffffff3;

	((volatile uint32 *)0x3ff20a00)[150] = 0xffffffff;
	((volatile uint32 *)0x3ff20a00)[151] = 0x0001ffff;	
	((volatile uint32 *)0x3ff20a00)[142] = 0xffffffff;
	((volatile uint32 *)0x3ff20a00)[143] = 0x0000ffff;	

	((volatile uint32 *)0x3ff20600)[128] &= 0xfcffffff;
	((volatile uint32 *)0x3ff20600)[128] |= 0x00010000;
	((volatile uint32 *)0x3ff20600)[129] &= 0xfcffffff;
	((volatile uint32 *)0x3ff20600)[129] |= 0x00010000;

	((volatile uint32 *)0x3ff20a00)[162] &= 0xfffbffff;
	((volatile void **)0x3ff1fe00)[131] = &(wDevCtrl.f_24);

	wDevCtrl.f_5 = 0;
}

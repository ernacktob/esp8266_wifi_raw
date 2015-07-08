struct _wdev_ctrl {
	/* stub... most of the fields I have no idea what they mean */
	uint16	a_0;	/* some counter ? */
	uint16	a_2;	/* other counter ? */
	uint8 	a_5;	/* whether in promiscuous mode or not */
	void  **a_8;	/* type(a_8[1]) = (struct RxControl *) (or struct sniffer_buf * ?) */
	void **a_12;
	void **a_16;
	void **a_20;
	void **a_24;
	void **a_48;
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
static void _0x40103a14(uint16 arg1, uint8 **arg2)
{
	$a10 = ((uint16 *)&wDevCtrl)[1];
	((uint16 *)&wDevCtrl)[0] -= arg1;

	$a6 = ((uint16 *)&wDevCtrl)[0];
	$a2 = 0;

	if ($a6 < 2) {
		if ($a6 != 1) {
			((uint16 *)&wDevCtrl)[1] = 0;
			((uint16 *)&wDevCtrl)[0] = $a10;
			$a6 = ((uint32 *)&wDevCtrl)[5];
			$a11 = ((uint32 *)&wDevCtrl)[4];
			((uint32 *)&wDevCtrl)[2] = $a11;
			((uint32 *)&wDevCtrl)[4] = 0;
			((uint32 *)&wDevCtrl)[3] = $a6;
			((uint32 *)&wDevCtrl)[5] = 0;
			$a10 &= 0xffff;

			if ($a10 == 1) {
				$a4 = (uint32 *)&wDevCtrl + 6;
				((uint32 *)$a6)[2] = $a4;
			}

			((uint32 *)$arg2)[2] = 0;
			$a7 = 0x3ff1fe00;
			$a6 = ((uint32 *)&wDevCtrl)[2];
			((volatile uint32 *)0x3ff1fe00)[130] = $a6;
		} else {
			$a9 = arg2[2];
			((uint32 *)&wDevCtrl)[2] = $a9;
			arg2[2] = 0;
			$a8 = ((uint32 *)&wDevCtrl)[3];
			$a7 = ((uint32 *)&wDevCtrl)[6];
			((uint32 *)$a8)[2] = $a7;
		}
	} else {
		$a6 = arg2[2];
		((uint32 *)&wDevCtrl)[2] = $a6;
		arg2[2] = 0;
	}

	$a9 = ((uint16 *)&wDevCtrl)[1];
	$a8 = ((uint16 *)&wDevCtrl)[0];
	$a8 += $a9;

	if ($a8 < 2) {
		((volatile uint32 *)0x3ff20a00)[170] &= 0x0fffffff;
		((volatile uint32 *)0x3ff20a00)[171] &= 0x0fffffff;
	}
}

/* <trc_NeedRTS+0x1d4> */
static void _0x40103b24(uint8 **arg1, uint16 arg2)
{
	_0x40103a14(arg2, arg1);	/* <trc_NeedRTS+0xc4> */
	wDev_AppendRxBlocks(((uint32 *)&wDevCtrl)[2], arg1, arg2);	
}

/* <trc_NeedRTS+0x204> */
static void _0x40103b54(uint8 *arg1, uint8 arg2)
{
	((uint32 *)&wDevCtrl)[12] = *(uint32 *)(arg1 + 8);
	*(uint32 *)(arg1 + 8) = 0;
	((uint32 *)&wDevCtrl)[1] -= arg2;
	wDev_AppendRxAmpduLensBlocks(((uint32 *)&wDevCtrl)[12], arg1);
}

/* <wDevDisableRx+0x34> */
static void _0x4010462c()
{
	struct RxControl *rx_ctl_p;
	struct RxControl *rx_ctl_new_p;

	if (((uint16 *)&wDevCtrl)[0] < 2)
		return;

	$a2 = ((volatile uint32 *)0x3ff1fe00)[142] & 0xff;

	if ($a2 > 65 && !($a2 & (1 << 7)))
		return;

	rx_ctl_p = (struct RxControl *)(((uint8 ***)&wDevCtrl)[2][1]);

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

	if (((uint16 *)&wDevCtrl)[0] < 2)
		goto _0x40104794;

	((volatile uint32 *)0x3ff20a00)[137] = 4;

	$a2 = ((volatile uint32 *)0x3ff1fe00)[142] & 0xff;

	if (($a2 > 65) && !($a2 & (1 << 7)))
		goto _0x40104794;

	rx_ctl_p = (struct RxControl *)(((uint8 ***)&wDevCtrl)[2][1]);
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
	uint8 *data;			/* $a1 + 16 */
	uint16 counter;
	uint32 a1_0;			/* $a1 + 0 */
	uint32 a1_4;			/* $a1 + 4 */
	uint32 a1_8;			/* $a1 + 8 */
	struct Ampdu_Info *a1_12;	/* $a1 + 12 */

	$a2 = ((uint8 ***)&wDevCtrl)[2];
	sniff_buf = (struct sniffer_buf *)(((uint8 ***)&wDevCtrl)[2][1]); /* looks like this is a sniffer_buf struct */

	data = sniff_buf->buf;

	if (sniff_buf->rx_ctrl.Aggregation) {
		/* AMPDU packets (A stands for Aggregation?) */
		/* Seems that in an AMPDU packet, there is one RxControl header, followed by
		   a bunch of (36-byte data buf || 2-byte cnt || 10-byte struct Ampdu_Info), ampdu_info.cnt times. */
		/* They fill the new sniffer_buf with the first frame, and all other frames are skipped,
		   only placing the Ampdu_Info structs concatenated in the sniffer_buf.ampdu_info array. */
		$a15 = $a2;
		counter = 1;

		while (((((volatile uint8 *)$a15)[3] & 0x7f) >> 6) == 0) {
			$a15 = ((uint8 **)$a15)[2];
			++counter;
		}

		a1_4 = ((uint32 *)&wDevCtrl)[12];

		/* sizeof (struct Ampdu_Info) == 10
		   sizeof (struct sniffer_buf) (minus the Ampdu_Info) == 50.
		   Thus in total, they allocate memory for a sniffer_buf struct
		   with ampdu_cnt Ampdu_Info structs at the tail. */
		new_buf = (struct sniffer_buf *)pvPortMalloc((sniff_buf->rx_ctrl.ampdu_cnt) * 10 + 50);

		if (new_buf == NULL) {
			_0x40103b24($a15, 1);				/* <trc_NeedRTS+0x1d4> */
			_0x40103b54(((uint32 *)&wDevCtrl)[12], 1);	/* <trc_NeedRTS+0x204> */
			return;
		}

		/* 0x4010491a */
		/* This happens in Aggregation == 1. Packet has length ampdu_cnt * 10 + 50 */
		ets_memcpy(new_buf, sniff_buf, 60);	/* Copy the sniffer_buf and first ampdu_info packet */

		$a1_12 = new_buf->ampdu_info; /* Points to the struct Ampdu_Info array */
		new_buf->cnt = 0;

		/* I don't understand what this does... */
		/* Ok so I think a1_0 and $a5 are pointers. Initially, $a5 points
		   to the end of something. a1_0 then gets incremented at each
		   iteration of a loop... The length is encoded in the 12 bits. */
		for (a1_0 = *(uint8 **)((uint8 *)a1_4 + 4); ; a1_0 += 4) {
			$a5 = *(uint8 **)((uint8 *)a1_4 + 4);
			$a6 = *(volatile uint16 *)((uint8 *)a1_4 + 2) << 16;
			$a6 |= *(volatile uint16 *)((uint8 *)a1_4 + 0);
			$a6 &= 0x00ffffff;
			$a6 >>= 12;	/* $a6 = bits 12, ..., 23 of a1_4[0] + (a1_4[1] << 16) (as uint16's) */

			$a5 += $a6;

			if (a1_0 >= $a5)	/* This doesn't fit in the 'for-loop' header yet, but they probably used bitfields */
				break;

			if (new_buf->cnt >= sniff_buf->rx_ctl.ampdu_cnt)
				break;

			$a2 = ((uint8 *)a1_0)[0];

			if ($a2 != 0 && (($a2 <= 224) || ($a2 > 252)))
				continue;

			$a5 = ((uint32 *)&wDevCtrl)[2];
			$a5 = ((uint16 *)$a5)[0];
			$a11 = data + 24;	/* In an IEEE-802.11 frame, this points to the Addr4 field. */
			$a2 = (struct sniffer_buf *)(((uint8 ***)&wDevCtrl)[2][1]);	/* same as sniff_buf? */
			$a5 &= 0xfff;	/* These are the lower 12 bits that were ignored in the other length above */
			$a2 += $a5;	/* So it's another offset, within the sniffer_buf? */
			/* Note: in RxControl, 12 bits were used to represent packet lengths. */

			if ($a11 >= $a2)	/* If $a2 points before the Addr4 field, break the loop */
				break;

			new_buf->cnt += 1; /* Increment new_buf->cnt */

			/* ehh so here they do something similar to the beginning
			   of the loop, but the bits they use are different: 8, ..., 19.
			   Also, it's wrt a1_0, which is one of the fields of a1_4. (???) */
			$a5 = ((uint16 *)a1_0)[1];
			$a6 = ((uint16 *)a1_0)[0];
			$a5 <<= 16;
			$a5 |= $a6;
			$a5 = ($a5 >> 8) & 0xfff;

			a1_12->length = $a5;
			a1_12->seq = *((uint16 *)(data + 22)); /* Sequence Control field */

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

		_0x40103b24($a15, counter);	/* <trc_NeedRTS+0x1d4> */
		_0x40103b54(((uint32 *)&wDevCtrl)[12], 1);
	} else {
		/* Not an AMPDU packet.*/
		/* Look at the Frame Control type field (proto is always 0) */
		$a1_8 = (((sniff_buf->buf[0]) & 0x0f) == 0 ? 1 : 0);	/* 1 if type == 0 (Management frame) */

		/* Management frames will be given 128 bytes, otherwise 60 bytes */
		if ($a1_8 != 0)
			new_buf = (struct sniffer_buf2 *)pvPortMalloc(128);
		else
			new_buf = (struct sniffer_buf *)pvPortMalloc(60);

		if (new_buf == NULL) {
			_0x40103b24(((uint32 *)&wDevCtrl)[2], 1); /* <trc_NeedRTS+0x1d4> */
			return;
		}

		if ($a1_8 == 0) {
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

		_0x40103b24(((uint32 *)&wDevCtrl)[2], 1); /* <trc_NeedRTS+0x1d4> */
	}
	
	if (pp_post2(32, 9, new_buf) != 1)
		return;	/* Again check for memory leaks... */

	vPortFree(new_buf);
	return;
}

/* <trc_NeedRTS+0x240> */
static void _0x40103b90()
{
	// stub
}

void wDev_ProcessFiq()
{
	uint32 a1_0;	/* $a1 + 0 */
	uint32 a1_4;	/* $a1 + 4 */
	uint32 a1_8;	/* $a1 + 8 */
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
			$a0 = ((volatile uint8 *)&wDevCtrl)[5];
			$a3 = ((volatile uint32 *)0x3ff1fe00)[135];

			/* if in sniffer mode... */
			if ($a0) {
				$a14 = ((volatile uint32 **)0x3ff1fe00)[133];
				$a4 = ((volatile uint32 *)0x3ff20e00)[191];
				$a3 = ((volatile uint32 *)0x3ff1fe00)[134];	

				if ($a14 == NULL) {
					ets_printf("%s %u\n", "wdev.c", 1097);
					while (1);
				}

				if (((uint32 *)$a14)[2] == 0) {
					ets_printf("%s %u\n", "wdev.c", 1098);
					while (1);
				}

				if (!(((uint8 *)$a14)[3] & (1 << 6))) {
					ets_printf("%s %u\n", "wdev.c", 1099);
					while (1);
				}

				$a2 = ((uint8 **)&wDevCtrl)[2];

				while ($a2 != NULL) {
					$a13 = $a2;

					while (!(((uint8 *)$a13)[3] & (1 << 6)))
						$a13 = ((uint8 **)$a13)[2];

					_0x40104838();	/* <wDevDisableRx+0x240> */
					((uint32 *)&wDevCtrl)[89] += 1;

					if ($a13 == $a14)
						break;

					$a2 = ((uint8 **)$a13)[2];
				}
			} else {
				$a0 = ((volatile uint32 *)0x3ff20e00)[191];
				$a14 = ((volatile uint32 **)0x3ff1fe00)[133];
				$a2 = ((volatile uint32 *)0x3ff20e00)[134];

				if ($a14 == NULL) {
					ets_printf("%s %u\n", "wdev.c", 1130);
					while (1);	
				}
				
				if (((uint32 *)$a14)[2] == 0) {
					ets_printf("%s %u\n", "wdev.c", 1131);
					while (1);		
				}

				if (!(((uint8 *)$a14)[3] & (1 << 6))) {
					ets_printf("%s %u\n", "wdev.c", 1132);
					while (1);
				}
					
				a1_4 = $a0;
				$a13 = ((uint32 *)&wDevCtrl)[2];

				do {
					a1_8 = ((uint32 *)$a13)[2];
					_0x40103b90($a13, 1, a1_4, ($a13 == $a14 ? 1 : 0)); /* <trc_NeedRTS+0x240> */
					$a3 = $a14 - $a13;
					((uint32 *)&wDevCtrl)[89] += 1;
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

			if (((($a15 >> 12) & 0xf) >= 8) && ((($a15 >> 12) & 0xf) != 10)) {
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
	((uint8 *)&wDevCtrl)[5] = 1;

	((volatile uint32 *)0x3ff1fe00)[131] = ((uint32 *)&wDevCtrl)[12];
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
	if (((uint8 *)&wDevCtrl)[5] == 0)
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
	((volatile uint32 *)0x3ff1fe00)[131] = &wDevCtrl + 24;

	((uint8 *)&wDevCtrl)[5] = 0;
}

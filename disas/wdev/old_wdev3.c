struct _wdev_ctrl {
	/* stub... most of the fields I have no idea what they mean */
	uint16	a_0;	/* some counter ? */
	uint16	a_2;	/* other counter ? */
	uint8 	a_5;	/* whether in promiscuous mode or not */
	void  **a_8;	/* type(a_8[1]) = (struct RxControl *) (or struct sniffer_buf * ?) */
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
	// stub
	struct sniffer_buf *sniff_buf;	/* $a12 */
	struct sniffer_buf *new_buf;	/* $a13 */
	uint8 *data;	/* $a1 + 16 */
	uint16 counter;	/* $a14 */
	uint32 a1_0;	/* $a1 + 0 */
	uint32 a1_4;	/* $a1 + 4 */
	uint32 a1_8;	/* $a1 + 8 */
	uint32 a1_12;	/* $a1 + 12 */

	$a2 = ((uint8 ***)&wDevCtrl)[2];
	sniff_buf = (struct sniffer_buf *)(((uint8 ***)&wDevCtrl)[2][1]); /* looks like this is a sniffer_buf struct */

	$a15 = 1;
	data = sniff_buf->buf;
	$a0 = data; /* just a note */

	if (sniff_buf->rx_ctrl.Aggregation) {
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
	} else {
		/* 0x401048bb */
		$a4 = (sniff_buf->buf[0]) & 0x0f;
		$a3 = ($a4 == 0 ? 1 : 0);
		a1_8 = $a3;

		if ($a3 != 0)
			new_buf = (struct sniffer_buf *)pvPortMalloc(128);
		else
			new_buf = (struct sniffer_buf *)pvPortMalloc(60);

		if (new_buf == NULL) {
			_0x40103b24(((uint32 *)&wDevCtrl)[2], 1); /* <trc_NeedRTS+0x1d4> */
			return;
		}

		$a3 = a1_8;

		if ($a3 == 0)	/* buffer will be size 60 */
			goto _0x40104a8b;

		ets_memcpy(new_buf, sniffer_buf, 128);	/* What is the size of sniffer_buf??
							   What is the size of the sniffer_buf.buf array?? */
		$a15 = *((uint16 *)(new_buf + 124));
		$a4 = sniffer_buf->rx_ctrl.sig_mode;
		$a0 = sniffer_buf->rx_ctrl.HT_length;

		if ($a4 != 0)
			goto _0x40104a79;

		$a0 = sniffer_buf->rx_ctrl.legacy_length;
		goto _0x40104a85;
	}

	/* 0x4010491a */
	/* This happens in Aggregation == 1. Packet has length ampdu_cnt * 10 + 50 */
	ets_memcpy(new_buf, sniffer_buf, 60);	/* Copy the sniffer_buf and first ampdu_info packet */
	$a8 = 224;	/* 0xe0 = 1110 0000 */
	$a9 = 252;	/* 0xfc = 1111 1100 */
	$a2 = 0;
	$a1_12 = new_buf->ampdu_info; /* Points to the struct Ampdu_Info array */
	$a3 = a1_4;	/* ((uint8 **)&wDevCtrl)[12] */

	$a4 = 0x00ffffff;
	$a0 = *(uint8 **)((uint8 *)a1_4 + 4);
	new_buf->cnt = 0;

	for ($a0 = *(uint8 **)((uint8 *)a1_4 + 4); ; $a0 += 4) {
		/* I don't understand what this does... */
		/* Ok so I think $a0 and $a5 are pointers. Initially, $a5 points
			to the end of something. $a0 then gets incremented at each
			iteration of a loop... The length is encoded in the 12 bits. */
		$a5 = *(uint8 **)((uint8 *)a1_4 + 4);
		$a6 = *(volatile uint16 *)((uint8 *)a1_4 + 2) << 16;
		$a6 |= *(volatile uint16 *)((uint8 *)a1_4 + 0);
		$a6 &= 0x00ffffff;
		$a6 >>= 12;	/* $a6 = bits 12, ..., 23 of a1_4[0] + (a1_4[1] << 16) (as uint16's) */

		$a5 += $a6;

		if ($a0 >= $a5)	/* This doesn't fit in the 'for-loop' header yet, but they probably used bitfields */
			break;

		$a10 = sniff_buf->rx_ctrl.ampdu_cnt;
		$a7 = new_buf->cnt;

		if (new_buf->cnt < sniff_buf->rx_ctl.ampdu_cnt) {
			$a2 = ((uint8 *)$a0)[0];

			if ($a2 != 0 && (($a2 <= 224) || ($a2 > 252)))
				continue;

			$a5 = ((uint32 *)&wDevCtrl)[2];
			a1_0 = $a0;	/* The pointer that iterates in the loop? */
			$a5 = ((uint16 *)$a5)[0];
			$a11 = data + 24;	/* In an IEEE-802.11 frame, this points to the Addr4 field. */
			$a2 = (struct sniffer_buf *)(((uint8 ***)&wDevCtrl)[2][1]);	/* same as sniff_buf? */
			$a5 &= 0xfff;	/* These are the lower 12 bits that were ignored in the other length above */
			$a2 += $a5;	/* So it's another offset, within the sniffer_buf? */
					/* Note: in RxControl, 12 bits were used to represent packet lengths. */

			if ($a11 >= $a2)	/* If $a2 points before the Addr4 field, break the loop */
				break;

			$a3 = data;
			$a2 = a1_12;	/* new_buf->ampdu_info */
			$a6 = a1_0;	/* pointer iterating in loop? */
			$a7 += 1;	/* Increment new_buf->cnt */
			new_buf->cnt = $a7;

			/* ehh so here they do something similar to the beginning
			   of the loop, but the bits they use are different: 8, ..., 19.
			   Also, it's wrt a1_0, which is one of the fields of a1_4. (???) */
			$a5 = ((uint16 *)$a6)[1];
			$a6 = ((uint16 *)$a6)[0];
			$a5 <<= 16;
			$a5 |= $a6;
			$a5 = ($a5 >> 8) & 0xfff;

			(struct Ampdu_Info *)a1_12->length = $a5;
			$a4 = *((uint16 *)(data + 22));	/* Sequence Control field in IEEE-802.11 frames */
			(struct Ampdu_Info *)a1_12->seq = *((uint16 *)(data + 22));

			$a3 += 16;	/* Now points to Addr3 in IEEE-802.11 frames */
			$a2 += 4;	/* ampdu_info.address3 */
			$a4 = 6;
			ets_memcpy(a1_12->address3, $a3, 6);	/* Copy the Addr3 MAC Address to a1_12->address3 */

			$a3 = 28;
			$a2 = 34;
			$a8 = data;
			$a5 = a1_12;
			$a8 = ((uint8 *)$a8)[1];	/* Byte 1 of Frame Control field. */
			$a5 += 10;	/* sizeof (struct Ampdu_Info) */
			a1_12 = $a5;	/* a1_12 now points to next ampdu_info struct. */
			$a4 = $a8 & 0x3;	/* To DS and From DS bits? */
			$a4 -= 3;

			if ($a4 != 0)		/* if not (To DS == 1 && From DS == 1) */
				$a2 = $a3;	/* lolwat */

			$a2 += 2;
			$a2 &= 0xffff;		/* I feel like this is a uint16 variable... */

			if ($a8 & (1 << 7)) {	/* Possibly Rsvd bit? */
				$a2 += 4;	/* Not sure what this points to, but apparently they skip fields in data */
				$a2 &= 0xffff;
			}

			$a3 = 4;
			roundup2($a2, 4);	/* So $a2 is an offset in data, and they round up to multiples of 4? */
			$a0 = a1_0;		/* Pointer iterating in loop. Restore? */
			$a3 = a1_4;		/* The weird-ass pointer in wDevCtrl... */
			$a4 = 0x00ffffff;
			$a8 = 224;
			$a9 = 252;
			$a5 = data;
			$a6 = $a2 & 0xffff;
			$a5 += $a6;		/* $a6 could be 36, 32 or 40 at this point,
						   depending on bits in data. This might be due to the additional
						   fields like QoS Control and HT Control and encryption headers... */
			data = $a5;		/* Hmm so $a2 was calculated as a length to skip,
						   and data now points to the next data packet? */
		}
	}

	_0x40103b24($a15, counter);	/* <trc_NeedRTS+0x1d4> */
	_0x40103b54(((uint32 *)&wDevCtrl)[12], 1);
	goto _0x40104a59;

_0x40104a28:
	// XXX fill this!

_0x40104a59:
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

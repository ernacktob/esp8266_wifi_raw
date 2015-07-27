struct _wdev_ctrl_sub1 {
	unsigned:12		 f_b0;	/* length of the sniffer_buf ? */
	unsigned:12		 f_b12;	
	unsigned:8		 f_b24;	/* last packet in queue? */
	struct sniffer_buf	*f_4;	/* the sniffer buf data */
	struct _wdev_ctrl_sub1	*f_8;	/* pointer to next element in queue? */
};

/* total size of this struct should be 4 bytes */
struct _wdev_ctrl_sub3 {
	unsigned:8		 f_b0;	/* Thing that should be 0 or between 224 and 252 */
	unsigned:12		 f_b8;	/* length field? */
	unsigned:12		 f_b20;
};

/* This might actually be the same as struct _wdev_ctrl_sub1.
   There are cases where a pointer to one is used as pointer to the other.
   The only difference between these structs is that f_4 is a sniffer_buf
   or a pointer to an array of _wdev_ctrl_sub3. */
struct _wdev_ctrl_sub2 {
	unsigned:12		 f_b0;
	unsigned:12		 f_b12;	/* length field ?*/
	unsigned:8		 f_b24;
	struct _wdev_ctrl_sub3	*f_4;	/* Points to an array of these structs */
	struct _wdev_ctrl_sub2	*f_8;	/* pointer to next element in queue? */
};

/* It is possible that some of the fields are part of separate structs
   which have been declared as static, and are thus not associated with
   a name in the symbol table. I can't really determine this, so for
   now, every field referenced relatively to wDevCtrl is assumed to be
   a subfield of this hypothetical _wdev_ctrl struct. This remark
   also applies to most of the other struct reconstructions and pointer casts. */
struct _wdev_ctrl {
	/* stub... most of the fields I have no idea what they mean */
	uint16			 f_0;	/* some counter ? */
	uint16			 f_2;	/* other counter ? */
	uint8			 f_4;	/* counter or flag? */
	uint8			 f_5;	/* whether in promiscuous mode or not */
	struct _wdev_ctrl_sub1	*f_8;	/* possibly a queue for sniffer bufs */
	struct _wdev_ctrl_sub1	*f_12;
	struct _wdev_ctrl_sub1	*f_16;	/* also a queue, move from here to f_8 */
	struct _wdev_ctrl_sub1	*f_20;

	struct _wdev_ctrl_sub1	 f_24;	/* This could be a static struct... Also, might not be a _sub1. */
	struct _wdev_ctrl_sub1	 f_36;	/* This could be a static struct... Also, might not be a _sub1. */
	struct _wdev_ctrl_sub2	*f_48;
	struct _wdev_ctrl_sub2	*f_52;
	uint32			 f_356;	/* counts number of full packets? */
	// bunch of counters, I'll add them later
	// other fields...
};

struct _wdev_ctrl wDevCtrl;	/* XXX Don't know the size yet... */

/* These functions are stored in the .text section, and thus
   do not start with the ICACHE_FLASH_ATTR attribute. */

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

/* <trc_NeedRTS+0x154> */
static void _0x40103aa4(void *arg1, uint32 arg2, struct _wdev_ctrl_sub1 *arg3, uint16 arg4, uint32 arg5, uint16 arg6)
{
	void *a1_8 = arg1;	/* $a1 + 8 */	/* = arg1 */
	uint32 a1_4 = arg5;	/* $a1 + 4 */
	uint16 a1_0 = arg6;	/* $a1 + 0 */
	void *esf_buf;		/* $a12 */

	$a2 = 7;
	esf_buf = esf_rx_buf_alloc(7);	/* what is 7? */

	if (esf_buf == NULL) {
		_0x40103b24(arg3, arg4);	/* <trc_NeedRTS+0x1d4> */
		return;
	}

	$a6 = ((uint32 **)esf_buf)[9];	/* wat? esf_buf [36] ? */
	((uint32 *)$a6)[1] = arg5;
	((void **)$a6)[2] = arg1;
	((struct _wdev_ctrl_sub1 **)esf_buf)[2] = arg3;
	((struct _wdev_ctrl_sub1 **)esf_buf)[1] = wDevCtrl.f_8;
	((uint16 *)esf_buf)[6] = arg4;
	((struct sniffer_buf **)esf_buf)[4] = wDevCtrl.f_8->f_4;
	*((volatile uint8 *)(&(arg3->f_b24)) &= 0xbf;	/* 10111111 */

	((uint16 *)$a12)[12] = arg6;

	$a3 = arg3;
	$a2 = arg4;
	_0x40103a14(arg4, arg3);	/* <trc_NeedRTS+0xc4> */

	$a2 = esf_buf;
	lmacRxDone(esf_buf);
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
	wDev_AppendRxAmpduLensBlocks(wDevCtrl.f_48, arg1, arg2);
}

/* <trc_NeedRTS+0x240> */
static void _0x40103b90(struct _wdev_ctrl_sub1 *arg1, uint32 arg2, uint32 arg3, uint32 arg4)
{
	struct sniffer_buf *sniff_buf;	/* $a13 */
	uint32 a1_28;	/* $a1 + 28 */
	uint32 a1_24;	/* $a1 + 24 */
	uint32 a1_0;	/* $a1 + 0 */

	a1_24 = arg2;
	a1_0 = arg3;
	a1_28 = arg4;

	sniff_buf = wDevCtrl.f_8->f_4;

	if (($a15 = chm_get_current_channel()) == NULL) {
		ets_printf("%s %u\n", "wdev.c", 371);
		while (1);
	}

	$a2 = *(uint16 *)(sniff_buf->rx_ctrl.buf); /* Frame Control field in IEEE-802.11 frames */

	/* XXX isn't channel 4-bits? how can you AND it with 0xf0 ?*/
	$a0 = sniff_buf->rx_ctrl.channel = (sniff_buf->rx_ctrl.channel & 0xf0) | (((uint8 *)$a15)[6] & 0x0f);

	if (sniff_buf->rx_ctrl.Aggregation == 0) {
		if (sniff_buf->rx_ctrl.damatch0 || sniff_buf->rx_ctrl.damatch1) {
			/* Note: the bit ordering confused me here.
				 I think the way to read it is, |proto|type |  subtype  |  ...
								0  1  2  3  4  5  6  7  8  ...
				 so the bits at position i in the first FC byte are stored
				 as the i-th bit (from LSB) in the buffer data byte.
				 Example: a frame of type Control and subtype CTS would have,
					  in an IEEE 802.11 frame, the first FC byte: |00|10|0011|
					  (where the bits are in little-endian, i.e. from bit0 to bit7 left-to-right).
					  In the buffer, this would correspond to the byte 1100 0100 = 0xc4 */
			/* Type in frame control? */
			switch ($a2 & 0x0f) {
				case 0:	/* Management frame: |00|00|xxxx| */
					/* Subtype in frame control? */
					if ($a2 & 0xf0 == 128) {	/* Beacon: 1000 0000 = |00|00|0001| */
						if (sniff_buf->rx_ctrl.damatch0 == 0 || arg4 == 0) {
							$a7 = 0;
							$a13 = 1;
						} else {
							$a7 = (signed uint16)phy_get_bb_freqoffset();
							$a13 = 0;
						}
					} else {			/* Not beacon */
						$a7 = 0;
						$a13 = 0;
					}

					$a2 = 1;
					((uint32 *)&wDevCtrl)[95] += 1;
					break;

				case 4:	/* Control frame: |00|10|xxxx| */
					/* Subtype in frame control? */
					switch ($a2 & 0xf0) {
						case 128:	/* Block Ack Request: 1000 0100 = |00|10|0001| */
							$a13 = 1;
							((uint32 *)&wDevCtrl)[91] += 1;
							break;

						case 144:	/* Block Ack: 1001 0100 = |00|10|1001| */
							$a13 = 1;
							((uint32 *)&wDevCtrl)[92] += 1;
							break;

						case 160:	/* PS-Poll: 1010 0100 = |00|10|0101| */
							$a13 = 0;
							((uint32 *)&wDevCtrl)[93] += 1;
							break;

						default:
							$a13 = 1;
							((uint32 *)&wDevCtrl)[94] += 1;
							break;
					}

					$a2 = 0;
					$a7 = 0;
					break;

				case 8:	/* Data frame: |00|01|xxxx| */
					((uint32 *)&wDevCtrl)[96] += 1;
					rcUpdateDataRxDone(sniff_buf);

					$a2 = 0;
					$a7 = 0;
					$a13 = 0;
					break;

				default:/* Reserved */
					$a2 = 0;
					$a7 = 0;
					$a13 = 1;
					((uint32 *)&wDevCtrl)[99] += 1;
					break;
			}
		} else if (sniff_buf->rx_ctrl.bssidmatch0 || sniff_buf->rx_ctrl.bssidmatch1) {
			/* Type in frame control? */
			switch ($a2 & 0x0f) {
				case 0:	/* Management frame: |00|00|xxxx| */
					/* Subtype in frame control? (here 0xff same as 0xf0 since $a2 & 0x0f == 0) */
					switch ($a2 & 0xff) {
						case 64:	/* Probe request: 0100 0000 = |00|00|0010| */
							$a7 = 0;
							$a13 = 0;
							break;

						case 80:	/* Probe response: 0101 0000 = |00|00|1010| */
							$a7 = 0;
							$a13 = 1;
							break;

						case 128:	/* Beacon: 1000 0000 = |00|00|0001| */
							if (arg4 == 0)
								$a7 = 0;
								$a13 = 1;
							} else {
								$a7 = (signed uint16)phy_get_bb_freqoffset();
								$a13 = 0;
								((uint32 *)&wDevCtrl)[98] += 1;
							}

							break;

						default:
							$a7 = 0;
							$a13 = 1;
							break;
					}

					$a2 = 1;
					break;

				case 4:	/* Control frame: |00|10|xxxx| */
					$a2 = 0;
					$a7 = 0;
					$a13 = 1;
					break;

				case 8:	/* Data frame: |00|01|xxxx| */
					$a2 = 0;
					$a7 = 0;
					$a13 = 0;
					break;

				default:/* Reserved */
					((uint32 *)&wDevCtrl)[99] += 1;
					break;
			}

			((uint32 *)&wDevCtrl)[97] += 1;
		} else if (sniffer_buf->rx_ctrl.bssidmatch1 == 0) {
			$a2 = 0;
			$a7 = 0;

			if (((uint8 *)0x3ffeff5c)[100] == 1)	/* &g_ic + 0x180 */
				$a13 = 0;
			else
				$a13 = 1;
		}
	} else {
		$a7 = 0;
		$a2 = 0;
		$a13 = 1;
	}

	if (wDevCtrl.f_0 + wDevCtrl.f_0 < 2) {
		if ($a2 == 0)
			$a13 = 1;
	}

	/* By this point, my theory is that $a13 is 0 if the packet should be forwarded to
	   the higher layers of the networking stack, otherwise not. This makes sense,
	   since the data frames are always passed. */
	if ($a13 == 0) {
		if (*(volatile *)(&(arg1->f_b12)) == 0) {
			ets_printf("%s %u\n", "wdev.c", 557);
			while (1);
		}

		_0x40103aa4($a15, sniff_buf->rx_ctrl.Aggregation, arg1, arg2, arg3, $a7);	/* <trc_NeedRTS+0x154> */
		return;
	}

	_0x40103b24(arg1, arg2);	/* <trc_NeedRTS+0x1d4> */
}

/* <wDev_MacTim1Arm+0x68> */
static void _0x40103e64(uint32 arg1)
{
	/* arg1 can be 0 or 1 */
	if (((void **)&tcb + 159)[arg1] != NULL)
		((void (*)())(((void **)&tcb + 159)[arg1]))();
}

/* 0x40103e8c */
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
				   packet is noted by a 1 in the f_b24 field of _wdev_ctrl_sub1.
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
				lmacProcessTXStartData($a13);
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

			lmacProcessTXStartData(($a15 >> 12) & 0xf);
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

/* 0x401041ac */
void wDev_EnableTransmit(uint8 arg1, uint8 arg2, uint32 arg3)
{
	/* arg2 is unused? */
	$a8 = 0x3ff20a00;
	$a7 = (int16)(-24 * arg1);
	$a7 += $a8;
	*(volatile uint32 *)((uint8 *)$a7 + 0x3c0) = (arg3 & 0x3ff) << 12;
	*(volatile uint32 *)((uint8 *)$a7 + 0x3c4) |= 0xc0000000;
	((uint32 *)0x3ffe9940)[arg1] += 1;
}

/* 0x401041e8 */
void wDev_DisableTransmit(uint8 arg1)
{
	$a5 = 0x3ff20a00;
	$a4 = (int16)(-24 * arg1);
	$a4 += $a5;
	$a3 = *(volatile uint32 *)((uint8 *)$a4 + 0x3c4) &= 0x3fffffff;
}

/* 0x40104378 */
void wDev_AppendRxBlocks(struct _wdev_ctrl_sub1 *arg1, struct _wdev_ctrl_sub1 *arg2, uint16 arg3)
{
	struct _wdev_ctrl_sub1 *wcs1_p;	/* $a2 */
	uint16 counter;			/* $a0 */

	wcs1_p = arg1;

	if (arg1 == NULL) {
		counter = 0;
	} else {
		counter = 0;

		do {
			*(volatile uint8 *)(&(wcs1_p->f_b24)) |= 0x80;
			*(volatile uint8 *)(&(wcs1_p->f_b24)) &= 0xbf;
			*(volatile uint8 *)(&(wcs1_p->f_b24)) &= 0xdf;

			/* XXX Really not sure about this line.
			   Check from 0x401043d5 to 0x40104404. */
			*(volatile *)(&(wcs1_p->f_b12)) = *(volatile *)(&(wcs1_p->f_b0));

			wcs1_p = wcs1_p->f_8;
			++counter;
		} while (wcs1_p != NULL);
	}

	if (counter != arg3) {
		ets_printf("%s %u\n", "wdev.c", 1440);
		while (1);
	}

	ets_intr_lock();

	if (wDevCtrl.f_0 == 0) {
		wDevCtrl.f_8 = arg1;
		((volatile struct _wdev_ctrl_sub1 **)0x3ff1fe00)[130] = arg1;

		wDevCtrl.f_0 += arg3;
		wDevCtrl.f_12 = arg2;

		if (wDevCtrl.f_0 == 1)
			wDevCtrl.f_12->f_8 = &(wDevCtrl.f_24); /* address of array or actual struct? */
	} else if (wDevCtrl.f_0 == 1) {
		if (wDevCtrl.f_16 != NULL) {
			wDevCtrl.f_20->f_8 = arg1;
			arg3 += wDevCtrl.f_2;
		} else {
			wDevCtrl.f_16 = arg1;
		}

		wDevCtrl.f_20 = arg2;
		wDevCtrl.f_2 = arg3;
	} else {
		wDevCtrl.f_12->f_8 = arg1;
		wDevCtrl.f_12 = arg2;
		wDevCtrl.f_0 += arg3;
	}

	if (wDevCtrl.f_0 + wDevCtrl.f_2 >= 2) {
		$a2 = ((volatile uint32 *)0x3ff20a00)[170];
		$a2 &= 0x0fffffff;
		$a2 |= 0x10000000;
		((volatile uint32 *)0x3ff20a00)[170] = $a2;

		$a12 = ((volatile uint32 *)0x3ff20a00)[171];
		$a12 &= 0x0fffffff;
		$a12 |= 0x50000000;
		((volatile uint32 *)0x3ff20a00)[171] = $a12;
	}

	ets_intr_unlock();
}

/* 0x401044d8 */
void wDev_AppendRxAmpduLensBlocks(struct _wdev_ctrl_sub2 *arg1, struct _wdev_ctrl_sub2 *arg2, uint8 arg3)
{
	struct _wdev_ctrl_sub2 *a1_0;	/* $a1 + 0 */
	struct _wdev_ctrl_sub2 *wcs2_p;	/* $a2 */
	uint16 counter;			/* $a0 */

	a1_0 = arg2;
	wcs2_p = arg1;

	if (arg1 == NULL) {
		counter = 0;
	} else {
		counter = 0;

		do {
			*(volatile uint8 *)(&(wcs2_p->f_b24)) |= 0x80;
			*(volatile uint8 *)(&(wcs2_p->f_b24)) &= 0xbf;
			*(volatile uint8 *)(&(wcs2_p->f_b24)) &= 0xdf;

			/* XXX Really not sure about this line.
			   Check from 0x40104533 to 0x40104562. */
			*(volatile *)(&(wcs2_p->f_b12)) = *(volatile *)(&(wcs2_p->f_b0));

			wcs2_p = wcs2_p->f_8;
			++counter;
		} while (wcs2_p != NULL);
	}

	if (counter != (uint16)arg3) {
		ets_printf("%s %u\n", "wdev.c", 1514);
		while (1);
	}

	ets_intr_lock();

	if (wDevCtrl.f_48 == NULL) {
		wDevCtrl.f_48 = arg1;
		((volatile struct _wdev_ctrl_sub2 **)0x3ff1fe00)[131] = arg1;
	} else {
		wDevCtrl.f_52->f_8 = arg1;
	}

	wDevCtrl.f_52 = arg2;
	wDevCtrl.f_4 += arg3;
	ets_intr_unlock();
}

/* 0x401045f8 */
void wDevDisableRx()
{
	((volatile uint32 *)0x3ff1fe00)[129] &= 0x7fffffff;
}

/* <wDevDisableRx+0x1c> */
static void _0x40104614(struct _wdev_ctrl_sub1 *arg1)
{
	uint16 len;

	/* what the hell are they doing here? */
	*((uint32 *)(arg1->f_4)) = 0xdeadbeef;

	len = *(volatile *)(&(arg1->f_b0));
	*((uint32 *)((uint8 *)(arg1->f_4) + len)) = 0xdeadbeef;
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

	/* XXX isn't channel 4-bits? How can you AND it with 0xf0? */
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

	/* XXX isn't channel 4-bits? How can you AND it with 0xf0? */
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
	struct _wdev_ctrl_sub3 *a1_0;	/* $a1 + 0 */
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

		/* a1_0 iterates over an array of struct _wdev_ctrl_sub3.
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

/* From here onwards, functions are stored in the .irom0.text section,
   and thus they are prefixed with the ICACHE_FLASH_ATTR attribute */

/* 0x40247298 */
void ICACHE_FLASH_ATTR wDev_Option_Init()
{
	((volatile uint32 *)0x3ff20a00)[162] |= 0x00800a00;
	((volatile uint32 *)0x3ff20a00)[162] &= 0x7fdfbff7;
	((volatile uint32 *)0x3ff20a00)[164] |= 0x00000008;
	((volatile uint32 *)0x3ff20a00)[165] |= 0x00000003;

	((volatile uint32 *)0x3ff20e00)[2] &= 0xffffff0f;

	$a8 = ((volatile uint32 *)0x3ff20a00)[154]; 
	$a8 = ($a8 >> 16) & 0xff;
	$a8 += 18;
	$a8 &= 0xff;
	$a8 <<= 16;

	$a7 = ((volatile uint32 *)0x3ff20a00)[154];
	$a7 &= 0xff00ffff;
	$a7 |= $a8;
	((volatile uint32 *)0x3ff20a00)[154] = $a7;

	$a4 = ((volatile uint32 *)0x3ff20a00)[155];
	$a4 &= 0xffffff00;
	$a4 |= 0x00000016;
	((volatile uint32 *)0x3ff20a00)[155] = $a4;

	$a10 = ((volatile uint32 *)0x3ff20a00)[155];
	$a10 &= 0xffff00ff;
	$a10 |= 0x00001600;
	((volatile uint32 *)0x3ff20a00)[155] = $a10;

	$a7 = ((volatile uint32 *)0x3ff20a00)[133];
	$a7 &= 0xfffff000;
	$a7 |= 0x000000f0;
	((volatile uint32 *)0x3ff20a00)[133] = $a7;

	((volatile uint32 *)0x3ff20a00)[133] |= 0x80000000;
	((volatile uint32 *)0x3ff20a00)[133] |= 0x40000000;
}

/* 0x4024738c */
void ICACHE_FLASH_ATTR wDev_Enable_Beacon_Tsf()
{
	// stub
}

/* 0x402473a8 */
void ICACHE_FLASH_ATTR wDev_Disable_Beacon_Tsf()
{
	// stub
}

/* 0x402473c4 */
void ICACHE_FLASH_ATTR wDev_Set_Beacon_Int()
{
	// stub
}

/* <wDev_Get_Next_TBTT+0x5c> */
static void ICACHE_FLASH_ATTR _0x40247458()
{
	struct _wdev_ctrl_sub1 *wcs1_p;

	/* 0x3ffe9ab0 = &WdevTimOffSet + 0x0c */
	/* 0x3ffe9b00 = &WdevTimOffSet + 0x5c */
	lldesc_build_chain(0x3ffe9ab0, 72, 0x3ffe9b00, 0xff1fffff, 0x644, 1, &(wDevCtrl.f_8), &(wDevCtrl.f_12));

	wcs1_p = wDevCtrl.f_8;
	wDevCtrl.f_0 = 6;

	if (wDevCtrl.f_8 != NULL) {
		do {
			/* Not sure about these 2 lines.
			   Check from 0x4024749f to 0x402474ed. */
			*(volatile *)(&(wcs1_p->f_b0)) -= 4;
			*(volatile *)(&(wcs1_p->f_b12)) = *(volatile *)(&(wcs1_p->f_b0));

			_0x40104614(wcs1_p);	/* <wDevDisableRx+0x1c> */
			wcs1_p = wcs1_p->f_8;
		} while (wcs1_p != NULL);
	}

	/* 0x3ffec0a0 = &WdevTimOffSet + 0x25fc */
	/* 0x3ffec0f0 = &WdevTimOffSet + 0x263c */
	/* XXX the field types of wDevCtrl.f_36.f_4 / .f_8 don't match... */
	lldesc_build_chain(0x3ffec0a0, 60, 0x3ffec0f0, 0x00001f40, 0x640, 0, &(wDevCtrl.f_36.f_4), &(wDevCtrl.f_36.f_8));

	wDevCtrl.f_24.f_8 = NULL;
	wDevCtrl.f_2 = 0;
	wDevCtrl.f_24.f_4 = &(wDevCtrl.f_36);	/* XXX pointer types do not match... */

	*(volatile uint16 *)(&(wDevCtrl.f_24.f_b0)) = 4;
	*(volatile uint8 *)(&(wDevCtrl.f_24.f_b24)) |= 0x80;
	*(volatile uint8 *)(&(wDevCtrl.f_24.f_b24)) &= 0xbf;
	*(volatile uint8 *)(&(wDevCtrl.f_24.f_b24)) &= 0xdf;

	/* 0x3ffee020 = &WdevTimOffSet + 0x457c */
	/* 0x3ffee080 = &WdevTimOffSet + 0x45dc */
	lldesc_build_chain(0x3ffee020, 84, 0x3ffee080, 0x00000700, 0x100, 1, &(wDevCtrl.f_48), &(wDevCtrl.f_52));

	wDevCtrl.f_4 = 7;

	((volatile void **)0x3ff1fe00)[160] = 0x3ffe9ab0;	/* &WdevTimOffSet + 0xc */
	((volatile void **)0x3ff1fe00)[159] = 0x3ffec098;	/* &WdevTimOffSet + 0x25f4 */
	((volatile void **)0x3ff1fe00)[162] = 0x3ffee020;	/* &WdevTimOffSet + 0x457c */
	((volatile void **)0x3ff1fe00)[161] = 0x3ffee780;	/* &WdevTimOffSet + 0x4cdc */

	((volatile uint32 *)0x3ff1fe00)[128] &= 0xffffff00;

	((volatile struct _wdev_ctrl_sub1 **)0x3ff1fe00)[130] = wDevCtrl.f_8;
	((volatile struct _wdev_ctrl_sub2 **)0x3ff1fe00)[131] = wDevCtrl.f_48;

	wDevCtrl.f_5 = 0;

	((volatile uint32 *)0x3ff1fe00)[132] = 0;
	((volatile uint32 *)0x3ff1fe00)[128] &= 0xdfffffff;
}

/* <wDev_Get_Next_TBTT+0x224> */
static void ICACHE_FLASH_ATTR _0x40247620()
{
	((volatile uint32 *)0x3ff20200)[128] = 0x76503210;
	((volatile uint32 *)0x3ff20200)[129] = 0xbbbbbbbb;
	((volatile uint32 *)0x3ff20200)[130] = 0xbbbbbbbb;
}

/* <wDev_Get_Next_TBTT+0x248> */
static void ICACHE_FLASH_ATTR _0x40247644()
{
	((volatile uint32 *)0x3ff1fe00)[155] |= 0x00000707;
	((volatile uint32 *)0x3ff1fe00)[155] &= 0xffffffef;
	((volatile uint32 *)0x3ff1fe00)[155] &= 0xfffffffe;

	wDev_SetRxPolicy(0, 0, NULL);
	wDev_SetRxPolicy(0, 1, NULL);
}

/* 0x402476a4 */
void ICACHE_FLASH_ATTR wDev_Initialize()
{
	((volatile uint32 *)0x3ff20a00)[134] = 0;
	((volatile uint32 *)0x3ff20a00)[137] = 0xffffffff;

	wDev_Option_Init();
	_0x40247a94();	/* <wDev_Crypto_Conf+0x4c> */
	_0x40247458();	/* <wDev_Get_Next_TBTT+0x5c> */
	_0x40247620();	/* <wDev_Get_Next_TBTT+0x224> */
	_0x40247644();	/* <wDev_Get_Next_TBTT+0x248> */

	((volatile uint32 *)0x3ff20a00)[134] = 0x0c9f0300;
	((volatile uint32 *)0x3ff1fe00)[222] |= 2;
	((volatile uint32 *)0x3ff1fe00)[129] &= 0x7fffffff;
}

/* 0x4024772c */
void ICACHE_FLASH_ATTR wDev_SetMacAddress(uint32 arg1, uint8 *arg2)
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

/* 0x402477cc */
void ICACHE_FLASH_ATTR wDev_SetRxPolicy(uint32 arg1, uint32 arg2, uint8 *arg3)
{
	switch (arg1) {
		case 0:
			if (arg2 == 0)
				((volatile uint32 *)0x3ff20a00)[143] &= 0xfffeffff;
			else
				((volatile uint32 *)0x3ff20a00)[145] &= 0xfffeffff;

			break;

		case 1:
			wDev_SetBssid(arg2, &ieee80211_addr_bcast);
			break;

		case 2:
			if (arg3 != NULL)
				wDev_SetBssid(arg2, arg3);

			break;

		default:
			break;
	}
}

/* 0x40247820 */
void ICACHE_FLASH_ATTR wDev_SetBssid(uint32 arg1, uint8 *arg2)
{
	if (arg1 == 0) {
		((volatile uint32 *)0x3ff20a00)[143] &= 0xfffeffff;
		((volatile uint32 *)0x3ff20a00)[138] = ((arg2[3] << 24) | (arg2[2] << 16) | (arg2[1] << 8) | arg2[0]);
		((volatile uint32 *)0x3ff20a00)[139] = ((arg2[5] << 8) | arg2[4]);
		((volatile uint32 *)0x3ff20a00)[142] = 0xffffffff;
		((volatile uint32 *)0x3ff20a00)[143] = 0x0000ffff;
		((volatile uint32 *)0x3ff20a00)[143] |= 0x00010000;
	} else {
		((volatile uint32 *)0x3ff20a00)[145] &= 0xfffeffff;
		((volatile uint32 *)0x3ff20a00)[140] = ((arg2[3] << 24) | (arg2[2] << 16) | (arg2[1] << 8) | arg2[0]);
		((volatile uint32 *)0x3ff20a00)[141] = ((arg2[5] << 8) | arg2[4]);
		((volatile uint32 *)0x3ff20a00)[144] = 0xffffffff;
		((volatile uint32 *)0x3ff20a00)[145] = 0x0000ffff;
		((volatile uint32 *)0x3ff20a00)[145] |= 0x00010000;
	}
}

/* 0x402478f8 */
void ICACHE_FLASH_ATTR wDev_ClearBssid(uint32 arg1)
{
	if (arg1 == 0)
		((volatile uint32 *)0x3ff20a00)[143] &= 0xfffeffff;
	else
		((volatile uint32 *)0x3ff20a00)[145] &= 0xfffeffff;
}

/* 0x40247930 */
/* Not too sure about that one... Alot of weird operations in assembly. */
void ICACHE_FLASH_ATTR wDev_Insert_KeyEntry(uint32 arg1, uint32 arg2, uint32 arg3, uint8 *arg4,
							uint32 arg5, uint8 *arg6, uint32 arg7)
{
	uint32 a1_4;		/* $a1 + 4 */ /* Note: this never gets initialized... */
	uint32 a0, a9;		/* $a0, $a9 */

	a9 = (arg1 == 5 ? 1 : 0);
	a0 = (arg1 == 5 ? 7 : (arg5 < 6 ? 6 : 2));

	if (arg1 == 5)
		arg1 = 1;

	((volatile uint32 *)0x3ff21200 + 10 * arg5)[128] = (arg4[3] << 24) + (arg4[2] << 16) + (arg4[1] << 8) + arg4[0];
	((volatile uint32 *)0x3ff21200 + 10 * arg5)[129] = (arg3 << 30) + ((arg2 & 0x1) << 24) + \
							   ((a0 & 0x7) << 21) + ((arg1 & 0x7) << 18) + \
							   ((a9 & 0x3) << 16) + (arg4[5] << 8) + arg4[4];

	((volatile uint32 *)0x3ff20600)[130] |= (1 << arg5);

	if ((arg2 < 2) && (arg5 >= 2))
		*((uint32 *)&wDevCtrl + 14 + arg2) |= (1 << arg5);

	ets_memcpy((uint8 *)0x3ff21408 + 40 * arg5, arg6, arg7);
}

/* 0x40247a04 */
void ICACHE_FLASH_ATTR wDev_Remove_KeyEntry(uint32 arg1)
{
	((volatile uint32 *)0x3ff20600)[131] &= ((1 << arg1) ^ 0xffffffff);
	((volatile uint32 *)0x3ff21200 + 10 * arg1)[129] &= 0xff1fffff;
}

/* <wDev_Crypto_Conf+0x4c> */
static void ICACHE_FLASH_ATTR _0x40247a94()
{
	((volatile uint32 *)0x3ff20600)[128] = 0x00030000;
	((volatile uint32 *)0x3ff20600)[129] = 0x00030000;

	wDev_Insert_KeyEntry(3, 0, 0, &ieee80211_addr_bcast, 0, &ieee80211_addr_bcast, 6);
	wDev_Insert_KeyEntry(3, 1, 0, &ieee80211_addr_bcast, 1, &ieee80211_addr_bcast, 6);

	((volatile uint32 *)0x3ff20600)[130] = 0;
}

/* 0x40247b20 */
void ICACHE_FLASH_ATTR wDevEnableRx()
{
	((volatile uint32 *)0x3ff1fe00)[129] |= 0x80000000;
}

/* 0x40247b4c */
void ICACHE_FLASH_ATTR wdev_go_sniffer()
{
	wDevCtrl.f_5 = 1;

	((volatile struct _wdev_ctrl_sub2 **)0x3ff1fe00)[131] = wDevCtrl.f_48;
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

/* 0x40247c28 */
void ICACHE_FLASH_ATTR wdev_set_sniffer_addr(uint8 *arg1)
{
	((volatile uint32 *)0x3ff1fe00)[155] |= 1;
	((volatile uint32 *)0x3ff1fe00)[155] |= 2;
	((volatile uint32 *)0x3ff1fe00)[155] |= 4;
	wDev_SetMacAddress(0, arg1);
}

/* 0x40247c84 */
void ICACHE_FLASH_ATTR wdev_exit_sniffer()
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
	((volatile struct _wdev_ctrl_sub1 **)0x3ff1fe00)[131] = &(wDevCtrl.f_24);

	wDevCtrl.f_5 = 0;
}

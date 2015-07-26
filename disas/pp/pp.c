/* 0x4010273c */
int ppProcessTxQ(uint8 sig)
{
	$a12 = sig;
	ets_intr_lock();

	if (!lmacIsIdle(sig)) {
		ets_intr_unlock();
		return -1;
	}

	$a2 = sig;
	$a13 = $a2 = _0x40102954(sig);	/* <ppEnqueueRxq+0xa8> */

	if ($a2 == NULL) {
		ets_intr_unlock();
		return -2;
	}

	$a0 = *(uint32 *)((uint8 *)$a2 + 36);
	$a3 = 0x3ffe8014;
	$a0 = *(uint32 *)((uint8 *)$a0 + 0);
	$a3 = *(uint32 *)((uint8 *)$a3 + 16);	/* 0x3ffe9114 --> the queue of esf_bufs used in ppTxPkt! */

	if ($a0 & (1 << 29)) {
		*(uint32 *)((uint8 *)$a3 + 0x158) += 1;
		*(uint32 *)((uint8 *)$a3 + 0x15c) -= 1;
	}

	ets_intr_unlock();

	lmacTxFrame($a13, sig);
	return 0;
}

/* 0x40102878 */
void ppEnqueueTxDone(struct esf_buf *ebuf)
{
	if (ebuf == NULL) {
		ets_printf("%s %u\n", "pp.c", 2204);
		while (1);
	}

	$a4 = *(struct esf_buf ***)((uint8 *)0x3ffe9114 + 0x12c);
	ebuf->next = NULL;

	*(struct esf_buf **)$a4 = ebuf;
	*(struct esf_buf ***)((uint8 *)0x3ffe9114 + 0x12c) = &ebuf->next;
}

/* <ppEnqueueRxq+0x18> */
static struct esf_buf *_0x401028c4(uint8 arg1)
{
	uint32 a1_0;

	$a12 = 0x3ffe8014;
	$a0 = *(uint32 *)((uint8 *)$a12 + 16);
	$a13 = arg1 * 32;
	$a0 += arg1;
	$a0 = *(uint8 *)((uint8 *)$a0 + 36);

	if ($a0 == 0 || $a0 == 3)
		$a2 = 2;
	else if ($a0 < 3)
		$a2 = 3;
	else if ($a0 < 6)
		$a2 = 1;
	else
		$a2 = 0;

	$a2 = GetAccess($a2);

	$a5 = *((uint8 *)$a12 + 16);
	$a0 = $a2;
	$a5 += $a13;
	$a6 = *(uint8 *)((uint8 *)$a5 + 33);
	$a12 = NULL;

	if ($a6 != 0)
		return NULL;

	$a2 = *(uint32 *)((uint8 *)$a5 + 44);

	if ($a2 == 0) {
		if ($a6 == 0) {
			a1_0 = $a0;

			while (($a12 = *(uint32 *)((uint8 *)$a5 + 24)) != NULL) {
				$a0 = 0;
				$a6 = *(uint32 *)((uint8 *)$a12 + 32);
				*(uint32 *)((uint8 *)$a5 + 24) = $a6;

				if ($a6 == NULL) {
					$a7 = $a5 + 24;
					*(uint32 *)((uint8 *)$a5 + 28) = $a7;
				}

				$a2 = $a12;
				*(uint32 *)((uint8 *)$a12 + 32) = NULL;
				$a2 = lmacMSDUAged($a2);

				if ($a2 == NULL)
					break;

				$a3 = $a12;
				$a2 = a1_0;
				$a4 = 0;
				lmacDiscardAgedMSDU($a2, $a3, $a4);

				$a5 = 0x3ffe8014;
				$a5 = *(uint32 *)((uint8 *)$a5 + 16);
				$a5 += $a13;
			}
		}
	} else {
		$a12 = $a2;
	}

	$a2 = $a12;
	return $a2;
}

/* <ppEnqueueRxq+0xa8> */
static struct esf_buf *_0x40102954(uint8 sig)
{
	struct esf_buf *ebuf;
	uint8 i;
	uint32 a1_24;
	uint32 a1_0;

	$a13 = sig;
	_0x40102a4c(sig);	/* <ppEnqueueRxq+0x1a0> */

	$a0 = 0x3ffe8014;
	$a15 = 1;
	$a0 = *(uint32 *)((uint8 *)0x3ffe8014 + 16);	/* 0x3ffe9114 --> the queue of esf_bufs used in ppTxPkt! */
	$a14 = 4 * sig;
	$a2 = $a0 + sig;
	$a4 = $a0 + 4 * sig;
	$a4 = *(uint32 *)((uint8 *)$a4 + 4);
	$a2 = *(uint8 *)((uint8 *)$a2 + 20);

	if ((sig == 0) || !(($a4 >> $a2) & 0x01) || (*(uint8 *)((uint8 *)$a0 + 32 * $a2 + 40) != 1)) {
		a1_24 = $a2;

		for (i = 0; i < 2; i++) {
			ebuf = _0x401028c4(i);	/* <ppEnqueueRxq+0x18> */

			if (ebuf != NULL)
				return ebuf;
		}

		$a4 = 0x3ffe8014;
		$a4 = *(uint32 *)((uint8 *)0x3ffe8014 + 16);
		$a2 = a1_24;
		$a4 += $a14;
		$a4 = *(uint32 *)((uint8 *)$a4 + 4);
	}

	a1_0 = sig;
	$a12 = $a15 << $a2;
	$a12 = $a12 - 1;
	$a12 = $a4 & $a12;
	$a14 = $a4 ^ $a12;

	/* computing the FCS checksum (last 4 bytes appended to packet?) maybe? */
	while (1) {
		$a13 = ~$a14;
		$a14 = $a13 & $a14;
		$a13 = _nsau($a13);
		$a13 = ~$a13;
		$a13 += 31;

		if ($a13 < 0) {
			do {
				$a13 = ~$a12;
				$a13 &= $a12;
				$a13 = _nsau($a13);
				$a13 = ~$a13;
				$a13 += 31;

				if ($a13 < 0)
					return NULL;

				$a2 = $a13 & 0xff;
				ebuf = _0x401028c4($a13 & 0xff);	/* <ppEnqueueRxq+0x18> */

				if (ebuf == NULL) {
					$a0 = -1;
					$a14 = $a15 << $a13;
					$a14 ^= $a0;
					$a12 &= $a14;
				}
			} while (ebuf == NULL);
			
			$a0 = 0x3ffe8014;
			$a3 = sig;
			$a0 = *(uint32 *)((uint8 *)$a0 + 16);
			$a3 = $a0 + sig;
			*(uint8 *)((uint8 *)$a3 + 20) = $a13;
			break;
		}
		
		$a2 = $a13 & 0xff;
		ebuf = _0x401028c4();	/* <ppEnqueueRxq+0x18> */

		if (ebuf != NULL) {
			$a0 = 0x3ffe8014;
			$a14 = sig;
			$a0 = *(uint32 *)((uint8 *)$a0 + 16);
			$a14 += $a0;
			*(uint8 *)((uint8 *)$a14 + 20) = $a13;
			break;
		}

		$a5 = -1;
		$a4 = $a15 << $a13;
		$a4 = $a4 ^ $a5;
		$a14 = $a14 & $a4;
	}

	$a3 = $a13 << 5;
	$a3 = $a0 + $a3;
	$a15 = *(uint8 *)((uint8 *)$a3 + 40);
	return ebuf;
}

/* <ppEnqueueRxq+0x1a0> */
static void _0x40102a4c(uint8 sig)
{
	// stub
}

/* <ppEnqueueRxq+0x55c> */
static void _0x40102e08(struct esf_buf *ebuf)
{
	
}

/* 0x40102f88 */
void ppDiscardMPDU(struct esf_buf *ebuf)
{
	ebuf->ep->data[14] = 0;
	ebuf->ep->data[15] = 4;
	ppEnqueueTxDone(ebuf);
	pp_post(4);
}

/* 0x40102fac */
int pp_post(uint8 arg1)
{
	int ret;

	ets_intr_lock();

	if (((uint8 *)0x3ffe9280)[arg1] == 0) {
		++((uint8 *)0x3ffe9280)[arg1];
		ets_intr_unlock();
		ret = ets_post(32, arg1, NULL);
	} else {
		ets_intr_unlock();
		ret = 0;
	}

	return ret;
}

/* 0x40103094 */
void ppCalFrameTimes(struct esf_buf *ebuf)
{
	$a2 = *(uint32 *)(ebuf->ep->data + 0);

	if ($a2 & (1 << 25))
		$a2 = RC_GetBlockAckTime(ebuf->ep->data[8]);
	else if (($a2 & (1 << 16)) || ($a2 & (1 << 7)))
		$a2 = 0;
	else
		$a2 = RC_GetAckTime(ebuf->ep->data[8]);

	$a5 = *(uint16 *)(ebuf->ep->data + 6) & 0x7f;
	$a6 = ($a2 & 0x01ff) << 7;
	$a5 |= $a6;
	*(uint16 *)(ebuf->ep->data + 6) = $a5;

	$a4 = *(uint16 *)(ebuf->ep->data + 6) >> 7;

	$a2 = RC_GetCtsTime(ebuf->ep->data[8], ebuf->len1 + ebuf->len2, $a4);
	*(uint16 *)(ebuf->ep->data + 10) = $a2;
}

/* task prio = 32 */
static void ICACHE_FLASH_ATTR _0x40245ce4(struct ETSEventTag *e)
{
	struct ETSEventTag *a1_0;

	$a0 = e->sig;
	$a4 = 0x3ffe9280;

	if ($a0 >= 4)
		goto _0x40245d0d;

	$a3 = $a4;
	$a3 = $a0 + $a3;
	$a4 = *(uint8 *)((uint8 *)$a3 + 0);
	$a2 = $a0 & 0xff;
	$a0 = $a4 - 1;
	*(uint8 *)((uint8 *)$a3 + 0) = $a0;
	ppProcessTxQ($a2);
	goto _0x40245d06;

_0x40245d06:
	return;

_0x40245d0d:
	a1_0 = e;

	if ($a0 == 4)
		goto _0x40245d7f;

	if ($a0 == 5)
		goto _0x40245da7;

	if ($a0 == 7)
		goto _0x40245d06;

	if ($a0 == 8)
		goto _0x40245dcd;

	if ($a0 == 9)
		goto _0x40245d77;

	if ($a0 == 10)
		goto _0x40245de3;

	if ($a0 == 11)
		goto _0x40245e09;

	if ($a0 == 12)
		goto _0x40245d35;

	goto _0x40245d06;

_0x40245d35:
	/* ... */	
}

/* <ppPeocessRxPktHdr+0x1d4> */
static void _0x40245e38(struct esf_buf *ebuf)
{
	$a7 = ebuf->e_data;
	$a3 = ebuf->e_data.i_addr1[0];
	$a0 = *(uint32 *)(ebuf->ep->data + 0);

	/* IEEE80211_IS_MULTICAST(ebuf->e_data.i_addr1) */
	if ($a3 & 0x01) {
		$a3 = *(uint32 *)(ebuf->ep->data + 0) >> 6;
		$a0 = *(uint32 *)(ebuf->ep->data + 0) & 0x3f;
		$a3 |= 0x02;
		$a3 <<= 6;
		$a0 |= $a3;
		*(uint32 *)(ebuf->ep->data + 0) = $a0;
	}

	$a4 = ebuf->e_data.i_fc & 0x0c;	/* Type in frame control */
	$a6 = ebuf->e_data.i_fc & 0xf0;	/* Subtype in frame control */

	if ($a4 == 0x08) {
		/* Type = 2 (Data frame) */
		$a8 = *(uint32 *)(ebuf->ep->data + 0) & 0x3f;
		$a3 = 0x08;
		$a0 = *(uint32 *)(ebuf->ep->data + 0) >> 6;
		$a0 |= $a3;
		$a0 <<= 6;
		$a0 |= $a8;
		*(uint32 *)(ebuf->ep->data + 0) = $a0;

		if ($a6 == 0x40 || $a6 == 0xc0) {
			/* Subtype = 4 (Null) or 12 (QoS Null) */
			$a6 = $a0 & 0x3f;
			$a0 >>= 6;
			$a0 &= 0x03fffff7;
			$a0 <<= 6;
			$a0 |= $a6;
			*(uint32 *)(ebuf->ep->data + 0) = $a0;
		}
	} else if ($a4 == 0x00) {
		/* Type = 0 (Management frame) */
		if ($a6 == 0x80) {
			/* Subtype = 8 (Beacon) */
			$a9 = $a0 & 0x3f;
			$a10 = $a4 | (1 << 20);
			$a10 <<= 6;
			$a9 |= $a10;
			*(uint32 *)(ebuf->ep->data + 0) = $a9;
			$a6 = *(uint32 *)((uint8 *)0x3ffe9114 + 0x15c);
			$a6 += 1;
			*(uint32 *)((uint8 *)0x3ffe9114 + 0x15c) = $a6;

			if ($a6 >= 6)
				os_printf_plus("y");

			$a0 = *(uint32 *)(ebuf->ep->data + 0);
		} else if ($a6 == 0x40) {
			/* Subtype = 4 (Probe Request) */
			if (!($a0 & (1 << 7))) {
				$a3 = 0x00000800;
				$a0 &= 0x3f;
				$a3 = $a4 | (1 << 11);
				$a3 <<= 6;
				$a0 |= $a3;
				*(uint32 *)(ebuf->ep->data + 0) = $a0;
			}
		}
	}

	$a4 = $a0 >> 6;

	if ($a4 & (1 << 18))
		((uint8 *)ebuf->np1->data)[3] = (uint8)(ebuf->e_data.i_seq >> 4);
}

/* 0x40245f38 */
int ICACHE_FLASH_ATTR ppTxPkt(struct esf_buf *ebuf)
{
	$a0 = ebuf->ep->data[4] & 0x0f;
	$a3 = (ebuf->ep->data[6] >> 4) & 0x07;

	if ($a0 == 0 || $a0 == 3)
		$a2 = 2;
	else if ($a0 < 3)
		$a2 = 3;
	else if ($a0 < 6)
		$a2 = 1;
	else
		$a2 = 0;

	if ($a2 != $a3) {
		ets_uart_printf("%s %u\n", "pp.c", 527);
		while (1);
	}

	_0x40245e38(ebuf);	/* <ppPeocessRxPktHdr+0x1d4> */

	if (_0x40246a94(ebuf) == 1) {	/* <pp_attach+0x100> */
		ets_intr_lock();
		ppDiscardMPDU(ebuf);
		ets_intr_unlock();
		return 1;
	}

	ets_intr_lock();
	rcGetSched(ebuf->type1, ebuf->ep);
	ets_intr_unlock();

	ppCalFrameTimes(ebuf);
	_0x40102e08(ebuf);

	switch ($a2) {
		case 0:
			/* This whole block puts the ebuf in some queue dependent on 4 bits in ebuf->ep->data[0] */
			ets_intr_lock();
			ebuf->next = NULL;

			$a6 = *((uint32 *)0x3ffe9114);	/* main queue array (uint8 *)&soft_wdt_interval + 16 */
			$a8 = ((ebuf->ep->data[0] >> 2) & 0x0f) * 32;
			$a8 = $a6 + $a8;
			$a8 = *(uint32 *)((uint8 *)$a8 + 28);
			*(struct esf_buf **)((uint8 *)$a8 + 0) = ebuf;

			$a7 = ((ebuf->ep->data[0] >> 2) & 0x0f) * 32;
			$a6 += $a7;
			*(struct esf_buf ***)((uint8 *)$a6 + 28) = &ebuf->next;

			ets_intr_unlock();

			return pp_post((ebuf->ep->data[6] >> 4) & 0x07);

		case 1:
			ebuf->next = NULL;

			$a7 = *((uint32 *)0x3ffe9114);	/* main queue array (uint8 *)&soft_wdt_interval + 16 */
			$a8 = (ebuf->ep->data[0] >> 1) & 0x01;
			$a8 = $a7 + 8 * $a8;
			$a8 = *(uint32 *)((uint8 *)$a8 + 0x11c);
			*(struct esf_buf **)((uint8 *)$a8 + 0) = ebuf;

			$a6 = (ebuf->ep->data[0] >> 1) & 0x01;
			$a6 = $a7 + 8 * $a6;
			*(struct esf_buf ***)((uint8 *)$a6 + 0x11c) = &ebuf->next;

			return 0;

		case 2:
			os_printf_plus("map fail\n");
			ets_intr_lock();
			ppDiscardMPDU(ebuf);
			ets_intr_unlock();

			return 1;

		default:
			return 0;
	}
}

/* 0x40246994 */
void ICACHE_FLASH_ATTR pp_attach()
{
	uint8 i;

	$a3 = *(uint32 *)((uint8 *)0x0c42ca0c + 16);

	for (i = 0; i < 8; i++) {
		$a2 = $a3 + 32 * i;
		*(uint32 *)((uint8 *)$a2 + 24) = 0;
		*(uint8 *)((uint8 *)$a2 + 39) = 0x00;
		*(uint32 *)((uint8 *)$a2 + 28) = $a2 + 24;
	}

	for (i = 0; i < 2; i++) {
		$a2 = $a3 + 32 * i;
		*(uint8 *)((uint8 *)$a2 + 39) = 0x01;
		*(uint8 *)((uint8 *)$a2 + 36) = 0x07;
	}

	$a4 = *(uint32 *)((uint8 *)$a3 + 0);

	for (i = 0; i < 4; i++) {
		$a6 = 3 * i;
		$a5 = $a3 + 4 * i;
		*(uint32 *)((uint8 *)$a5 + 4) = $a4;
		$a6 <<= 1;
		$a5 = $a3 + i;
		$a2 = $a6 + 3;

		if ($a6 >= 0)
			$a2 = $a6;

		(signed)$a2 >>= 2;
		$a2 += 2;
		*(uint8 *)((uint8 *)$a5 + 20) = $a2;
	}

	for (i = 0; i < 2; i++) {
		$a2 = $a3 + 8 * i;
		*(uint32 *)((uint8 *)$a2 + 0x118) = $a4;
		*(uint32 *)((uint8 *)$a2 + 0x11c) = $a2 + 0x118;
	}

	*(uint32 *)((uint8 *)$a3 + 0x130) = $a4;
	*(uint32 *)((uint8 *)$a3 + 0x128) = $a4;
	*(uint32 *)((uint8 *)$a3 + 0x134) = $a3 + 0x130;
	*(uint32 *)((uint8 *)$a3 + 0x12c) = $a3 + 0x128;

	*(volatile uint32 *)((uint8 *)0x60000e00 + 0x3f8) = 0x00000806;

	esf_buf_setup();
	ets_task(_0x40245ce4, 32, 0x3ffe9290, 34);
	ets_timer_set_fn(0x3ffe90e4, pp_tx_idle_timeout, 0);

	$a5 = *(uint8 *)((uint8 *)&chip6_phy_init_ctrl + 106);
	$a10 = 600000;
	$a0 = 10000 * $a5;

	if ($a5 != 0)
		$a10 = $a0;

	*(uint32 *)((uint8 *)&soft_wdt_interval + 8) = $a10;	/* 0x3ff1910c */
	ets_timer_setfn(0x3ffe90c4, pp_noise_test, 0);
	pp_enable_noise_timer();
}

/* <pp_attach+0x100> */
static int ICACHE_FLASH_ATTR _0x40246a94(struct esf_buf *ebuf)
{
	$a0 = ebuf->ep;
	$a5 = ebuf->type1;
	$a0 = ebuf->ep->data[13];
	$a10 = ebuf->len2;
	$a0 &= 0x0f;

	switch ($a0) {
		case 0: $a0 = 4;
			break;

		case 1: $a0 = 8;
			break;

		case 2: $a0 = 16;
			break;

		case 3: $a0 = 12;
			break;

		default: $a0 = 4;
			 break;
	}

	$a10 += $a0;
	ebuf->len2 = $a10;
	$a11 = 0x00ffffff;
	$a8 = ebuf->np2;	/* --> not a pbuf!!! */
	$a10 = 0xff000fff;
	$a9 = *(volatile *)(&ebuf->np2->len2) += $a0;	/* Convoluted compiler code... */
	$a6 = *(uint32 *)(ebuf->ep->data + 0);
	$a3 = 0x40;

	if (!($a6 & (1 << 24))) {
		$a4 = 0xbf;
		$a0 = ebuf->np2;
		$a11 = *(volatile uint8 *)(&ebuf->np2->pad1);
		$a11 &= 0xbf;
		$a11 |= 0x40;
		*(volatile uint8 *)(&ebuf->np2->pad1) = $a11;
		return 0;
	}

	if ($a5 == NULL)
		return 1;

	$a6 = 0x20;
	$a7 = 0xdf;
	$a4 = ebuf->np1;
	$a3 = *(volatile uint8 *)(&ebuf->np1->pad1);
	$a3 &= 0xdf;
	$a3 |= 0x20;
	*(volatile uint8 *)(&ebuf->np1->pad1) = $a3;

	$a0 = ebuf->ep->data[13] & 0x0f;

	if ($a0 != 0 && $a0 != 3) {
		ets_printf("%s %u\n", "pp.c", 2798);
		while (1);
	}

	$a4 = 0x0000fffc;
	$a3 = ebuf->len2;
	$a2 = ebuf->np1;
	$a3 += $a4;
	$a2 = ebuf->np1->data;
	$a4 = 0x0000f000;
	$a0 = *(uint16 *)((uint8 *)$a2 + 0);
	$a3 &= 0x0fff;
	$a0 &= 0xf000;
	$a0 |= $a3;
	$a3 = *(uint8 *)((uint8 *)$a2 + 2);
	*(uint16 *)((uint8 *)$a2 + 0) = $a0;
	$a5 = *(uint8 *)((uint8 *)ebuf->type1 + 116);
	$a6 = *(uint8 *)((uint8 *)$a2 + 1);
	$a0 &= 0x0fff;
	$a8 = $a0 + 4;
	$a3 <<= 8;
	$a3 |= $a6;

	if ($a5 < 8) {
		$a0 = 0;
	} else {
		$a0 = $a5 - $a0;
		$a0 = (signed)$a0 >> 2;
	}

	$a5 = 0x0000f80f;
	$a4 = $a0 & 0x7f;
	$a4 <<= 4;
	$a3 &= $a5;
	$a3 |= $a4;
	*(uint8 *)((uint8 *)$a2 + 1) = $a3;
	$a4 = 0xcf;
	$a3 = ($a3 >> 8) & 0xff;
	$a3 &= $a4;
	*(uint8 *)((uint8 *)$a2 + 2) = $a3;
	return 0;
}

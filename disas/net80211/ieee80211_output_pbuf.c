err_t ieee80211_output_pbuf(struct netif *netif, struct pbuf *pbuf)
{
	uint32 stuff;	/* at $a1 + 12 */
	void *payload;	/* at $a1 + 8 */
	uint32 number;	/* at $a1 + 4 */
	uint32 res;	/* at $a1 + 0 */

	$a13 = netif->state;
	$a0 = *((uint32 *)($a13 + 44));	/* *(uint32 *)(netif->state + 44) */
	$a14 = pbuf;
	$a0 = $a0 - 5; 			/* *(uint32 *)(netif->state + 44) - 5 */

	if ($a0 != 0)		/* if *(uint32 *)(netif->state + 44) != 5 */	/* dhcp status flag? */
		return 1;
	if ($a3 == 0)		/* if (pbuf == NULL) */
		return 1;

	$a2 = *((uint32 *)($a14 + 4));		/* pbuf->payload */
	$a3 = *((uint32 *)($a13 + 160));	/* *(uint32 *)(netif->state + 160) */

	payload = pbuf->payload;

	if ($a3 == 0) {
		$a12 = *((uint32 *)($a13 + 120));	/* *(uint32 *)(netif->state + 120) */
	} else {
		payload = $a2;	/* pbuf->payload */
		$a12 = cnx_node_search();
	}
	
	if ($a12 == NULL)
		return 1;

	$a4 = *((uint16 *)($a12 + 26));

	if ($a4 == 0) {
		$a5 = *((uint32 *)($a12 + 8));

		if ($a5 & (1 << 17))
			return 1;
	}

	$a2 = $a12;	/* result of cnx_node_search()? */
	$a6 = 1;
	$a9 = payload;
	$a3 = 0;
	$a8 = ((uint8 *)payload)[13];
	$a9 = ((uint8 *)payload)[12];
	$a8 <<= 8;	/* ((uint8 *)payload)[13] <<= 8 */
	$a8 |= $a9;	/* (((uint8 *)payload[13]) << 8) | ((uint8 *)payload)[12] */
	$a7 = ($a8 >> 8) & 0xff;
	$a8 = $a8 & 0xff;
	$a8 <<= 8;
	$a7 |= $a8;	/* $a7 = (payload[12] << 8) | payload[13].
			   Note: this corresponds to EtherTye field in an Ethernet frame */
	$a8 = *((uint32 *)0x402529f0); /* 0x888e: EtherType for EAP over LAN (IEEE 802.1X) */
	$a7 &= 0xffff;
	$a7 -= $a8;

	if ($a7 == 0)		/* if (payload[12] == 0x88 && payload[13] == 0x8e) */
		$a3 = $a6;	/* $a3 = 1 */

	number = $a3;

	if ((*0x40253b20)($a2, $a3) == 0)	/* _0x40253c68 */
		return 1;

	*((uint32 *)($a13 + 120)) = $a12;	/* *(uint32 *)(netif->state + 120) = cnx_node_search() ?? */
	res = (*0x4025328c)($a12, $a14)		/* (*0x4025328c)(cnx_node, pbuf) */

	if (($a3 = $a2 = (**0x4025054c)(pbuf, 1, 0)) == 0)	/* (*0x40104af0) esf_buf_alloc(pbuf, 1, 0) */
		return 1;

	$a5 = 4;
	$a10 = number;
	$a0 = 8;

	if ($a10 != 0) {	/* number == 0 */
		$a2 = *((uint32 *)($a2 + 36));	/* *((uint32 *)(esf_buf_alloc_result + EP_OFFSET)) */
		$a4 = *((uint32 *)$a2);		/* **((uint32 **)(esf_buf_alloc_result + EP_OFFSET)) */
		$a6 = $a4 >> 6;
		$a4 = $a4 & 0x3f;
		$a6 |= $a5;	/* $a6 = ($a4 >> 6) | 4 */
		$a6 <<= 6;	/* $a6 = (($a4 >> 6) | 4) << 6 */
		$a4 |= $a6;	/* $a4 = *((uint32 *)$a2) | (1 << 8), in total */
		*((uint32 *)$a2) = $a4;	/* *((uint32 *)$a2) |= (1 << 8), in total */

		$a11 = *((uint32 *)($a13 + 176));	/* *(uint32 *)(netif->state + 176) */

		if ($a11 == 0)				/* wat?! */
			*((uint8 *)($a2 + 14)) = $a0;	/* ((uint8 *)(*((uint32 *)(esf_buf_alloc_result + EP_OFFSET))))[14] = 8 */
	}

	$a2 = (*0x40252fe8)($a13);	/* (*0x40252fe8)(netif->state) */
	$a5 = res;

	if ($a2 == 0)	/* result from (*0x40252fe8($a13)) */
		return 1;

	stuff = $a2;	/* stuff = (*0x40252fe8)(netif->state) */
	$a11 = 8;
	$a3 = 1;
	$a7 = *((uint32 *)($a2 + 36));	/* *((uint32 *)($a2 + EP_OFFSET)) (maybe ??) */
	$a8 = *((uint32 *)0x4024000c);	/* $a8 = 0x32fefdbc (&g_ic) */
	$a10 = *((uint32 *)($a7));	/* **((uint32 **)($a2 + EP_OFFSET)) (maybe ??) */
	$a6 = *((uint32 *)($a8 + 16));	/* (uint32)(g_ic._16) */
	$a9 = $a10 & 0x3f;
	$a6 -= $a13;	/* $a6 = (uint32)(g_ic._16) - (uint32)(netif->state) */

	if ($a6 != 0)	/* if (g_ic._16 != netif->state) */
		$a6 = $a3;	/* $a6 = 1 */ /* in total, $a6 = (g_ic._16 != netif->state) ? 1 : 0 */

	$a10 >>= 6;
	$a10 |= $a11;	/* $a10 |= 8 */
	$a3 = 143;	/* 0x8f = 0b10001111 */
	$a11 = *((uint8 *)($a7 + 6));	/* ??? */
	$a10 <<= 6;
	$a9 |= $a10;	/* in total, $a9 = **((uint32 **)($a2 + EP_OFFSET)) | (1 << 9) */
	$a11 &= $a3;	/* $a11 &= 143 = 0x8f = 0b10001111 */
	*((uint32 *)($a7)) = $a9; /* in total, **((uint32 **)($a2 + EP_OFFSET)) |= (1 << 9) */
	$a3 = $a5 & 7;	/* $a3 = res & 7 */
	$a3 <<= 4;	/* $a3 = (res & 7) << 4 */
	$a11 |= $a3;	/* $a11 |= (res & 7) << 4 */

	if ($a6 == 0)	/* if (g_ic._16 == netif->state) */ /* wat?! */
		$a0 = 0;
	else
		$a0 = *((uint16 *)($a12 + 26));	/* $a0 = *((uint16 *)(&cnx_node + 26)) (???) Referenced above too. */

	$a3 = 240;	/* 0xf0 = 0b11110000 */
	$a10 = stuff;
	*((uint8 *)($a7 + 6)) = $a11;	/* the ??? = $a11 */
	$a9 = *((uint32 *)($a10 + 36));	/* $a9 = *((uint32 *)(stuff + 36)) */

	if ($a5 != 0) {			/* if (res != 0) */
		if ($a5 == 1) {		/* if (res == 1) */
			$a5 = 5;
		} else {
			$a7 = $a5 - 3	/* res - 3 */
			$a4 = 1;
			$a5 = 0;

			if ($a7 == 0) /* if (res == 3) */
				$a5 = $a4;	/* $a5 = (res == 3) */
		}
	} else {
		$a5 = 6;
	}

	$a8 = *((uint8 *)($a9 + 4));
	$a11 = $a5 & 0xf;
	$a8 &= $a3;	/* $a8 &= 0xf0 */
	$a8 |= $a11;	/* $a8 = (($a8 & 0xf0) | ($a11 & 0x0f)) */
	*((uint8 *)($a9 + 4)) = $a8;

	$a7 = *((uint32 *)0x40241124); /* 0x3ff20a00 */
	$a5 = 0;
	$a3 = *((uint32 *)($a10 + 36)); /* $a3 = *((uint32 *)(stuff + 36)) */
	memw(); /* volatile ?? */
	
	$a4 = 254;	/* 0xfe = 0b11111110 */
	$a11 = *((uint8 *)$a3);
	$a7 = *((uint32 *)($a7 + 0x200)); /* *((uint32 *)(0x3ff20c00)) */
	*((uint32 *)($a3 + 16)) = $a7; /* ??? */
	$a11 &= $a4;	/* $a11 &= 0xfe */
	*((uint32 *)($a3)) = $a11;

	$a3 = 253; /* 0xfd = 0b11111101 */
	$a10 = *((uint32 *)($a10 + 36));	/* $a10 = *((uint32 *)(stuff + 36)) */
	$a11 = $a6 & 1;	/* $a11 = (g_ic._16 == netif->stuff) */
	$a9 = *((uint8 *)($a10));
	$a11 <<= 1;
	$a9 &= $a3;	/* $a9 &= 0xfd */
	$a9 |= $a11;	/* $a9 bit 1 = (g_ic._16 == netif->stuff). ???? WTF ???? */
	*((uint8 *)($a10)) = $a9;

		
	/* eventually: ppTxPkt(stuff) */
}

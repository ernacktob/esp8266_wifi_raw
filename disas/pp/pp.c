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
	$a13 = $a2 = _0x40102954(sig);

	if ($a2 == NULL) {
		ets_intr_unlock();
		return -2;
	}

	$a0 = *(uint32 *)((uint8 *)$a2 + 36);
	$a3 = 0x3ffe8014;
	$a0 = *(uint32 *)((uint8 *)$a0 + 0);
	$a3 = *(uint32 *)((uint8 *)$a3 + 16);

	if ($a0 & (1 << 29)) {
		*(uint32 *)((uint8 *)$a3 + 0x158) += 1;
		*(uint32 *)((uint8 *)$a3 + 0x15c) -= 1;
	}

	ets_intr_unlock();

	lmacTxFrame($a13, sig);
	return 0;
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

/* 0x40245f38 */
int ICACHE_FLASH_ATTR ppTxPkt(struct esf_buf *ebuf)
{
	int ret;

	$a5 = *(uint32 *)((uint8 *)ebuf + 36);
	$a12 = ebuf;
	$a3 = *(uint8 *)((uint8 *)$a5 + 6);
	$a0 = *(uint8 *)((uint8 *)$a5 + 4);
	$a3 = ($a3 >> 4) & 0x7;
	$a0 &= 0x0f;

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

	_0x40245e38(ebuf);

	if (_0x40246a94(ebuf) == 1) {
		ets_intr_lock();
		ppDiscardMPDU(ebuf);
		ets_intr_unlock();
		ret = 1;
		return 1;
	}

	ets_intr_lock();
	$a2 = *(uint32 *)((uint8 *)ebuf + 28);
	$a3 = *(uint32 *)((uint8 *)ebuf + 36);
	rcGetSched($a2, $a3);
	ets_intr_unlock();

	ppCalFrameTimes(ebuf);
	_0x40102e08(ebuf);

	$a5 = 0;
	$a4 = &soft_wdt_interval;

	switch ($a2) {
		case 0:
			/* This whole block puts the ebuf in some queue dependent on 4 bits in ebuf->ep[0] */
			ets_intr_lock();
			$a6 = &soft_wdt_interval;
			$a7 = *(uint32 *)((uint8 *)ebuf + 36);
			$a8 = 0;
			*(uint32 *)((uint8 *)ebuf + 32) = 0;
			$a8 = *(uint8 *)((uint8 *)$a7 + 0);
			$a6 = *(uint32 *)((uint8 *)&soft_wdt_interval + 16);	/* main queue array 0x3ffe9114 */
			$a8 = ($a8 >> 2) & 0xf;
			$a8 <<= 5;
			$a8 = $a6 + $a8;
			$a8 = *(uint32 *)((uint8 *)$a8 + 28);
			*(struct esf_buf *)((uint8 *)$a8 + 0) = ebuf;
			$a7 = *(uint8 *)((uint8 *)$a7 + 0);
			$a5 = (uint8 *)ebuf + 32;
			$a7 = ($a7 >> 2) & 0xf;
			$a7 <<= 5;
			$a6 += $a7;
			*(uint32 *)((uint8 *)$a6 + 28) = $a5;
			ets_intr_unlock();

			$a2 = *(uint32 *)((uint8 *)ebuf + 36);
			$a2 = *(uint8 *)((uint8 *)$a2 + 6);
			$a2 = ($a2 >> 4) & 0x7;
			pp_post($a2);
			ret = $a2;
			break;

		case 1:
			$a6 = *(uint32 *)((uint8 *)ebuf + 36);
			*(uint32 *)((uint8 *)ebuf + 32) = $a5;
			$a8 = *(uint8 *)((uint8 *)$a6 + 0);
			$a7 = *(uint32 *)((uint8 *)&soft_wdt_interval + 16);	/* 0x3ffe9114 */
			$a8 = ($a8 >> 1) & 0x1;
			$a8 += 8 * $a7;
			$a8 = *(uint32 *)((uint8 *)$a8 + 0x11c);
			*(struct esf_buf *)((uint8 *)$a8 + 0) = ebuf;
			$a6 = *(uint8 *)((uint8 *)$a6 + 0);
			$a3 = (uint8 *)ebuf + 32;
			$a6 = ($a6 >> 1) & 0x1;
			$a6 += 8 * $a7;
			*(uint32 *)((uint8 *)$a6 + 0x11c) = $a3;
			ret = 0;
			break;

		case 2:
			$a2 = 0x40267490;
			os_printf_plus($a2);
			ets_intr_lock();
			ppDiscardMPDU(ebuf);
			ets_intr_unlock();
			ret = 1;
			break;

		default:
			ret = 0;
			break;
	}

	return ret;
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

	*(uint32 *)((uint8 *)&soft_wdt_interval + 8) = $a10;
	ets_timer_setfn(0x3ffe90c4, pp_noise_test, 0);
	pp_enable_noise_timer();
}

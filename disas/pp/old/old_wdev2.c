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
	$a0 = ((uint32 **)0x3ffe82a0)[0];

	if (((uint16 **)$a0)[0] < 2)
		return;

	$a2 = ((volatile uint32 *)0x3ff1fe00)[142] & 0xff;

	if ($a2 > 65 && !($a2 & (1 << 7)))
		return;

	$a13 = ((uint8 ***)$a0)[2][1];
	$a0 = ((uint8 *)$a13)[4];

	if ((((uint8 *)$a13)[1] << 6) == 0)
		return;

	$a2 = $a0 >> 7;

	if (($a2 == 0) && (($a0 & 0x7f) < 8)) {
		$a4 = ((uint8 *)$a13)[7];

		if (!($a4 & (1 << 6)))
			return;
	}

	$a12 = $a2 = pvPortMalloc(12);

	if ($a2 == NULL)
		return;

	ets_memcpy($a2, $a13, 12);

	if (($a2 = chm_get_current_channel()) == NULL) {
		ets_printf("%s %u\n", "wdev.c", 1862);
		while (1);
	}

	((uint8 *)$a12)[10] = (((uint8 *)$a2)[10] & 0xf0) | (((uint8 *)$a12)[6] & 0x0f);

	if ((((uint8 *)$a12)[1] << 6) == 0)
		goto _0x401046a1;

	$a6 = ((uint8 *)$a12)[4];
	$a0 = $a6 & 0x7f;
	$a6 >>= 7;

	if ($a6 == 0 && $a0 < 8 && !(((uint8 *)$a12)[7] & (1 << 6)))
		goto _0x401046a1;

	if ($a0 >= 32)
		goto _0x401046a1;

	if (pp_post2(32, 9, $a12) != 1)
		return;	/* memory leak? Did not free $a12. Maybe freed in pp_post2. */

	vPortFree($a12);
	return;

_0x401046a1:
	vPortFree($a12);
	return;
}

/* <wDevDisableRx+0x108> */
static void _0x40104700()
{
	uint32 a1_0;	/* $a1 + 0 */

	excw();
	$a0 = ((volatile uint32 *)0x3ff20e00)[191];

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

	$a0 = ((volatile uint32 **)0x3ffe82a0)[0];

	if (((volatile uint16 *)$a0)[0] < 2)
		goto _0x40104794;

	((volatile uint32 *)0x3ff20a00)[137] = 4;

	$a2 = ((volatile uint32 *)0x3ff1fe00)[142] & 0xff;

	if (($a2 > 65) && !($a2 & (1 << 7)))
		goto _0x40104794;

	a1_0 = ((uint8 ***)$a0)[2][1];
	$a13 = $a2 = pvPortMalloc(12);

	if ($a2 == NULL)
		goto _0x40104794;

	ets_memcpy($a2, a1_0, 12);
	
	if (($a2 = chm_get_current_channel()) == NULL) {
		ets_printf("%s %u\n", "wdev.c", 1922);
		while (1);
	}

	((uint8 *)$a13)[10] = (((uint8 *)$a13)[10] & 0xf0) | (((uint8 *)$a2)[6] & 0x0f);

	if (((uint8 *)$a13)[1] >> 6 == 0)
		goto _0x40104829;

	$a7 = ((uint8 *)$a13)[4];
	$a0 = $a7 & 0x7f;
	$a7 >>= 7;

	if ($a7 == 0 && $a0 < 8)
		goto _0x40104829;

	if ($a0 >= 32)
		goto _0x40104829;

	if (pp_post2(32, 9, $a13) != 1)
		goto _0x40104794;

	vPortFree($a13);
	goto _0x40104794;

_0x40104829:
	vPortFree($a13);

_0x40104794:
	((volatile uint32 *)0x3ff20a00)[156] |= (1 << 1);
}

/* <wDevDisableRx+0x240> */
static void _0x40104838()
{
	// stub
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
			if (!($a12 & 4))
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

			switch (!(($a15 >> 28) & 0xf)) {
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

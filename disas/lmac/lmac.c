/* 0x401011e4 */
void lmacIsActive()
{
	/* 0x3ffe8e50 = &lmacConfMib - 0x4 */
	if (((uint8 *)&lmacConfMib)[-0x4] < 8)
		return 1;

	return 0;
}

/* 0x40102398 */
void lmacProcessRtsStart(uint8 arg1)
{
	/* 0x3ffe8e50 = &lmacConfMib - 0x4 */
	((uint8 *)&lmacConfMib)[-0x4] = arg1;
}

/* 0x401024cc */
void lmacTxFrame(uint8 *arg1, uint8 arg2)
{
	$a2 = arg1;
	$a3 = arg2;

	$a12 = (uint8 *)&lmacConfMib + 44 + 36 * arg2;
	$a0 = ((uint8 *)&lmacConfMib + 44 + 36 * arg2)[17];

	if ($a0 == 0 || $a0 == 3) {
		((uint8 **)&lmacConfMib + 11 + 9 * arg2)[0] = arg1;

		if (arg1 == NULL) {
			ets_printf("%s %u\n", "lmac.c", 1519);
			while (1);
		}

		$a5 = ((uint16 *)arg1)[10] + ((uint16 *)arg1)[11];
		$a6 = ((uint32 **)arg1)[9];
		$a3 = ((uint32 **)arg1)[9][0];
		$a4 = ((uint16 *)&lmacConfMib)[5];

		if (($a4 < $a5) && !($a3 & (1 << 7))) {
			/* Set bit 14 of $a3, clear bit 18 of $a3.
			   Done in a convoluted way by the compiler. */
			((uint32 **)arg1)[9][0] |= (1 << 14);
			((uint32 **)arg1)[9][0] &= 0xfffbffff;
		}

		/* Again, this this was done in a convoluted way by the compiler,
		   perhaps for optimization or processor quirks...? */
		if ($a3 & (1 << 18)) {
			$a9 = ((uint8 *)&lmacConfMib + 44 + 36 * arg2)[17];

			if ($a9 == 3) {
				if ((((uint8 **)arg1)[9][4] >> 4) >= 3) {
					$a5 = ((uint8 **)arg1)[9][5];
					$a11 = ($a5 & 0x3f) << 6;
					$a10 = (((uint8 **)arg1)[9][6] << 8) | ((uint8 **)arg1)[9][5];
					$a10 &= 0x0000f03f;
					$a10 |= $a11;
					((uint8 **)arg1)[9][5] = $a10;
					((uint8 **)arg1)[9][6] = $a10 >> 8;
					((uint32 **)arg1)[9][0] |= (1 << 14);
					((uint32 **)arg1)[9][0] &= 0xfffbffff;
					((uint8 **)arg1)[9][5] &= 0xc0;
				}
			}
		}

		_0x401012ac($a12, 0);	/* <lmacIsIdle+0xb4> */
	} else if ($a0 == 4) {
		if (arg1 != ((uint8 **)&lmacConfMib + 11 + 9 * arg2)[0]) {
			ets_printf("%s %u\n", "lmac.c", 1514);
			while (1);
		}
	} else {
		ets_printf("%s %u\n", "lmac.c", 1511);
		while (1);
	}

	$a7 = ((uint8 *)$a12)[6];
	$a5 = (1 << $a7) - 1;
	$a4 = ((volatile uint32 *)0x3ff20e00)[17] & $a5;
	((uint8 *)$a12)[17] = 1;
	$a4 &= 0xffff;
	wDev_EnableTransmit(((uint8 *)$a12)[4], ((uint8 *)$a12)[5], $a4);
}

/* 0x40102680 */
void lmacRxDone(void *arg1)
{
	ppEnqueueRxq(arg1);
	pp_post(5);
}

/* 0x40101694 */
void lmacProcessTXStartData(uint8 arg1)
{
	uint8 a5;	/* $a5 */

	a5 = arg1;

	if (arg1 == 10)
		a5 = ((uint8 *)&lmacConfMib)[-0x4];

	if (a5 >= 8) {
		ets_printf("%s %u\n", "lmac.c", 559);
		while (1);
	}

	$a0 = (uint8 *)&lmacConfMib + 44 + 36 * a5;

	if (((uint8 *)$a0)[17] != 1) {
		ets_printf("%s %u\n", "lmac.c", 567);
		while (1);
	}

	((uint8 *)&lmacConfMib)[-0x4] = a5;

	if (arg1 == 10) {
		_0x40101254($a0, ((uint8 *)&lmacConfMib)[40]);	/* <lmacIsIdle+0x5c> */
		((uint32 *)&lmacConfMib)[8] = 0;
	}

	if (((uint32 *)$a0)[0] == 0) {
		ets_printf("%s %u\n", "lmac.c", 575);
		while (1);
	}

	((uint8 *)$a0)[17] = 2;

	_0x40101728($a0);	/* <lmacProcessTXStartData+0x94> */
	lmacProcessCollisions();
}

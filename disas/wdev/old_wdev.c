void _0x40103e64(uint32 arg1)
{
	/* arg1 can be 0 or 1 */
	if (((void **)0x3ffe82a8)[arg1] != NULL)
		((void (*)())((0x3ffe82a8)[arg1]))();
}

void _0x40104700()
{
	// stub
}

void _0x4010462c()
{
	// stub	
}

void _0x40104838()
{
	// stub
}

void _0x40103b90()
{
	// stub
}

void wDev_ProcessFiq()
{
	uint32 a1_0;	/* $a1 + 0 */
	uint32 a1_4;	/* $a1 + 4 */
	uint32 a1_8;	/* $a1 + 8 */
	uint32 a1_32;	/* $a1 + 32 */

_0x40103e9b:
	$a12 = ((volatile uint32 *)0x3ff20a00)[136];

	if ($a12 == 0)
		return;

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
		$a0 = ((volatile uint8 *)0x3ffe9900)[5];
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

			$a2 = ((uint8 **)0x3ffe9900)[2];

			while ($a2) {
				$a13 = $a2;

				while (!(((uint8 *)$a13)[3] & (1 << 6)))
					$a13 = ((uint32 *)$a13)[2];

				_0x40104838();	/* <wDevDisableRx+0x240> */
				((uint32 *)0x3ffe9900)[89] += 1;

				if ($a13 == $a14)
					break;

				$a2 = ((uint32 *)$a13)[2];
			}
		} else {
			goto _0x40104036;
		}
	}
	
_0x40103fce:
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
		} else {
			goto _0x401040cb;
		}
	} else {
_0x40104018:
		if (!($a12 & (1 << 17)))
			goto _0x40104069;

		if (((($a15 >> 12) & 0xf) >= 8) && ((($a15 >> 12) & 0xf) != 10)) {
			ets_printf("%s %u\n", "wdev.c", 1209);
			while (1);
		} else {
			goto _0x40104063;
		}
	}

_0x40104036:
	$a0 = ((volatile uint32 *)0x3ff20e00)[191];
	$a14 = ((volatile uint32 *)0x3ff1fe00)[133];
	$a2 = ((volatile uint32 *)0x2ff20e00)[134];

	if ($a14 == 0) {
		ets_printf("%s %u\n", "wdev.c", 1130);
		while (1);	
	} else {
		goto _0x40104094;
	}

_0x40104063:
	lmacProcessTXStartData();

_0x40104069:
	if ($a12 & (1 << 20))
		lmacProcessCollisions();

	if ($a12 & (1 << 9))
		((uint32 *)0x3ffe9900)[101] += 1;
	else
		goto _0x40103e9b;

	if ($a4 & (1 << 23))
		((uint32 *)0x3ffe9900)[102] += 1;

	goto _0x40103e9b;

_0x40104094:
	if (!(((uint32 *)$a14)[2])) {
		ets_printf("%s %u\n", "wdev.c", 1131);
		while (1);		
	}

	if (!(((uint8 *)$a14)[3] & (1 << 6))) {
		ets_printf("%s %u\n", "wdev.c", 1132);
		while (1);
	} else {
		goto _0x4010411d;
	}

_0x401040cb:
	a1_0 = $a12 & 0x00040000;
	
	if ((!lmacIsActive()) && ($a12 & 0x00040000)) {
		lmacProcessTXStartData($a13) /* $a13 would be result from the loop (while, $a13 == $a14) */
		$a12 &= 0xfffbffff;
		$a11 = $a12 & 0x00040000;
		a1_0 = $a11;
	}

	switch (!(($a15 >> 28) & 0xf)) {
		case 0: /* 0x4010416a */
			lmacProcessTxSuccess($a13, $a14 & 0xff);
			a1_32 = $a13;
			goto _0x40104155;

		case 1: /* 0x4010417a */
			lmacProcessTxRtsError(($a15 >> 16) & 0xff, $a13);
			a1_32 = $a13;
			goto _0x40104155;

		case 2: /* _0x4010418a */
			lmacProcessCtsTimeout($a13);
			a1_32 = $a13;
			goto _0x40104155;

		case 4: /* _0x40104197 */
			lmacProcessTxError(($a15 >> 16) & 0xff);
			a1_32 = $a13;
			goto _0x40104155;

		case 5: /* 0x4010414f */
			a1_32 = $a13;
			lmacProcessAckTimeout();
_0x40104155:
			((uint32 **)0x3ffe9940)[a1_32][12] += 1;
			$a0 = a1_0;
			goto _0x40104018;
	
		default: ets_printf("%s %u\n", "wdev.c", 1202);
			 while (1);
	}

_0x4010411d:
	$a13 = 0x3ffe9900;
	a1_4 = $a0;
	$a13 = ((uint32 *)$a13)[2];
_0x40104124:
	a1_8 = ((uint32 *)$a13)[2];
	_0x40103b90($a13, 1, a1_4, ($a13 == $a14 ? 1 : 0));
	$a5 = 0x3ffe9900;
	$a3 = $a14 - $a13;
	((uint32 *)0x3ffe9900)[89] += 1;
	$a13 = a1_8;

	if ($a3 == 0)
		goto _0x40103fce;

	goto _0x40104124;
}

void wDevDisableRx()
{
	((volatile uint32 *)0x3ff1fe00)[129] &= 0x7fffffff;
}

void wDevEnableRx()
{
	((volatile uint32 *)0x3ff1fe00)[129] |= 0x80000000;
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
	
	((volatile uint32 *)0x3ff20a00)[165] &= (~1);
}

void wdev_set_sniffer_addr()
{
	// stub
}

void wdev_exit_sniffer()
{
	// stub
}

int wifi_promiscuous_enable(uint8 promiscuous)
{
	if (((uint8 *)&user_init_flag)[0] == 0)
		return 0;

	if (wifi_get_opmode() != 1)
		return 0;

	if (promiscuous == 0) {
		if (((uint8 **)&g_ic)[96][100] != 0) {
			wDevDisableRx();
			wdev_exit_sniffer();
			((volatile uint32 *)0x3ff1fe00)[155] |= 1;
			((volatile uint32 *)0x3ff1fe00)[155] |= 2;
			((volatile uint32 *)0x3ff1fe00)[155] |= 4;
			wDev_SetMacAddress(0, (uint8 *)&info + 30);
			((uint8 **)&g_ic)[96][100] = 0;
			wDevEnableRx();
			return 1;
		} else {
			return 1;
		}
	}

	if (((uint8 **)&g_ic)[96][100] == 1)
		return 1;

	wifi_station_disconnect();
	wDevDisableRx();
	((volatile uint32 *)0x3ff1fe00)[155] &= (~1);
	((volatile uint32 *)0x3ff1fe00)[155] &= (~2);
	((volatile uint32 *)0x3ff1fe00)[155] &= (~4);
	((uint8 **)&g_ic)[96][100] = 1;
	wdev_go_sniffer();
	wDevEnableRx();
	return 1;
}

void wifi_set_promiscuous_rx_cb(wifi_promiscuous_cb_t cb)
{
	promiscuous_cb = cb;
}

int ppTxPkt(struct esf_buf *ebuf)
{
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

	if (_0x40246a94(ebuf) == 1)
		goto _0x4024603b;

	ets_intr_lock();
	$a2 = *(uint32 *)((uint8 *)ebuf + 28);
	$a3 = *(uint32 *)((uint8 *)ebuf + 36);
	rcGetSched($a2, $a3);
	ets_intr_unlock();

	ppCalFrameTimes(ebuf);
	_0x40102e08(ebuf);

	$a5 = 0;
	/* 0x40245fb7 */
}

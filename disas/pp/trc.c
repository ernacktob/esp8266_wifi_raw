/* 0x40103478 */
void rcGetSched(void *arg1, struct _ebuf_sub1 *ep)
{
	struct _ebuf_sub1 *a1_8;

	$a12 = arg1;
	$a2 = ep;

	if (arg1 == NULL || *(uint32 *)((uint8 *)arg1 + 96) == 0) {
		$a3 = ep->data[0];
		$a0 = *(uint32 *)0x40103464;	/* 0x3ffe80d4 */

		if (!($a3 & 0x01))
			$a0 = *(uint32 *)0x4010346c;	/* 0x3ffe8194 */
	} else {
		$a0 = *(uint32 *)((uint8 *)ep + 0);
		$a3 = $a0 << 6;

		if ($a0 & (1 << 7)) {
			$a0 = *(uint32 *)((uint8 *)arg1 + 100);
		} else {
			a1_8 = $a2;

			if ($a0 & (1 << 9)) {
				$a2 = arg1;
				_0x4010377c(arg1);	/* <rcGetRate+0x258> */
				$a2 = a1_8;
				$a0 = *(uint32 *)((uint8 *)arg1 + 96);
				$a4 = *(uint32 *)(ep->data + 0);
				$a5 = 0x00001000;
				$a3 = $a4 & 0x3f;
				$a4 >>= 6;
				$a4 |= $a5;
				$a4 <<= 6;
				$a3 |= $a4;
				*(uint32 *)(ep->data + 0) = $a3;
			} else {
				$a5 = 0x00200800;

				if (!($a5 & $a3)) {
					$a0 = *(uint32 *)((uint8 *)arg1 + 100);
				} else {
					$a6 = *(uint32 *)((uint8 *)arg1 + 92);
					$a0 = *(uint32 *)0x40103474;	/* 0x3ffe8070 */

					if ($a6 == 0)
						$a0 = *(uint32 *)((uint8 *)arg1 + 100);
				}
			}
		}
	}

	a1_8 = $a2;
	*(uint32 *)(ep->data + 20) = $a0;

	if ($a0 == 0) {
		os_printf_plus("==dl\n");
		$a2 = a1_8;
	}

	if (arg1 == NULL) {
		$a0 = *(uint32 *)(ep->data + 20);
		$a0 = *(uint8 *)((uint8 *)$a0 + 0);
	} else {
		$a4 = *(uint8 *)((uint8 *)arg1 + 20);
		$a0 = *(uint8 *)((uint8 *)arg1 + 7);

		if (!($a4 & (1 << 6))) {
			$a0 = *(uint32 *)(ep->data + 20);
			$a0 = *(uint8 *)((uint8 *)$a0 + 0);
		} else {
			$a5 = *(uint32 *)(ep->data + 0);

			if (!($a5 & (1 << 9))) {
				$a0 = *(uint32 *)(ep->data + 20);
				$a0 = *(uint8 *)((uint8 *)$a0 + 0);
			}
		}
	}

	ep->data[8] = $a0;
}

static void _0x4010377c(void *arg1)
{

}

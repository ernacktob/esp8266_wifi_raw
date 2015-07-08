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

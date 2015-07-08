bool wifi_set_broadcast_if(uint8 interface)
{
	if (interface == 0 || interface >= 4) {
		os_printf_plus("error parameter\n");
		return false;
	}

	switch (interface) {
		case 1:
			if ((wifi_get_opmode() != 1) && (wifi_get_opmode() != 3))
				return false;

			if (((uint8 **)g_ic)[4] == NULL)
				return false;

			netif_set_default(((struct netif **)g_ic)[4]);
			break;

		case 2:
			if ((wifi_get_opmode() != 2) && (wifi_get_opmode() != 3))
				return false;

			if (((uint8 **)g_ic)[5] == NULL)
				return false;

			netif_set_default(((struct netif **)g_ic)[4]);
			break;

		case 3:
			if (wifi_get_opmode() != 3)
				return false;

			break;

		default:
			return true;
	}

	((uint8 *)default_interface)[128] = mode;
	return true;	
}

int func1(int val)
{
	if (!pm_is_open())
		return 0;

	if (((uint8 *)default_interface)[0] == 0) {
		__func2(promiscuous_cb, 0x4024081c, 0);
		((uint8 *)default_interface)[0] = 1;
	}

	if (!pm_is_waked() || (((uint8 *)user_init_flag)[60] == 1)) {
		if (((uint8 *)user_init_flag)[60] == 0) {
			pm_post(1);
			__func3(((uint8 *)user_init_flag)[24]);
			ets_timer_arm_new(((uint8 *)user_init_flag)[24], 10, 0, 1);
			((uint8 *)user_init_flag)[60] = 1;
		}

		((uint8 *)user_init_flag)[61] = (((uint8 *)user_init_flag)[61] + 1) & 0xff;

		if (((uint8 *)user_init_flag)[61] > 10) {
			os_printf_plus("DEFERRED FUNC NUMBER IS BIGGER THAN 10\n");
			((uint8 *)user_init_flag)[61] = 10;
		}

		((uint8 *)user_init_flag)[62] += 9;

		if (((uint8 *)user_init_flag)[62] >= 10) {
			*(((uint8 *)user_init_flag)[62] + ((uint8 *)user_init_flag)[175] + 122) = val;
			return -1;
		}

		*(((uint8 *)user_init_flag)[62] + ((uint8 *)user_init_flag)[175] + 132) = val;
		return -1;
	}

	return 0;
}

bool wifi_set_opmode_deeper(uint8 mode, uint8 a)
{
	int res;
	int stuff;

	if (mode >= 4)
		return false;

	if (((uint8 *)g_ic)[468] == 1)
		return false;

	res = func(5);

	if (res == -1) {
		if (((uint8 *)g_ic)[480] == mode)
			return true;

		stuff = (uint8 *)g_ic + 352;

		if (((uint8 *)user_init_flag)[0] != 1) {
			*((uint8 *)(stuff + 128)) = a;
			wifi_param_save_protect((uint8 *)g_ic + 472);
		}

		return true;
	}

	*(res + 130) = mode;
	return true;
}

bool wifi_set_opmode(uint8 mode)
{
	return wifi_set_opmode_deeper(mode, 1);
}

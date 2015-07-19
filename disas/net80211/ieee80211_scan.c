/* 0x40254704 */
void ICACHE_FLASH_ATTR ieee80211_scan_attach(struct ieee80211com *ic)
{
	*(uint32 *)((uint8 *)ic + 4) = &gScanStruct;
	*(uint32 *)((uint8 *)&gScanStruct + 52) = 120;
	*(uint32 *)((uint8 *)&gScanStruct + 56) = 120;
	*(uint32 *)((uint8 *)&gScanStruct + 60) = 0x168;
	*(uint8 *)((uint8 *)&gScanStruct + 155) = 0xff;

	ets_timer_setfn((uint8 *)&gScanStruct + 8, _0x40254df0, 0);
	ets_timer_setfn((uint8 *)&gScanStruct + 28, _0x40254de0, 0);

	*(uint32 *)((uint8 *)ic + 0x1d0) = NULL;
	*(uint32 *)((uint8 *)ic + 0x1d4) = (uint8 *)ic + 0x1d0;
}

/* 0x402502cc */
void ICACHE_FLASH_ATTR ieee80211_ifattach(struct ieee80211com *ic)
{
	_0x4025023c(ic);

	*(uint16 *)((uint8 *)ic + 40) = 100;	/* ic->ic_bintval or ic->ic_txpowlimit */
	ieee80211_crypto_attach(ic);
	ieee80211_proto_attach(ic);

	$a4 = (uint8 *)ic + 0x500;
	$a2 = *(uint32 *)((uint8 *)$a4 + 44);
	*(uint32 *)((uint8 *)$a4 + 28) = 0;

	if ($a2 == 3)
		ieee80211_ht_attach(ic);

	chm_init(&g_ic);
	ieee80211_scan_attach(ic);
}

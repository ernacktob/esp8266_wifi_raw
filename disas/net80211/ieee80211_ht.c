/* 0x402517e0 */
void ICACHE_FLASH_ATTR ieee80211_ht_attach(struct ieee80211com *ic)
{
	$a4 = (uint8 *)ic + 0x4a6;
	$a5 = (uint8 *)ic + 0x2bc;

	*(uint32 *)((uint8 *)$a5 + 0x260) = 0x04080000;
	$a5 += 0x200;

	*(uint16 *)((uint8 *)$a5 + 102) = 28;
	*(uint16 *)((uint8 *)$a5 + 104) = 0;
	*(uint16 *)((uint8 *)$a5 + 100) = 0x110c;
	*(uint8 *)((uint8 *)$a4 + 128) = 1;
	*(uint8 *)((uint8 *)$a4 + 129) = 1;

	ieee80211_recv_action_register(3, 0, _0x40252064);
	ieee80211_send_action_register(3, 1, _0x402520d8);
}

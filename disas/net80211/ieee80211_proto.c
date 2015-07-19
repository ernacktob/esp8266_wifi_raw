/* 0x402545a0 */
void ICACHE_FLASH_ATTR ieee80211_proto_attach(struct ieee80211com *ic)
{
	ic->ic_protmode = IEEE80211_PROT_CTSONLY;
	*(uint32 *)((uint8 *)ic + 28) |= 0x00040000;
	ppRegisterTxCallback(ieee80211_tx_mgt_cb, 2);	/* same as ieee80211_add_callback ? */
}

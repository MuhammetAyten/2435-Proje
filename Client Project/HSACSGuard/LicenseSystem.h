#pragma once
#include "stdafx.h"
#include "HSACSEngine.h"
// IPLISANS

static string serverAdlari[] = { xorstr("SunucuName") };
// gün, ay, yýl
static uint32 lisansTarih[] = { 01, 10, 2026 };
// iki lisans þekli de ayný anda çalýþýr
//static string ipLisanslari[] = { xorstr("127.0.0.1"), xorstr("185.160.30.104"), xorstr("193.169.250.183") };
static std::string ipLisanslari[] = { xorstr("127.0.0.1"), xorstr("212.68.34.22"), xorstr("213.142.151.42"), xorstr("213.142.151.55"), xorstr("46.31.77.26"), xorstr("91.151.88.109"), xorstr("217.195.197.104"), xorstr("213.238.177.211"), xorstr("213.142.151.153"), xorstr("80.253.246.23"), xorstr("188.132.197.51"), xorstr("95.214.177.105"), xorstr("46.31.77.170"), xorstr("45.94.4.251"), xorstr("5.178.111.194"), xorstr("212.68.34.62"), xorstr("213.142.148.63"), xorstr("213.142.151.161"), xorstr("45.11.96.210"), xorstr("5.178.98.39"), xorstr("5.180.155.62"), xorstr("141.95.10.235"), xorstr("45.141.151.7"), xorstr("45.147.45.133"), xorstr("213.142.148.24"), xorstr("45.94.4.155"), xorstr("213.142.151.180"), xorstr("91.151.88.253"), xorstr("46.31.77.130"), xorstr("188.132.197.134"), xorstr("195.26.242.135")};
// x den öncesine bakar
static string subnetLisanlar[] = { xorstr("127.0.0.xxx"), xorstr("213.142.151.xxx"), xorstr("212.68.34.xxx") , xorstr("46.31.77.xxx"), xorstr("192.168.1.xxx"), xorstr("91.151.88.xxx"), xorstr("217.195.197.xxx"), xorstr("213.238.177.xxx"), xorstr("213.142.151.xxx"), xorstr("80.253.246.xxx"), xorstr("188.132.197.xxx"), xorstr("95.214.177.xxx"), xorstr("46.31.77.xxx"), xorstr("45.94.4.xxx"), xorstr("5.178.111.xxx"), xorstr("212.68.34.xxx"), xorstr("213.142.148.xxx"), xorstr("45.11.96.xxx"), xorstr("5.178.98.39.xxx"), xorstr("5.180.155.xxx") , xorstr("141.95.10.xxx"), xorstr("45.141.151.xxx"), xorstr("45.147.45.xxx"), xorstr("195.26.242.xxx")};  //999 olan yere ip eklenecek son noktadan sonra xx koyulmalý. Örneðin : 127.0.0.xx

extern DWORD WINAPI LicenseEngine(HSACSEngine* e);
extern bool IsLicensed(std::string ip);



//	188.132.151.63	Source Test
//	78.135.90.245	Sinan Pk Online
//	78.135.90.249	Muhammet
//	78.135.90.251	Fazlý Beramus Online
//	78.135.90.252	MEhmet Myko Zone
//	78.135.90.253	v2 Test Ömer
//---------------------------------------------------------------------------

#include "stdafx.h"

#pragma hdrstop

#include "TERASIC_PCIE.h"

//---------------------------------------------------------------------------

//#pragma package(smart_init)

#if 0
TERASIC_PCIE::TERASIC_PCIE()
{
}

TERASIC_PCIE::~TERASIC_PCIE()
{
}

bool TERASIC_PCIE::LoadDll(void) {
	bool bSuccess = false;

	//check OS 
	BOOL bIsWow64 = FALSE;
	typedef BOOL(WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
	LPFN_ISWOW64PROCESS fnIsWow64Process;

	fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle("kernel32"), "IsWow64Process");
	if (NULL != fnIsWow64Process)
	{
		fnIsWow64Process(GetCurrentProcess(), &bIsWow64);
	}

	if (bIsWow64)
	{
		m_hDLL = ::LoadLibrary("TERASIC_PCIEx64.DLL");
	}
	else
	{
		m_hDLL = ::LoadLibrary("TERASIC_PCIE.dll");
	}

	if (m_hDLL) {
		m_PCIE_IsDriverAvailable = (LP_PCIE_IsDriverAvailable)::GetProcAddress(m_hDLL, "PCIE_IsDriverAvailable");
		m_PCIE_ScanCard = (LP_PCIE_ScanCard)::GetProcAddress(m_hDLL, "PCIE_ScanCard");
		m_PCIE_Open = (LP_PCIE_Open)::GetProcAddress(m_hDLL, "PCIE_Open");
		m_PCIE_Close = (LP_PCIE_Close)::GetProcAddress(m_hDLL, "PCIE_Close");
		m_PCIE_Read32 = (LP_PCIE_Read32)::GetProcAddress(m_hDLL, "PCIE_Read32");
		m_PCIE_Write32 = (LP_PCIE_Write32)::GetProcAddress(m_hDLL, "PCIE_Write32");
		m_PCIE_DmaWrite = (LP_PCIE_DmaWrite)::GetProcAddress(m_hDLL, "PCIE_DmaWrite");
		m_PCIE_DmaRead = (LP_PCIE_DmaRead)::GetProcAddress(m_hDLL, "PCIE_DmaRead");

		if (!m_PCIE_IsDriverAvailable || !m_PCIE_ScanCard ||
			!m_PCIE_Open || !m_PCIE_Close || !m_PCIE_Read32 || !m_PCIE_Write32 ||
			!m_PCIE_DmaWrite || !m_PCIE_DmaRead) {
			//strMsg.printf("load TERASIC_DOWNLOAD.DLL fail. Cannot find all of desired API");
			//MessageDlg(strMsg, mtError , TMsgDlgButtons() << mbOK, 0);
		}
		else {
			bSuccess = true;
		}
	}
	else {
		//strMsg.printf("fail to load dll - %s", strDLL.c_str());
		//MessageDlg(strMsg, mtError , TMsgDlgButtons() << mbOK, 0);
	}

	if (!bSuccess && m_hDLL) {
		FreeDll();
	}
	return true;
}

void __fastcall TERASIC_PCIE::FreeDll(void) {
	if (m_hDLL) {
		::FreeLibrary(m_hDLL);
		m_hDLL = NULL;
	}
}


bool __fastcall TERASIC_PCIE::IsDriverAvailable(void) {
	if (m_hDLL && m_PCIE_IsDriverAvailable) {
		return m_PCIE_IsDriverAvailable();
	}
	return false;
}


bool __fastcall TERASIC_PCIE::ScanCard(WORD wVendorID, WORD wDeviceID, DWORD *pdwDeviceNum, TERASIC_PCIE_CONFIG szConfigList[]) {
	if (!m_hDLL || !m_PCIE_ScanCard)
		return false;
	return m_PCIE_ScanCard(wVendorID, wDeviceID, pdwDeviceNum, szConfigList);
}

bool __fastcall TERASIC_PCIE::Open(WORD wVendorID, WORD wDeviceID, WORD wCardIndex) {
	if (!m_hDLL || !m_PCIE_Open)
		return false;
	m_hPCIe = m_PCIE_Open(wVendorID, wDeviceID, wCardIndex);
	if (m_hPCIe)
		return true;
	return false;
}

void __fastcall TERASIC_PCIE::Close(void) {
	if (m_hDLL && m_hPCIe && m_PCIE_Close) {
		m_PCIE_Close(m_hPCIe);
		m_hPCIe = NULL;
	}
}

bool __fastcall TERASIC_PCIE::Read32(TERASIC_PCIE_BAR PcieBar, TERASIC_PCIE_ADDRESS PcieAddress, TERASIC_U32 *pData32) {
	if (!m_hDLL || !m_hPCIe)
		return false;
	return m_PCIE_Read32(m_hPCIe, PcieBar, PcieAddress, pData32);
}

bool __fastcall TERASIC_PCIE::Write32(TERASIC_PCIE_BAR PcieBar, TERASIC_PCIE_ADDRESS PcieAddress, TERASIC_U32 Data32) {
	if (!m_hDLL || !m_hPCIe)
		return false;
	return m_PCIE_Write32(m_hPCIe, PcieBar, PcieAddress, Data32);
}

bool __fastcall TERASIC_PCIE::DmaWrite(TERASIC_PCIE_ADDRESS PcieAddress, TERASIC_U32 DataByteLen, void *pData) {
	if (!m_hDLL || !m_hPCIe)
		return false;
	return m_PCIE_DmaWrite(m_hPCIe, PcieAddress, DataByteLen, pData);
}

bool __fastcall TERASIC_PCIE::DmaRead(TERASIC_PCIE_ADDRESS PcieAddress, TERASIC_U32 DataByteLen, void *pBuffer) {
	if (!m_hDLL || !m_hPCIe)
		return false;
	return m_PCIE_DmaRead(m_hPCIe, PcieAddress, DataByteLen, pBuffer);
}


#endif

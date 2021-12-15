#pragma once
#pragma warning (disable: 4068)

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <setupapi.h>
#include <devguid.h>
#include <regstr.h>
#include <bluetoothleapis.h>

#pragma comment(lib, "SetupAPI")
#pragma comment(lib, "BluetoothApis.lib")

int ConnBle();

class WinBluetooth
{
public:
	WinBluetooth();
	~WinBluetooth();

public:
	int ConnectBLEDevice();
	int SendBLEData(unsigned char* data);
	int ScanBLEDevices();
	int CloseBLE();


private:
	HANDLE GetBLEHandle(__in GUID AGuid);

public:
	HANDLE m_hLEDevice							= INVALID_HANDLE_VALUE;
	PBTH_LE_GATT_CHARACTERISTIC m_WriteGattChar = nullptr;
	PBTH_LE_GATT_CHARACTERISTIC m_ReadGattChar	= nullptr;
	HDEVINFO					m_hDI;
	SP_DEVICE_INTERFACE_DATA	m_did;
	SP_DEVINFO_DATA				m_dd;
	GUID						m_BluetoothInterfaceGUID;
	HANDLE						m_hComm				= NULL;
	USHORT						m_serviceBufferCount;
	PBTH_LE_GATT_SERVICE		m_pServiceBuffer	= nullptr;
	PBTH_LE_GATT_CHARACTERISTIC_VALUE  m_newValue	= nullptr;
	PBTH_LE_GATT_CHARACTERISTIC m_pCharBuffer		= nullptr;
	BLUETOOTH_GATT_EVENT_HANDLE m_EventHandle;
};

#endif
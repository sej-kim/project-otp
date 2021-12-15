#include "EventManager.h"
#include "WinBluetooth.h"

#define MOSS_DEVICE_UUID	"{6e400001-b5a3-f393-e0a9-e50e24dcca9e}"
#define MOSS_DEVICE_UUID_TX "{6e400002-b5a3-f393-e0a9-e50e24dcca9e}"
#define MOSS_DEVICE_UUID_RX "{6e400003-b5a3-f393-e0a9-e50e24dcca9e}"

WinBluetooth* gs_WinBle = nullptr;
std::string m_strBuffer;
VOID HandleBLENotification(BTH_LE_GATT_EVENT_TYPE EventType, PVOID EventOutParameter, PVOID Context)
{
	printf("notification obtained ");
	PBLUETOOTH_GATT_VALUE_CHANGED_EVENT ValueChangedEventParameters = (PBLUETOOTH_GATT_VALUE_CHANGED_EVENT)EventOutParameter;

	HRESULT hr;
	if (0 >= ValueChangedEventParameters->CharacteristicValue->DataSize) 
	{
		hr = E_FAIL;
		printf("datasize 0\n");
	}
	else 
	{
		m_strBuffer = std::string(reinterpret_cast<char const*>(ValueChangedEventParameters->CharacteristicValue->Data), 19);
		EventManager::GetInstance()->DataIn(m_strBuffer);
	}

	hr = BluetoothGATTUnregisterEvent(gs_WinBle->m_EventHandle, BLUETOOTH_GATT_FLAG_NONE);
	if (S_OK != hr)
	{
		cocos2d::log("BluetoothGATTGetCharacteristicValue - Actual Data %d", hr);
	}

	//cocos2d::log("Setting Notification for ServiceHandle %d", currGattChar->ServiceHandle);
	BTH_LE_GATT_EVENT_TYPE EventType1 = CharacteristicValueChangedEvent;

	BLUETOOTH_GATT_VALUE_CHANGED_EVENT_REGISTRATION EventParameterIn;
	EventParameterIn.Characteristics[0] = *gs_WinBle->m_ReadGattChar;
	EventParameterIn.NumCharacteristics = 1;
	hr = BluetoothGATTRegisterEvent(
		gs_WinBle->m_hLEDevice,
		EventType1,
		&EventParameterIn,
		HandleBLENotification,
		NULL,
		&gs_WinBle->m_EventHandle,
		BLUETOOTH_GATT_FLAG_NONE);

	if (S_OK != hr)
	{
		cocos2d::log("BluetoothGATTRegisterEvent - Actual Data %d", hr);
	}
}

WinBluetooth::WinBluetooth()
	: m_serviceBufferCount( 0 )
{
	m_newValue = (BTH_LE_GATT_CHARACTERISTIC_VALUE*)malloc(19);
	gs_WinBle = this;
}

WinBluetooth::~WinBluetooth()
{
	CC_SAFE_DELETE(m_newValue);
	CC_SAFE_DELETE(m_pServiceBuffer);
	CC_SAFE_DELETE_ARRAY(m_pCharBuffer);

	CloseBLE();
}

HANDLE WinBluetooth::GetBLEHandle(__in GUID AGuid)
{
	m_BluetoothInterfaceGUID = AGuid;

	m_hDI = SetupDiGetClassDevs(&m_BluetoothInterfaceGUID, NULL, NULL, DIGCF_DEVICEINTERFACE | DIGCF_PRESENT);

	if (m_hDI == INVALID_HANDLE_VALUE) return NULL;

	m_did.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
	m_dd.cbSize = sizeof(SP_DEVINFO_DATA);

	for (DWORD i = 0; SetupDiEnumDeviceInterfaces(m_hDI, NULL, &m_BluetoothInterfaceGUID, i, &m_did); i++)
	{
		SP_DEVICE_INTERFACE_DETAIL_DATA DeviceInterfaceDetailData;

		DeviceInterfaceDetailData.cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

		DWORD size = 0;
		if (!SetupDiGetDeviceInterfaceDetail(m_hDI, &m_did, NULL, 0, &size, 0))
		{
			int err = GetLastError();

			if (err == ERROR_NO_MORE_ITEMS) break;

			PSP_DEVICE_INTERFACE_DETAIL_DATA pInterfaceDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)GlobalAlloc(GPTR, size);

			pInterfaceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

			if (!SetupDiGetDeviceInterfaceDetail(m_hDI, &m_did, pInterfaceDetailData, size, &size, &m_dd))
				break;

			m_hComm = CreateFile(
				pInterfaceDetailData->DevicePath,
				GENERIC_WRITE | GENERIC_READ,
				FILE_SHARE_READ | FILE_SHARE_WRITE,
				NULL,
				OPEN_EXISTING,
				0,
				NULL);

			GlobalFree(pInterfaceDetailData);
		}
	}

	SetupDiDestroyDeviceInfoList(m_hDI);

	return m_hComm;
}

int WinBluetooth::ConnectBLEDevice()
{
	// Step 1: find the BLE device handle from its GUID
	GUID AGuid, aGuidTX, aGuidRX;
	// GUID can be constructed from "{xxx....}" string using CLSID
	CLSIDFromString(TEXT(MOSS_DEVICE_UUID), &AGuid);
	CLSIDFromString(TEXT(MOSS_DEVICE_UUID_TX), &aGuidTX);
	CLSIDFromString(TEXT(MOSS_DEVICE_UUID_RX), &aGuidRX);
	// Get the handle 
	m_hLEDevice = GetBLEHandle(AGuid);

	// Step 2: Get a list of services that the device advertises
	// first send 0, NULL as the parameters to BluetoothGATTServices inorder to get the number of
	// services in serviceBufferCount
	
	////////////////////////////////////////////////////////////////////////////
	// Determine Services Buffer Size
	////////////////////////////////////////////////////////////////////////////
	HRESULT hr = BluetoothGATTGetServices(
		m_hLEDevice,
		0,
		NULL,
		&m_serviceBufferCount,
		BLUETOOTH_GATT_FLAG_NONE);

	if (HRESULT_FROM_WIN32(ERROR_MORE_DATA) != hr) 
	{
		cocos2d::log("BluetoothGATTGetServices - Buffer Size %d", hr);
	}

	m_pServiceBuffer = (PBTH_LE_GATT_SERVICE)malloc(sizeof(BTH_LE_GATT_SERVICE) * m_serviceBufferCount);
	if (NULL == m_pServiceBuffer)
	{
		cocos2d::log("pServiceBuffer out of memory");
	}
	else 
	{
		RtlZeroMemory(m_pServiceBuffer, sizeof(BTH_LE_GATT_SERVICE) * m_serviceBufferCount);
	}

	////////////////////////////////////////////////////////////////////////////
	// Retrieve Services
	////////////////////////////////////////////////////////////////////////////

	USHORT numServices;
	hr = BluetoothGATTGetServices(
		m_hLEDevice,
		m_serviceBufferCount,
		m_pServiceBuffer,
		&numServices,
		BLUETOOTH_GATT_FLAG_NONE);

	if (S_OK != hr) 
	{
		cocos2d::log("BluetoothGATTGetServices - Buffer Size %d", hr);
	}

	// Step 3: now get the list of charactersitics. note how the pServiceBuffer is required from step 2
	////////////////////////////////////////////////////////////////////////////
	// Determine Characteristic Buffer Size
	////////////////////////////////////////////////////////////////////////////

	USHORT charBufferSize;
	hr = BluetoothGATTGetCharacteristics(
		m_hLEDevice,
		m_pServiceBuffer,
		0,
		NULL,
		&charBufferSize,
		BLUETOOTH_GATT_FLAG_NONE);

	if (HRESULT_FROM_WIN32(ERROR_MORE_DATA) != hr) 
	{
		cocos2d::log("BluetoothGATTGetCharacteristics - Buffer Size %d", hr);
	}

	if (charBufferSize > 0) 
	{
		m_pCharBuffer = (PBTH_LE_GATT_CHARACTERISTIC)malloc(charBufferSize * sizeof(BTH_LE_GATT_CHARACTERISTIC));

		if (NULL == m_pCharBuffer)
		{
			cocos2d::log("pCharBuffer out of memory");
		}
		else 
		{
			RtlZeroMemory(m_pCharBuffer, charBufferSize * sizeof(BTH_LE_GATT_CHARACTERISTIC));
		}

		////////////////////////////////////////////////////////////////////////////
		// Retrieve Characteristics
		////////////////////////////////////////////////////////////////////////////
		USHORT numChars;
		hr = BluetoothGATTGetCharacteristics(
			m_hLEDevice,
			m_pServiceBuffer,
			charBufferSize,
			m_pCharBuffer,
			&numChars,
			BLUETOOTH_GATT_FLAG_NONE);

		if (S_OK != hr)
		{
			cocos2d::log("BluetoothGATTGetCharacteristics - Actual Data %d", hr);
		}

		if (numChars != charBufferSize)
		{
			cocos2d::log("buffer size and buffer size actual size mismatch");
		}
	}

	PBTH_LE_GATT_CHARACTERISTIC currGattChar;
	for (int ii = 0; ii <charBufferSize; ii++)
	{
		currGattChar = &m_pCharBuffer[ii];
		PBTH_LE_GATT_CHARACTERISTIC_VALUE pCharValueBuffer = NULL;

		if (!currGattChar->CharacteristicUuid.IsShortUuid && currGattChar->CharacteristicUuid.Value.LongUuid == aGuidTX)
		{
			m_WriteGattChar = currGattChar;
		}

		if (!currGattChar->CharacteristicUuid.IsShortUuid && currGattChar->CharacteristicUuid.Value.LongUuid == aGuidRX)
		{
			m_ReadGattChar = currGattChar;


			///////////////////////////////////////////////////////////////////////////
			// Determine Descriptor Buffer Size
			////////////////////////////////////////////////////////////////////////////
			USHORT descriptorBufferSize;
			hr = BluetoothGATTGetDescriptors(
				m_hLEDevice,
				currGattChar,
				0,
				NULL,
				&descriptorBufferSize,
				BLUETOOTH_GATT_FLAG_NONE);

			if (HRESULT_FROM_WIN32(ERROR_MORE_DATA) != hr)
			{
				cocos2d::log("BluetoothGATTGetDescriptors - Buffer Size %d", hr);
			}

			PBTH_LE_GATT_DESCRIPTOR pDescriptorBuffer;
			if (descriptorBufferSize > 0)
			{
				pDescriptorBuffer = (PBTH_LE_GATT_DESCRIPTOR)
					malloc(descriptorBufferSize
					* sizeof(BTH_LE_GATT_DESCRIPTOR));

				if (NULL == pDescriptorBuffer)
				{
					cocos2d::log("pDescriptorBuffer out of memory");
				}
				else
				{
					RtlZeroMemory(pDescriptorBuffer, descriptorBufferSize);
				}

				////////////////////////////////////////////////////////////////////////////
				// Retrieve Descriptors
				////////////////////////////////////////////////////////////////////////////

				USHORT numDescriptors;
				hr = BluetoothGATTGetDescriptors(
					m_hLEDevice,
					currGattChar,
					descriptorBufferSize,
					pDescriptorBuffer,
					&numDescriptors,
					BLUETOOTH_GATT_FLAG_NONE);

				if (S_OK != hr)
				{
					cocos2d::log("BluetoothGATTGetDescriptors - Actual Data %d", hr);
				}

				if (numDescriptors != descriptorBufferSize)
				{
					cocos2d::log("buffer size and buffer size actual size mismatch");
				}

				for (int kk = 0; kk < numDescriptors; kk++) {
					PBTH_LE_GATT_DESCRIPTOR  currGattDescriptor = &pDescriptorBuffer[kk];
					////////////////////////////////////////////////////////////////////////////
					// Determine Descriptor Value Buffer Size
					////////////////////////////////////////////////////////////////////////////
					USHORT descValueDataSize;
					hr = BluetoothGATTGetDescriptorValue(
						m_hLEDevice,
						currGattDescriptor,
						0,
						NULL,
						&descValueDataSize,
						BLUETOOTH_GATT_FLAG_NONE);

					if (HRESULT_FROM_WIN32(ERROR_MORE_DATA) != hr)
					{
						cocos2d::log("BluetoothGATTGetDescriptorValue - Buffer Size %d", hr);
					}

					PBTH_LE_GATT_DESCRIPTOR_VALUE pDescValueBuffer = (PBTH_LE_GATT_DESCRIPTOR_VALUE)malloc(descValueDataSize);

					if (NULL == pDescValueBuffer)
					{
						cocos2d::log("pDescValueBuffer out of memory");
					}
					else
					{
						RtlZeroMemory(pDescValueBuffer, descValueDataSize);
					}

					////////////////////////////////////////////////////////////////////////////
					// Retrieve the Descriptor Value
					////////////////////////////////////////////////////////////////////////////

					hr = BluetoothGATTGetDescriptorValue(
						m_hLEDevice,
						currGattDescriptor,
						(ULONG)descValueDataSize,
						pDescValueBuffer,
						NULL,
						BLUETOOTH_GATT_FLAG_NONE);
					if (S_OK != hr)
					{
						cocos2d::log("BluetoothGATTGetDescriptorValue - Actual Data %d", hr);
					}

					// you may also get a descriptor that is read (and not notify) andi am guessing the attribute handle is out of limits
					// we set all descriptors that are notifiable to notify us via IsSubstcibeToNotification
					if (currGattDescriptor->AttributeHandle < 255) {

						BTH_LE_GATT_DESCRIPTOR_VALUE newValue;

						RtlZeroMemory(&newValue, sizeof(newValue));

						newValue.DescriptorType = ClientCharacteristicConfiguration;
						newValue.ClientCharacteristicConfiguration.IsSubscribeToNotification = TRUE;

						hr = BluetoothGATTSetDescriptorValue(
							m_hLEDevice,
							currGattDescriptor,
							&newValue,
							BLUETOOTH_GATT_FLAG_NONE);
						if (S_OK != hr) {
							cocos2d::log("BluetoothGATTGetDescriptorValue - Actual Data %d", hr);
						}
					}

				}
			}

			// set the appropriate callback function when the descriptor change value
			//BLUETOOTH_GATT_EVENT_HANDLE EventHandle;
			if (currGattChar->IsNotifiable)
			{
				cocos2d::log("Setting Notification for ServiceHandle %d", currGattChar->ServiceHandle);
				BTH_LE_GATT_EVENT_TYPE EventType = CharacteristicValueChangedEvent;

				BLUETOOTH_GATT_VALUE_CHANGED_EVENT_REGISTRATION EventParameterIn;
				EventParameterIn.Characteristics[0] = *currGattChar;
				EventParameterIn.NumCharacteristics = 1;
				hr = BluetoothGATTRegisterEvent(
					m_hLEDevice,
					EventType,
					&EventParameterIn,
					HandleBLENotification,
					NULL,
					&m_EventHandle,
					BLUETOOTH_GATT_FLAG_NONE);

				if (S_OK != hr)
				{
					cocos2d::log("BluetoothGATTRegisterEvent - Actual Data %d", hr);
				}

			}
		}
	}

	EventManager::GetInstance()->StatusChanged(2);
	return 0;
}

struct _BTH_LE_GATT_CHARACTERISTIC_VALUE1 {
	ULONG DataSize;
	UCHAR* Data;
};

int WinBluetooth::SendBLEData(unsigned char* data)
{
	int nLen = 19;
	RtlZeroMemory(m_newValue, (sizeof(BTH_LE_GATT_CHARACTERISTIC_VALUE) + nLen));

	m_newValue->DataSize = nLen;
	for (int i_1 = 0; i_1 < nLen; ++i_1)
	{
		m_newValue->Data[i_1] = data[i_1];
	}

	HRESULT hr = BluetoothGATTSetCharacteristicValue(m_hLEDevice,
		m_WriteGattChar,
		(BTH_LE_GATT_CHARACTERISTIC_VALUE*)m_newValue,
		NULL,
		BLUETOOTH_GATT_FLAG_NONE);

	if (S_OK != hr)
	{
		cocos2d::log("BluetoothGATTGetCharacteristicValue - Actual Data %d", hr);
	}

	return 1;
}

int WinBluetooth::CloseBLE()
{
	EventManager::GetInstance()->StatusChanged(3);

	if (m_hLEDevice == INVALID_HANDLE_VALUE)
	{
		return 0;
	}

	CloseHandle(m_hLEDevice);

	if (GetLastError() != NO_ERROR &&
		GetLastError() != ERROR_NO_MORE_ITEMS)
	{

		// Insert error handling here.
		return 1;
	}

	m_hLEDevice = INVALID_HANDLE_VALUE;
	

	return 1;
}

int WinBluetooth::ScanBLEDevices()
{
	HDEVINFO hDI;
	SP_DEVINFO_DATA did;
	DWORD i;

	// Create a HDEVINFO with all present devices.
	hDI = SetupDiGetClassDevs(&GUID_DEVCLASS_BLUETOOTH, NULL, NULL, DIGCF_PRESENT);

	if (hDI == INVALID_HANDLE_VALUE)
	{
		return 1;
	}

	// Enumerate through all devices in Set.
	did.cbSize = sizeof(SP_DEVINFO_DATA);
	for (i = 0; SetupDiEnumDeviceInfo(hDI, i, &did); i++)
	{
		bool hasError = false;

		DWORD nameData;
		LPTSTR nameBuffer = NULL;
		DWORD nameBufferSize = 0;

		while (!SetupDiGetDeviceRegistryProperty(
			hDI,
			&did,
			SPDRP_FRIENDLYNAME,
			&nameData,
			(PBYTE)nameBuffer,
			nameBufferSize,
			&nameBufferSize))
		{
			if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
			{
				if (nameBuffer) delete(nameBuffer);
				nameBuffer = new wchar_t[nameBufferSize * 2];
			}
			else
			{
				hasError = true;
				break;
			}
		}

		DWORD addressData;
		LPTSTR addressBuffer = NULL;
		DWORD addressBufferSize = 0;

		while (!SetupDiGetDeviceRegistryProperty(
			hDI,
			&did,
			SPDRP_HARDWAREID,
			&addressData,
			(PBYTE)addressBuffer,
			addressBufferSize,
			&addressBufferSize))
		{
			if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
			{
				if (addressBuffer) delete(addressBuffer);
				addressBuffer = new wchar_t[addressBufferSize * 2];
			}
			else
			{
				hasError = true;
				break;
			}
		}

		LPTSTR deviceIdBuffer = NULL;
		DWORD deviceIdBufferSize = 0;

		while (!SetupDiGetDeviceInstanceId(
			hDI,
			&did,
			deviceIdBuffer,
			deviceIdBufferSize,
			&deviceIdBufferSize))
		{
			if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
			{
				if (deviceIdBuffer) delete(deviceIdBuffer);
				deviceIdBuffer = new wchar_t[deviceIdBufferSize * 2];
			}
			else
			{
				hasError = true;
				break;
			}
		}

		if (hasError)
		{
			continue;
		}

		//std::string name		= util::to_narrow(nameBuffer);
		//std::string address		= util::to_narrow(addressBuffer);
		//std::string deviceId	= util::to_narrow(deviceIdBuffer);
		//std::cout << "Found " << name << " (" << deviceId << ")" << std::endl;
	}

	return 0;
}

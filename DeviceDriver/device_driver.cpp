#include "device_driver.h"
#include "custom_exception.h"
#include <iostream>

DeviceDriver::DeviceDriver(FlashMemoryDevice* hardware) : m_hardware(hardware)
{
}

int DeviceDriver::read(long address)
{
	int ret = 0;
	int prev = 0;
	for (int i = 0; i < 5; i++) {
		ret = m_hardware->read(address);
		if (i == 0) {
			prev = ret;
			continue;
		}
		if (ret != prev)
			throw CustomException("ReadFailException");

		prev = ret;
	}

	return ret;
}

void DeviceDriver::write(long address, int data)
{
	if (read(address) != 0xFF)
		throw CustomException("WriteFailException");
	m_hardware->write(address, (unsigned char)data);
}
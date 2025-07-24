#include "device_driver.h"
#include "custom_exception.h"

DeviceDriver::DeviceDriver(FlashMemoryDevice* hardware) : m_hardware(hardware)
{
}

int DeviceDriver::read(long address)
{
	int ret = 0;
	int prev = 0;
	for (int i = 0; i < 5; i++) {
		ret = m_hardware->read(address);
		if (i == 0) continue;
		if (ret != prev)
			throw CustomException("ReadFailException");
		prev = ret;
	}

	return ret;
}

void DeviceDriver::write(long address, int data)
{
	// TODO: implement this method
	m_hardware->write(address, (unsigned char)data);
}
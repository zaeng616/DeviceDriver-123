#include "gmock/gmock.h"
#include "device_driver.h"
#include "custom_exception.h"
class FlashMock : public FlashMemoryDevice {
public:
	MOCK_METHOD(unsigned char, read, (long address), (override));
	MOCK_METHOD(void, write, (long address, unsigned char data), (override));
};

TEST(DeviceDriver, ReadFromHW) {
	FlashMock mock;
	EXPECT_CALL(mock, read(0xFF)).Times(5);
	DeviceDriver driver{ &mock };
	int data = driver.read(0xFF);
	EXPECT_EQ(0, data);
}

TEST(DeviceDriver, ReadException) {
	FlashMock mock;
	EXPECT_CALL(mock, read(0xFF))
		.WillOnce(testing::Return(0))
		.WillOnce(testing::Return(1));
	DeviceDriver driver{ &mock };
	try {
		int data = driver.read(0xFF);
		FAIL();
	}
	catch (CustomException& e) {
		EXPECT_EQ(strcmp(e.what(), "ReadFailException"), 0);
	}
}

TEST(DeviceDriver, WriteToHW) {
	FlashMock mock;
	EXPECT_CALL(mock, read(0xFF)).Times(5)
		.WillRepeatedly(testing::Return(0xff));
	EXPECT_CALL(mock, write(0xFF, 'a')).Times(1);
	DeviceDriver driver{ &mock };
	driver.write(0xFF, 'a');
}

TEST(DeviceDriver, WriteException) {
	FlashMock mock;
	EXPECT_CALL(mock, write(0xFF, 'a')).Times(1);
	EXPECT_CALL(mock, read(0xFF))
		.WillOnce(testing::Return(0xff))
		.WillOnce(testing::Return(0xff))
		.WillOnce(testing::Return(0xff))
		.WillOnce(testing::Return(0xff))
		.WillOnce(testing::Return(0xff))
		.WillRepeatedly(testing::Return('a'));
	DeviceDriver driver{ &mock };
	driver.write(0xFF, 'a');
	try {
		driver.write(0xFF, 'a');
		FAIL();
	}
	catch (CustomException& e) {
		EXPECT_EQ(strcmp(e.what(), "WriteFailException"), 0);
	}
}

int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}
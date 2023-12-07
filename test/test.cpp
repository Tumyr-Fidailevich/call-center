#include "pch.h"
#include "gtest/gtest.h"
#include "call_center.h"
#include "call.h"
#include "operator.h"
#include "cdr.h"


TEST(CallCenterTest, getPhoneNumberFromRequestTest)
{
	auto phoneNumber = CallCenter::getPhoneNumberFromRequest(
			R"(GET /call-center?phone=+79224338364 HTTP/1.1\r\n\r\n)");
	EXPECT_EQ(phoneNumber, "+79224338364");
}

TEST(CallCenterTest, getMessageForUserTest)
{
	boost::asio::io_context ioContext;
	auto newCall = std::make_shared<Call>(ioContext, "+79224338364");
	newCall->getCDR().status = CDR::Status::OK;
	auto userMessage = CallCenter::getMessageForUser(newCall);
	userMessage = userMessage.substr(0, userMessage.find(' '));
	EXPECT_EQ(userMessage, "OK");
}

TEST(CallCenterTest, isNumberValidTest)
{
	EXPECT_TRUE(CallCenter::isNumberValid("+11111111111"));
	EXPECT_TRUE(CallCenter::isNumberValid("+79224338364"));
	EXPECT_FALSE(CallCenter::isNumberValid("+79224338"));
	EXPECT_FALSE(CallCenter::isNumberValid(""));
}

TEST(CallTest, getOperatorTest)
{
	boost::asio::io_context ioContext;
	auto newCall = std::make_shared<Call>(ioContext, "+79224338364");
	auto op = std::make_shared<Operator>(1);
	newCall->setOperator(op);
	EXPECT_EQ(newCall->getOperator()->getId(), 1);
}


int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
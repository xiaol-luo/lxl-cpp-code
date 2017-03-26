
#include <string>
#include <stdio.h>
#include <google/protobuf/compiler/importer.h>
#include <google/protobuf/dynamic_message.h>
#include <windows.h>
#include "GlobalDef.h"
#include "compat/Compat.h"
#include <stdio.h>
#include "code.pb.h"

using namespace google::protobuf;

class ErrorCollector : public compiler::MultiFileErrorCollector
{
public:
	ErrorCollector() : compiler::MultiFileErrorCollector() {}
	virtual ~ErrorCollector() {}

	virtual void AddError(const std::string& filename, int line, int column,
		const std::string& message) {}
};

int main(int argc, char **argv)
{
	char cwdBuff[512] = { 0 };
	Compat::GetCurrWorkDir(cwdBuff, sizeof(cwdBuff) / sizeof(cwdBuff[0]));

	compiler::DiskSourceTree dst;
	ErrorCollector ec;
	compiler::Importer importer(&dst,&ec);

	char protobufMsgDir[512] = { 0 };
	Compat::SNPrintf(protobufMsgDir, sizeof(protobufMsgDir) / sizeof(protobufMsgDir[0]), "%s/protobuf-msg", cwdBuff);

	dst.MapPath("", protobufMsgDir);

	std::string outPath;
	dst.VirtualFileToDiskFile("msg.proto", &outPath);
	printf("diskFile %s \n", outPath.c_str());

	const FileDescriptor *fdsc = importer.Import("msg.proto");
	printf("descript:%s\n", fdsc->DebugString().c_str());

	const Descriptor *pingDescriptor = fdsc->pool()->FindMessageTypeByName("Ping");
	printf("pingDescriptor:%s\n", pingDescriptor->DebugString().c_str());

	const Descriptor *pongDescriptor = fdsc->pool()->FindMessageTypeByName("Pong");
	printf("pongDescriptor:%s\n", pongDescriptor->DebugString().c_str());

	const FieldDescriptor *fileDescriptor = fdsc->pool()->FindFieldByName("Ping.msgID");
	printf("fileDescriptor:%s\n", fileDescriptor->DebugString().c_str());

	{
		const DescriptorPool *generatedPool = DescriptorPool::generated_pool();
		const DescriptorPool *otherPool = fdsc->pool();
		const DescriptorPool *importerPool = importer.pool();
	}

	DynamicMessageFactory dynamicMF(importer.pool());
	{
		
		const Message *pingDefaultMsg = dynamicMF.GetPrototype(pingDescriptor);
		Message *pingMsg = pingDefaultMsg->New();
		const Message::Reflection *pingReflection = pingMsg->GetReflection();
		const FieldDescriptor *msgIDField = pingDescriptor->FindFieldByLowercaseName("msgid");
		const FieldDescriptor *useridField = pingDescriptor->FindFieldByName("userid");
		const FieldDescriptor *usernameField = pingDescriptor->FindFieldByName("username");

		pingReflection->SetInt32(pingMsg, msgIDField, 10);
		pingReflection->SetInt32(pingMsg, useridField, 100);
		pingReflection->SetString(pingMsg, usernameField, "luoxiaolong");
		printf("pingMsg debug string : %s", pingMsg->DebugString().c_str());
	}

	{
		int fieldCount = pingDescriptor->field_count();
		for (int i = 0; i < fieldCount; ++i)
		{
			const FieldDescriptor *fieldDesc = pingDescriptor->field(i);
			printf("pingDescriptor %s\n", fieldDesc->DebugString().c_str());
		}
	}
	
	int a = 0;
	a++;
}


#include "udpclient.h"

#define TF_TYPE_BEGIN	1
#define TF_TYPE_DATA	2
#define TF_TYPE_END 	3

class clitask :public udptask
{
public:
	clitask()
	{

	}

	virtual int parsemsg(const char *buf, int len)
	{
		printf("收到数据 %s,%d\n", buf, len);
		return 0;
	}
};

int main(int argc, char *argv[])
{
	if (argc < 3)
	{
		printf("%s ip 文件名\n", argv[0]);
		return -1;
	}
#ifdef _WIN32
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

	srand((unsigned)time(NULL));

	udpclient<clitask> c;
	c.connect(argv[1], 9001, rand());

	char *filename = argv[2];

	FILE *fp = fopen(filename, "rb");
	if (fp == NULL)
	{
		return -1;
	}

	char buf[10240] = {0};

	buf[0] = TF_TYPE_BEGIN;
	strncpy(buf + 1, filename, strlen(filename));

	c.send(buf, strlen(filename) + 1);

	buf[0] = TF_TYPE_DATA;
	for (;;)
	{
		size_t rc = fread(buf+1, 1, sizeof(buf)-1, fp);
		if (rc <= 0)
		{
			break;
		}
		int nret = c.send(buf, rc+1);
		if (nret < 0)
		{
			printf("发送失败 %d\n", nret);
			break;
		}
	}
	fclose(fp);

	buf[0] = TF_TYPE_END;
	c.send(buf, 1);

	while (c.isalive())
	{
		std::chrono::milliseconds dura(10);
		std::this_thread::sleep_for(dura);
	}

	c.shutdown();
#ifdef _WIN32
	WSACleanup();
#endif
	return 0;
}


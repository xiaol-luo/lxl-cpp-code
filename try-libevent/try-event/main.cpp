#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/buffer.h>
#include <signal.h>
#include "Log/Log.h"
#include "def/GlobalDef.h"

extern void signal_int_cb(evutil_socket_t fd, short sig_num, void *arg);
extern void conn_write_cb(evutil_socket_t sock_fd, short what, void *user_data);
extern void conn_read_cb(evutil_socket_t sock_fd, short what, void *user_data);
extern void timer_event_cb(evutil_socket_t sock_fd, short what, void *user_data);

bool is_exit = false;

struct buffer
{
	ev_uint32_t capacity;
	ev_uint32_t size;
	char *data;
};

struct ConnectData
{
	struct event *read_event;
	struct event *write_event;
	struct buffer read_buffer;
	struct buffer write_buffer;
};

int main(int argc, char **argv)
{
	struct event_base *base;
	struct event *signal_event;
	struct event *cnn_read_event;
	struct event *cnn_write_event;
	struct event *timer_event;
	ConnectData cnn_data;
	struct timeval tv;

	memset(&cnn_data, 0, sizeof(cnn_data));
	cnn_data.read_buffer.capacity = 1024;
	cnn_data.read_buffer.data = (char *)malloc(cnn_data.read_buffer.capacity);
	cnn_data.write_buffer.capacity = 1024;
	cnn_data.write_buffer.data = (char *)malloc(cnn_data.write_buffer.capacity);

#ifdef _WIN32
	WSADATA wsa_data;
	WSAStartup(0x0201, &wsa_data);
#endif

	LogInit();

	base = event_base_new();


	{
		// signal
		signal_event = evsignal_new(base, SIGINT, signal_int_cb, NULL);
		event_add(signal_event, NULL);
	}

	{
		// connect
		evutil_socket_t sock_fd;
		sockaddr_in sock_addr;
		int sock_addr_len;
		char buff[1024];

		sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (sock_fd < 0)
		{
			LogError(LogConsole, "create socket fail");
			return -1;
		}

		/*
		sock_addr.sin_family = AF_INET;
		sock_addr.sin_port = htons(TRY_LISTEN_PORT);
		evutil_inet_pton(AF_INET, "127.0.0.1", &sock_addr.sin_addr);

		evutil_inet_ntop(AF_INET, &sock_addr.sin_addr, buff, sizeof(buff));
		LogInfo(LogConsole, "{}", buff);
		*/

		evutil_snprintf(buff, sizeof(buff), "%s:%d", "127.0.0.1", TRY_LISTEN_PORT);
		sock_addr_len = (int)sizeof(sock_addr);
		evutil_parse_sockaddr_port("127.0.0.1:9555", (struct sockaddr *)&sock_addr, &sock_addr_len);

		if (0 != connect(sock_fd, (sockaddr *)&sock_addr, sizeof(sock_addr)))
		{
			LogError(LogConsole, "connect fail");
			return -1;
		}

		evutil_make_socket_nonblocking(sock_fd);

		cnn_write_event = event_new(base, sock_fd, EV_WRITE | EV_PERSIST, conn_write_cb, &cnn_data);
		cnn_read_event = event_new(base, sock_fd, EV_READ | EV_PERSIST, conn_read_cb, &cnn_data);

		cnn_data.read_event = cnn_read_event;
		cnn_data.write_event = cnn_write_event;

		event_add(cnn_read_event, NULL);
		//event_add(cnn_write_event, NULL);
	}

	{
		// timer
		// timer_event = evtimer_new(base, timer_event_cb, (void *)&cnn_data); onetime
		timer_event = event_new(base, -1, EV_TIMEOUT | EV_PERSIST, timer_event_cb, (void *)&cnn_data);
		tv.tv_sec = 1; tv.tv_usec = 0;
		event_add(timer_event, &tv);
	}
	
	while (!is_exit)
	{
		event_base_dispatch(base);
	}

	event_base_free(base);

	LogUninit();

	return 0;
}

void signal_int_cb(evutil_socket_t fd, short sig_num, void *arg)
{
	is_exit = true;
}

void conn_write_cb(evutil_socket_t sock_fd, short what, void *user_data)
{
	struct ConnectData *cnn_data = (struct ConnectData *)user_data;

	if (cnn_data->write_buffer.size > 0)
	{
		bool isOk = true;
		int ret = send(sock_fd, cnn_data->write_buffer.data, cnn_data->write_buffer.size, 0);
		if (ret > 0)
		{
			memmove(cnn_data->write_buffer.data, cnn_data->write_buffer.data + ret,
				cnn_data->write_buffer.size - ret);
			cnn_data->write_buffer.size -= (ev_uint32_t)ret;
		}
		else if (ret < 0)
		{
			if (0 != errno && EAGAIN != errno && EWOULDBLOCK != errno)
				isOk = false;
		}

		if (!isOk)
		{
			LogError(LogConsole, "read error {}", errno);
			event_free(cnn_data->read_event);
			cnn_data->read_event = NULL;
		}
	}

	if (cnn_data->write_buffer.size <= 0 && cnn_data->write_event)
	{
		event_del(cnn_data->write_event);
	}
}

void conn_read_cb(evutil_socket_t sock_fd, short what, void *user_data)
{
	struct ConnectData *cnn_data = (struct ConnectData *)user_data;

	int ret = 0;
	do
	{
		ret = recv(sock_fd, cnn_data->read_buffer.data + cnn_data->read_buffer.size,
			cnn_data->read_buffer.capacity - cnn_data->read_buffer.size, 0);
		if (ret > 0)
		{
			cnn_data->read_buffer.size += ret;
			if (cnn_data->read_buffer.size >= cnn_data->read_buffer.capacity)
			{
				// just for test
				cnn_data->read_buffer.size = cnn_data->read_buffer.capacity / 2;
			}
			continue;
		}
		else if (ret < 0)
		{
			if (0 == errno || EAGAIN == errno || EWOULDBLOCK == errno)
				break;
		}
		
		LogError(LogConsole, "read error {}", errno);
		event_free(cnn_data->read_event);
		cnn_data->read_event = NULL;
		break;

	} while (ret > 0);

	if (cnn_data->read_buffer.size > 0)
	{
		int endPos = cnn_data->read_buffer.size;
		if (endPos >= cnn_data->read_buffer.capacity)
			endPos -= 1;
		cnn_data->read_buffer.data[endPos] = 0;

		LogInfo(LogConsole, "cnn_read_cb {}", cnn_data->read_buffer.data);
	}
	cnn_data->read_buffer.size = 0;

	/*
	if (cnn_data->read_buffer.size > 0)
	{
		ev_uint32_t write_buffer_left_size = cnn_data->write_buffer.capacity - cnn_data->write_buffer.size;
		ev_uint32_t copy_size = (cnn_data->read_buffer.size < write_buffer_left_size ? \
			cnn_data->read_buffer.size : write_buffer_left_size);
		if (write_buffer_left_size > 0)
		{
			memcpy(cnn_data->write_buffer.data + cnn_data->write_buffer.size, cnn_data->read_buffer.data, copy_size);
			memmove(cnn_data->read_buffer.data, cnn_data->read_buffer.data + copy_size, cnn_data->read_buffer.size - copy_size);
			

			if (cnn_data->write_buffer.size <= 0)
			{
				event_add(cnn_data->write_event, NULL);
			}

			cnn_data->write_buffer.size += copy_size;
			cnn_data->read_buffer.size -= copy_size;
		}
	}
	*/

}

void timer_event_cb(evutil_socket_t sock_fd, short what, void *user_data)
{
	ConnectData *cnn_data = (ConnectData *)user_data;

	if (cnn_data->write_buffer.data && cnn_data->write_event)
	{
		int ret = evutil_snprintf(cnn_data->write_buffer.data + cnn_data->write_buffer.size,
			cnn_data->write_buffer.capacity - cnn_data->write_buffer.size, "timer call back\n");
		if (0 == cnn_data->write_buffer.size)
		{
			event_add(cnn_data->write_event, NULL);
		}
		cnn_data->write_buffer.size += ret;
	}
}


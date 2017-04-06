#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/util.h>
#include <event2/buffer.h>
#include "Log/Log.h"
#include "def/GlobalDef.h"

void event_cb(struct bufferevent *bev, short events, void *ptr)
{
	if (events & BEV_EVENT_CONNECTED) {
		/* We're connected to 127.0.0.1:9995.   Ordinarily we'd do
		something here, like start reading or writing. */
	}
	else if (events & BEV_EVENT_ERROR || events & BEV_EVENT_EOF) {
		event_base *base = bufferevent_get_base(bev);
		timeval tv = { 1, 1 };
		event_base_loopexit(base, &tv);
		*(bool *)ptr = true;
	}
}

void read_cb(struct bufferevent *bev, void *ctx)
{
	/*
	struct evbuffer *in_buffer = bufferevent_get_input(bev);
	int len = evbuffer_get_length(in_buffer);
	if (len > 0)
	{
		char *msg = (char *)malloc(len + 1);
		int ret = evbuffer_remove(in_buffer, msg, len);
		msg[len] = 0;
		LogInfo(LogConsole, "recv msg is {}", msg);
		free(msg); msg = NULL;
	}
	*/

	char buff[1024];
	int ret = bufferevent_read(bev, buff, sizeof(buff) - 1);
	if (ret > 0)
	{
		buff[ret] = 0;
		LogInfo(LogConsole, "recv msg is {}", buff);
		// bufferevent_write(bev, buff, ret);
	}

	/*
	struct evbuffer *buf = evbuffer_new();
	int ret = bufferevent_read_buffer(bev, buf);
	evbuffer_free(buf); buf = NULL;
	*/
}

void time_cb(evutil_socket_t fd, short, void *arg)
{
	struct bufferevent *bev = (bufferevent *)arg;
	char *msg = "hello world";
	bufferevent_write(bev, msg, strlen(msg));
}

void write_cb(struct bufferevent *bev, void *ctx)
{
	LogInfo(LogConsole, "write_cb");
}

int main(int argc, char **argv)
{
	struct event_base *base;
	struct event *ev_timer;
	struct timeval one_sec = { 1, 0 };
	struct bufferevent *bev;
	struct sockaddr_in sin;
	bool is_exit = false;

#ifdef _WIN32
	WSADATA wsa_data;
	WSAStartup(0x0201, &wsa_data);
#endif

	LogInit();

	base = event_base_new();
	bev = bufferevent_socket_new(base, -1, BEV_OPT_CLOSE_ON_FREE);
	bufferevent_setcb(bev, read_cb, write_cb, event_cb, &is_exit);
	bufferevent_enable(bev, EV_READ);
	bufferevent_enable(bev, EV_WRITE);
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = htonl(0x7f000001); 
	sin.sin_port = htons(TRY_LISTEN_PORT); 

	if (bufferevent_socket_connect(bev,(struct sockaddr *)&sin, sizeof(sin)) < 0) {
		bufferevent_free(bev);
		LogError(LogConsole,"Connect Fail");
		return -1;
	}

	ev_timer = event_new(base, -1, EV_PERSIST, time_cb, bev);
	event_add(ev_timer, &one_sec);

	while (!is_exit)
	{
		event_base_dispatch(base);
	}

	event_free(ev_timer);
	bufferevent_free(bev);
	event_base_free(base);
	
	LogUninit();
}

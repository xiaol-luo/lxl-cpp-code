#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/buffer.h>
#include <signal.h>
#include "Log/Log.h"
#include "def/GlobalDef.h"

extern void accept_conn_cb(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *address, int socklen, void *ctx);
extern void accept_error_cb(struct evconnlistener *listener, void *ctx);
extern void signal_int_cb(evutil_socket_t fd, short sig_num, void *arg);
extern void conn_write_cb(struct bufferevent *bev, void *user_data);
extern void conn_read_cb(struct bufferevent *bev, void *user_data);
extern void conn_event_cb(struct bufferevent *bev, short events, void *user_data);

bool is_exit = false;

int main(int argc, char **argv)
{
	struct event_base *base;
	struct event *signal_event;
	struct evconnlistener *listener;
	struct sockaddr_in sin;

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
		// listen
		memset(&sin, 0, sizeof(sin));
		sin.sin_family = AF_INET;
		sin.sin_addr.s_addr = htonl(0);
		sin.sin_port = htons(TRY_LISTEN_PORT);

		listener = evconnlistener_new_bind(base, accept_conn_cb, NULL,
			LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE, -1,
			(struct sockaddr*)&sin, sizeof(sin));
		if (!listener) {
			LogError(LogConsole, "Couldn't create listener");
			return 1;
		}
		
		evconnlistener_set_error_cb(listener, accept_error_cb);
	}	

	
	while (!is_exit)
	{
		event_base_dispatch(base);
	}

	evconnlistener_free(listener);
	event_base_free(base);

	LogUninit();

	return 0;
}

void accept_conn_cb(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *address, int socklen, void *ctx)
{
	struct event_base *base;
	struct bufferevent *bev;

	base = evconnlistener_get_base(listener);
	bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
	if (!bev) {
		LogError(LogConsole, "Error constructing bufferevent!");
		event_base_loopbreak(base);
		return;
	}

	bufferevent_setcb(bev, conn_read_cb, conn_write_cb, conn_event_cb, NULL);
	bufferevent_enable(bev, EV_WRITE);
	bufferevent_enable(bev, EV_READ);
}

void accept_error_cb(struct evconnlistener *listener, void *ctx)
{
	int a = 0;
	a++;
}

void signal_int_cb(evutil_socket_t fd, short sig_num, void *arg)
{
	is_exit = true;
}

void conn_write_cb(struct bufferevent *bev, void *user_data)
{
	
}

void conn_read_cb(struct bufferevent *bev, void *user_data)
{
	const static int BUFF_SIZE = 128;
	char buff[BUFF_SIZE];

	size_t ret = bufferevent_read(bev, buff, BUFF_SIZE - 1);
	while (ret > 0)
	{
		buff[ret] = 0;
		LogInfo(LogConsole, buff);
		bufferevent_write(bev, buff, ret);
		ret = bufferevent_read(bev, buff, BUFF_SIZE - 1);
	}
}

void conn_event_cb(struct bufferevent *bev, short events, void *user_data)
{
	if (events & BEV_EVENT_EOF) {
		printf("Connection closed.\n");
	}
	else if (events & BEV_EVENT_ERROR) {
		printf("Got an error on the connection: %s\n",
			strerror(errno));/*XXX win32*/
	}
	/* None of the other events can happen here, since we haven't enabled
	* timeouts */
	bufferevent_free(bev);
}

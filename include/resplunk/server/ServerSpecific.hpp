#ifndef resplunk_server_ServerSpecific_HeaderPlusPlus
#define resplunk_server_ServerSpecific_HeaderPlusPlus

namespace resplunk
{
	namespace server
	{
		struct Server;

		struct ServerSpecific
		{
			ServerSpecific() = delete;
			ServerSpecific(ServerSpecific const &) = default;
			ServerSpecific &operator=(ServerSpecific const &) = delete;
			ServerSpecific(ServerSpecific &&) = default;
			ServerSpecific &operator=(ServerSpecific &&) = delete;
			virtual ~ServerSpecific() = 0;

			virtual Server &server() final
			{
				return server;
			}
			virtual Server const &server() const final
			{
				return server;
			}

		protected:
			ServerSpecific(Server &s)
			: serv(s)
			{
			}

		private:
			Server &serv;
		};
		inline ServerSpecific::~ServerSpecific() = default;
	}
}

#endif

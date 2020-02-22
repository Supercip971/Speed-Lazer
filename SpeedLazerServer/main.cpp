#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Main.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>
#include <thread>
#include <future>
//#include <vector>
const unsigned short PORT = 8000;
const std::string IPADDRESS("192.168.1.81");//change to suit your needs

bool server = false;
std::string msgSend;

sf::TcpSocket* socket;
sf::Mutex globalMutex;
bool quit = false;

void DoStuff(void)
{
	static std::string oldMsg;
	while (!quit)
	{
		sf::Packet packetSend;
		globalMutex.lock();
		packetSend << msgSend;
		globalMutex.unlock();
		for (int i = 0; i < sizeof(sf::TcpSocket) / sizeof(socket); i++)
		{
			socket[i].send(packetSend);
			std::string msg;
			sf::Packet packetReceive;

			socket[i].receive(packetReceive);
			if ((packetReceive >> msg) && oldMsg != msg && !msg.empty())
			{
				std::cout << msg << std::endl;
				oldMsg = msg;
			}
		}
	}
}

//bool Client(void)
//{
//	if (socket[0].connect(IPADDRESS, PORT) == sf::Socket::Done)
//	{
//		std::cout << "Connected\n";
//		return true;
//	}
//	return false;
//}

void GetInput(void)
{
	std::string s;
	std::cout << "\nEnter \"exit\" to quit or message to send: ";
	getline(std::cin, s);
	if (s == "exit")
		quit = true;
	globalMutex.lock();
	msgSend = s;
	globalMutex.unlock();
};
int servCliN = 0;

sf::TcpListener socklisten;
int connectn = 0;

const int numberClient = 10;
class servClient {
public:
	sf::TcpSocket clientSocket;

	bool connected = false;
	bool fconnected = false;
	bool connectcan = false;
	std::string msg = "";
	sf::IpAddress address = "null";
	void update() {
		if (clientSocket.getRemoteAddress() == sf::IpAddress::None)
		{
			connected = false;
		}
		else
		{
			address = clientSocket.getRemoteAddress();
		}
	}
	void actualize() {
		sf::Packet packetReceive;
		if (connected == false)
		{
			socklisten.accept(clientSocket);
			printf("connecting...\n");
			if (!connectcan)
			{
				printf("LIMIT CONNECTION : %s \n", address.toString());
				sf::Packet pack;
				pack << false;
				clientSocket.send(pack);
				connected = false;
				fconnected = false;
			}
			else
			{
				connectn++;
				printf("new connection : address : %s , num : %i / %i \n", address.toString(), connectn, numberClient);
				//		printf("new connection : %i \n", clientList[i].address.toInteger());
				sf::Packet pack;
				pack << true;
				clientSocket.send(pack);
				connected = true;

				fconnected = false;
			}
			connected = true;
			fconnected = true;
		}
		if (clientSocket.receive(packetReceive) == sf::Socket::Disconnected) {
			printf("[disconnecting] %s \n", address.toString());
			clientSocket.disconnect();
			connected = false;
			printf("[disconnected] %s \n", address.toString());
		}
		else
		{
			packetReceive >> msg;
			if (msg == "getName")
			{
			}
		};
	}
	std::string get() {
		return msg;
	}

	servClient() {
		++servCliN;
		printf("creating serv client... %i \n", servCliN);
	}
};

class ClientSelf {
	// self client
public:
	sf::TcpSocket sock;

	void connect() {
		if (sock.connect(IPADDRESS, PORT) == sf::Socket::Done)
		{
			sf::Packet packetReceive;

			sock.receive(packetReceive);
			bool dat = false;
			(packetReceive >> dat);
			if (dat == true)
			{
				printf("connected ! \n");
			}
			else
			{
				printf("error can't connect \n");
				sock.disconnect();
			}
		}
	}
	void send() {
		std::string elsed = "";
		//std::getline( std::cin, elsed);
		std::cin >> elsed;

		sf::Packet packetSend;
		packetSend << elsed;
		sock.send(packetSend);
	}
	void get() {
	}
};
void nullcall() {
}
class Server {
public:
	servClient* clientList = new servClient[numberClient + 2];
	std::future<void>* thrList = new std::future<void>[numberClient + 2];

	void update() {
		for (int i = 0; i < numberClient + 2; i++)
		{
			if (i < numberClient)
			{
				clientList[i].connectcan = true;
			}
			if (!clientList[i].connected)
			{
				thrList[i] = std::async(&servClient::actualize, &clientList[i]);
			}
			else
			{
				if (i > numberClient)
				{
					if (clientList[i].fconnected) {
					};
				}
				else
				{
					if (clientList[i].fconnected) {
						//	thrList.at(i) = new sf::Thread(&servClient::actualize, &clientList[i]);
					};
				}
			}
		}
		printf("update");
	}
	void getl() {
		for (int i = 0; i < numberClient; i++)
		{
			if (clientList[i].connected)
			{
				if (thrList[i].valid())
				{
					if (thrList[i].wait_for(std::chrono::nanoseconds(0)) == std::future_status::ready)
					{
						thrList[i].wait();
						printf("client %s, id : %i, %s \n", clientList[i].clientSocket.getRemoteAddress().toString(), i, clientList[i].get());
						if (clientList[i].get() == "get")
						{
							sf::Ftp ftp;

							sf::Ftp::Response response = ftp.login(); // just an example, could be any function
							std::cout << "Response status: " << response.getStatus() << std::endl;
							std::cout << "Response message: " << response.getMessage() << std::endl;
						}

						thrList[i] = std::async(&servClient::actualize, &clientList[i]);
					}
				}
				else
				{
				}
				//thrList.insert(sf::Thread())
			}
			else
			{
				if (thrList[i].valid())
				{
					if (thrList[i].wait_for(std::chrono::nanoseconds(0)) == std::future_status::ready)
					{
						thrList[i].wait();
						printf("client %i, update \n", i);

						thrList[i] = std::async(&servClient::actualize, &clientList[i]);
					}
				}
				else
				{
				}
			}
		}
	}

	Server() {
		printf("crating server...\n");
		socklisten.listen(PORT);

		printf("listen loading finish\n");
		printf("vector update thread\n");
	}
};

int main(int argc, char* argv[])
{
	sf::Window wnd;
	printf("loading...");
	sf::Thread* thread = 0;
	sf::TcpListener listener;
	char who;
	std::cout << "Do you want to be a server (s) or a client (c) ? ";
	std::cin >> who;
	if (who == 's') {
		printf("launching server... \n");
		server = true;
		Server serv;
		printf("launching thread... \n");
		thread = new sf::Thread(&Server::update, &serv);
		thread->launch();
		while (!quit)
		{
			sf::Event event;
			serv.getl();
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
				printf("\n >");
				std::string str;
				std::getline(std::cin, str);
				printf(str.c_str());
			};
		}
	}
	else {
		ClientSelf cli;
		cli.connect();
		while (!quit)
		{
			cli.send();
		}
	}

	//while (!quit)
	//{
	//
	//	if (server)
	//	{
	//		sf::TcpSocket sock;
	//		if (listener.accept(socket[sizeof(sf::TcpSocket) / sizeof(socket) + 1]) == sf::Socket::Status::Done)
	//		{
	//
	//			std::cout << "New client connected: " << sock.getRemoteAddress() << std::endl;
	//

	//		}
	//	}
	//	else
	//	{
	//		GetInput();
	//	}
	//
	//
	//}

	//if (thread)
	//{
	//	thread->wait();
	//	delete thread;
	//}
	return 0;
}
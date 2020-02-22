//#include <iostream>
//#include <SFML/System.hpp>
//#include <SFML/Main.hpp>
//#include <SFML/Window.hpp>
//#include <SFML/Network.hpp>
//#include <thread>
//#include <future>
//#define __LINE__ printf("LINE %i", __LINE__);
//const unsigned short PORT = 9222;
//const std::string IPADDRESS("192.168.1.81");//change to suit your needs
//sf::TcpListener socklisten;
//void loging(std::string stl) {
//
//	printf("[LOG] %s \n", stl.c_str());
//
//}
//class servClient {
//public:
//	std::string name = "null";
//	sf::TcpSocket clientSocket;
//	sf::IpAddress address = "null";
//	std::string msg = "null";
//	bool connected = false;
//	int tryToConnect = 0;
//	void get() {
//		if (!connected)
//		{
//
//			if (socklisten.accept(clientSocket) == sf::Socket::Status::Done) {
//				printf("new connection : %s ", clientSocket.getRemoteAddress().toString());
//				address = clientSocket.getRemoteAddress().toString();
//				sf::Packet p;
//				while (clientSocket.receive(p) != sf::Socket::Done)
//				{
//					tryToConnect++;
//					if (tryToConnect % 10 == 1)
//					{
//						printf("try to connect... \n");
//
//					}if (tryToConnect == 1000)
//					{
//						printf("timed out\n");
//						return;
//					}
//				}
//				printf("name : %s", p.getData());
//				sf::Packet pi;
//				pi << "true";
//				clientSocket.send(pi);
//				connected = true;
//			};
//		}
//	}
//	void update() {
//		if (connected)
//		{
//			if (clientSocket.getRemotePort() == 0)
//			{
//				printf("disconnect : %s ", address);
//				clientSocket.disconnect();
//				connected = true;
//			}
//			else
//			{
//
//			}
//		}
//		else
//		{
//			get();
//		}
//
//	}
//	servClient() {
//		loging("creating serv client");
//		clientSocket.setBlocking(false);
//	}
//	std::string getdata() {
//		return msg;
//	}
//private:
//
//};
//class Client {
//public:
//	int tryToConnect = 0;
//	sf::TcpSocket sock;
//	bool connected = false;
//
//	void connect() {
//		if (connected == false)
//		{
//			if (sock.connect(IPADDRESS, PORT) == sf::Socket::Done)
//			{
//				loging("Connecting !");
//				sf::Packet packetSend;
//				packetSend << "bob";
//
//				sock.send(packetSend);
//				sf::Packet packetReceive;
//				while (sock.receive(packetReceive) != sf::Socket::Done)
//				{
//					tryToConnect++;
//					if (tryToConnect % 10 == 1)
//					{
//						printf("try to connect... \n");
//
//					}if (tryToConnect == 1000)
//					{
//						printf("timed out\n");
//						return;
//					}
//				}
//				if (packetReceive.getData() == "true")
//				{
//
//					connected = true;
//					loging("Connected !");
//				}
//				/*
//				sf::Packet packetReceive;
//
//				sock.receive(packetReceive);*/
//				/*bool dat = false;
//				(packetReceive >> dat);
//				if (dat == true)
//				{
//					printf("connected ! \n");
//				}
//				else
//				{
//					printf("error can't connect \n");
//					sock.disconnect();
//				}*/
//			}
//		}
//
//	}
//	void update() {
//		if (!connected)
//		{
//			connect();
//		}
//		else
//		{
//
//		}
//	}
//	Client() {
//		loging("loading client");
//		sock.setBlocking(false);
//	}
//private:
//
//};
//class Server
//{
//public:
//	int maxClient = 3;
//
//	servClient* client = new servClient[maxClient];
//	Server()
//	{
//		loging("creating server");
//	}
//	void update() {
//
//		for (int i = 0; i < maxClient; i++)
//		{
//			client[i].update();
//		}
//	}
//	void load() {
//		for (int i = 0; i < maxClient; i++)
//		{
//			client[i].clientSocket.setBlocking(false);
//		}
//	}
//private:
//
//};
//
//
//
//
//
//
//
//
//int Nmain() {
//	loging("starting...");
//	loging("starting socket listen...");
//	socklisten.setBlocking(false);
//
//	socklisten.listen(PORT);
//	loging("s to be an server and c to be an client");
//	if (std::cin.get()== 's')
//	{
//
//		loging("starting server...");
//		Server srv;
//
//		loging("server loading...");
//		srv.load();
//		loging("server loaded");
//		while (true)
//		{
//			srv.update();
//		}
//	}
//	else
//	{
//
//		loging("starting client...");
//		Client cl;
//
//		loging("client connecting...");
//		cl.sock.setBlocking(false);
//		loging("client loaded...");
//		while (true)
//		{
//
//			cl.connect();
//		}
//	}
//
//	socklisten.close();
//
//}
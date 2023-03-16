using namespace std;
#include "../lib/json.hpp"
#include "WebSocketWS.h"
#include <boost/algorithm/string.hpp>
#include <ctime>
#include <iterator>
#include <list>
#include <unordered_map>

using json = nlohmann::json;

class PeerData {
public:
  shared_ptr<WsServer::Connection> m_hConnection;
  string m_name;
  string m_email;
  string m_privIP;
  string m_pubIP;
  string m_devType;
  string m_devName;
  string m_roomID;
  bool m_isAlive = true;
  bool m_registered;
  bool video = false;
};

class WebSocketMainWS : public WebSocketWS {
private:
  static pthread_mutex_t peerMapLock;
  static pthread_mutex_t networkMapLock;
  static unordered_map<string, PeerData *> peerMap;
  static unordered_map<string, list<PeerData *> *> networkMap;

  bool RegisterPeer(shared_ptr<WsServer::Connection> hConnection, json data);
  void DeletePeer(shared_ptr<WsServer::Connection> hConnection, string email);
  void RelayMessages(shared_ptr<WsServer::Connection> hConnection, json msg);

  void SendPeersInfo(PeerData *pData, string type);
  string FindPeerSameNetwork(shared_ptr<WsServer::Connection> hConnection);
  void JoinRoom(shared_ptr<WsServer::Connection> hConnection, string roomID,
                string callType);
  void PartRoom(shared_ptr<WsServer::Connection> hConnection);

  void Video(shared_ptr<WsServer::Connection> hConnection);

  bool CheckURL(string authKey);
  bool EmailCheck(string email);
  void ChangeName(shared_ptr<WsServer::Connection> hConnection, string name);

public:
  void OnConnected(shared_ptr<WsServer::Connection> hConnection);
  void OnMessage(shared_ptr<WsServer::Connection> hConnection, string sMessage);
  void OnDisconnected(shared_ptr<WsServer::Connection> hConnection);
  void OnError(shared_ptr<WsServer::Connection> hConnection, string sError);
  void NetworkDetection();
};

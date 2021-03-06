#include <list>
#include <string>

// TODO Features
#define USE_TODO_FEATURES true

#include <x2struct/x2struct.hpp>
using namespace x2struct;

#ifndef V2CONFIG_H
#define V2CONFIG_H

using namespace std;
/*------------------------------------------------------------------------------------------------------------*/

namespace Qv2ray
{
    namespace V2ConfigModels
    {
        struct VMessProtocolConfigObject {
            string v, ps, add, port, id, aid, net, type, host, path, tls;
            XTOSTRUCT(O(v, ps, add, port, id, aid, net, type, host, path, tls))
        };

        /// Used in config generation
        struct AccountObject {
            string user;
            string pass;
            XTOSTRUCT(O(user, pass))
        };

        struct ApiObject {
            string tag = "api";
            list<string> services;
            ApiObject() : tag(), services() {}
            XTOSTRUCT(O(tag, services))
        };


        namespace POLICYObjects
        {
            struct SystemPolicyObject {
                bool statsInboundUplink;
                bool statsInboundDownlink;
                SystemPolicyObject() : statsInboundUplink(), statsInboundDownlink() {}
                XTOSTRUCT(O(statsInboundUplink, statsInboundDownlink))
            };

            struct LevelPolicyObject {
                int handshake;
                int connIdle;
                int uplinkOnly;
                int downlinkOnly;
                bool statsUserUplink;
                bool statsUserDownlink;
                int bufferSize;
                LevelPolicyObject(): handshake(), connIdle(), uplinkOnly(), downlinkOnly(), statsUserUplink(), statsUserDownlink(), bufferSize() {}
                XTOSTRUCT(O(handshake, connIdle, uplinkOnly, downlinkOnly, statsUserUplink, statsUserDownlink, bufferSize))
            };
        }
        struct PolicyObject {
            map<string, POLICYObjects::LevelPolicyObject> level;
            list<POLICYObjects::SystemPolicyObject> system;
            PolicyObject(): level(), system() {}
            XTOSTRUCT(O(level, system))
        };

        namespace _TransferSettingObjects
        {
            namespace TRANSFERObjectsInternal
            {
                struct HTTPRequestObject {
                    string version ;
                    string method ;
                    list<string> path;
                    map<string, list<string>> headers;
                    HTTPRequestObject(): version("1.1"), method("GET"), path(), headers() {}
                    XTOSTRUCT(O(version, method, path, headers))
                };

                struct HTTPResponseObject {
                    string version;
                    string status ;
                    string reason ;
                    map<string, list<string>> headers;
                    HTTPResponseObject(): version("1.1"), status("200"), reason("OK"), headers() {}
                    XTOSTRUCT(O(version, status, reason, headers))
                };
                struct TCPHeader_M_Object {
                    string type;
                    HTTPRequestObject request;
                    HTTPResponseObject response;
                    TCPHeader_M_Object(): type("none"), request(), response() {}
                    XTOSTRUCT(O(type, request, response))
                };
                struct HeaderObject {
                    string type;
                    HeaderObject(): type("none") {}
                    XTOSTRUCT(O(type))
                };
            }


            struct TCPObject {
                TRANSFERObjectsInternal::TCPHeader_M_Object header;
                TCPObject(): header() {}
                XTOSTRUCT(O(header))
            };


            struct KCPObject {
                int mtu = 1350;
                int tti = 20;
                int uplinkCapacity = 5;
                int downlinkCapacity = 20;
                bool congestion = false;
                int readBufferSize = 1;
                int writeBufferSize = 1;
                TRANSFERObjectsInternal::HeaderObject header;
                KCPObject(): header() {}
                XTOSTRUCT(O(mtu, tti, uplinkCapacity, downlinkCapacity, congestion, readBufferSize, writeBufferSize, header))
            };


            struct WebSocketObject {
                string path;
                map<string, string> headers;
                WebSocketObject(): path("/"), headers() {}
                XTOSTRUCT(O(path, headers))
            };

            struct HttpObject {
                list<string> host;
                string path;
                HttpObject() : host(), path("/") {}
                XTOSTRUCT(O(host, path))
            };

            struct DomainSocketObject {
                string path;
                DomainSocketObject(): path("/") {}
                XTOSTRUCT(O(path))
            };

            struct QuicObject {
                string security;
                string key;
                TRANSFERObjectsInternal::HeaderObject header;
                QuicObject(): security(""), key(""), header() {}
                XTOSTRUCT(O(security, key, header))
            };
        }
        //
        namespace INBOUNDObjects
        {
            struct SniffingObject {
                bool enabled = false;
                list<string> destOverride;
                SniffingObject(): enabled(), destOverride() {}
                XTOSTRUCT(O(enabled, destOverride))
            };
        }

        namespace _StreamSettingObjects
        {
            struct SockoptObject {
                int mark;
                bool tcpFastOpen;
                string tproxy;
                SockoptObject(): mark(0), tcpFastOpen(), tproxy("off") {}
                XTOSTRUCT(O(mark, tcpFastOpen, tproxy))
            };

            struct CertificateObject {
                string usage;
                string certificateFile;
                string keyFile;
                list<string> certificate;
                list<string> key;
                CertificateObject(): usage(), certificateFile(), keyFile(), certificate(), key() {}
                XTOSTRUCT(O(usage, certificateFile, keyFile, certificate, key))
            };

            struct TLSObject {
                string serverName;
                bool allowInsecure;
                list<string> alpn;
                list<CertificateObject> certificates;
                bool disableSystemRoot;
                TLSObject(): serverName(), allowInsecure(), certificates(), disableSystemRoot() {}
                XTOSTRUCT(O(serverName, allowInsecure, alpn, certificates, disableSystemRoot))
            };
        }

        struct StreamSettingsObject  {
            string network;
            string security;
            _StreamSettingObjects::SockoptObject sockopt;
            _StreamSettingObjects::TLSObject tlsSettings;
            _TransferSettingObjects::TCPObject tcpSettings;
            _TransferSettingObjects::KCPObject kcpSettings;
            _TransferSettingObjects::WebSocketObject wsSettings;
            _TransferSettingObjects::HttpObject httpSettings;
            _TransferSettingObjects::DomainSocketObject dsSettings;
            _TransferSettingObjects::QuicObject quicSettings;
            StreamSettingsObject(): network("tcp"), security(), sockopt(),  tlsSettings(), tcpSettings(), kcpSettings(), wsSettings(), httpSettings(), dsSettings(), quicSettings() {}
            XTOSTRUCT(O(network, security, sockopt, tcpSettings, tlsSettings, kcpSettings, wsSettings, httpSettings, dsSettings, quicSettings))
        };

        struct MuxObject {
            bool enabled;
            int concurrency;
            MuxObject(): enabled(), concurrency() {}
            XTOSTRUCT(O(enabled, concurrency))
        };

        struct RootObject {
            ApiObject api;
            PolicyObject policy;
            RootObject(): api(), policy() {}
            XTOSTRUCT(O(api, policy))
        };
        //
        /// Some protocols from: https://v2ray.com/chapter_02/02_protocols.html
        namespace Protocols
        {
            struct XOutBoundsType {};
            struct XInBoundsType {};

            /// DNS, OutBound
            struct DNSOut: XOutBoundsType {
                string network;
                string address;
                int port;
                XTOSTRUCT(O(network, address, port))
            };


            /// MTProto, InBound || OutBound
            struct MTProtoIn: XInBoundsType, XOutBoundsType {
                struct UserObject {
                    string email;
                    int level;
                    string secret;
                    XTOSTRUCT(O(email, level, secret))
                };
                list<UserObject> users;
                XTOSTRUCT(O(users))
            };

            /// Socks, OutBound
            struct SocksOut: XOutBoundsType {
                struct ServerObject {
                    struct UserObject {
                        string user;
                        string pass;
                        int level;
                        XTOSTRUCT(O(user, pass, level))
                    };

                    string address;
                    int port;
                    list<UserObject> users;

                    XTOSTRUCT(O(address, port, users))
                };
                list<ServerObject> servers;
                XTOSTRUCT(O(servers))
            };

            struct VMessOut: XOutBoundsType {
                struct ServerObject {
                    struct UserObject {
                        string id;
                        int alterId;
                        string security;
                        int level;
                        UserObject() : id(""), alterId(64), security("auto"), level(0) {}
                        XTOSTRUCT(O(id, alterId, security, level))
                    };
                    // OUTBound;
                    string address;
                    int port;
                    list<UserObject> users;
                    ServerObject(): address(""), port(0), users() {}
                    XTOSTRUCT(O(address, port, users))
                };
                list<ServerObject> vnext;
                XTOSTRUCT(O(vnext))
            };

        }
    }
}

using namespace Qv2ray::V2ConfigModels;
using namespace Qv2ray::V2ConfigModels::Protocols;

#endif

#pragma once

class QByteArray;
namespace ControlPackets
{
struct Packet;
struct ServerVersion;
}

extern void encServerVersion(QByteArray &tgt,const ControlPackets::ServerVersion &vers);
extern ControlPackets::Packet * decPacket(QByteArray &ba);

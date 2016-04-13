#include "CommCodec.h"
#include "CommProtocol.h"

#include <QByteArray>
#include <QtCore/QString>
#include <QtCore/QLatin1Literal>

using namespace ControlPackets;
namespace
{
template<class T>
void enc(QByteArray &tgt,const T &v)
{
    tgt.append((const char *)&v,sizeof(T));
}
template<>
void enc(QByteArray &tgt,const QString &s) {
    uint16_t len(s.length());
    tgt.append((const char *)&len,sizeof(len));
    tgt.append(s.toLocal8Bit());
}

template<class T>
bool dec(QByteArray &src, T &v)
{
    if(src.size()<sizeof(T))
        return false;
    memcpy(&v,src.data(),sizeof(T));
    src.remove(0,sizeof(T));
    return true;
}
template<>
bool dec(QByteArray &src, QString &v)
{
    uint16_t len;
    if(!dec(src,len))
        return false;
    if(src.size()<len)
        return false;
    v = QString::fromLocal8Bit(src.mid(0,len));
    src.remove(0,len);
    return true;
}
template<>
bool dec(QByteArray &src, StringWithLen &v)
{
    uint16_t len;
    if(!dec(src,len))
        return false;
    if(src.size()<len)
        return false;
    v.contents = new char[len];
    memcpy(v.contents,src.data(),len);
    src.remove(0,len);
    return true;
}

}
void encServerVersion(QByteArray &tgt,const ServerVersion &vers) {
    enc(tgt,"ServerVersion");
    enc(tgt,vers.major);
    enc(tgt,vers.minor);
    int32_t len = tgt.size()+sizeof(int32_t);
    tgt.prepend((const char *)&len,sizeof(int32_t));
}

Packet * decPacket(QByteArray &ba) {
    int32_t packet_size;
    if(!dec(ba,packet_size))
        return nullptr;
    if(packet_size>ba.size())
        return nullptr;
    QString packetname;
    if(!dec(ba,packetname))
        return nullptr;

    if(packetname == QLatin1String("OpenSolution")) {
        OpenSolution * res = new OpenSolution;
        res->kind = eOpenSolution;
        if(!dec(ba,res->solution_dir)) {
            delete res;
            return nullptr;
        }
        return res;
    }
    else if(QLatin1String("SaveAllOpenDocuments") == packetname) {
        SaveAllOpenDocuments * res = new SaveAllOpenDocuments;
        res->kind = eSaveAllOpenDocuments;
        return res;
    }
    else if(QLatin1String("OpenFileAtLine") == packetname) {
        OpenFileAtLine * res = new OpenFileAtLine;
        res->kind = eOpenFileAtLine;
        if(!dec(ba,res->filepath)) {
            delete res;
            return nullptr;
        }
        if(!dec(ba,res->line_no)) {
            delete res;
            return nullptr;
        }
        return res;
    }
    else if(QLatin1String("OpenFiles") == packetname) {
        OpenFiles * res = new OpenFiles;
        res->kind = eOpenFiles;
        if(!dec(ba,res->file_count)) {
            delete res;
            return nullptr;
        }
        res->filepaths = new StringWithLen[res->file_count];
        for(int i=0; i<res->file_count; ++i) {
            if(!dec(ba,res->filepaths[i])) {
                delete res;
                return nullptr;
            }
        }
        return res;
    }
    return nullptr;
}

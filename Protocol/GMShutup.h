#ifndef GMSHUTUP_H
#define GMSHUTUP_H

#include <QObject>

#include <Utils.h>
#include <Network/OctetsStream.h>
#include <AuthServer.h>
#include <Database.h>

class GMShutup : public OctetsStream
{
public:
    GMShutup() : OctetsStream() {}
    virtual void Unmarshal() override;

    int gmroleid;
    int localsid;
    int dstuserid;
    int forbid_time;
    Octets reason;
};

#endif // GMSHUTUP_H

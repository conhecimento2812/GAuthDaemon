#include "GMShutup.h"

void GMShutup::Unmarshal()
{
    this->operator >>(gmroleid);
    this->operator >>(localsid);
    this->operator >>(dstuserid);
    this->operator >>(forbid_time);
    this->operator >>(reason);
    Utils::print(QString("GMShutup: gm: %1 localsid: %2 role: %3 forbid time: %4 reason: %5")
                 .arg(gmroleid).arg(localsid).arg(dstuserid).arg(forbid_time).arg(Utils::toUtf8(reason)));

    Reset();
    writeInt32(0);
    writeInt32(dstuserid);
    writeInt32(forbid_time);
   	Send(0x165);
}

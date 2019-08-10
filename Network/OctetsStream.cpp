#include "OctetsStream.h"

void OctetsStream::init()
{
    qds = new QDataStream(&data, QIODevice::ReadWrite);
    qds->setByteOrder(QDataStream::LittleEndian);
    qds->setFloatingPointPrecision(QDataStream::SinglePrecision);
}

uint OctetsStream::calcSession()
{
    return session - 2147483648;
}

void OctetsStream::Send(uint opcode)
{
    QByteArray out;
    out.append(OctetsStream::scompact_uint32(opcode));
    out.append(OctetsStream::scompact_uint32(static_cast<uint>(data.length())));
    out.append(data);

    if (socket->isOpen())
        socket->write(out);
}

void OctetsStream::Reset()
{
    data.clear();
    init();
}

uint OctetsStream::uncompact_uint32()
{
    char byte = readByte();

    switch (byte & 0xE0)
    {
        case 0x0E:
            return static_cast<uint>(readInt32());
        case 0xC0:
            qds->device()->seek(qds->device()->pos() - 1);
            return static_cast<uint>(readInt32() & 0x3FFFFFFF);
        case 0x80:
        case 0xA0:
            qds->device()->seek(qds->device()->pos() - 1);
            return static_cast<uint>(readInt16() & 0x7FFF);
        default:
            return static_cast<uint>(byte);
    }
}

long long OctetsStream::unmarshal_long()
{
    long long value;
    qds->readRawData(reinterpret_cast<char*>(&value), 8);

    return qbswap(value);
}

uint OctetsStream::readUInt32()
{
    uint value;
    this->operator >>(value);
    return value;
}

int OctetsStream::readInt32()
{
    int value;
    this->operator >>(value);
    return value;
}

short OctetsStream::readInt16()
{
    short value;
    this->operator >>(value);
    return value;
}

char OctetsStream::readByte()
{
    char value;
    this->operator >>(value);
    return value;
}

quint8 OctetsStream::readUByte()
{
    quint8 value;
    this->operator >>(value);
    return value;
}

Octets OctetsStream::readOctet()
{
    Octets value;
    this->operator >>(value);
    return value;
}

void OctetsStream::compact_uint32(const uint &value)
{
    if (value < 127)
        writeByte(static_cast<const char>(value));
    else if (value < 16383)
        writeInt16(static_cast<const short>(value | 0x8000));
}

QByteArray OctetsStream::scompact_uint32(const uint &value)
{
    QByteArray out;
    QDataStream stream(&out, QIODevice::WriteOnly);
    if (value < 127)
        stream << static_cast<int8_t>(value);
    else if (value < 16383)
        stream << static_cast<short>(value | 0x8000);
    return out;
}

void OctetsStream::writeLong(const long long &value)
{
	this->operator <<(value);
}

void OctetsStream::writeUInt32(const uint &value)
{
    this->operator <<(value);
}

void OctetsStream::writeInt32(const int &value)
{
    this->operator <<(value);
}

void OctetsStream::writeInt16(const short &value)
{
    this->operator <<(value);
}

void OctetsStream::writeByte(const char &value)
{
    this->operator <<(value);
}

void OctetsStream::writeUByte(const quint8 &value)
{
    this->operator <<(value);
}

void OctetsStream::writeOctet(const Octets &value)
{
    this->operator <<(value);
}

OctetsStream &OctetsStream::operator >>(long long &value)
{
    value = unmarshal_long();
    return *this;
}

OctetsStream &OctetsStream::operator >>(uint &value)
{
    qds->operator >>(value);
    value = qbswap(value);
    return *this;
}

OctetsStream &OctetsStream::operator >>(int &value)
{
    qds->operator >>(value);
    value = qbswap(value);
    return *this;
}

OctetsStream &OctetsStream::operator >>(short &value)
{
    qds->operator >>(value);
    value = qbswap(value);
    return *this;
}

OctetsStream &OctetsStream::operator >>(char &value)
{
    qds->readRawData(&value, 1);
    return *this;
}

OctetsStream &OctetsStream::operator >>(quint8 &value)
{
    qds->operator >>(value); 
    return *this;
}

OctetsStream &OctetsStream::operator >>(Octets &value) {
    uint length = uncompact_uint32();

    QByteArray buffer(static_cast<int>(length), Qt::Uninitialized);

    if (length > 1)
        qds->readRawData(buffer.data(), static_cast<int>(length));

    value = Octets(buffer);
	
    return *this;
}

OctetsStream &OctetsStream::operator <<(uint value)
{
    qds->operator <<(qbswap(value));
    return *this;
}

OctetsStream &OctetsStream::operator <<(int value)
{
    qds->operator <<(qbswap(value));
    return *this;
}

OctetsStream &OctetsStream::operator <<(short value)
{
    qds->operator <<(qbswap(value));
    return *this;
}

OctetsStream &OctetsStream::operator <<(char value)
{
    qds->writeRawData(&value, 1);
    return *this;
}

OctetsStream &OctetsStream::operator <<(quint8 value)
{
    qds->operator <<(value);
    return *this;
}

OctetsStream &OctetsStream::operator <<(long long value)
{
	qds->operator <<(qbswap(value));
	return *this;
}

QByteArray OctetsStream::getData() const
{
    return data;
}

OctetsStream &OctetsStream::operator <<(Octets value)
{
    compact_uint32(static_cast<uint>(value.lenght));

    if (value.lenght> 1)
        qds->writeRawData(value.text, value.lenght);
    return *this;
}

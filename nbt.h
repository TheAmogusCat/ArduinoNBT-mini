#pragma once

#define MAX_NAME_LEN 64

#define ID_TAG_END 0x00
#define ID_TAG_BYTE 0x01
#define ID_TAG_SHORT 0x02
#define ID_TAG_INT 0x03
#define ID_TAG_LONG 0x04
#define ID_TAG_FLOAT 0x05
#define ID_TAG_DOUBLE 0x06
#define ID_TAG_BYTE_ARRAY 0x07
#define ID_TAG_STRING 0x08
#define ID_TAG_LIST 0x09
#define ID_TAG_COMPOUND 0x0A
#define ID_TAG_INT_ARRAY 0x0B
#define ID_TAG_LONG_ARRAY 0x0C

namespace nbt {
namespace detail {

inline static void writeNumberInBigEndian(byte* out, const void* valPtr, size_t bytes) {
  const unsigned char* raw = reinterpret_cast<const unsigned char*>(valPtr);
  for (size_t i = 0; i < bytes; ++i) {
    out[i] = raw[bytes - 1 - i];
  }
}

template <typename U>
inline static void writeNumberInBigEndian(byte* out, U val) {
  writeNumberInBigEndian(out, &val, sizeof(U));
}

inline size_t writeName(byte* buffer, const char* name, uint16_t nameLength = 0) {
  size_t pos = 0;
  if (nameLength == 0)
    nameLength = strlen(name);

  // Write name length in big-endian format
  buffer[pos++] = (nameLength >> 8) & 0xFF;
  buffer[pos++] = nameLength & 0xFF;

  // Write name
  memcpy(buffer + pos, name, nameLength);
  pos += nameLength;

  return pos;
}

}


class NBTCompoundTag {
private:
  byte* buffer;
  size_t size;
  size_t pos;
  uint16_t nameLength;

  template <typename TagType, typename ValueType>
  size_t pushNBT(const char* key, ValueType value) {
    TagType tag(key, value);
    if (pos + tag.getBufferSize() <= size)
      pos += tag.serialize(buffer + pos);
    return pos;
  }

  template<typename T>
  void pushNumberInPlace(const char* key, T value, byte tagId) {
    size_t need = 1 /* tag id */ + 2 /* name length */ + strlen(key) + sizeof(T);
    if (pos + need > size) return;

    buffer[pos++] = tagId;
    pos += detail::writeName(buffer + pos, key);
    detail::writeNumberInBigEndian(buffer + pos, value);
    pos += sizeof(T);
  }
public:
  char nameBuf[MAX_NAME_LEN];

  NBTCompoundTag(const char* n, size_t s) : size(s), pos(1) {
    size_t len = strlen(n);
    if (len >= MAX_NAME_LEN) {
      len = MAX_NAME_LEN - 1;
    }

    memcpy(nameBuf, n, len);
    nameBuf[len] = '\0';
    nameLength = static_cast<uint16_t>(len);

    buffer = new byte[s];

    buffer[0] = ID_TAG_COMPOUND;

    // Writing name
    pos += detail::writeName(buffer + pos, n);
  }

  void pushByte(const char* key, byte value) { pushNumberInPlace(key, value, ID_TAG_BYTE); }

  void pushShort(const char* key, int16_t value) { pushNumberInPlace(key, value, ID_TAG_SHORT); }

  void pushInt(const char* key, int32_t value) { pushNumberInPlace(key, value, ID_TAG_INT); }

  void pushLong(const char* key, int64_t value) { pushNumberInPlace(key, value, ID_TAG_LONG); }

  void pushFloat(const char* key, float value) { pushNumberInPlace(key, value, ID_TAG_FLOAT); }

  void pushDouble(const char* key, double value) { pushNumberInPlace(key, value, ID_TAG_DOUBLE); }

  void pushString(const char* key, const char* value) {
    uint16_t valueLen = static_cast<uint16_t>(strlen(value));
    size_t need = 1 /* tag id */ + 2 /* name length */ + strlen(key) + 2 /* string length */ + valueLen;
    if (pos + need > size) return;

    buffer[pos++] = ID_TAG_STRING;
    pos += detail::writeName(buffer + pos, key);
    buffer[pos++] = (valueLen >> 8) & 0xFF;
    buffer[pos++] = valueLen & 0xFF;
    memcpy(buffer + pos, value, valueLen);
    pos += valueLen;
  }

  void pushBytes(byte* value, size_t s) {
    if (pos + s <= size) {
      memcpy(buffer + pos, value, s);
      pos += s;
    }
  }

  size_t serialize(byte* buf) const {
    memcpy(buf, buffer, pos);
    buf[pos] = ID_TAG_END;
    return pos + 1;
  }

  const char* getName() const {
    return nameBuf;
  }

  ~NBTCompoundTag() { 
    delete[] buffer;
  }
};

}

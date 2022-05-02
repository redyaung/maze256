#include "Comms.hpp"
#include "Macros.hpp"

using namespace Comms;

#define BUF_CAP 64      // size of the serial buffer

static const char typeDelimiter = ':';
static const char terminator = ']';
static const char initiator = '[';

static byte buf[BUF_CAP];
static int bufSz = 0;

template<typename Enum>
static byte encode(Enum element) {
  return static_cast<int>(element);
}

template<typename Enum>
static Enum decode(byte encoded) {
  return static_cast<Enum>(encoded);
}

static int readBytesTillTerm(byte terminator, byte *buffer, int count, bool &terminated) {
  int i = 0;
  while (i < count && Serial.available()) {
    byte c = Serial.read();
    if (c == terminator) {
      terminated = true;
      return i;
    }
    buffer[i++] = c;
  }
  terminated = false;
  return i;
}

static Type toType(String type) {
  if (type == "NormalVision") {
    return Type::NormalVision;
  } else if (type == "WideVisionH") {
    return Type::WideVisionH;
  } else if (type == "WideVisionV") {
    return Type::WideVisionV;
  } else if (type == "Compass") {
    return Type::Compass;
  } else if (type == "IsGameOver") {
    return Type::IsGameOver;
  } else {
    return Type::Invalid;
  }
}

static Type processBuffer(byte *content, int &size) {
  // buf contains "[<Type>:<Contents>" without the terminator.
  int delimIndex;
  for (
    delimIndex = 0;
    delimIndex < bufSz && buf[delimIndex] != typeDelimiter;
    delimIndex++
  ) {}
  buf[delimIndex] = '\0';
  String typeStr = String((char *) buf);
  size = bufSz - delimIndex - 1;
  for (int i = 0; i < size; i++)
    content[i] = buf[delimIndex + i + 1];
#ifndef NDEBUG
  __Debug("Contents: ");
  for (int i = 0; i < size; i++) {
    __Debug(String(content[i]));
    __Debug(" ");
  }
  __Debugln();
#endif
  return toType(typeStr);
}

// Modifies the content and size arguments if the processed type is valid.
// Assumes content has at least 64 bytes.
Type Comms::process(byte *content, int &size) {
  assert(Serial.available() > 0);

  static bool initiatorRead = false;
  if (!initiatorRead) {
    initiatorRead = Serial.find(initiator);
    bufSz = 0;
    __Debug("Initiator Read: ");
    __Debugln(initiatorRead);
  } else {
    bool terminated = false;
    size_t newSz = readBytesTillTerm(terminator, buf + bufSz, BUF_CAP, terminated);
    bufSz += newSz;
    initiatorRead = !terminated;
    if (terminated) {
      buf[bufSz] = '\0';
      return processBuffer(content, size);
    }
  }
  return Type::Invalid;
}

void Comms::sendMove(Direction d) {
  assert(d != Direction::None);
  Serial.print("[Move:");
  Serial.write(encode(d));
  Serial.print("]");
}

void Comms::sendConfig(Difficulty diff) {
  Serial.print("[Config:");
  Serial.write(encode(diff));
  Serial.print("]");
}

void Comms::sendAutopilotRequest() {
  Serial.print("[AutopilotRequest:]");
}

void Comms::sendGiveUpRequest() {
  Serial.print("[GiveUpRequest:]");
}

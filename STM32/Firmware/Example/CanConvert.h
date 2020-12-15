#ifndef _CANCONVERT_H_
#define _CANCONVERT_H_

#include <stdint.h>

typedef enum {
  NodeAddressBoardMonitor = 1,
  NodeAddressBatteryMonitor = 2,
  NodeAddressLogger = 3,
  NodeAddressTx = 4,
  NodeAddressEscSensor = 5,
  NodeAddressIqController = 6,
  NodeAddressDbms = 0x0A,
  NodeAddressSsr  = 0x0B
} NodeAddress;

typedef enum {
  BuildRepoMain = 0,
  BuildRepoCommon,
  NumBuildRepo
} BuildRepo;

void SerializeBool(bool v, int bit_offset, uint8_t *data);
bool DeserializeBool(const uint8_t *data, int bit_offset);
void SerializeBoolBe(bool v, int bit_offset, uint8_t *data);
bool DeserializeBoolBe(const uint8_t *data, int bit_offset);

void SerializeInt8(int8_t v, int bit_offset, uint8_t *data);
int8_t DeserializeInt8(const uint8_t *data, int bit_offset);
uint8_t DeserializeUInt8(const uint8_t *data, int bit_offset);

void SerializeInt16(int16_t v, int bit_offset, uint8_t *data);
int16_t DeserializeInt16(const uint8_t *data, int bit_offset);
uint16_t DeserializeUInt16(const uint8_t *data, int bit_offset);
void SerializeInt16Be(int16_t v, int bit_offset, uint8_t *data);
int16_t DeserializeInt16Be(const uint8_t *data, int bit_offset);
uint16_t DeserializeUInt16Be(const uint8_t *data, int bit_offset);

void SerializeInt32(int32_t v, int bit_offset, uint8_t *data);
int32_t DeserializeInt32(const uint8_t *data, int bit_offset);
uint32_t DeserializeUInt32(const uint8_t *data, int bit_offset);
void SerializeInt32Be(int32_t v, int bit_offset, uint8_t *data);
int32_t DeserializeInt32Be(const uint8_t *data, int bit_offset);
uint32_t DeserializeUInt32Be(const uint8_t *data, int bit_offset);

void SerializeFloat8(float v, int bit_offset, float offset, float scale, uint8_t *data);
float DeserializeFloat8(const uint8_t *data, int bit_offset, float offset, float scale);
void SerializeFloat16(float v, int bit_offset, float offset, float scale, uint8_t *data);
float DeserializeFloat16(const uint8_t *data, int bit_offset, float offset, float scale);
void SerializeFloat16Be(float v, int bit_offset, float offset, float scale, uint8_t *data);
float DeserializeFloat16Be(const uint8_t *data, int bit_offset, float offset, float scale);
void SerializeFloat32(float v, int bit_offset, float offset, float scale, uint8_t *data);
float DeserializeFloat32(const uint8_t *data, int bit_offset, float offset, float scale);
void SerializeFloat32Be(float v, int bit_offset, float offset, float scale, uint8_t *data);
float DeserializeFloat32Be(const uint8_t *data, int bit_offset, float offset, float scale);

#endif

#include "CanConvert.h"

void SerializeBool(bool v, int bit_offset, uint8_t *data) {
  // Currently LE and BE bools are the same. (inverted bit order)
  int byte = bit_offset / 8;
  uint8_t bit = 1 << (bit_offset % 8);
  if (v) {
    data[byte] |= bit;
  } else {
    data[byte] &= ~bit;
  }
}

bool DeserializeBool(const uint8_t *data, int bit_offset) {
  // Currently LE and BE bools are the same. (inverted bit order)
  int byte = bit_offset / 8;
  uint8_t bit = 1 << (bit_offset % 8);
  return data[byte] & bit;
}

void SerializeBoolBe(bool v, int bit_offset, uint8_t *data) {
  // Currently LE and BE bools are the same. (inverted bit order)
  int byte = bit_offset / 8;
  uint8_t bit = 1 << (bit_offset % 8);
  if (v) {
    data[byte] |= bit;
  } else {
    data[byte] &= ~bit;
  }
}

bool DeserializeBoolBe(const uint8_t *data, int bit_offset) {
  // Currently LE and BE bools are the same. (inverted bit order)
  int byte = bit_offset / 8;
  uint8_t bit = 1 << (bit_offset % 8);
  return data[byte] & bit;
}

void SerializeInt8(int8_t v, int bit_offset, uint8_t *data) {
  // We do not check if value is byte aligned. Assume so for now.
  data[bit_offset / 8] = v;
}

int8_t DeserializeInt8(const uint8_t *data, int bit_offset) {
  // We do not check if value is byte aligned. Assume so for now.
  return (int8_t)data[bit_offset / 8];
}

uint8_t DeserializeUInt8(const uint8_t *data, int bit_offset) {
  // We do not check if value is byte aligned. Assume so for now.
  return data[bit_offset / 8];
}

void SerializeInt16(int16_t v, int bit_offset, uint8_t *data) {
  data[bit_offset / 8] = v & 0xff;
  data[bit_offset / 8 + 1] = (v >> 8) & 0xff;
}

int16_t DeserializeInt16(const uint8_t *data, int bit_offset) {
  return (int16_t)(data[bit_offset / 8] | (data[bit_offset / 8 + 1] << 8));
}

uint16_t DeserializeUInt16(const uint8_t *data, int bit_offset) {
  return (uint16_t)(data[bit_offset / 8] | (data[bit_offset / 8 + 1] << 8));
}

void SerializeInt16Be(int16_t v, int bit_offset, uint8_t *data) {
  data[bit_offset / 8 + 1] = v & 0xff;
  data[bit_offset / 8] = (v >> 8) & 0xff;
}

int16_t DeserializeInt16Be(const uint8_t *data, int bit_offset) {
  return (int16_t)(data[bit_offset / 8 + 1] | (data[bit_offset / 8] << 8));
}

uint16_t DeserializeUInt16Be(const uint8_t *data, int bit_offset) {
  return (uint16_t)(data[bit_offset / 8 + 1] | (data[bit_offset / 8] << 8));
}

void SerializeInt32(int32_t v, int bit_offset, uint8_t *data) {
  data[bit_offset / 8] = v & 0xff;
  data[bit_offset / 8 + 1] = (v >> 8) & 0xff;
  data[bit_offset / 8 + 2] = (v >> 16) & 0xff;
  data[bit_offset / 8 + 3] = (v >> 24) & 0xff;
}

int32_t DeserializeInt32(const uint8_t *data, int bit_offset) {
  return (int32_t)(data[bit_offset / 8]
                   | (data[bit_offset / 8 + 1] << 8)
                   | (data[bit_offset / 8 + 2] << 16)
                   | (data[bit_offset / 8 + 3] << 24));
}

uint32_t DeserializeUInt32(const uint8_t *data, int bit_offset) {
  return (uint32_t)(data[bit_offset / 8]
                    | (data[bit_offset / 8 + 1] << 8)
                    | (data[bit_offset / 8 + 2] << 16)
                    | (data[bit_offset / 8 + 3] << 24));
}

void SerializeInt32Be(int32_t v, int bit_offset, uint8_t *data) {
  data[bit_offset / 8 + 3] = v & 0xff;
  data[bit_offset / 8 + 2] = (v >> 8) & 0xff;
  data[bit_offset / 8 + 1] = (v >> 16) & 0xff;
  data[bit_offset / 8] = (v >> 24) & 0xff;
}

int32_t DeserializeInt32Be(const uint8_t *data, int bit_offset) {
  return (int32_t)(data[bit_offset / 8 + 3]
                   | (data[bit_offset / 8 + 2] << 8)
                   | (data[bit_offset / 8 + 1] << 16)
                   | (data[bit_offset / 8] << 24));
}

uint32_t DeserializeUInt32Be(const uint8_t *data, int bit_offset) {
  return (uint32_t)(data[bit_offset / 8 + 3]
                    | (data[bit_offset / 8 + 2] << 8)
                    | (data[bit_offset / 8 + 1] << 16)
                    | (data[bit_offset / 8] << 24));
}

void SerializeFloat8(float v, int bit_offset, float offset, float scale, uint8_t *data) {
  data[bit_offset / 8] = (int8_t)((v - offset) / scale);
}

float DeserializeFloat8(const uint8_t *data, int bit_offset, float offset, float scale) {
  return data[bit_offset / 8] * scale + offset;
}

void SerializeFloat16(float v, int bit_offset, float offset, float scale, uint8_t *data) {
  int16_t i = (int16_t)((v - offset) / scale);
  SerializeInt16(i, bit_offset, data);
}

float DeserializeFloat16(const uint8_t *data, int bit_offset, float offset, float scale) {
  return DeserializeInt16(data, bit_offset) * scale + offset;
}

void SerializeFloat16Be(float v, int bit_offset, float offset, float scale, uint8_t *data) {
  int16_t i = (int16_t)((v - offset) / scale);
  SerializeInt16Be(i, bit_offset, data);
}

float DeserializeFloat16Be(const uint8_t *data, int bit_offset, float offset, float scale) {
  return DeserializeInt16Be(data, bit_offset) * scale + offset;
}

void SerializeFloat32(float v, int bit_offset, float offset, float scale, uint8_t *data) {
  int32_t i = (int32_t)((v - offset) / scale);
  SerializeInt32(i, bit_offset, data);
}

float DeserializeFloat32(const uint8_t *data, int bit_offset, float offset, float scale) {
  return DeserializeInt32(data, bit_offset) * scale + offset;
}

void SerializeFloat32Be(float v, int bit_offset, float offset, float scale, uint8_t *data) {
  int32_t i = (int32_t)((v - offset) / scale);
  SerializeInt32Be(i, bit_offset, data);
}

float DeserializeFloat32Be(const uint8_t *data, int bit_offset, float offset, float scale) {
  return DeserializeInt32Be(data, bit_offset) * scale + offset;
}

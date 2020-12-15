#pragma once

#define DEFAULT_PRINTF_BUFFER_LEN 64

// This is just a circular buffer that takes strings as input and provides a
// pointer to a contiguous section of its internal buffer of up to the
// requested size. For simplicity and efficiency, if the available bytes would
// wrap at the end of the buffer array, only the buffer tail is returned.
// Input data that would overflow the buffer is thrown out.
class PrintBuffer {
public:
  uint8_t *buf;
  int bufLen;
  uint8_t bufStart = 0;
  uint8_t bufEnd = 0;

  PrintBuffer() : PrintBuffer(DEFAULT_PRINTF_BUFFER_LEN) { };

  PrintBuffer(int len) : buf(new uint8_t[len]), bufLen(len) { };

  // Returns 1 if data is thrown out due to buffer overflow.
  int Append(const char *c) {
    while (*c) {
      uint8_t next = (bufEnd + 1) % bufLen;
      if (next == bufStart) return 1;
      buf[bufEnd] = *c++;
      bufEnd = next;
    }
    return 0;
  };

  // Sets start to buffer start, and returns buffer len.
  int Get(int size, uint8_t **start) {
    if (bufStart == bufEnd) return 0;
    int len = size;
    if (bufEnd > bufStart) {
      len = min(len, bufEnd - bufStart);
    } else {
      len = min(len, bufLen - bufStart);
    }
    *start = buf + bufStart;
    bufStart = (bufStart + len) % bufLen;
    return len;
  };

  bool Available() {
    return bufStart != bufEnd;
  };
};

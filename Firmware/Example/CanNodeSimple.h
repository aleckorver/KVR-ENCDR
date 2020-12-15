#ifndef CANNODE_SIMPLE_H
#define CANNODE_SIMPLE_H

#include <Arduino.h>

#include <math.h>
#include <mcp_can.h>  // From: https://github.com/coryjfowler/MCP_CAN_lib
#include <SPI.h>

//#include "JetfoilCan.h"
//#include "JetfoilCommon.h"
#include "PrintBuffer.h"

#define CAN_INT 13

#define CAN_REINIT_DELAY 1000

class CanNodeSimple {
  public:
  
    uint8_t node_id;
    uint8_t can_rx_buf[8];
    uint8_t can_tx_buf[8];
    char msgString[128];  // Array to store serial string

    const int NODE_PROCESS_INPUT_PERIOD = 20;
    const int NODE_STATUS_TX_PERIOD = 100;
    const int NODE_TEMP_TX_PERIOD   = 1000;
    const int PRINTF_OUTPUT_RATE_MS = 2;
    const uint8_t PRINTF_MSG_ID = 0x70;

    long next_process_input;
    long next_status_tx;
    uint8_t readRegister(uint8_t reg);
    void modifyRegister(uint8_t reg, const uint8_t mask, const uint8_t data);
    bool setModeLoopBack();
    void startSPI();
    void endSPI();
    CanNodeSimple(uint8_t node_id_,
                  int select_pin = SS,
                  int interrupt_pin = CAN_INT);
    virtual void HandleCanMessage(uint32_t id, int len, uint8_t *buf);
    bool SendBuffer(uint32_t id, int len, uint8_t *data);
    virtual void CanUpdate(long now);
    bool CanInit();
    void CanErrorLoop();
    int Printf(const char *c);
    virtual void Setup(boolean enable_can = true);
    virtual void ProcessInputs(long now);
    unsigned long getMeanBlockingMs() const {
      return (blockingMs / (can_msg_num ? can_msg_num : can_msg_num + 1));
    }
    unsigned long getMaxBlockingMs() const { return maxBlockingMs; }
    void resetCanDiagnostics();
    uint8_t getTimeoutsBuff() const { return buff_timeout; }
    uint8_t getTimeoutsBus() const { return bus_timeout; }
    //	protected:
    unsigned long blockingMs = 0;
    unsigned long maxBlockingMs = 0;
    uint8_t buff_timeout = 0, bus_timeout = 0, can_msg_num = 0;
    int interrupt_pin;
    MCP_CAN CAN;
    void CanReceive();
    void UpdatePrintf(long now);
    boolean can_enabled = true;
    long last_can_reinit = 0;
    uint16_t send_fails = 0;
    unsigned long lastPrintf = 0;
    PrintBuffer printBuffer;
  private:

    
};


#endif  // CANNODE_SIMPLE_H

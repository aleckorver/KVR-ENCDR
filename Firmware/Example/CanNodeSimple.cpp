#include "Arduino.h"
#include "CanNodeSimple.h"
#include "CanConvert.h"

CanNodeSimple::CanNodeSimple(uint8_t node_id_,
                             int select_pin,
                             int interrupt_pin_)
  : CAN(select_pin) {
	node_id = node_id_;
  interrupt_pin = interrupt_pin_;
}

bool CanNodeSimple::SendBuffer(uint32_t id, int len, uint8_t *data) {
  if (!can_enabled) return false;
  unsigned long blockingStart = millis();
  byte sndStat = CAN.sendMsgBuf(id, 1, len, data);
  unsigned long timeElapsed = millis() - blockingStart;
  blockingMs += timeElapsed;
  maxBlockingMs = (maxBlockingMs > timeElapsed) ? maxBlockingMs : timeElapsed;
  can_msg_num++;
  switch (sndStat){
    case CAN_GETTXBFTIMEOUT:
      buff_timeout++;
      break;
    case CAN_SENDMSGTIMEOUT:
      bus_timeout++;
      break;
    case CAN_OK: 
      return false;
      break; 
    default:
      break;
  }

  if (send_fails++ % 100 == 0) {
    Serial.print("CAN send fail ");
    Serial.println(send_fails - 1);
  }
  if (blockingStart - last_can_reinit > CAN_REINIT_DELAY) {
    CanInit();
    last_can_reinit = blockingStart;
    Serial.print("CAN send fail ");
    Serial.println(send_fails);
  }
  return true;
}

bool CanNodeSimple::CanInit() {
  // Initialize MCP2515 running at 16MHz with a baudrate of 500kb/s and the masks and filters disabled.
  if (CAN.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ) == CAN_OK) {
    Serial.println("MCP2515 Initialized Successfully!");
  } else {
    Serial.println("Error Initializing MCP2515...");
    return true;
  }
  setModeLoopBack();
  //CAN.setMode(MCP_NORMAL);   // Change to normal mode to allow messages to be transmitted
  return false;
}

void CanNodeSimple::CanErrorLoop() {
  // Blink red LED at 2Hz until reset.
  while (1) {
    digitalWrite(LED_BUILTIN, (millis() / 250) % 2);
    if (!(millis() % 1000)) {
      Serial.println("CAN setup error");
      delay(1);
    }
  }
}

void CanNodeSimple::Setup(boolean enable_can) {
  can_enabled = enable_can;
  if (can_enabled) {
	  if (CanInit()) {
      CanErrorLoop();
    }
  }

  long now = millis();
  next_process_input = now;
  next_status_tx = now + NODE_STATUS_TX_PERIOD + 20;
}

void CanNodeSimple::HandleCanMessage(uint32_t id, int len, uint8_t *buf) {
}

void CanNodeSimple::CanReceive() {
  long unsigned int rxId;
  unsigned char len = 0;
  const int max_reads = 20;
  int reads = 0;
  while (can_enabled && !digitalRead(interrupt_pin) && max_reads > reads++) {
    while (!digitalRead(interrupt_pin) && max_reads > reads++) {
      CAN.readMsgBuf(&rxId, &len, can_rx_buf);
      HandleCanMessage(rxId, len, can_rx_buf);
    }
    delayMicroseconds(2);
  }
  if (reads > 1) {
    Serial.print("R "); Serial.println(reads);
  }
}

void CanNodeSimple::ProcessInputs(long now) {
}

int CanNodeSimple::Printf(const char *c) {
  return printBuffer.Append(c);
}

// Drip feed low-priority printf CAN messages.
void CanNodeSimple::UpdatePrintf(long now) {
  if (!printBuffer.Available()) return;
  if ((now - lastPrintf) >= PRINTF_OUTPUT_RATE_MS) {
    lastPrintf = now;
    uint8_t *start;
    int len = printBuffer.Get(8, &start);
    if (len > 0) {
      CAN.sendMsgBuf(node_id << 8 | PRINTF_MSG_ID, 1, len, start);
    }
  }
}

void CanNodeSimple::CanUpdate(long now) {
  CanReceive();
  UpdatePrintf(now);
}

void CanNodeSimple::resetCanDiagnostics() {
  blockingMs = 0;
  maxBlockingMs = 0;
  can_msg_num = 0;
  buff_timeout = 0;
  bus_timeout = 0;
}


//static const uint8_t INSTRUCTION_BITMOD = 0x05;

//const uint8_t INSTRUCTION_BITMOD = 0x05;
//const uint8_t CANCTRL_REQOP = 0xE0;
//const uint8_t CANCTRL_REQOP_LOOPBACK = 0x40;
//const uint8_t MCP_CANCTRL = 0x0F;
//const uint8_t MCP_CANSTAT = 0x0E;
//const uint8_t CANSTAT_OPMOD = 0xE0;
//const uint8_t INSTRUCTION_WRITE = 0x02;
//const uint8_t INSTRUCTION_READ = 0x03;
//static const uint32_t SPI_CLOCK = 10000000; // 10MHz

void CanNodeSimple::startSPI() {
    SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0));
    digitalWrite(11, LOW);
}

void CanNodeSimple::endSPI() {
    digitalWrite(11, HIGH);
    SPI.endTransaction();
}

uint8_t CanNodeSimple::readRegister(uint8_t reg)
{
    startSPI();
    SPI.transfer(0x03);
    SPI.transfer(reg);
    uint8_t ret = SPI.transfer(0x00);
    endSPI();
    return ret;
}
void CanNodeSimple::modifyRegister(uint8_t reg, const uint8_t mask, const uint8_t data)
{
    startSPI();
    SPI.transfer(0x05);
    SPI.transfer(reg);
    SPI.transfer(mask);
    SPI.transfer(data);
    endSPI();
}

bool CanNodeSimple::setModeLoopBack()
{
    modifyRegister(0x0F, 0xE0, 0x40);
    unsigned long endTime = millis() + 10;
    bool modeMatch = false;
    while (millis() < endTime) {
        uint8_t newmode = readRegister(0x0E);
        newmode &= 0xE0;
        modeMatch = newmode == 0x40;
        if (modeMatch) {
            Serial.println();
            Serial.print("MCP MODE: LOOPBACK");
            break;
        }
    }

    //return //modeMatch ? ERROR_OK : ERROR_FAIL;
}

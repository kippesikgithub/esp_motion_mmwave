#include "esphome.h"

class LD2410 : public Component, public UARTDevice
{
public:
  LD2410(UARTComponent *parent) : UARTDevice(parent) {}
  std::vector<uint8_t> bytes;
  const std::vector<uint8_t> config_header = {0xFD, 0xFC, 0xFB, 0xFA, 0x1C, 0x00};
  const std::vector<uint8_t> target_header = {0xF4, 0xF3, 0xF2, 0xF1, 0x0D, 0x00};
  const std::vector<uint8_t> ld2410_end_conf = {0x04, 0x03, 0x02, 0x01};

  void ESP_LOGD_HEX(std::vector<uint8_t> bytes, uint8_t separator) {
    std::string res;
    size_t len = bytes.size();
    char buf[5];
    for (size_t i = 0; i < len; i++) {
      if (i > 0) {
        res += separator;
      }
      sprintf(buf, "%02X", bytes[i]);
      res += buf;
    }
    ESP_LOGD("custom", "%s", res.c_str());
  }

  void sendCommand(char *commandStr, char *commandValue, int commandValueLen)
  {
    uint16_t len = 2;
    if (commandValue != nullptr) {
      len += commandValueLen;
    }
    std::vector<uint8_t> ld2410_conf = {0xFD, 0xFC, 0xFB, 0xFA, lowByte(len), highByte(len), commandStr[0], commandStr[1]};
    if (commandValue != nullptr)
    {
      for (int i = 0; i < commandValueLen; i++)
      {
        ld2410_conf.push_back(commandValue[i]);
      }
    }
    for (int i = 0; i < ld2410_end_conf.size(); i++) 
    {
      ld2410_conf.push_back(ld2410_end_conf[i]);
    }
    // ESP_LOGD_HEX(ld2410_conf,':');
    write_array(std::vector<uint8_t>(ld2410_conf.begin(), ld2410_conf.end()));
  }

  int twoByteToInt(char firstByte, char secondByte)
  {
    return (int16_t)(secondByte << 8) + firstByte;
  }

  void handleTargetData(std::vector<uint8_t> buffer)
  {
    TARGETUnion targetUnion;
    std::copy(buffer.begin(), buffer.end(), targetUnion.bytes);
    if (id(show_stats).state == 1 && targetUnion.target.type == 0x02 && targetUnion.target.state != 0x00)
    {
      int movdist = twoByteToInt(targetUnion.target.movdist, targetUnion.target.movdist2);
      if (id(movingTargetDistance).state != movdist)
      {
        id(movingTargetDistance).publish_state(movdist);
      }
      if (id(movingTargetEnergy).state != targetUnion.target.movval)
      {
        id(movingTargetEnergy).publish_state(targetUnion.target.movval);
      }
      int stadist = twoByteToInt(targetUnion.target.stadist, targetUnion.target.stadist2);
      if (id(stillTargetDistance).state != stadist)
      {
        id(stillTargetDistance).publish_state(stadist);
      }
      if (id(stillTargetEnergy).state != targetUnion.target.staval)
      {
        id(stillTargetEnergy).publish_state(targetUnion.target.staval);
      }
      int decdist = twoByteToInt(targetUnion.target.decdist, targetUnion.target.decdist2);
      if (id(detectDistance).state != decdist)
      {
        id(detectDistance).publish_state(decdist);
      }
    }
    else 
    {
      return; 
    }
    // Engineering data - datasheet is horrible
    // if (targetUnion.target.type == 0x01)
    // }
  }

  void handleConfData(std::vector<uint8_t> buffer)
  {
    CONFUnion confUnion;
    std::copy(buffer.begin(), buffer.end(), confUnion.bytes);
    if (confUnion.conf.cmd == 0x61 && confUnion.conf.cmd_val == 0x01 && confUnion.conf.ack_stat == 0x00 && confUnion.conf.head == 0xAA)
    {
      id(maxconfigDistance).publish_state(float(confUnion.conf.max_sta_dist * 0.75));
      id(allSensitivity).publish_state(confUnion.conf.mov0sen);
      id(noneDuration).publish_state(confUnion.conf.none);
    }
  }

  void setConfigMode(bool confenable)
  {
    char cmd[2] = {confenable ? 0xFF : 0xFE,0x00};
    char value[2] = {0x01, 0x00};
    sendCommand(cmd, confenable ? value : nullptr, 2);
  }

  void queryParameters()
  {
    char cmd_query[2] = {0x61, 0x00};
    sendCommand(cmd_query, nullptr, 0);
  }

  void setup() override
  {  }

  void loop() override
  {
    while (available())
    {
      bytes.push_back(read());
      if (bytes.size() < 6)
      {
        continue;
      }
      if (doesHeaderMatch(bytes, config_header))
      {
        if (bytes.size() < sizeof(CONF))
        {
          continue;
        }
        handleConfData(bytes);
        bytes.clear();
      }
      else if (doesHeaderMatch(bytes, target_header)) {
        if (bytes.size() < sizeof(TARGET))
        {
          continue;
        }
        handleTargetData(bytes);
        bytes.clear();
      }
      else
      {
        bytes.erase(bytes.begin());
        continue;
      }
    }
  }

  void setEngineeringMode(bool engenable)
  {
    char cmd[2] = {engenable ? 0x62 : 0x63,0x00};
    sendCommand(cmd, nullptr, 0);
  }

  void setMaxDistancesAndNoneDuration(int maxMovingDistanceRange, int maxStillDistanceRange, int noneDuration)
  {
    char cmd[2] = {0x60, 0x00};
    char value[18] = {0x00, 0x00, lowByte(maxMovingDistanceRange), highByte(maxMovingDistanceRange), 0x00, 0x00, 0x01, 0x00, lowByte(maxStillDistanceRange), highByte(maxStillDistanceRange), 0x00, 0x00, 0x02, 0x00, lowByte(noneDuration), highByte(noneDuration), 0x00, 0x00};
    sendCommand(cmd, value, sizeof(value));
  }

  void setAllSensitivity(int senval)
  {
    //  64 00  00 00  FF FF 00 00 01 00  28 00 00 00 02 00 28 00 00 00 04 03 02 01
    // {cmd  }{dword}{   dgate  }{mword} {   mval   }{sword}{   sval   }{    MFR  }
    char cmd[2] = {0x64, 0x00};
    char value[18] = {0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x01, 0x00, lowByte(senval), highByte(senval), 0x00, 0x00, 0x02, 0x00, lowByte(senval), highByte(senval), 0x00, 0x00};
    sendCommand(cmd, value, sizeof(value));
  }

  void factoryReset()
  {
    char cmd[2] = {0xA2, 0x00};
    sendCommand(cmd, nullptr, 0);
  }

  void reboot()
  {
    char cmd[2] = {0xA3, 0x00};
    sendCommand(cmd, nullptr, 0);
    // not need to exit config mode because the ld2410 will reboot automatically
  }

  void setBaudrate(int index)
  {
    char cmd[2] = {0xA1, 0x00};
    char value[2] = {index, 0x00};
    sendCommand(cmd, value, sizeof(value));
  }

  bool doesHeaderMatch(std::vector<uint8_t> bytes, std::vector<uint8_t> header)
  {
    bool is_equal = std::equal(header.begin(), header.end(), bytes.begin());
    return (is_equal == true ? true : false);
  }

/*
TARGET EXAMPLE DATA
{F4:F3:F2:F1}:{0D:00}:{02}:{AA}: 02  : 4B:00:  4F  : 00:00 : 64   :  29:00 :{55}: {00} :{F8:F7:F6:F5}
{  header   }  {len}  {typ}{hd}{state}{mdist}{mval}{stadis}{staval}{decdis} {tl} {chck} {    MFR    }
*/
  typedef struct
  {
    uint32_t MFR;
    uint16_t len;
    uint8_t type;                 // target or engineering
    uint8_t head;                // fixed head
    uint8_t state;              // state
    uint8_t movdist;          // movement distance
    uint8_t movdist2;          // movement distance
    uint8_t movval;           // movement energy value
    uint8_t stadist;        // stationary distance
    uint8_t stadist2;        // stationary distance
    uint8_t staval;         // stationary energy value
    uint8_t decdist;      // detection distance
    uint8_t decdist2;      // detection distance
    uint8_t tail;         // tail
    uint8_t chk;         // unused
    uint32_t MFR_end ;  // end
  } TARGET;

  typedef union
  {
    TARGET target;
    uint8_t bytes[sizeof(TARGET)];
  } TARGETUnion;

/*
CONF EXAMPLE DATA
FD:FC:FB:FA MFR[0-3]
1C:00 len[4-5]
61:01 CMD[6-7]
00:00 ACKstat[8-9]
AA Head [10]
08 maxDist [11]
06 maxMovDist[12]
06 maxStaDist[13]
1E:1E:1E:1E:1E:1E:1E:1E:1E:1E:1E:1E:1E:1E:1E:1E:1E:1E (9mov & 9sta sensitivities)
5A:00 none[32-33]
04:03:02:01[34-37]
*/
  typedef struct
  {
    uint32_t MFR;
    uint16_t len;
    uint8_t cmd;
    uint8_t cmd_val;
    uint8_t ack_stat;
    uint8_t ack_stathigh;
    uint8_t head;
    uint8_t max_dist;
    uint8_t max_mov_dist;
    uint8_t max_sta_dist;
    uint8_t mov0sen;
    uint8_t mov1sen;
    uint8_t mov2sen;
    uint8_t mov3sen;
    uint8_t mov4sen;
    uint8_t mov5sen;
    uint8_t mov6sen;
    uint8_t mov7sen;
    uint8_t mov8sen;
    uint8_t sta0sen;
    uint8_t sta1sen;
    uint8_t sta2sen;
    uint8_t sta3sen;
    uint8_t sta4sen;
    uint8_t sta5sen;
    uint8_t sta6sen;
    uint8_t sta7sen;
    uint8_t sta8sen;
    uint16_t none;
    uint32_t MFR_end;
  } CONF;

  typedef union
  {
    CONF conf;
    uint8_t bytes[sizeof(CONF)];
  } CONFUnion;
};

#ifndef ESPHOMELIB_API_UTIL_H
#define ESPHOMELIB_API_UTIL_H

#include "esphomelib/defines.h"

#ifdef USE_API

#include "esphomelib/helpers.h"
#include "esphomelib/component.h"
#include "esphomelib/controller.h"

ESPHOMELIB_NAMESPACE_BEGIN

namespace api {

class APIBuffer {
 public:
  APIBuffer(uint8_t *buffer, size_t max_len);
  APIBuffer(const APIBuffer&) = delete;

  size_t get_length() const;
  bool get_overflow() const;
  void write(uint8_t value);

  void encode_int32(uint32_t field, int32_t value);
  void encode_uint32(uint32_t field, uint32_t value);
  void encode_sint32(uint32_t field, int32_t value);
  void encode_bool(uint32_t field, bool value);
  void encode_string(uint32_t field, const std::string &value);
  void encode_string(uint32_t field, const char *string, size_t len);
  void encode_fixed32(uint32_t field, uint32_t value);
  void encode_float(uint32_t field, float value);
  void encode_nameable(Nameable *nameable);

  size_t begin_nested(uint32_t field);
  void end_nested(size_t begin_index);

  void encode_field_(uint32_t field, uint32_t type);
  void encode_varint_(uint32_t value);
  uint32_t varint_length_(uint32_t value);

 protected:
  uint8_t *buffer_;
  size_t at_{0};
  size_t max_len_;
  bool overflow_{false};
};

optional<uint32_t> proto_decode_varuint32(uint8_t *buf, size_t len, uint32_t *consumed = nullptr);

std::string as_string(const uint8_t *value, size_t len);
int32_t as_sint32(uint32_t val);
float as_float(uint32_t val);

class ComponentIterator {
 public:
  ComponentIterator(StoringController *controller);

  void begin();
  void advance();
  virtual bool on_begin();
#ifdef USE_BINARY_SENSOR
  virtual bool on_binary_sensor(binary_sensor::BinarySensor *binary_sensor) = 0;
#endif
#ifdef USE_COVER
  virtual bool on_cover(cover::Cover *cover) = 0;
#endif
#ifdef USE_FAN
  virtual bool on_fan(fan::FanState *fan) = 0;
#endif
#ifdef USE_LIGHT
  virtual bool on_light(light::LightState *light) = 0;
#endif
#ifdef USE_SENSOR
  virtual bool on_sensor(sensor::Sensor *sensor) = 0;
#endif
#ifdef USE_SWITCH
  virtual bool on_switch(switch_::Switch *switch_) = 0;
#endif
#ifdef USE_TEXT_SENSOR
  virtual bool on_text_sensor(text_sensor::TextSensor *text_sensor) = 0;
#endif
  virtual bool on_end();

 protected:
  enum class IteratorState {
    NONE = 0,
    BEGIN,
#ifdef USE_BINARY_SENSOR
    BINARY_SENSOR,
#endif
#ifdef USE_COVER
    COVER,
#endif
#ifdef USE_FAN
    FAN,
#endif
#ifdef USE_LIGHT
    LIGHT,
#endif
#ifdef USE_SENSOR
    SENSOR,
#endif
#ifdef USE_SWITCH
    SWITCH,
#endif
#ifdef USE_TEXT_SENSOR
    TEXT_SENSOR,
#endif
    MAX,
  } state_{IteratorState::NONE};
  size_t at_{0};

  StoringController *controller_;
};

} // namespace api

ESPHOMELIB_NAMESPACE_END

#endif //USE_API

#endif //ESPHOMELIB_API_UTIL_H

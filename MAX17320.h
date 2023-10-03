#include "register.h"
#include "config.h"
#include "struct.h"

#define DEVICE_ADDRESS 0x36
#define NVM_ADDRESS 0x0B

namespace MAX17320
{
  float r_sense = 5;

  uint16_t read_named_register(uint8_t reg)
  {
    uint8_t numBytes = 2;
    Wire.beginTransmission(DEVICE_ADDRESS);
    Wire.write(reg);
    Wire.endTransmission(false);
    Wire.requestFrom(DEVICE_ADDRESS, numBytes);

    while (Wire.available())
    {
      uint8_t buf[numBytes];
      Wire.readBytes(buf, numBytes);
      uint16_t n;
      n = ((uint16_t)buf[1] << 8) | buf[0];
      return n;
    }
  }

  uint16_t read_named_register_nvm(uint8_t reg)
  {
    uint8_t numBytes = 2;
    Wire.beginTransmission(NVM_ADDRESS);
    Wire.write(reg);
    Wire.endTransmission(false);
    Wire.requestFrom(NVM_ADDRESS, numBytes);

    while (Wire.available())
    {
      uint8_t buf[numBytes];
      Wire.readBytes(buf, numBytes);
      uint16_t n;
      n = ((uint16_t)buf[1] << 8) | buf[0];
      return n;
    }
  }

  void write_named_register(uint8_t reg, uint16_t value)
  {
    Wire.beginTransmission(DEVICE_ADDRESS);
    Wire.write(reg);                   // Register address
    Wire.write((uint8_t)(value >> 8)); // MSB of value
    Wire.write((uint8_t)value);        // LSB of value
  }

  void write_named_register_nvm(uint8_t reg, uint16_t value)
  {
    Wire.beginTransmission(NVM_ADDRESS);
    Wire.write(reg);                   // Register address
    Wire.write((uint8_t)(value >> 8)); // MSB of value
    Wire.write((uint8_t)value);        // LSB of value
  }

  bool has_code(uint16_t look_for, uint16_t within)
  {
    return (look_for & within) > 0;
  };

  // Set the kth bit (0 indexed) of n
  uint16_t set_bit(uint16_t n, uint8_t k)
  {
    return n | (1 << k);
  };

  // Clear the kth bit (0 indexed) of n
  uint16_t clear_bit(uint16_t n, uint8_t k)
  {
    return n & ~(1 << k);
  };

  // Read the n bits for reading different values in register
  uint16_t mask_bit_read(uint16_t data, uint8_t read_size, uint8_t left_shift_size)
  {
    data = data >> left_shift_size;
    uint8_t mask = 0xFF;
    mask = mask >> (8 - read_size);
    return data & mask;
  }

  /////////////////////////
  const float VALRTTH_LSB_RESOLUTION = 0.02; // mV
  bool is_valid_voltage_threshold(float raw)
  {
    return fmod(raw, VALRTTH_LSB_RESOLUTION) < 0.0001 && raw >= 0.0 && raw <= (255.0 * VALRTTH_LSB_RESOLUTION);
  }
  float convert_to_time(uint16_t raw)
  {
    return raw * 5.625;
  }
  float convert_to_cell_voltage(uint16_t raw)
  {
    return raw * 0.078125 / 1000.0;
  }
  float convert_to_batt_voltage(uint16_t raw)
  {
    return raw * 0.3125 / 1000.0;
  }
  float convert_to_percentage(uint16_t raw)
  {
    return raw / 256.0;
  }
  float convert_to_temperature(int16_t raw)
  {
    return raw / 256.0;
  }
  float convert_to_capacity(uint16_t raw, float r_sense)
  {
    return raw * 5.0 / r_sense;
  }
  float convert_to_current(int16_t raw, float r_sense)
  {
    return raw * 1.5625 / (r_sense / 1000.0);
  }

  /////////////////////////////
  uint16_t read_device_name()
  {
    return read_named_register(Register::DevName);
  }
  uint16_t read_status()
  {
    return read_named_register(Register::Status);
  }
  float read_capacity()
  {
    uint16_t raw = read_named_register(Register::RepCap);
    return convert_to_capacity(raw, r_sense);
  }
  float read_state_of_charge()
  {
    uint16_t raw = read_named_register(Register::RepSoc);
    return convert_to_percentage(raw);
  }
  float read_vcell()
  {
    uint16_t raw = read_named_register(Register::VCell);
    return convert_to_cell_voltage(raw);
  }
  float read_temperature()
  {
    int16_t raw = read_named_register(Register::Temp);
    return convert_to_temperature(raw);
  }
  float read_die_temperature()
  {
    int16_t raw = read_named_register(Register::DieTemp);
    return convert_to_temperature(raw);
  }
  float read_current()
  {
    int16_t raw = read_named_register(Register::Current);
    return convert_to_current(raw, r_sense);
  }
  float read_time_to_empty()
  {
    uint16_t raw = read_named_register(Register::TimeToEmpty);
    return convert_to_time(raw);
  }
  float read_time_to_full()
  {
    uint16_t raw = read_named_register(Register::TimeToFull);
    return convert_to_time(raw);
  }
  uint16_t read_protection_status()
  {
    return read_named_register(Register::ProtStatus);
  }
  uint16_t read_protection_alert()
  {
    return read_named_register(Register::ProtAlrt);
  }
  float read_cell1()
  {
    uint16_t raw = read_named_register(Register::Cell1);
    return convert_to_cell_voltage(raw);
  }
  float read_cell2()
  {
    uint16_t raw = read_named_register(Register::Cell2);
    return convert_to_cell_voltage(raw);
  }
  float read_cell3()
  {
    uint16_t raw = read_named_register(Register::Cell3);
    return convert_to_cell_voltage(raw);
  }
  float read_cell4()
  {
    uint16_t raw = read_named_register(Register::Cell4);
    return convert_to_cell_voltage(raw);
  }
  float read_batt()
  {
    uint16_t raw = read_named_register(Register::Batt);
    return convert_to_batt_voltage(raw);
  }
  float read_pckp()
  {
    uint16_t raw = read_named_register(Register::Pckp);
    return convert_to_batt_voltage(raw);
  }

  //////////////////////////////////
  uint16_t read_battery_status()
  {
    return read_named_register_nvm(RegisterNvm::NBattStatus);
  }
  uint16_t read_pack_config()
  {
    return read_named_register_nvm(RegisterNvm::NPackCfg);
  }

  uint16_t read_charge_voltage_config()
  {
    return read_named_register_nvm(RegisterNvm::NJEITAV);
  }
  //////////////////////////////////
  void clear_protection_alert()
  {
    write_named_register(Register::ProtAlrt, 0x0000);
  }
  void unlock_write_protection()
  {
    write_named_register(Register::CommStat, 0x0000);
    write_named_register(Register::CommStat, 0x0000);
  }
  void lock_write_protection()
  {
    write_named_register(Register::CommStat, 0x00F9);
    write_named_register(Register::CommStat, 0x00F9);
  }

  void set_pack_config(uint8_t n_cells, uint8_t n_therms, ThermistorType therm_type, ChargePumpVoltageConfiguration charge_pump_voltage_config,
                       AlwaysOnRegulatorConfiguration always_on_regulator_config, BatteryPackUpdate battery_pack_update)
  {
    // if (n_cells < 2 || n_cells > 4)
    // {
    //   //    throw InvalidConfigurationValue(n_cells);
    // }

    // if (n_therms > 4)
    // {
    //   //    throw InvalidConfigurationValue(n_therms);
    // }
    if ((n_cells >= 2 || n_cells <= 4) && n_therms <= 4)
    {
      n_cells = n_cells - 2;
      n_therms = n_therms << 2;
      uint16_t code = n_cells | n_therms | static_cast<uint16_t>(therm_type) | static_cast<uint16_t>(charge_pump_voltage_config) | static_cast<uint16_t>(always_on_regulator_config) | static_cast<uint16_t>(battery_pack_update);
      unlock_write_protection();
      write_named_register_nvm(RegisterNvm::NPackCfg, code);
      lock_write_protection();
    }
  }

  void set_alert_shutdown_enable(bool enable)
  {
    uint8_t current_nconfig = read_named_register_nvm(RegisterNvm::NConfig);
    uint8_t new_nconfig = enable ? set_bit(current_nconfig, 5) : clear_bit(current_nconfig, 5);
    write_named_register_nvm(RegisterNvm::NConfig, new_nconfig);
  }

  void set_alert_output_enable(bool enable)
  {
    uint16_t current_config = read_named_register(Register::Config);
    uint16_t new_config;
    if (enable)
    {
      new_config = set_bit(current_config, 2);
      set_alert_shutdown_enable(false);
    }
    else
    {
      new_config = clear_bit(current_config, 2);
    }
    write_named_register(Register::Config, new_config);
  }

  void set_voltage_alert_threshold(float min_v, float max_v)
  {
    if (!is_valid_voltage_threshold(max_v))
    {
      //        throw InvalidConfigurationValue(max_v);
    }
    if (!is_valid_voltage_threshold(min_v))
    {
      //        throw InvalidConfigurationValue(min_v);
    }
    uint8_t threshold_array[2] = {
        static_cast<uint8_t>(max_v / VALRTTH_LSB_RESOLUTION),
        static_cast<uint8_t>(min_v / VALRTTH_LSB_RESOLUTION)};
    uint16_t threshold_code = (static_cast<uint16_t>(threshold_array[0]) << 8) | threshold_array[1];
    write_named_register(Register::VAlrtTh, threshold_code);
  }

  void set_undervoltage_protection(float UVP, float UOCVP, float UVShdn)
  {
    if ((UVP >= 2.20 || UVP <= 3.46) && (UOCVP >= UVP || UOCVP <= UVP + 1.28) && (UVShdn >= UVP - 0.32 || UVShdn <= UVP + 0.28))
    {
      uint16_t uvp = (UVP - 2.20) * 1000 / 20;
      uint16_t uocvp = (UOCVP - UVP) * 1000 / 40;
      uint16_t uvshdn = (UVShdn - UVP - 0.32) * 1000 / 40;
      uvp = uvp << 10;
      uocvp = uocvp << 4;
      uint16_t code = uvp | uocvp | uvshdn;
      unlock_write_protection();
      write_named_register_nvm(RegisterNvm::NUVPrtTh, code);
      lock_write_protection();
    }
  }

  void set_cell_balancing(BatteryBalancingZener zener_enable, CellBalancingConfig bal_cfg, float r_mismatch, uint8_t imbalance)
  {
    r_mismatch = 32 * r_mismatch / 125;
    uint16_t RMismatch = round(r_mismatch) << 5;
    imbalance = round(imbalance / 10);
    if (imbalance <= 31 && r_mismatch <= 31)
    {
      uint16_t code = zener_enable | static_cast<uint16_t>(bal_cfg) | static_cast<uint16_t>(RMismatch) | static_cast<uint16_t>(imbalance);
      unlock_write_protection();
      write_named_register_nvm(RegisterNvm::NBalTh, code);
      lock_write_protection();
    }
  }

  void set_overvoltage_protection(uint16_t OVPPermFail, uint16_t ChgDetTh, uint16_t dOVP, uint16_t dOVPR)
  {
    // nJEITAV ChargeVoltage;
    // uint16_t charge_voltage = read_charge_voltage_config();
    // mask_bit_read(charge_voltage, uint8_t read_size, uint8_t left_shift_size)
    if (OVPPermFail <= 150, ChgDetTh <= 80, dOVP <= 150, dOVPR <= 150)
    {
      OVPPermFail = (OVPPermFail / 20) << 12;
      ChgDetTh = (ChgDetTh / 10) << 8;
      dOVP = (dOVP / 10) << 4;
      dOVPR = dOVPR / 10;
      uint16_t code = OVPPermFail | ChgDetTh | dOVP | dOVPR;

      unlock_write_protection();
      write_named_register_nvm(RegisterNvm::NOVPrtTh, code);
      lock_write_protection();
    }
  }
  // array<float,2> read_voltage_alert_threshold() {
  //     uint16_t code = read_named_register(Register::VAlrtTh);
  //     uint8_t raw[2] = {
  //         static_cast<uint8_t>(code >> 8),
  //         static_cast<uint8_t>(code & 0xFF)
  //     };
  //     return raw;
  // }

}

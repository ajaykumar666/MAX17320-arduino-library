#ifndef CONFIG_H
#define CONFIG_H

/// Type of thermistor
enum ThermistorType
{
  /// 10kΩ NTC thermistor
  Ntc10KOhm = 0,
  /// 100kΩ NTC thermistor
  Ntc100KOhm = 1 << 11,
};

/// Charge Pump Voltage Configuration.
/// Set according to the desired gate drive. Note that there is a trade-off in
/// quiescent vs. gate-drive.
enum ChargePumpVoltageConfiguration
{
  /// 6V setting
  Cp6V = 0,
  /// 8V setting
  Cp8V = 1 << 8,
  /// 10V setting
  Cp10V = 1 << 9,
};

/// Always-on Regulator Configuration.
enum AlwaysOnRegulatorConfiguration
{
  /// ALDO is disabled.
  Disabled = 0,
  /// ALDO is enabled. Output is 3.4V
  Enabled3p4V = 1 << 14,
  /// ALDO is enabled Output is 1.8V.
  Enabled1p8V = 1 << 15,
};

/// Enable Pckp and Batt Channels update.
enum BatteryPackUpdate
{
  /// Pckp/Batt channels update every 22.4s
  UpdateEvery22p4s = 0,
  /// Pckp/Batt channels update after all cell measurements are completed
  AfterMeasurementsCompleted = 1 << 13,
};

enum BatteryBalancingZener
{
  ZenerDisable = 0,
  ZenerEnable = 1 << 13,
};

enum CellBalancingConfig
{
  Disable = 0,
  /// For 2.5mV
  mV25 = 1 << 10,
  /// For 5mV
  mV50 = 2 << 10,
  /// For 10mV
  mV100 = 3 << 10,
  /// For 20mV
  mV200 = 4 << 10,
  /// For 40mV
  mV400 = 5 << 10,
  /// For 80mV
  mV800 = 6 << 10,
  /// For 160mV
  mV1600 = 7 << 10,
};
#endif

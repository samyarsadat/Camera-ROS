#include "type_extent.hpp"
#include "exceptions.hpp"
#include "libcamera_version_utils.hpp"
#include <libcamera/base/span.h>
#include <libcamera/control_ids.h>
#include <libcamera/controls.h>
#include <stdexcept>
#include <string>
#include <type_traits>


template<typename T, std::enable_if_t<!libcamera::details::is_span<T>::value, bool> = true>
std::size_t
get_extent(const libcamera::Control<T> &)
{
  // return an extent of 0 for non-span types
  return 0;
}

template<typename T, std::enable_if_t<libcamera::details::is_span<T>::value, bool> = true>
std::size_t
get_extent(const libcamera::Control<T> &)
{
  // return the span extent, excluding 0
  // This assumes that libcamera does not define control types
  // with a fixed size span that does not hold any elements
  constexpr std::size_t extent = libcamera::Control<T>::type::extent;
  static_assert(extent != 0);
  return extent;
}

#define IF(T)                                  \
  if (id->id() == libcamera::controls::T.id()) \
    return get_extent(libcamera::controls::T);


std::size_t
get_extent(const libcamera::ControlId *const id)
{
#if LIBCAMERA_VER_GE(0, 1, 0)
  IF(AeEnable)
  IF(AeLocked)
  IF(AeMeteringMode)
  IF(AeConstraintMode)
  IF(AeExposureMode)
  IF(ExposureValue)
  IF(ExposureTime)
  IF(AnalogueGain)
  IF(Brightness)
  IF(Contrast)
  IF(Lux)
  IF(AwbEnable)
  IF(AwbMode)
  IF(AwbLocked)
  IF(ColourGains)
  IF(ColourTemperature)
  IF(Saturation)
  IF(SensorBlackLevels)
  IF(Sharpness)
  IF(FocusFoM)
  IF(ColourCorrectionMatrix)
  IF(ScalerCrop)
  IF(DigitalGain)
  IF(FrameDuration)
  IF(FrameDurationLimits)
  IF(SensorTimestamp)
  IF(AfMode)
  IF(AfRange)
  IF(AfSpeed)
  IF(AfMetering)
  IF(AfWindows)
  IF(AfTrigger)
  IF(AfPause)
  IF(LensPosition)
  IF(AfState)
  IF(AfPauseState)
#endif

#if LIBCAMERA_VER_GE(0, 2, 0)
  IF(HdrMode)
  IF(AeFlickerPeriod)
  IF(AeFlickerMode)
  IF(draft::NoiseReductionMode)
#ifdef LIBCAMERA_HAS_RPI_VENDOR_CONTROLS
  IF(rpi::StatsOutputEnable)
  IF(rpi::Bcm2835StatsOutput)
#endif
#endif

#if LIBCAMERA_VER_GE(0, 4, 0)
#ifdef LIBCAMERA_HAS_RPI_VENDOR_CONTROLS
#ifdef RASPBERRY_PI_LIBCAMERA
  IF(rpi::CnnEnableInputTensor)
#endif
#endif
#endif

  throw unknown_control(id);
}

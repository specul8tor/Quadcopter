#ifndef LIBSPINE_COMMON_INC_WIRELESS_RADIO_SELECT_H
#define LIBSPINE_COMMON_INC_WIRELESS_RADIO_SELECT_H

#include "A7125Radio.h"
#include "A7196Radio.h"
#include "RT569Radio.h"
#include "RT569DigFPGA.h"
#include "RT569Dig4MFPGA.h"
#include "NoneRadio.h"

enum class RadioType {
  A7125,
  A7196,
  RT569,
  RT569DIG,
  RT569DIG4M,
  None
};

template <RadioType T, typename ...Args>
struct RadioSelect {};

template <typename ...Args>
struct RadioSelect<RadioType::A7196, Args...> {
  using Type = A7196Radio<typename std::remove_reference<Args>::type...>;

  static Type Make(Args&&... args) {
    return MakeA7196Radio(std::forward<Args>(args)...);
  }
};

template <typename ...Args>
struct RadioSelect<RadioType::A7125, Args...> {
  using Type = A7125Radio<typename std::remove_reference<Args>::type...>;

  static Type Make(Args&&... args) {
    return MakeA7125Radio(std::forward<Args>(args)...);
  }
};

template <typename ...Args>
struct RadioSelect<RadioType::RT569, Args...> {
  using Type = RT569Radio<typename std::remove_reference<Args>::type...>;

  static Type Make(Args&&... args) {
    return MakeRT569Radio(std::forward<Args>(args)...);
  }
};

template <typename ...Args>
struct RadioSelect<RadioType::RT569DIG, Args...> {
  using Type = RT569DigFPGA<typename std::remove_reference<Args>::type...>;

  static Type Make(Args&&... args) {
    return MakeRT569DigFPGA(std::forward<Args>(args)...);
  }
};

template <typename ...Args>
struct RadioSelect<RadioType::RT569DIG4M, Args...> {
  using Type = RT569DigFPGA4M<typename std::remove_reference<Args>::type...>;

  static Type Make(Args&&... args) {
    return MakeRT569DigFPGA4M(std::forward<Args>(args)...);
  }
};

template <typename ...Args>
struct RadioSelect<RadioType::None, Args...> {
  using Type = NoneRadio<typename std::remove_reference<Args>::type...>;

  static Type Make(Args&&... args) {
    return MakeNoneRadio(std::forward<Args>(args)...);
  }
};

template <RadioType T, typename ...Args>
typename RadioSelect<T, Args...>::Type MakeRadio(Args&&... args) {
  return RadioSelect<T, Args...>::Make(std::forward<Args>(args)...);
}

#endif


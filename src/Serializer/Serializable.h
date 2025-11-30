//
// serializable.h
//

#ifndef PROJEKT_SERIALIZABLE_H
#define PROJEKT_SERIALIZABLE_H
#include <format>
#include <functional>
#include <string>
#include <memory>
#include <sstream>
#include <optional>
#include <concepts>

#include "ISerializable.h"

template <typename U>
concept HasStaticName = requires {
  { U::static_name() } -> std::convertible_to<const std::string &>;
};

template<typename Derived>
struct InharitanceDescriptor {
    InharitanceDescriptor() {
        static bool factoryRegistered = false;
        if (!factoryRegistered) {
            // Derived::static_name() must exist.
            registerFactory(Derived::static_name(), [](){ return std::make_shared<Derived>(); });
            factoryRegistered = true;
        }
    }
};

template <typename Derived> class Serializable : public ISerializable
{
public:
  Serializable()
  {
    static_assert(HasStaticName<Derived>,
                  "Derived must provide: static const std::string& "
                  "static_name()"); // check concept for static_name

    static bool factoryRegistered = false;
    if(!factoryRegistered)
      { // used to create the child classes during deserialization
        registerFactory(Derived::static_name(),
                        []() { return std::make_shared<Derived>(); });
        factoryRegistered = true;
      }
  }
  ~Serializable() override = default;

  template <typename... Args>
  static std::shared_ptr<Derived> create(Args &&...args)
  {
    return std::make_shared<Derived>(std::forward<Args>(args)...);
  }

  template <typename MemberType, typename ClassType, typename TConverter>
  void registerField(const std::string &field_name,
                     MemberType ClassType::*member_ptr, TConverter converter)
  {
    std::cout << "Registering field: " << field_name << " for " << this->name
              << std::endl;

    auto conv_ptr = std::make_shared<TConverter>(std::move(converter));
    ConverterEntry entry;

    entry.to_str
      = [this, conv_ptr, member_ptr]() -> std::optional<std::string> {
      auto *self = static_cast<ClassType *>(this);
      return conv_ptr->to_string(self->*member_ptr);
    };

    Loader fieldLoader
      = [conv_ptr, member_ptr](std::shared_ptr<ISerializable> obj,
                               const std::string &s) -> bool {
      auto derived_obj = std::dynamic_pointer_cast<ClassType>(obj);
      if(!derived_obj)
        {
          return false; // wrong cast
        }

      auto opt = conv_ptr->from_string(s);
      if(!opt)
        {
          return false;
        }

      (derived_obj.get()->*member_ptr) = *opt;

      return true;
    };

    registerLoader(this->name, fieldLoader, field_name);
    converters[field_name] = std::move(entry);
  }
};

template <typename T>
concept StreamExtractable
  = requires(std::istringstream &is, T &t) { is >> t; };

template <typename T> struct defaultString
{
  std::optional<std::string> to_string(const T &v) const
  {
    if constexpr(std::is_same_v<T, std::string>)
      {
        return v;
      }
    if constexpr(std::is_arithmetic_v<T>)
      {
        return std::to_string(v);
      }
    std::stringstream ss;
    ss << v;
    return ss.str();
  }

  std::optional<T> from_string(const std::string &s) const
    requires StreamExtractable<T>
  {
    if constexpr(std::is_same_v<T, std::string>)
      {
        return s;
      }
    std::istringstream iss(s);
    if(T val; !(iss >> val))
      {
        return std::nullopt;
      }
    else
      {
        return val;
      }
  }
};

#endif // PROJEKT_SERIALIZABLE_H

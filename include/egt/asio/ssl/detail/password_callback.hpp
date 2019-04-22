//
// ssl/detail/password_callback.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2018 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef EGT_ASIO_SSL_DETAIL_PASSWORD_CALLBACK_HPP
#define EGT_ASIO_SSL_DETAIL_PASSWORD_CALLBACK_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <egt/asio/detail/config.hpp>

#include <cstddef>
#include <string>
#include <egt/asio/ssl/context_base.hpp>

#include <egt/asio/detail/push_options.hpp>

namespace egt {
namespace asio {
namespace ssl {
namespace detail {

class password_callback_base
{
public:
  virtual ~password_callback_base()
  {
  }

  virtual std::string call(std::size_t size,
      context_base::password_purpose purpose) = 0;
};

template <typename PasswordCallback>
class password_callback : public password_callback_base
{
public:
  explicit password_callback(PasswordCallback callback)
    : callback_(callback)
  {
  }

  virtual std::string call(std::size_t size,
      context_base::password_purpose purpose)
  {
    return callback_(size, purpose);
  }

private:
  PasswordCallback callback_;
};

} // namespace detail
} // namespace ssl
} // namespace asio
} // namespace egt

#include <egt/asio/detail/pop_options.hpp>

#endif // EGT_ASIO_SSL_DETAIL_PASSWORD_CALLBACK_HPP
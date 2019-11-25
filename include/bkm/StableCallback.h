//    Copyright (C) 2019, Antonio Coratelli
//
//    This library is free software; you can redistribute it and/or
//    modify it under the terms of the GNU Lesser General Public
//    License as published by the Free Software Foundation; either
//    version 2.1 of the License, or (at your option) any later version.
//
//    This library is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//    Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public
//    License along with this library; if not, write to the Free Software
//    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//
#pragma once

#include <functional>
#include <memory>
#include <glibmm/signalproxy.h>

namespace bkm {

class StableCallback {
public:
    using func_type = std::function<void()>;

    StableCallback(Glib::SignalProxy<void>&& signal_proxy, func_type func):
        m_func{std::make_unique<func_type>(std::move(func))},
        m_conn{std::make_unique<sigc::connection>()} {
        auto f = sigc::mem_fun(m_func.get(), &func_type::operator());
        *m_conn = signal_proxy.connect(f);
    }

    ~StableCallback() noexcept {
        if (m_conn and m_conn->connected())
            m_conn->disconnect();
    }

    StableCallback(StableCallback&& other) noexcept = default;
    StableCallback& operator=(StableCallback&& other) noexcept = default;

    StableCallback(StableCallback const& other) = delete;
    StableCallback& operator=(StableCallback const& other) = delete;

private:
    std::unique_ptr<func_type> m_func;
    std::unique_ptr<sigc::connection> m_conn;
};

} // namespace bkm

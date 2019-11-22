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

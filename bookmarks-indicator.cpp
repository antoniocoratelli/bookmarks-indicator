#include <bkm/Config.h>
#include <bkm/MainWindow.h>

#include <gtkmm/application.h>

#include <string>
#include <vector>

Glib::RefPtr<Gtk::Application> gApp;

static void quit_on_signal(int s) {
    std::cout << __func__ << " " << s << std::endl;
    gApp->quit();
}

int main(int argc, char const* const* argv) {
    auto const args = std::vector<std::string>{argv + 1, argv + argc};
    auto const config_path = args.empty() ? std::string{"config"} : args[0];
    auto config = std::make_shared<bkm::Config>(config_path);
    gApp = Gtk::Application::create("com.coratelli.antonio.bookmarks-indicator");
    ::signal(SIGINT, quit_on_signal);
    ::signal(SIGTERM, quit_on_signal);
    auto main_window = std::make_unique<bkm::MainWindow>(config);
    return gApp->run(*main_window);
}

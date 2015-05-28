#include "gui.hpp"
RaftWindow& GUI (int argc, char* argv[]) {
  Glib::RefPtr<Gtk::Application> app =
      Gtk::Application::create(argc, argv,"org.gtkmm.examples.base");
  RaftWindow* raftwindow = new RaftWindow();
  app->run(*raftwindow);
  return *raftwindow
}

RaftWindow::RaftWindow() : m_button("Hello World") {
  set_border_width(10);
  // When the button receives the "clicked" signal, it will call the
  // on_button_clicked() method defined below.
  m_button.signal_clicked().connect(sigc::mem_fun(*this,
              &RaftWindow::on_button_clicked));
  add(m_button);
  m_button.show();
}

void RaftWindow::on_button_clicked()
{
  std::cout << "Hello World" << std::endl;
}




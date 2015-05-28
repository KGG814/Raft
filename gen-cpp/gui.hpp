#include <iostream>
#include <gtkmm.h>


class RaftWindow : public Gtk::Window {

  public:
    RaftWindow();

  protected:
    //Signal handlers:
    void on_button_clicked();

    //Member widgets:
    Gtk::Button m_button;
};

RaftWindow& GUI (int argc, char* argv[]);

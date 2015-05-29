#include <iostream>
#include <gtkmm.h>


class RaftWindow : public Gtk::Window
{
public:
  RaftWindow();
  virtual ~RaftWindow();
  void setText(std::string text);
protected:
  
  //Signal handlers:
  void onButtonQuit();
  void onButtonTransitions();
  void onButtonDebug();

  //Child widgets:
  Gtk::Box box;

  Gtk::ScrolledWindow scrolledWindow;
  Gtk::TextView textView;
  
  Glib::RefPtr<Gtk::TextBuffer> transitions, debug;

  Gtk::ButtonBox buttonBox;
  Gtk::Button buttonQuit, buttonTransitions, buttonDebug;
};


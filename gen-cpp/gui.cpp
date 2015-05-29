#include "gui.hpp"
RaftWindow::RaftWindow()
: m_VBox(Gtk::ORIENTATION_VERTICAL),
  m_Button_Quit("_Quit", true),
  m_Button_Buffer1("Transitions"),
  m_Button_Buffer2("Debug")
{
  set_title("Raft Server");
  set_border_width(5);
  set_default_size(400, 200);

  add(m_VBox);

  //Add the TreeView, inside a ScrolledWindow, with the button underneath:
  scrolledWindow.add(m_TextView);

  //Only show the scrollbars when they are necessary:
  scrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

  box.pack_start(m_ScrolledWindow);

  //Add buttons:
  box.pack_start(buttonBox, Gtk::PACK_SHRINK);

  buttonBox.pack_start(m_Button_Buffer1, Gtk::PACK_SHRINK);
  buttonBox.pack_start(m_Button_Buffer2, Gtk::PACK_SHRINK);
  buttonBox.pack_start(m_Button_Quit, Gtk::PACK_SHRINK);
  buttonBox.set_border_width(5);
  buttonBox.set_spacing(5);
  buttonBox.set_layout(Gtk::BUTTONBOX_END);

  //Connect signals:
  buttonQuit.signal_clicked().connect(sigc::mem_fun(*this,
              &RaftWindow::onButtonQuit) );
  transitions.signal_clicked().connect(sigc::mem_fun(*this,
              &RaftWindow::onButtonTransitions) );
  debug.signal_clicked().connect(sigc::mem_fun(*this,
              &RaftWindow::onButtonDebug) );

  onbuttonTransitions();

  show_all_children();
}


void RaftWindow::setText(std::string text) {
  m_refTextBuffer1->set_text(text);
}

void RaftWindow::fill_buffers()
{
  m_refTextBuffer1 = Gtk::TextBuffer::create();
  m_refTextBuffer1->set_text("This is the text from TextBuffer #1.");

  m_refTextBuffer2 = Gtk::TextBuffer::create();
  m_refTextBuffer2->set_text(
          "This is some alternative text, from TextBuffer #2.");

}

RaftWindow::~RaftWindow()
{
}

void RaftWindow::on_button_quit()
{
  hide();
}

void RaftWindow::on_button_buffer1()
{
  m_TextView.set_buffer(m_refTextBuffer1);
}

void RaftWindow::on_button_buffer2()
{
  m_TextView.set_buffer(m_refTextBuffer2);
}



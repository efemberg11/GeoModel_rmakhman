#ifndef _BETTERTEXTBROWSER_H_
#define _BETTERTEXTBROWSER_H_
#include <QTextBrowser>
#include <QMenu>
#include <QAction>
#include <iostream>
#include <QContextMenuEvent>
#include <QIcon>
#include <QStyle>
#include "fsl_mainwindow.h"
class BetterTextBrowser:public QTextBrowser {
  Q_OBJECT


 public:

  // Constructor:
  BetterTextBrowser(QWidget * parent):QTextBrowser(parent){}

  // Destructor:
  ~BetterTextBrowser() {}

  // Set main window:
  void setMainWindow(FSLMainWindow *mainWindow) {
    m_mainWindow=mainWindow;
  }
  
  // Override from base class. Add an action for "save text to file"
  void contextMenuEvent(QContextMenuEvent *event) {

    QMenu *menu = createStandardContextMenu();
    QAction *action=menu->addAction(tr("Save to file"));

    
    action->setIcon((style()->standardIcon(QStyle::StandardPixmap(QStyle::SP_DialogSaveButton), nullptr, this)));
    action->setEnabled(!document()->isEmpty());

    action->setShortcut(QKeySequence(QKeySequence(Qt::CTRL + Qt::Key_S)));
    connect(action, &QAction::triggered, m_mainWindow, &FSLMainWindow::save_display_output);
    menu->exec(event->globalPos());
    delete menu;
  }

  
 private:
  FSLMainWindow *m_mainWindow {nullptr};

};
#endif

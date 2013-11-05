//
// Copyright (c) 2013 Hong Jen Yee (PCMan) <pcman.tw@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#ifndef XDG_DESKTOPSETTINGS_H
#define XDG_DESKTOPSETTINGS_H

#ifdef COMPILE_XDSETTINGS
#define XDSETTINGS_API	Q_DECL_EXPORT
#else
#define XDSETTINGS_API	Q_DECL_IMPORT
#endif

#include <QObject>
#include <QString>
#include <QFileSystemWatcher>
#include <QApplication> // for x11EventFilter
#include <QVector>
#include <QTimer>

// We cannot include #include <X11/Xlib.h> along with Qt headers due to some name clashes
// caused by various macros defined in Xlib.
typedef struct _XSettingsClient XSettingsClient;
typedef struct _XSettingsSetting XSettingsSetting;

namespace Xdg {

// cross-desktop global settings, such as icon theme name, ...
class XDSETTINGS_API DesktopSettings : public QObject {
  Q_OBJECT
public:

  DesktopSettings();
  virtual ~DesktopSettings();
  
  QString iconThemeName() const {
    return iconThemeName_;
  }

  QString desktopEnvironment() const {
    return desktopEnvironment_;
  }

  // FIXME: this will stop working in Qt 5 :-(
  bool x11EventFilter(XEvent* event); // needs to be called by QApplication

Q_SIGNALS:
  void changed();

private:
  void detectDesktopEnvironment();
  void loadSettings();

  static void xsettingsNotify(const char *name,
                              int action, // we replace enum with int here
                              XSettingsSetting *setting, DesktopSettings* pThis);
  static void xsettingsWatch(unsigned long window, int is_start, long mask, DesktopSettings* pThis);

  void queueEmitChanged();

private Q_SLOTS:
  void onConfigFileChanged(QString path);
  void emitChanged();
  
private:
  QString desktopEnvironment_;
  QString iconThemeName_;
  QString configFilePath_;
  QFileSystemWatcher* configFileWatcher_;
  XSettingsClient* xsettings_;
  QVector<unsigned long> watchedWindows_;
  QTimer* delayEmitTimeout_;
};

};

#endif // XDG_DESKTOPSETTINGS_H

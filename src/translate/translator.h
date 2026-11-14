#pragma once

#include "stfwd.h"

#include <QWidget>

class QWebEngineView;
class QTabWidget;
class QLineEdit;
class QVBoxLayout;
class QWidget;

class WebPage;

class Translator : public QWidget
{
  Q_OBJECT
public:
  Translator(Manager &manager, const Settings &settings);
  ~Translator();

  void translate(const TaskPtr &task);
  void updateSettings();
  void finish(const TaskPtr &task);

  static QStringList availableTranslators(const QString &path);
  static QStringList availableLanguageNames();

protected:
  void timerEvent(QTimerEvent *event) override;
  void hideEvent(QHideEvent *event) override;
  void showEvent(QShowEvent *event) override;

private:
  WebPage *currentPage() const;
  void udpateCurrentPage();
  void updateUrl();
  void setPageLoadImages(bool isOn);
  void processQueue();
  void markTranslated(const TaskPtr &task);
  void createPage(const QString &scriptName, const QString &scriptText);
  void destroyPage(const QString &scriptName);
  void showDebugView();

  // A translator page has to live in a view which Qt considers visible,
  // otherwise the browser engine stops updating the page: the translator
  // site never produces a result and the translation times out until the
  // translator window is opened. The pages are therefore kept in the views
  // of an off-screen host window (WA_DontShowOnScreen), which is "shown"
  // for Qt but never appears on the screen.
  void ensureRenderHost();
  void attachPageView(WebPage *page);
  void detachPageView();

  Manager &manager_;
  const Settings &settings_;
  QWidget *viewSlot_;
  QVBoxLayout *viewSlotLayout_;
  std::unique_ptr<QWebEngineView> debugView_;
  QLineEdit *url_;
  QAction *loadImages_;
  QAction *showDebugAction_;
  QTabWidget *tabs_;
  std::vector<TaskPtr> queue_;
  std::map<QString, std::unique_ptr<WebPage>> pages_;
  std::map<QString, QWebEngineView *> pageViews_;
  QWidget *renderHost_{nullptr};
  QString shownPageName_;
  quint16 debugPort_{0};
};

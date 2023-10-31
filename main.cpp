#include <QApplication>
#include <QMainWindow>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QRegularExpression>
#include <QLabel>
#include <QSettings>
#include <QFile>
#include <QDebug>

#include "ui_mainwindow.h"

#define VERSION "0.1.0"
#define APPNAME "Giduba"
#define CONFIG_PATH QDir::homePath()+"/.config/giduba/"
#define CONFIG_FILE_PATH CONFIG_PATH + "giduba.conf"

static QSettings * settings;

static bool is_saved = false;
static bool is_wrap = true;
static QString current_file = "";
static int line_count;
static int word_count;
static int char_count;

void updateStatus(Ui::MainWindow & main_ui){
    auto charCount = QString::number(char_count);
    auto lineCount = QString::number(line_count);
    auto wordCount = QString::number(word_count);
    auto saved = is_saved ? "[ saved ]" : "[ not saved ]";

    auto statusText =
    charCount+" chars | "+
    wordCount+" words | "+
    lineCount+" lines | "+
    saved;

    main_ui.status->showMessage(statusText);
}

void saveTxt(Ui::MainWindow & main_ui){
    assert(!current_file.isEmpty());
    QFile f(current_file);
    f.open(QIODevice::ReadWrite);
    auto txt = main_ui.editor->toPlainText().toUtf8();
    f.write(txt);
    f.close();
    is_saved=true;
    updateStatus(main_ui);
}

void openTxt(QWidget & window, Ui::MainWindow & main_ui){
    assert(!current_file.isEmpty());
    QFile f(current_file);
    auto max_file_size_mb = settings->value("max_file_size_mb", QVariant(512)).toInt();
    auto max_file_size = max_file_size_mb * 1024 * 1024;
    if(f.size() >= max_file_size){
        auto size_megs = QString::number(f.size() / 1024.0*1024.0)+"MB";
        auto max_size_megs = QString::number(max_file_size_mb) + "MB";
        QMessageBox::information(&window, "File too large",
                                 "The file has "+size_megs+" but the maximum is"+max_size_megs);
    }else{
        f.open(QIODevice::ReadOnly);
        QByteArray text = f.read(max_file_size);
        QTextStream stream(text);
        stream.setAutoDetectUnicode(true);
        main_ui.editor->setText(stream.readAll());
        f.close();
        is_saved=true;
    }
    updateStatus(main_ui);
}

void newTxt(Ui::MainWindow & main_ui){
    main_ui.editor->clear();
    is_saved=false;
    current_file="";
}

void setUpTextEditor(Ui::MainWindow & main_ui){
    if(is_wrap){
        main_ui.editor->setLineWrapMode(QTextEdit::WidgetWidth);
    }else{
        main_ui.editor->setLineWrapMode(QTextEdit::NoWrap);
    }

    main_ui.editor->setVerticalScrollBarPolicy(
        settings->value("always_show_bar", QVariant(true)).toBool()?
        Qt::ScrollBarAlwaysOn : Qt::ScrollBarAsNeeded
    );

    if(is_wrap){
        main_ui.editor->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }else{
        main_ui.editor->setHorizontalScrollBarPolicy(
            settings->value("always_show_bar", QVariant(true)).toBool()?
            Qt::ScrollBarAlwaysOn : Qt::ScrollBarAsNeeded
    );

    }
    main_ui.editor->setFontPointSize(
        settings->value("default_font_size", QVariant(12)).toInt()
    );
}

void addFileActions(QWidget & window, Ui::MainWindow & main_ui){

    QObject::connect(main_ui.actionSaveAs, &QAction::triggered, [&]() {
        QString filePath = QFileDialog::getSaveFileName(&window, "Save File", QDir::homePath());
        if (!filePath.isEmpty()) {
            current_file = filePath;
            saveTxt(main_ui);
        }
    });

    QObject::connect(main_ui.actionSave, &QAction::triggered, [&]() {
        if (current_file.isEmpty()) {
            main_ui.actionSaveAs->trigger();
        }else{
            saveTxt(main_ui);
        }
    });

    QObject::connect(main_ui.actionOpen, &QAction::triggered, [&]() {
        QString filePath = QFileDialog::getOpenFileName(&window, "Open File", QDir::homePath());
        if (!filePath.isEmpty()) {
            current_file = filePath;
            openTxt(window, main_ui);
        }
    });

    QObject::connect(main_ui.actionNew, &QAction::triggered, [&]() {
        if (! is_saved) {
            int response = QMessageBox::question(&window, "Unsaved Changes", "Do you want to save your changes?","Yes", "No");
            if (response == 0){ //yes
                main_ui.actionSave->trigger();
            }
        }
        newTxt(main_ui);
    });
    QObject::connect(main_ui.actionExit, &QAction::triggered, [&]() {
        if (! is_saved) {
            int response = QMessageBox::question(&window, "Unsaved Changes", "Do you want to save your changes?","Yes", "No");
            if (response == 0){ //yes
                main_ui.actionSave->trigger();
            }
        }
        window.close();

    });
}

void addEditActions(Ui::MainWindow & main_ui){
    QObject::connect(main_ui.actionSelectAll, &QAction::triggered, [&]() {
        main_ui.editor->selectAll();
    });

    QObject::connect(main_ui.actionUndo, &QAction::triggered, [&]() {
        main_ui.editor->undo();
    });

    QObject::connect(main_ui.actionRedo, &QAction::triggered, [&]() {
        main_ui.editor->redo();
    });

    QObject::connect(main_ui.actionCut, &QAction::triggered, [&]() {
        main_ui.editor->cut();
    });

    QObject::connect(main_ui.actionCopy, &QAction::triggered, [&]() {
        main_ui.editor->copy();
    });

    QObject::connect(main_ui.actionPaste, &QAction::triggered, [&]() {
        main_ui.editor->paste();
    });

    QObject::connect(main_ui.actionLineWrap, &QAction::triggered, [&]() {
        is_wrap = main_ui.actionLineWrap->isChecked();
        setUpTextEditor(main_ui);
    });
}

static QString helpStr = "";
static QString aboutStr =
QString(APPNAME) + "\n" +
"Version "+VERSION+"\n"+__DATE__"\n\n"+
"Licensed under the GNU Public License Version 3\n"+
"See LICENSE for details\n\n"+
"Built with:\n"+
"GCC " + __VERSION__+"\n"+
"QT "+QT_VERSION_STR;

void addHelpActions(QWidget & window, Ui::MainWindow & main_ui){

    QObject::connect(main_ui.actionHelp, &QAction::triggered, [&]() {
        QMessageBox::information(&window, "Help", helpStr);
    });
    QObject::connect(main_ui.actionAbout, &QAction::triggered, [&]() {
        QMessageBox::information(&window, "About",aboutStr);
    });
}

// used to disable menu tooltips on the statusbar
class StatusTipFilter : public QObject
{


protected:
    bool eventFilter(QObject *watched, QEvent *event) override
    {
        if (event->type() == QEvent::StatusTip)
        {
            return true;
        }
        return QObject::eventFilter(watched, event);
    }
};


void setUpStatusEvents(Ui::MainWindow & main_ui){
    // update status on text change
    QObject::connect(main_ui.editor, &QTextEdit::textChanged, [&]() {
        is_saved = false;

        auto text = main_ui.editor->toPlainText();
        const QRegularExpression word_delim_regex = QRegularExpression("\\s+");

        word_count = text.split(word_delim_regex, Qt::SkipEmptyParts).count();
        line_count = main_ui.editor->document()->lineCount();
        char_count = main_ui.editor->document()->characterCount();

        updateStatus(main_ui);

    });

    // disable menu tooltips on the statusbar
    main_ui.menu->installEventFilter(new StatusTipFilter());

    updateStatus(main_ui);
}

int main(int argc, char** argv)
{
    if (!QFile(CONFIG_FILE_PATH).exists()){
        QDir().mkdir(CONFIG_PATH);
        settings = new QSettings(CONFIG_FILE_PATH, QSettings::IniFormat);
        settings->setValue("max_file_size_mb",QVariant(512));
        settings->setValue("default_wrap",QVariant(true));
        settings->setValue("default_font_size",QVariant(12));
        settings->setValue("always_show_bar", QVariant(true));
        settings->sync();
    }

    settings = new QSettings(CONFIG_FILE_PATH, QSettings::IniFormat);

    is_wrap = settings->value("default_wrap", QVariant(true)).toBool();

    QApplication app(argc, argv);

    QMainWindow w;

    //set icon
    QIcon icon(":/icons/icon.ico");
    app.setWindowIcon(icon);

    Ui::MainWindow main;

    main.setupUi(&w);
    w.show();

    addFileActions(w, main);
    addEditActions(main);
    addHelpActions(w, main);

    setUpTextEditor(main);
    setUpStatusEvents(main);

    return app.exec();
}

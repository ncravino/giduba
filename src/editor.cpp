
#include "editor.hpp"

#if QT_VERSION >= QT_VERSION_CHECK(5, 13, 0)
#define SKIPEMPTYPARTS Qt::SkipEmptyParts
#else
#define SKIPEMPTYPARTS QString::SkipEmptyParts
#endif

static inline void save_txt(Ui::MainWindow * main_ui, QString & current_file){
    assert(!current_file.isEmpty());
    QFile f(current_file);
    f.open(QIODevice::ReadWrite);
    auto txt = main_ui->editor->toPlainText().toUtf8();
    f.write(txt);
    f.close();
}

static inline void open_txt(QWidget * window, Ui::MainWindow * main_ui, QString & current_file, Config * conf){
    assert(!current_file.isEmpty());
    QFile f(current_file);
    auto max_file_size_mb = conf->max_file_size_mb();
    auto max_file_size = max_file_size_mb * 1024 * 1024;
    if(f.size() >= max_file_size){
        auto size_megs = QString::number(f.size() / 1024.0*1024.0)+"MB";
        auto max_size_megs = QString::number(max_file_size_mb) + "MB";
        QMessageBox::information(window, "File too large",
                                 "The file has "+size_megs+" but the maximum is"+max_size_megs);
    }else{
        f.open(QIODevice::ReadOnly);
        QByteArray text = f.read(max_file_size);
        QTextStream stream(text);
        stream.setAutoDetectUnicode(true);
        main_ui->editor->setText(stream.readAll());
        f.close();
    }
}

void Editor::setup_file_actions(){
        QObject::connect(this->main->actionSaveAs, &QAction::triggered, [&]() {
        QString filePath = QFileDialog::getSaveFileName(this->window, "Save File", QDir::homePath());
        if (!filePath.isEmpty()) {
            this->current_file = filePath;
            save_txt(this->main, current_file);
            this->is_saved = true;
            this->update_status();
        }
    });

    QObject::connect(this->main->actionSave, &QAction::triggered, [&]() {
        if (this->current_file.isEmpty()) {
            this->main->actionSaveAs->trigger();
        }else{
            save_txt(this->main,current_file);
            this->is_saved = true;
            this->update_status();
        }
    });

    QObject::connect(this->main->actionOpen, &QAction::triggered, [&]() {
        QString filePath = QFileDialog::getOpenFileName(this->window, "Open File", QDir::homePath());
        if (!filePath.isEmpty()) {
            this->current_file = filePath;
            open_txt(this->window, this->main, this->current_file, this->config);
            this->is_saved = true;
            this->update_status();
        }
    });

    QObject::connect(this->main->actionNew, &QAction::triggered, [&]() {
        if (! this->is_saved) {
            int response = QMessageBox::question(this->window, "Unsaved Changes", "Do you want to save your changes?","Yes", "No");
            if (response == 0){ //yes
                this->main->actionSave->trigger();
            }
        }
        this->main->editor->clear();
        this->current_file = "";
        is_saved=false;
    });

    QObject::connect(this->main->actionExit, &QAction::triggered, [&]() {
        if (! this->is_saved) {
            int response = QMessageBox::question(this->window, "Unsaved Changes", "Do you want to save your changes?","Yes", "No");
            if (response == 0){ //yes
                this->main->actionSave->trigger();
            }
        }
        this->app->exit();

    });
}

void Editor::setup_edit_actions(){
    QObject::connect(this->main->actionSelectAll, &QAction::triggered, [&]() {
        this->main->editor->selectAll();
    });

    QObject::connect(this->main->actionUndo, &QAction::triggered, [&]() {
        this->main->editor->undo();
    });

    QObject::connect(this->main->actionRedo, &QAction::triggered, [&]() {
        this->main->editor->redo();
    });

    QObject::connect(this->main->actionCut, &QAction::triggered, [&]() {
        this->main->editor->cut();
    });

    QObject::connect(this->main->actionCopy, &QAction::triggered, [&]() {
        this->main->editor->copy();
    });

    QObject::connect(this->main->actionPaste, &QAction::triggered, [&]() {
        this->main->editor->paste();
    });

    QObject::connect(this->main->actionLineWrap, &QAction::triggered, [&]() {
        this->config->set_wrap(this->main->actionLineWrap->isChecked());
        this->setup_editor();
    });
}


void Editor::update_status(){
        auto charCount = QString::number(char_count);
        auto lineCount = QString::number(line_count);
        auto wordCount = QString::number(word_count);
        auto saved = is_saved ? "[ saved ]" : "[ not saved ]";

        auto statusText =
        charCount+" chars | "+
        wordCount+" words | "+
        lineCount+" lines | "+
        saved;

        this->main->status->showMessage(statusText);
}

static const QString aboutStr =
    "<h1>"+QString(APPNAME) + "</h1>" +
    "Version "+VERSION+"<br>"+__DATE__"<br><br>"+
    "<a href='https://github.com/ncravino/giduba'>https://github.com/ncravino/giduba</a><br><br>"+
    "Giduba Copyright (C) 2023 Nuno Cravino<br><br>"+
    "This program is free software: you can redistribute it and/or modify "+
    "it under the terms of the GNU General Public License as published by "+
    "the Free Software Foundation, either version 3 of the License, or "+
    "(at your option) any later version.<br><br>"+
    "This program is distributed in the hope that it will be useful, "+
    "but WITHOUT ANY WARRANTY; without even the implied warranty of "+
    "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the "+
    "GNU General Public License for more details.<br><br>"+
    "You should have received a copy of the GNU General Public License "+
    "along with this program. If not, see <a href='https://www.gnu.org/licenses'>"+
    "https://www.gnu.org/licenses</a>.<br><br>"+
    "Built with:<br>"+
    "GCC " + __VERSION__+"<br>"+
    "QT "+QT_VERSION_STR;

void Editor::setup_help_actions(){
    QObject::connect(this->main->actionAbout, &QAction::triggered, [&]() {
        QMessageBox msg(this->window);
        msg.setMinimumWidth(400);
        msg.setIconPixmap(this->icon.pixmap(150,150));
        msg.setWindowTitle("About");
        msg.setTextFormat(Qt::RichText);
        msg.setText(aboutStr);
        msg.exec();
    });
}

void Editor::setup_search_replace(){
        QObject::connect(this->main->actionToggleSearch, &QAction::triggered, [&]() {
        this->main->searchReplaceElement->setVisible(!this->main->searchReplaceElement->isVisible());
        this->main->searchBox->setFocus();
    });

    QObject::connect(this->main->actionSearch, &QAction::triggered, [&]() {
        if(!this->main->searchReplaceElement->isVisible()){
            this->main->actionToggleSearch->trigger();
            last_search_pos=0;
        }else if(this->main->searchBox->text().isEmpty()){
            qDebug("empty");
            last_search_pos=0;
        }else{
            auto searchTerm = this->main->searchBox->text();
            auto loc = this->main->editor->document()->find(searchTerm, last_search_pos);
            if(loc.position()>=0){
                this->main->editor->setTextCursor(loc);
                last_search_pos = loc.position();
            }else if(last_search_pos > 0){
                last_search_pos=0;
                this->main->actionSearch->trigger();
            }
        }
    });

    QObject::connect(this->main->actionReplace, &QAction::triggered, [&]() {
        if(!this->main->searchReplaceElement->isVisible()){
            this->main->actionToggleSearch->trigger();
            last_search_pos=0;
        }else if(this->main->searchBox->text().isEmpty()){

            last_search_pos=0;
        }else{
            auto searchTerm = this->main->searchBox->text();
            auto replaceText = this->main->replaceBox->text();
            auto start_loc = last_search_pos>=searchTerm.length() ? last_search_pos-searchTerm.length() : 0;
            auto loc = this->main->editor->document()->find(searchTerm, start_loc);
            if(loc.position()>=0){

                loc.removeSelectedText();
                loc.insertText(replaceText);
                this->main->editor->setTextCursor(loc);
                last_search_pos = loc.position();

            }else if(last_search_pos > 0){
                last_search_pos=0;
                this->main->actionReplace->trigger();
            }
        }
    });

    QObject::connect(this->main->searchButton, &QPushButton::clicked, [&]() {
        this->main->actionSearch->trigger();
    });

    QObject::connect(this->main->replaceButton, &QPushButton::clicked, [&]() {
        this->main->actionReplace->trigger();
    });


    QObject::connect(this->main->searchBox, &QLineEdit::returnPressed, [&]() {
        this->main->actionSearch->trigger();
    });

    QObject::connect(this->main->replaceBox, &QLineEdit::returnPressed, [&]() {
        this->main->actionReplace->trigger();
    });
}

void Editor::setup_status_events(){

    class StatusTipFilter : public QObject {
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
    //prevent menu bar events on status bar
    this->main->menu->installEventFilter(new StatusTipFilter());


    // update status on text change
    QObject::connect(this->main->editor, &QTextEdit::textChanged, [&]() {
        is_saved = false;

        auto text = this->main->editor->toPlainText();
        const QRegularExpression word_delim_regex = QRegularExpression("\\s+");

        word_count = text.split(word_delim_regex, SKIPEMPTYPARTS).count();
        line_count = this->main->editor->document()->lineCount();
        char_count = this->main->editor->document()->characterCount();

        this->update_status();
    });

    this->update_status();


}

void Editor::reload_geom(){

    QFile f(CONFIG_GEOM_FILE);
    if(f.exists()){
        f.open(QIODevice::ReadOnly);
        auto geom = f.readAll();
        f.close();
        this->window->restoreGeometry(geom);
    }
}

void Editor::save_geom(){
    auto geom = this->window->saveGeometry();
    QFile f(CONFIG_GEOM_FILE);
    f.open(QIODevice::ReadWrite);
    f.write(geom);
    f.close();

}

void Editor::setup_editor(){

    if(this->config->wrap()){
        this->main->editor->setLineWrapMode(QTextEdit::WidgetWidth);

        this->main->editor->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        this->main->editor->setVerticalScrollBarPolicy(
            this->config->show_scroll_bars()?
            Qt::ScrollBarAlwaysOn : Qt::ScrollBarAsNeeded);
    }else{
        this->main->editor->setLineWrapMode(QTextEdit::NoWrap);

        this->main->editor->setVerticalScrollBarPolicy(
            this->config->show_scroll_bars()?
            Qt::ScrollBarAlwaysOn : Qt::ScrollBarAsNeeded);

        this->main->editor->setHorizontalScrollBarPolicy(
            this->config->show_scroll_bars()?
            Qt::ScrollBarAlwaysOn : Qt::ScrollBarAsNeeded);
    }

    this->main->editor->setFontPointSize(
        this->config->font_size()
    );

}

int Editor::run(){

    this->reload_geom();

    this->setup_editor();
    this->setup_file_actions();
    this->setup_edit_actions();
    this->setup_help_actions();
    this->setup_search_replace();
    this->setup_status_events();


    this->window->show();
    auto exit_code = this->app->exec();

    this->save_geom();
    this->config->save();

    return exit_code;
}

Editor::Editor(int argc, char ** argv){

        this->config = new Config();
        this->config->load();

        this->app = new QApplication (argc, argv);
        this->window = new QMainWindow();
        this->main = new Ui::MainWindow();

        this->app->setWindowIcon(this->icon);
        QApplication::setWindowIcon(this->icon);
        QApplication::setDesktopFileName("com.ncravino.giduba"); //needed for wayland et al to show icon in window

        this->app->setApplicationVersion(QString(VERSION));
        this->app->setApplicationDisplayName("Giduba");
        this->app->setApplicationName("com.ncravino.giduba");
        this->app->setQuitOnLastWindowClosed(true);

        this->main->setupUi(this->window);

        QCommandLineParser cmd;
        auto version_opt = cmd.addVersionOption();
        auto help_opt = cmd.addHelpOption();
        cmd.addPositionalArgument("[filename]", "An optional filename to open");

        if (!cmd.parse(this->app->arguments())){
            std::fputs(qPrintable(cmd.errorText()+"\n") ,stderr);
            cmd.showHelp(1);
            //exit 1
        }

        if (cmd.isSet(version_opt)){
            cmd.showVersion();
            //exit 0
        }

        if (cmd.isSet(help_opt)){
            cmd.showHelp();
            //exit 0
        }

        auto  pos_args = cmd.positionalArguments();
        if (!pos_args.isEmpty()){
            this->current_file = pos_args.first();
            open_txt(this->window, this->main, this->current_file, this->config);
        }

}

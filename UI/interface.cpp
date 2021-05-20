//
// Created by afentev on 4/21/21.
//

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>
#include "interface.h"

#include <iostream>
#include <QMessageBox>

void Ui_MainWindow::setupUi() {
    if (this->objectName().isEmpty())
        this->setObjectName(QStringLiteral("this"));
    this->resize(888, 575);
    this->setMinimumSize(QSize(0, 0));
    this->setMaximumSize(QSize(1068, 620));
    centralwidget = new QWidget(this);
    centralwidget->setObjectName(QStringLiteral("centralwidget"));
    comboBox = new QComboBox(centralwidget);
    comboBox->setObjectName(QStringLiteral("comboBox"));
    comboBox->setGeometry(QRect(690, 386, 104, 26));
    comboBox->setCurrentText(QStringLiteral("Beginner"));
    label = new QLabel(centralwidget);
    label->setObjectName(QStringLiteral("label"));
    label->setGeometry(QRect(600, 390, 81, 16));
    checkBox = new QCheckBox(centralwidget);
    checkBox->setObjectName(QStringLiteral("checkBox"));
    checkBox->setGeometry(QRect(600, 420, 171, 20));
    checkBox->setChecked(true);
    checkBox_2 = new QCheckBox(centralwidget);
    checkBox_2->setObjectName(QStringLiteral("checkBox_2"));
    checkBox_2->setGeometry(QRect(600, 450, 171, 20));
    checkBox_2->setChecked(true);
    textBrowser = new QTextBrowser(centralwidget);
    textBrowser->setObjectName(QStringLiteral("textBrowser"));
    textBrowser->setGeometry(QRect(600, 10, 271, 341));
    textBrowser->setReadOnly(true);
    line = new QFrame(centralwidget);
    line->setObjectName(QStringLiteral("line"));
    line->setGeometry(QRect(580, 10, 16, 551));
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Sunken);
    line_2 = new QFrame(centralwidget);
    line_2->setObjectName(QStringLiteral("line_2"));
    line_2->setGeometry(QRect(590, 360, 281, 16));
    line_2->setFrameShape(QFrame::HLine);
    line_2->setFrameShadow(QFrame::Sunken);
    line_3 = new QFrame(centralwidget);
    line_3->setObjectName(QStringLiteral("line_3"));
    line_3->setGeometry(QRect(590, 480, 281, 16));
    line_3->setFrameShape(QFrame::HLine);
    line_3->setFrameShadow(QFrame::Sunken);
    pushButton_2 = new QPushButton(centralwidget);
    pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
    pushButton_2->setGeometry(QRect(590, 500, 281, 32));
    pushButton_3 = new QPushButton(centralwidget);
    pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
    pushButton_3->setGeometry(QRect(590, 530, 281, 32));
    this->setCentralWidget(centralwidget);

    retranslateUi(this);

    QMetaObject::connectSlotsByName(this);

    scene = new QGraphicsScene(centralwidget);
    view = new QGraphicsView(centralwidget);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setScene(scene);
    view->setGeometry(18, 8, 555, 555);
    scene->setSceneRect(19, 9, 552, 552);

    connect(pushButton_2, &QPushButton::pressed, this, &Ui_MainWindow::handleButton);

    init();
    zErrMsg = nullptr;
    sqlite3_open("../Engine/test1.db", &db);

    this->rules = new thc::ChessRules;
    rules->Init();
    auto* copy = new thc::ChessRules;
    *copy = *rules;
    this->history.push_back(copy);
    this->historicalIndex = 0;
    this->movesHistory.emplace_back(-1, -1);

    if (this->isAiBlack) {
        std::vector<bool> temp1;
        this->rules->GenLegalMoveList(moves,temp1, mates, stalemates);
    } else {
        std::string mv = getMove(rules, hist[hist.size() - 1], useOpenings, useEndings);
        thc::Move m{};
        std::string sM;

        m.NaturalInFast(rules, mv.c_str());
        sM = m.TerseOut();
        std::cout << sM << std::endl;

        hist.push_back(hist[hist.size() - 1].move(
                {parse(sM.substr(0, 2), isAiBlack, false),
                 parse(sM.substr(2, 2), isAiBlack, false)}));

        std::vector<bool> temp1;
        moves.clear();
        mates.clear();
        stalemates.clear();
        this->rules->GenLegalMoveList(moves, temp1, mates, stalemates);

        printPos(hist[hist.size() - 1]);
    }

    redrawField();
    this->pushButton_2->setText("Start game");

    fillTables(this->isAiBlack);
}

void Ui_MainWindow::startGame(bool redraw) {
    srand(time(nullptr));

    this->pushButton_2->setText("Resign");

    useOpenings = this->checkBox->isChecked();
    useEndings = this->checkBox_2->isChecked();
    if (this->comboBox->currentText() == "Beginner") {
        type = 0;
    } else if (this->comboBox->currentText() == "Standard") {
        type = 1;
    } else {
        type = 2;
    }
    this->checkBox->setEnabled(false);
    this->checkBox_2->setEnabled(false);
    this->comboBox->setEnabled(false);
    QCoreApplication::processEvents();

    delete this->rules;

    this->rules = new thc::ChessRules;
    rules->Init();

    for (auto pos: this->history) {
        delete pos;
    }

    this->moves.clear();
    this->mates.clear();
    this->stalemates.clear();
    this->history.clear();
    this->movesHistory.clear();

    this->hist.clear();
    this->hist.push_back(IterativeTablesEngine(initial, 0, {true, true}, {true, true},
                                               0, 0,std::unordered_map<std::string, int> (),
                                               76.0, 16, isAiBlack));

    this->textBrowser->clear();

    auto* copy = new thc::ChessRules;
    *copy = *rules;
    this->history.push_back(copy);
    this->historicalIndex = 0;
    this->movesHistory.emplace_back(-1, -1);

    this->choice = nullptr;

    if (this->isAiBlack) {
        std::vector<bool> temp1;
        this->rules->GenLegalMoveList(moves,temp1, mates, stalemates);
    } else {
        std::string mv = getMove(rules, hist[hist.size() - 1], useOpenings, useEndings);
        thc::Move m{};
        std::string sM;

        m.NaturalInFast(rules, mv.c_str());
        sM = m.TerseOut();
        std::cout << sM << std::endl;

        hist.push_back(hist[hist.size() - 1].move(
                {parse(sM.substr(0, 2), isAiBlack, false),
                 parse(sM.substr(2, 2), isAiBlack, false)}));

        std::vector<bool> temp1;
        moves.clear();
        mates.clear();
        stalemates.clear();
        this->rules->GenLegalMoveList(moves, temp1, mates, stalemates);

        printPos(hist[hist.size() - 1]);
    }

    if (redraw) {
        redrawField();
    }
}

void Ui_MainWindow::retranslateUi(QMainWindow *MainWindow) const {
    MainWindow->setWindowTitle(QApplication::translate("PerChess", "PerChess",
                                                       Q_NULLPTR));
    comboBox->clear();
    comboBox->insertItems(0, QStringList()
            << QApplication::translate("MainWindow", "Beginner", Q_NULLPTR)
            << QApplication::translate("MainWindow", "Standard", Q_NULLPTR)
            << QApplication::translate("MainWindow", "Pro", Q_NULLPTR)
    );
    label->setText(QApplication::translate("MainWindow", "Engine level", Q_NULLPTR));
    checkBox->setText(QApplication::translate("MainWindow", "Use openings database",
                                              Q_NULLPTR));
    checkBox_2->setText(QApplication::translate("MainWindow", "Use endings database",
                                                Q_NULLPTR));
    pushButton_2->setText(QApplication::translate("MainWindow", "Resign", Q_NULLPTR));
    pushButton_3->setText(QApplication::translate("MainWindow", "Swap colors",
                                                  Q_NULLPTR));
}

void Ui_MainWindow::redrawField() {
    if (!isAiBlack) {
        H8 = 91;
        A1 = 28;
        A8 = 98;
        H1 = 21;
    }

    scene->clear();

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            auto* item = new Square;
            item->parent = this;

            if (isAiBlack) {
                item->x = i;
                item->y = j;
            } else {
                item->x = i;
                item->y =  7 - j;
            }
            item->setRect(19 + i * 69, 9 + j * 69, 69, 69);
            int ind = 8 * j + i;
            if (ind == movesHistory[historicalIndex].first || ind == movesHistory[historicalIndex].second) {
                if ((ind % 8 + ind / 8) % 2 == 1) {
                    item->setBrush(QBrush(QColor(222, 203, 86, 255)));
                    item->setPen(QPen(QColor(222, 203, 86, 255)));
                } else {
                    item->setBrush(QBrush(QColor(249, 238, 126, 255)));
                    item->setPen(QPen(QColor(249, 238, 126, 255)));
                }
            } else {
                if ((i + j) % 2 == isAiBlack) {
                    item->setBrush(QBrush(QColor(189,147,110,255)));
                    item->setPen(QPen(QColor(189,147,110,255)));
                } else {
                    item->setBrush(QBrush(QColor(240,221,189,255)));
                    item->setPen(QPen(QColor(240,221,189,255)));
                }
            }

            this->scene->addItem(item);
            if (i == 0) {
                auto* text = new QGraphicsTextItem(QString::fromStdString(std::string(
                        1, isAiBlack? ('8' - j): ('1' + j)
                )));
                if ((i + j) % 2 == 1) {
                    text->setDefaultTextColor(QColor(240,221,189,255));
                } else {
                    text->setDefaultTextColor(QColor(189,147,110,255));
                }
                text->setPos(19 + i * 69, 9 + j * 69);
                this->scene->addItem(text);
            }

            if (j == 7) {
                auto *text = new QGraphicsTextItem(QString::fromStdString(std::string(
                        1, isAiBlack ? ('a' + i) : ('h' - i)
                )));
                if ((i + j) % 2 == 1) {
                    text->setDefaultTextColor(QColor(240, 221, 189, 255));
                } else {
                    text->setDefaultTextColor(QColor(189, 147, 110, 255));
                }
                text->setPos(19 + i * 69 + 55, 9 + j * 69 + 48);
                this->scene->addItem(text);
            }

            int index;
            if (isAiBlack) {
                index = 8 * j + i;
            } else {
                index = 8 * (7 - j) + i;
            }
            if (history[historicalIndex]->squares[index] == 'R') {
                auto* pixmap = new GraphicsPiece;
                pixmap->parent = this;
                pixmap->setPixmap(QPixmap("/Users/user/Downloads/proj/images/wr.png"));
                pixmap->setPos(19 + i * 69 + 14.7, 9 + j * 69 + 9.525);
                pixmap->setScale(0.45);
                this->scene->addItem(pixmap);
            } else if (history[historicalIndex]->squares[index] == 'N') {
                auto* pixmap = new GraphicsPiece;
                pixmap->parent = this;
                pixmap->setPixmap(QPixmap("/Users/user/Downloads/proj/images/wn.png"));
                pixmap->setPos(19 + i * 69 + 11.55, 9 + j * 69 + 7.5);
                pixmap->setScale(0.45);
                this->scene->addItem(pixmap);
            } else if (history[historicalIndex]->squares[index] == 'B') {
                auto* pixmap = new GraphicsPiece;
                pixmap->parent = this;
                pixmap->setPixmap(QPixmap("/Users/user/Downloads/proj/images/wb.png"));
                pixmap->setPos(19 + i * 69 + 14.7, 9 + j * 69 + 6.825);
                pixmap->setScale(0.45);
                this->scene->addItem(pixmap);
            } else if (history[historicalIndex]->squares[index] == 'Q') {
                auto* pixmap = new GraphicsPiece;
                pixmap->parent = this;
                pixmap->setPixmap(QPixmap("/Users/user/Downloads/proj/images/wq.png"));
                pixmap->setPos(19 + i * 69 + 7.725, 9 + j * 69 + 7.5);
                pixmap->setScale(0.45);
                this->scene->addItem(pixmap);
            } else if (history[historicalIndex]->squares[index] == 'K') {
                auto* pixmap = new GraphicsPiece;
                pixmap->parent = this;
                pixmap->setPixmap(QPixmap("/Users/user/Downloads/proj/images/wk.png"));
                pixmap->setPos(19 + i * 69 + 8.175, 9 + j * 69 + 6.6);
                pixmap->setScale(0.45);
                this->scene->addItem(pixmap);
            } else if (history[historicalIndex]->squares[index] == 'P') {
                auto *pixmap = new GraphicsPiece;
                pixmap->parent = this;
                pixmap->setPixmap(QPixmap("/Users/user/Downloads/proj/images/wp.png"));
                pixmap->setPos(19 + i * 69 + 16.725, 9 + j * 69 + 11.55);
                pixmap->setScale(0.45);
                this->scene->addItem(pixmap);
            } else if (history[historicalIndex]->squares[index] == 'r') {
                auto* pixmap = new GraphicsPiece;
                pixmap->parent = this;
                pixmap->setPixmap(QPixmap("/Users/user/Downloads/proj/images/br.png"));
                pixmap->setPos(19 + i * 69 + 14.7, 9 + j * 69 + 9.525);
                pixmap->setScale(0.45);
                this->scene->addItem(pixmap);
            } else if (history[historicalIndex]->squares[index] == 'n') {
                auto* pixmap = new GraphicsPiece;
                pixmap->parent = this;
                pixmap->setPixmap(QPixmap("/Users/user/Downloads/proj/images/bn.png"));
                pixmap->setPos(19 + i * 69 + 11.55, 9 + j * 69 + 7.5);
                pixmap->setScale(0.45);
                this->scene->addItem(pixmap);
            } else if (history[historicalIndex]->squares[index] == 'b') {
                auto* pixmap = new GraphicsPiece;
                pixmap->parent = this;
                pixmap->setPixmap(QPixmap("/Users/user/Downloads/proj/images/bb.png"));
                pixmap->setPos(19 + i * 69 + 14.7, 9 + j * 69 + 6.825);
                pixmap->setScale(0.45);
                this->scene->addItem(pixmap);
            } else if (history[historicalIndex]->squares[index] == 'q') {
                auto* pixmap = new GraphicsPiece;
                pixmap->parent = this;
                pixmap->setPixmap(QPixmap("/Users/user/Downloads/proj/images/bq.png"));
                pixmap->setPos(19 + i * 69 + 7.725, 9 + j * 69 + 7.5);
                pixmap->setScale(0.45);
                this->scene->addItem(pixmap);
            } else if (history[historicalIndex]->squares[index] == 'k') {
                auto* pixmap = new GraphicsPiece;
                pixmap->parent = this;
                pixmap->setPixmap(QPixmap("/Users/user/Downloads/proj/images/bk.png"));
                pixmap->setPos(19 + i * 69 + 8.175, 9 + j * 69 + 6.6);
                pixmap->setScale(0.45);
                this->scene->addItem(pixmap);
            } else if (history[historicalIndex]->squares[index] == 'p') {
                auto* pixmap = new GraphicsPiece;
                pixmap->parent = this;
                pixmap->setPixmap(QPixmap("/Users/user/Downloads/proj/images/bp.png"));
                pixmap->setPos(19 + i * 69 + 16.725, 9 + j * 69 + 11.55);
                pixmap->setScale(0.45);
                this->scene->addItem(pixmap);
            }
        }
    }
}

void Ui_MainWindow::playMove() {
    redrawField();

    view->viewport()->repaint();
    QCoreApplication::processEvents();

    moves.clear();
    mates.clear();
    stalemates.clear();
    std::vector<bool> temp1;
    this->rules->GenLegalMoveList(moves,temp1, mates, stalemates);

    thc::Move m{};
    std::string mv = getMove(rules, hist[hist.size() - 1], useOpenings, useEndings);
    if (type == 0) {
        if (mv.empty()) {
            int turn = rand() % moves.size();
            mv = moves[turn].NaturalOut(rules);
        }
    }

    std::pair<int, int> mv_;
    std::string sM;

    bool mate = false;
    bool stalemate = false;

    std::string movenatural;
    if (!mv.empty()) {
        m.NaturalInFast(rules, mv.c_str());
        sM = m.TerseOut();

        int index = 0;
        for (auto move: moves) {
            std::string s1 = sM.substr(0, 2);
            std::string s2 = sM.substr(2, 2);
            int code1 = ('8' - s1[1]) * 8 + (s1[0] - 'a');
            int code2 = ('8' - s2[1]) * 8 + (s2[0] - 'a');
            if (code1 == move.src && code2 == move.dst) {
                if (mates[index]) {
                    mate = true;
                    break;
                } else if (stalemates[index]) {
                    stalemate = true;
                    break;
                }
            }
            index++;
        }

        std::cout << sM << std::endl;

        hist1 = ('8' - sM[1]) * 8 + (sM[0] - 'a');
        hist2 = ('8' - sM[3]) * 8 + (sM[2] - 'a');
        movesHistory.emplace_back(hist1, hist2);

        movenatural = m.NaturalOut(rules);

        rules->PlayMove(m);

        hist.push_back(hist[hist.size() - 1].move(
                {parse(sM.substr(0, 2), isAiBlack, false),
                 parse(sM.substr(2, 2), isAiBlack, false)}));

        moves.clear();
        mates.clear();
        stalemates.clear();
        this->rules->GenLegalMoveList(moves,temp1, mates, stalemates);

        printPos(hist[hist.size() - 1]);
    } else {
        auto res = searcher.search(
                hist[hist.size() - 1],
                hist, type == 2 ? 20000: 500);
        int depth = std::get<0> (res);
        mv_ = std::get<1> (res);
        int score = std::get<2> (res);

        std::string beg, end;
        beg = backparse(mv_.first);
        end = backparse(mv_.second);

//        if (isAiBlack) {
//            beg[1] = beg[1] - '8' + 1u;
//            end[1] = end[1] - '8' + 1u;
//        }

        int index = 0;
        for (auto move: moves) {
            int code1 = ('8' - beg[1]) * 8 + (beg[0] - 'a');
            int code2 = ('8' - end[1]) * 8 + (end[0] - 'a');
            if (code1 == move.src && code2 == move.dst) {
                if (mates[index]) {
                    mate = true;
                    break;
                } else if (stalemates[index]) {
                    stalemate = true;
                    break;
                }
            }
            index++;
        }

        hist1 = ('8' - beg[1]) * 8 + (beg[0] - 'a');
        hist2 = ('8' - end[1]) * 8 + (end[0] - 'a');

        movesHistory.emplace_back(hist1, hist2);
        m.NaturalIn(rules, (beg + end).c_str());

        movenatural = m.NaturalOut(rules);

        std::cout << beg << ' ' << end << ' ' << score / 10.0 << ' ' << depth << std::endl;
        rules->PlayMove(m);
        hist.push_back(hist[hist.size() - 1].move(mv_));

        moves.clear();
        mates.clear();
        stalemates.clear();
        this->rules->GenLegalMoveList(moves,temp1, mates, stalemates);
    }

    if (historicalIndex % 2 == 1) {
        textBrowser->insertPlainText(
                QString::fromStdString(std::string(" ― ") + movenatural + std::string("\n"))
        );
    }

    this->historicalIndex++;
    auto* copy = new thc::ChessRules;
    *copy = *this->rules;
    this->history.push_back(copy);

    printPos(hist[hist.size() - 1]);

    if (mate) {
        msgBox = new QMessageBox;
        msgBox->setIcon(QMessageBox::Information);
        msgBox->setText("I won by checkmate!");
        msgBox->show();
        msgBox->raise();

        canMove = false;
        this->pushButton_2->setText("Play again");
        this->checkBox->setEnabled(true);
        this->checkBox_2->setEnabled(true);
        this->comboBox->setEnabled(true);

        std::cout << "Checkmate!" << std::endl;
    } else if (stalemate) {
        msgBox = new QMessageBox;
        msgBox->setIcon(QMessageBox::Information);
        msgBox->setText("Draw due to stalemate");
        msgBox->show();

        canMove = false;
        this->pushButton_2->setText("Play again");
        this->checkBox->setEnabled(true);
        this->checkBox_2->setEnabled(true);
        this->comboBox->setEnabled(true);

        msgBox->raise();
    }

    isWhiteTurn = !isWhiteTurn;
    redrawField();
}

bool Ui_MainWindow::eventFilter(QObject* object, QEvent* e) {
    if (e->type() == QEvent::KeyPress) {
        auto *event = static_cast<QKeyEvent*>(e);
        int key = event->key();

        if (key == Qt::Key_Left) {
            if (historicalIndex > 0) {
                historicalIndex--;
            }
            redrawField();
        } else if (key == Qt::Key_Right) {
            if (historicalIndex < static_cast<int> (history.size()) - 1) {
                historicalIndex++;
            }
            redrawField();
        }
        return true;
    }
    return false;
}

void Ui_MainWindow::handleButton() {
    if (this->pushButton_2->text() == QString::fromStdString("Start game")) {
        canMove = true;
        startGame();
    } else if (this->pushButton_2->text() == QString::fromStdString("Play again")) {
        canMove = true;
        startGame();
    } else if (this->pushButton_2->text() == QString::fromStdString("Resign")) {
        msgBox = new QMessageBox;
        msgBox->setIcon(QMessageBox::Information);
        msgBox->setText("I won by resignation!");
        msgBox->show();
        msgBox->show();
        msgBox->raise();

        this->pushButton_2->setText("Play again");
        this->checkBox->setEnabled(true);
        this->checkBox_2->setEnabled(true);
        this->comboBox->setEnabled(true);
        canMove = false;
    }
}

void Square::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    if (!this->parent->canMove) {
        if (this->parent->firstGamePlayed) {
            return;
        } else {
            this->parent->startGame(false);
            this->parent->firstGamePlayed = true;
            this->parent->canMove = true;
        }
    }
    std::cout << x << ' ' << y << std::endl;
    for (auto el: this->parent->circles) {
        delete el;
    }
    this->parent->circles.clear();

    std::cout << !pressed << ' ' << (this->parent->choice != nullptr) << std::endl;
    if (!pressed) {
        std::vector<int> moves;
        int index = 0;
        for (auto q: this->parent->moves) {
            if (this->parent->choice != nullptr) {
                int tx, ty;

                if (this->parent->isAiBlack) {
                    tx = this->parent->choice->x;
                    ty = this->parent->choice->y;
                } else {
                    tx = this->parent->choice->x;
                    ty = this->parent->choice->y;
                }

                bool cond = q.src == 8 * ty + tx && q.dst == 8 * y + x;
                if (cond) {
                    thc::Move m = thc::Move();
                    m.NaturalIn(this->parent->rules,
                                (std::string(1, 'a' + tx) + std::string(1, '8' - ty) +
                                 std::string(1, 'a' + x) + std::string(1, '8' - y)).c_str());

                    if (this->parent->historicalIndex % 2 == 0) {
                        this->parent->textBrowser->insertPlainText(
                                QString::fromStdString(
                                        std::to_string(
                                                this->parent->historicalIndex / 2 + 1
                                        ) + std::string(". ") + m.NaturalOut(this->parent->rules))
                                );
                    }

                    this->parent->rules->PlayMove(m);
                    std::string sM = m.TerseOut();
                    std::cout << sM << std::endl;
                    this->parent->hist.push_back(
                            this->parent->hist[this->parent->hist.size() - 1].move(
                                    {parse(sM.substr(0, 2), this->parent->isAiBlack),
                                           parse(sM.substr(2, 2), this->parent->isAiBlack)}));

                    if (this->parent->isAiBlack) {
                        this->parent->hist2 = 8 * y + x;
                        this->parent->hist1 = 8 * this->parent->choice->y + this->parent->choice->x;

                        this->parent->movesHistory.emplace_back(this->parent->hist1, this->parent->hist2);
                    } else {
                        this->parent->hist2 = 8 * y + x;
                        this->parent->hist1 = 8 * (7 - this->parent->choice->y) + this->parent->choice->x;
                        this->parent->movesHistory.emplace_back(this->parent->hist1, this->parent->hist2);
                    }
                    this->parent->choice = nullptr;
                    this->parent->isWhiteTurn = !this->parent->isWhiteTurn;

                    this->parent->historicalIndex++;
                    auto* copy = new thc::ChessRules;
                    *copy = *this->parent->rules;
                    this->parent->history.push_back(copy);

                    if (this->parent->mates[index]) {
                        this->parent->msgBox = new QMessageBox;
                        this->parent->msgBox->setIcon(QMessageBox::Information);
                        this->parent->msgBox->setText("You won by checkmate!");
                        this->parent->msgBox->show();

                        this->parent->canMove = false;
                        this->parent->pushButton_2->setText("Play again");
                        this->parent->checkBox->setEnabled(true);
                        this->parent->checkBox_2->setEnabled(true);
                        this->parent->comboBox->setEnabled(true);

                        this->parent->msgBox->raise();

                        this->parent->redrawField();
                    } else if (this->parent->stalemates[index]) {
                        this->parent->msgBox = new QMessageBox;
                        this->parent->msgBox->setIcon(QMessageBox::Information);
                        this->parent->msgBox->setText("Draw due to stalemate");
                        this->parent->msgBox->show();

                        this->parent->canMove = false;
                        this->parent->pushButton_2->setText("Play again");
                        this->parent->checkBox->setEnabled(true);
                        this->parent->checkBox_2->setEnabled(true);
                        this->parent->comboBox->setEnabled(true);

                        this->parent->msgBox->raise();

                        this->parent->redrawField();
                    } else {
                        this->parent->playMove();
                    }
                    return;
                }
            }
            index++;
            bool cond;
            if (this->parent->isAiBlack) {
                cond = q.src == 8 * y + x;
            } else {
                cond = q.src == 8 * y + x;
//                cond = q.src == 8 * (7 - y) + x;
            }
            if (cond) {
                int square = q.dst;
                int x_ = square % 8;
                int y_ = square / 8;
                int m;
                if (this->parent->isAiBlack) {
                    m = q.dst;
                } else {
                    m = 8 * (7 - y_) + x_;
                }
                moves.push_back(q.dst);
            }
        }

        for (auto q: moves) {
            auto* circle = new QGraphicsEllipseItem;

            int copy = q;
            if (!this->parent->isAiBlack) {
                copy = q;
                q = 8 * (7 - (q / 8)) + (q % 8);
            }
            if (this->parent->rules->squares[copy] != ' ') {
                circle->setRect(19 + (q % 8) * 69 + 2, 9 + (q / 8) * 69 + 2, 63,  63);
                circle->setBrush(QBrush(QColor(0, 0, 0, 0)));
                QPen pen(QColor(0, 0, 0, 25));
                pen.setWidth(6);
                circle->setPen(pen);
            } else {
                circle->setRect(19 + (q % 8) * 69 + 24, 9 + (q / 8) * 69 + 24, 21,  21);
                circle->setBrush(QBrush(QColor(0, 0, 0, 25)));
                circle->setPen(QPen(QColor(0, 0, 0, 0)));
            }

            this->parent->scene->addItem(circle);
            this->parent->circles.push_back(circle);
        }

        if (this->parent->choice != nullptr) {
            if ((this->parent->choice->x + this->parent->choice->y) % 2 == 1) {
                this->parent->choice->setBrush(QBrush(QColor(189, 147, 110, 255)));
                this->parent->choice->setPen(QPen(QColor(189, 147, 110, 255)));
            } else {
                this->parent->choice->setBrush(QBrush(QColor(240, 221, 189,255)));
                this->parent->choice->setPen(QPen(QColor(240, 221, 189, 255)));
            }
            this->parent->choice->pressed = false;
        }

        bool cond;
        if (this->parent->isAiBlack) {
            cond = (this->parent->rules->squares[8 * y + x] >= 'a' && !this->parent->isWhiteTurn ||
            (this->parent->rules->squares[8 * y + x] >= 'A' && this->parent->rules->squares[8 * y + x] < 'a'));
        } else {
            cond = (this->parent->rules->squares[8 * (7 - y) + x] >= 'a' && !this->parent->isWhiteTurn ||
            (this->parent->rules->squares[8 * (7 - y) + x] >= 'A' &&
            this->parent->rules->squares[8 * (7 - y) + x] < 'a'));
        }
        if (cond && this->parent->isWhiteTurn) {
            if ((x + y) % 2 == 1) {
                this->setBrush(QBrush(QColor(222, 203, 86, 255)));
                this->setPen(QPen(QColor(222, 203, 86, 255)));
            } else {
                this->setBrush(QBrush(QColor(249, 238, 126, 255)));
                this->setPen(QPen(QColor(249, 238, 126, 255)));
            }
        } else {
            int ind;
            if (this->parent->isAiBlack) {
                ind = 8 * y + x;
            } else {
                ind = 8 * (7 - y) + x;
            }
            if (ind == this->parent->movesHistory[this->parent->historicalIndex].first ||
                ind == this->parent->movesHistory[this->parent->historicalIndex].second) {
                return;
            }
        }

        this->parent->choice = this;
        pressed = true;
    } else {
        if ((x + y) % 2 == 1) {
            this->setBrush(QBrush(QColor(189, 147, 110, 255)));
            this->setPen(QPen(QColor(189, 147, 110, 255)));
        } else {
            this->setBrush(QBrush(QColor(240, 221, 189,255)));
            this->setPen(QPen(QColor(240, 221, 189, 255)));
        }

        this->parent->choice = nullptr;
        pressed = false;
    }

    QGraphicsItem::mousePressEvent(event);
}

//
// Created by afentev on 4/21/21.
//

#ifndef PROJ_INTERFACE_H
#define PROJ_INTERFACE_H


/********************************************************************************
** Form generated from reading UI file 'formux1134.ui'
**
** Created by: Qt User Interface Compiler version 5.9.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>
#include <QtWidgets/QGraphicsPixmapItem>
#include <QtWidgets/QGraphicsView>
#include <QMessageBox>

#include "../Engine/IterativeTablesEngine.h"

QT_BEGIN_NAMESPACE

class Square;

class Ui_MainWindow: public QMainWindow
{
Q_OBJECT
public:
    QWidget *centralwidget;
    QComboBox *comboBox;
    QLabel *label;
    QCheckBox *checkBox;
    QCheckBox *checkBox_2;
    QTextBrowser *textBrowser;
    QFrame *line;
    QFrame *line_2;
    QFrame *line_3;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;

    QGraphicsScene* scene;
    QGraphicsView* view;

    void setupUi();  // setupUi

    void retranslateUi(QMainWindow *MainWindow) const;  // retranslateUi

    void redrawField();

    void startGame(bool redraw = true);

    void playMove();

    bool isAiBlack = true;
    bool isWhiteTurn = true;
    std::vector<QGraphicsEllipseItem*> circles;
    std::vector<thc::Move> moves;
    std::vector<bool> mates, stalemates;
    Square* choice = nullptr;
    int hist1 = -1;
    int hist2 = -1;

    thc::ChessRules* rules = nullptr;
    Searcher searcher;
    std::vector<IterativeTablesEngine> hist {IterativeTablesEngine(initial, 0, {true, true},
                                                                   {true, true}, 0, 0,
                                                                   std::unordered_map<std::string, int> (),
                                                                   76.0, 16, isAiBlack)};

    std::vector<thc::ChessRules*> history;
    std::vector<std::pair<int, int>> movesHistory;
    int historicalIndex = 0;

    QMessageBox* msgBox = nullptr;

    bool useOpenings = true;
    bool useEndings = true;
    int type = 2;
    bool firstGamePlayed = false;
    bool canMove = false;

protected:
    bool eventFilter(QObject *object, QEvent *e) override;
    // works like keyPressEvent but also handles Left and Right keys

private slots:
    void handleButton();
};

class GraphicsPiece: public QObject, public QGraphicsPixmapItem {
Q_OBJECT
public:
    Ui_MainWindow* parent = nullptr;

protected:
//    void mouseMoveEvent(QGraphicsSceneMouseEvent*);
//    void mousePressEvent(QGraphicsSceneMouseEvent*);

    QPointF dragStartPosition, beg;
};

class Square: public QObject, public QGraphicsRectItem {
Q_OBJECT
public:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    Ui_MainWindow* parent = nullptr;
    int x, y;
    bool pressed = false;
};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE


#endif //PROJ_INTERFACE_H

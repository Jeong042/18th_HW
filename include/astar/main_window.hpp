/**
 * @file /include/astar/main_window.hpp
 *
 * @brief Qt based gui for %(package)s.
 *
 * @date August 2024
 **/

#ifndef astar_MAIN_WINDOW_H
#define astar_MAIN_WINDOW_H

/*****************************************************************************
** Includes
*****************************************************************************/

#include "../include/astar/astar.hpp"
#include <QMainWindow>
#include "QIcon"
#include "qnode.hpp"
#include "ui_mainwindow.h"
#include <QPainter>
#include <vector>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QMouseEvent>
#include <QWidget>
#include <QGridLayout>

using namespace std;
/*****************************************************************************
** Interface [MainWindow]
*****************************************************************************/
/**
 * @brief Qt central, all operations relating to the view part here.
 */
class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  QNode *qnode;

  QPushButton *button;
  QPushButton *button1;
  QPushButton *button2;
  QPushButton *button3;
  QPushButton *button4;
  QPushButton *button5;

  QPushButton *previousStartButton = nullptr;
  QPushButton *previousEndButton = nullptr;

  QSpinBox *col_value;
  QSpinBox *row_value;

  QGridLayout *gridLayout;

  int col, row;

  bool start = false;
  bool end = false;

  void on_Generate_Map_clicked();
  void on_Set_Start_clicked();
  void on_Set_End_clicked();
  void on_Astar_clicked();
  void on_Reset_clicked();

  vector<vector<int>> mapstate;

   AStar *astar;

private:
  Ui::MainWindowDesign *ui;
  void closeEvent(QCloseEvent *event);
};

#endif // astar_MAIN_WINDOW_H

/**
 * @file /src/main_window.cpp
 *
 * @brief Implementation for the qt gui.
 *
 * @date August 2024
 **/
/*****************************************************************************
** Includes
*****************************************************************************/

#include "../include/astar/main_window.hpp"

using namespace std;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindowDesign)
{
  ui->setupUi(this);

  QIcon icon("://ros-icon.png");
  this->setWindowIcon(icon);
  this->setStyleSheet("background-color:#333333;");
  this->resize(800, 600);

  col_value = new QSpinBox(this);
  col_value->setStyleSheet("color: white;");
  col_value->setRange(0, 30);
  col_value->setValue(0);
  col_value->setGeometry(650, 10, 130, 20);

  row_value = new QSpinBox(this);
  row_value->setStyleSheet("color: white;");
  row_value->setRange(0, 30);
  row_value->setValue(0);
  row_value->setGeometry(650, 40, 130, 20);

  QLabel *col_label = new QLabel("Col  :", this);
  col_label->setStyleSheet("color: white;");
  col_label->setGeometry(600, 10, 40, 20);

  QLabel *row_label = new QLabel("Row:", this);
  row_label->setStyleSheet("color: white;");
  row_label->setGeometry(600, 40, 40, 20);

  button1 = new QPushButton("Generate Map", this);
  button1->setStyleSheet("QPushButton {"
                         "background-color: gray;"
                         "color: white;"
                         "border-radius: 10px;"
                         "font-size: 14px;"
                         "font-weight: bold;"
                         "}"
                         "QPushButton:hover {"
                         "background-color: white;"
                         "color: black;"
                         "}"
                         "QPushButton:pressed {"
                         "background-color: gray;"
                         "border-style: inset;"
                         "}");
  button1->setGeometry(600, 70, 180, 25);
  connect(button1, &QPushButton::clicked, this, &MainWindow::on_Generate_Map_clicked);

  button2 = new QPushButton("Set Start", this);
  button2->setStyleSheet("QPushButton {"
                         "background-color: gray;"
                         "color: white;"
                         "border-radius: 10px;"
                         "font-size: 14px;"
                         "font-weight: bold;"
                         "}"
                         "QPushButton:hover {"
                         "background-color: white;"
                         "color: black;"
                         "}"
                         "QPushButton:pressed {"
                         "background-color: gray;"
                         "border-style: inset;"
                         "}");
  button2->setGeometry(600, 100, 85, 25);
  connect(button2, &QPushButton::clicked, this, &MainWindow::on_Set_Start_clicked);

  button3 = new QPushButton("Set End", this);
  button3->setStyleSheet("QPushButton {"
                         "background-color: gray;"
                         "color: white;"
                         "border-radius: 10px;"
                         "font-size: 14px;"
                         "font-weight: bold;"
                         "}"
                         "QPushButton:hover {"
                         "background-color: white;"
                         "color: black;"
                         "}"
                         "QPushButton:pressed {"
                         "background-color: gray;"
                         "border-style: inset;"
                         "}");
  button3->setGeometry(695, 100, 85, 25);
  connect(button3, &QPushButton::clicked, this, &MainWindow::on_Set_End_clicked);

  button4 = new QPushButton("Astar", this);
  button4->setStyleSheet("QPushButton {"
                         "background-color: gray;"
                         "color: white;"
                         "border-radius: 10px;"
                         "font-size: 14px;"
                         "font-weight: bold;"
                         "}"
                         "QPushButton:hover {"
                         "background-color: white;"
                         "color: black;"
                         "}"
                         "QPushButton:pressed {"
                         "background-color: gray;"
                         "border-style: inset;"
                         "}");
  button4->setGeometry(600, 130, 180, 25);
  connect(button4, &QPushButton::clicked, this, &MainWindow::on_Astar_clicked);

  button5 = new QPushButton("Reset", this);
  button5->setStyleSheet("QPushButton {"
                         "background-color: gray;"
                         "color: white;"
                         "border-radius: 10px;"
                         "font-size: 14px;"
                         "font-weight: bold;"
                         "}"
                         "QPushButton:hover {"
                         "background-color: white;"
                         "color: black;"
                         "}"
                         "QPushButton:pressed {"
                         "background-color: black;"
                         "border-style: inset;"
                         "}");
  button5->setGeometry(600, 160, 180, 25);
  connect(button5, &QPushButton::clicked, this, &MainWindow::on_Reset_clicked);

  button2->setEnabled(false);
  button3->setEnabled(false);
  button4->setEnabled(false);

  qnode = new QNode();

  QObject::connect(qnode, SIGNAL(rosShutDown()), this, SLOT(close()));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
  QMainWindow::closeEvent(event);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_Generate_Map_clicked()
{
  button2->setEnabled(true);
  button3->setEnabled(true);
  button4->setEnabled(true);

  if (centralWidget() != nullptr)
  {
    QWidget *existingWidget = centralWidget();
    delete existingWidget;
  }

  if (previousStartButton != nullptr)
  {
    previousStartButton->setStyleSheet("background-color: white;");
    previousStartButton = button;
  }
  if (previousEndButton != nullptr)
  {
    previousEndButton->setStyleSheet("background-color: white;");
    previousEndButton = button;
  }
  start = false;
  end = false;

  col = col_value->value();
  row = row_value->value();

  mapstate = vector<vector<int>>(row, vector<int>(col, 0));

  QWidget *gridWidget = new QWidget(this);
  gridWidget->setFixedSize(500, 500);
  gridLayout = new QGridLayout(gridWidget);
  setCentralWidget(gridWidget);

  gridLayout->setSpacing(0);
  gridLayout->setContentsMargins(0, 0, 0, 0);

  QLayoutItem *item;
  while ((item = gridLayout->takeAt(0)) != nullptr)
  {
    delete item->widget();
    delete item;
  }

  for (int i = 0; i < row; ++i)
  {
    for (int j = 0; j < col; ++j)
    {
      QPushButton *button = new QPushButton(gridWidget);
      button->setStyleSheet("background-color: white;");
      button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
      connect(button, &QPushButton::clicked, this, [=]()
              {
                if(!start && !end){
                  mapstate[i][j] = 1;
                  button->setStyleSheet("background-color: black;");
                }
                else if(start && !end){
                  if (previousStartButton != nullptr) {
                        previousStartButton->setStyleSheet("background-color: white;");
                    }
                    mapstate[i][j] = 2; 
                    button->setStyleSheet("background-color: red;");
                    previousStartButton = button;
                    start = false;
                    end = false;
                }
                else if(!start && end){
                  if (previousEndButton != nullptr) {
                        previousEndButton->setStyleSheet("background-color: white;");
                    }
                    mapstate[i][j] = 3;
                    button->setStyleSheet("background-color: blue;");
                    previousEndButton = button; 
                    start = false;
                    end = false;
                } });

      gridLayout->addWidget(button, i, j);
    }
  }
}
void MainWindow::on_Set_Start_clicked()
{
  start = true;
  end = false;
}
void MainWindow::on_Set_End_clicked()
{
  start = false;
  end = true;
}
void MainWindow::on_Astar_clicked()
{
  astar = new AStar(mapstate);

  int start_x, start_y, end_x, end_y;


  for (int i = 0; i < row; ++i)
  {
    for (int j = 0; j < col; ++j)
    {
      if (mapstate[i][j] == 2) 
      {
        start_x = i;
        start_y = j;
      }
      else if (mapstate[i][j] == 3)
      {
        end_x = i;
        end_y = j;
      }
    }
  }

  vector<pair<int, int>> path = astar->findPath(start_x, start_y, end_x, end_y);

  for (int i = 0; i < path.size(); i++)
  {
    int x = path[i].first;
    int y = path[i].second;

    QLayoutItem *item = gridLayout->itemAtPosition(x, y);
    if (item != nullptr)
    {
      QWidget *widget = item->widget();
      if (widget != nullptr)
      {
        QPushButton *button = qobject_cast<QPushButton *>(widget);
        if (button)
        {
          if (mapstate[x][y] != 2 && mapstate[x][y] != 3)
          {
            button->setStyleSheet("background-color: yellow;");
          }
        }
      }
    }
  }
}


void MainWindow::on_Reset_clicked()
{
  QLayoutItem *item;
  while ((item = gridLayout->takeAt(0)) != nullptr)
  {
    delete item->widget();
    delete item;
  }

  mapstate = vector<vector<int>>(row, vector<int>(col, 0));
}

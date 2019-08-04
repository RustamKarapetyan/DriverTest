#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QKeyEvent>
#include<QPixmap>
#include<QFileDialog>
#include<QByteArray>
#include<QVector>
#include<QMap>
#include<QSet>
#include<iomanip>
#include<ctime>
#include<QRadioButton>
#include<QLabel>
#include<QTextEdit>

#include"qdatabaseconnector.h"
#include"questiuonanswer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QDataBase *db;
    int current_id;
    QByteArray ba1,ba2;
    QQuestion question;
    QVector<QAnswer> answers;
    QStringListModel model,emptymodel;
    QStringList modelstrl;
    QAnswer removable_answer;
    QMap<int,Question_And_Answer> m;
    bool answered_clicked;
    Question_And_Answer current_question_and_answer,TMP_current_question_and_answer;
    QVector<int> random_id;
    QLabel *question_image_lbl,*answer1lbl,*answer2lbl,*answer3lbl;
    int questiont_qty;
    int current_quetion;
    int score;
    QRadioButton *rad;
    QTextEdit *qlbl;
    QPixmap picquestion,ans_1_pic,ans_2_pic,ans_3_pic;
    void generate_questions(int count);
    void select_questions();
    void set_first_checked();

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void my_request(QString s);
    void intit_questions(int count);
    void anull_questions();
private slots:
    void on_pushButton_clicked();

    void on_BackpushButton_clicked();

    void on_RandomButton_clicked();

    void on_SettingsButton_clicked();

    void keyPressEvent(QKeyEvent *ev);

    void on_LoadQuestionImagepushButton_clicked();

    void on_pushButton_3_clicked();

    void on_AddpushButton_clicked();

    void on_LoadAnserImagepushButton_4_clicked();

    void on_listView_doubleClicked(const QModelIndex &index);

    void on_RemovepushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_ExpertButton_clicked();

private:
    Ui::MainWindow *ui;
    void get_current_id();
};

#endif // MAINWINDOW_H

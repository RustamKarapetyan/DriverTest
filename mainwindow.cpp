#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->verticalLayout->setAlignment(Qt::AlignCenter);
    this->showFullScreen();
    ui->stackedWidget->setCurrentIndex(0);
    db=new QDataBase();

    this->get_current_id();
    ui->IDlabel->setText(QString::number(this->current_id));
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->FalseradioButton->setChecked(true);
    this->score=0;
    this->current_quetion=0;

    rad=new QRadioButton[3];
    qlbl=new QTextEdit;
    qlbl->setMaximumWidth(800);
    qlbl->setDisabled(true);
    question_image_lbl=new QLabel;
    answer1lbl=new QLabel;
    answer2lbl=new QLabel;
    answer3lbl=new QLabel;

    ui->questionhorizontalLayout_8->addWidget(qlbl);
    ui->questionhorizontalLayout_8->addWidget(question_image_lbl);

    ui->answer1Layout_17->addWidget(&(rad[0]));
    ui->answer1Layout_17->addWidget(answer1lbl);
    ui->answer2Layout_17->addWidget(&(rad[1]));
    ui->answer2Layout_17->addWidget(answer2lbl);
    ui->answer3Layout_17->addWidget(&(rad[2]));
    ui->answer3Layout_17->addWidget(answer3lbl);

    answered_clicked=false;
    ui->StatustextEdit->setDisabled(true);
}

MainWindow::~MainWindow()
{
    delete [] rad;
    delete qlbl;
    delete question_image_lbl;
    delete answer1lbl;
    delete answer2lbl;
    delete answer3lbl;
    delete ui;
}

void MainWindow::my_request(QString s)
{
    QSqlQuery & q=db->get_query();
    QString str;
    if(q.exec(s))
    {
        while(q.next())
        {
            str=str+"\n"+q.value(0).toString();
        }
    }
    QMessageBox::information(this,"INFO",str);
}

void MainWindow::intit_questions(int count)
{
    this->random_id.clear();
    this->m.clear();

    this->generate_questions(count);
    this->select_questions();

    //QMessageBox::information(this,"INFO",QString::number(this->m.size()));
}

void MainWindow::anull_questions()
{
        m.clear();
        this->random_id.clear();
        this->current_quetion=0;
        this->questiont_qty=0;
        ui->StatustextEdit->clear();
        qlbl->setText("");
        this->answer1lbl->hide();
        this->answer2lbl->hide();
        this->answer3lbl->hide();
        this->question_image_lbl->hide();
        TMP_current_question_and_answer.clear();

}

void MainWindow::generate_questions(int count)
{
    srand(time(NULL));
    QSet<int> s;
    while(s.size()!=count)
    {
        int i=1+rand()%(current_id-1);
        s.insert(i);
    }
    for(QSet<int>::iterator it=s.begin();it!=s.end();it++)
    {
        this->random_id.push_back(*it);
    }
}

void MainWindow::select_questions()
{
    if(random_id.empty())
    {
        QMessageBox::warning(this,"Warnhig","You have not any questions inserted yet.");
        return;
    }

    for(int i=0;i<this->random_id.size();i++)
    {
        Question_And_Answer qa;
        QQuestion q;
        QVector<QAnswer> a;
        q=db->get_question_where(random_id[i]);
        a=db->get_answer_where(random_id[i]);
        qa.set_question(q);
        qa.set_answer_as_vector(a);
        m.insert(random_id[i],qa);
    }
}

void MainWindow::set_first_checked()
{
   this->rad[0].setChecked(false);
}

void MainWindow::on_pushButton_clicked()
{
    qApp->quit();
}

void MainWindow::on_BackpushButton_clicked()
{
    QMessageBox::StandardButton reply;
    reply=QMessageBox::question(this,"Exit test","Do you want to exit test",QMessageBox::Yes|QMessageBox::No);
    if(reply==QMessageBox::No)
    {
        return;
    }
    this->anull_questions();
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_RandomButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    this->intit_questions(10);
    this->questiont_qty=10;


    if(random_id.isEmpty() || m.isEmpty())
    {
        QMessageBox::warning(this,"Warning","There are not selected questions.");
        return;
    }

    qlbl->setText(m[random_id[current_quetion]].get_question_text());
    qlbl->setStyleSheet("font: 75 18pt MS Shell Dlg 2;color: rgb(85, 123, 255);border-radius:3px;border-color:blue;");
    ui->questionhorizontalLayout_8->addWidget(qlbl);

    this->current_question_and_answer=m[random_id[this->current_quetion]];
    if(current_question_and_answer.get_question_type()==2 || current_question_and_answer.get_question_type()==3)
    {
        //QMessageBox::information(this,"INFO","WITH IMAGE");
        QByteArray ba=current_question_and_answer.get_question_image();

        picquestion.loadFromData(ba,"jpg");
        picquestion=picquestion.scaled(180,180,Qt::IgnoreAspectRatio,Qt::FastTransformation);
        this->question_image_lbl->setPixmap(picquestion);
        this->question_image_lbl->setAlignment(Qt::AlignCenter);
        question_image_lbl->show();
    }

    //Adding answers
    for(int i=0;i<3;i++)
    {
        rad[i].setText(m[random_id[this->current_quetion]].get_answer_text(i));
        rad[i].setStyleSheet("color: rgb(4, 207, 51);font: 14pt MS Shell Dlg 2;");
        if(m[random_id[this->current_quetion]].get_answer_type(i)==2 || m[random_id[this->current_quetion]].get_answer_type(i)==3)
        {
            if(i==0)
            {
              ans_1_pic.loadFromData(m[random_id[this->current_quetion]].get_anser_image(i));
              ans_1_pic=ans_1_pic.scaled(80,80,Qt::IgnoreAspectRatio,Qt::FastTransformation);
              this->answer1lbl->setPixmap(ans_1_pic);
              this->answer1lbl->setAlignment(Qt::AlignCenter);
              this->answer1lbl->show();
            }

            if(i==1)
            {
              ans_2_pic.loadFromData(m[random_id[this->current_quetion]].get_anser_image(i));
              ans_2_pic=ans_2_pic.scaled(80,80,Qt::IgnoreAspectRatio,Qt::FastTransformation);
              this->answer2lbl->setPixmap(ans_2_pic);
              this->answer2lbl->setAlignment(Qt::AlignCenter);
              this->answer2lbl->show();
            }

            if(i==2)
            {
              ans_3_pic.loadFromData(m[random_id[this->current_quetion]].get_anser_image(i));
              ans_3_pic=ans_3_pic.scaled(80,80,Qt::IgnoreAspectRatio,Qt::FastTransformation);
              this->answer3lbl->setPixmap(ans_3_pic);
              this->answer3lbl->setAlignment(Qt::AlignCenter);
              this->answer3lbl->show();
            }
        }
    }
}

void MainWindow::on_SettingsButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}


void MainWindow::keyPressEvent(QKeyEvent *ev)
{
    if(ev->key()==Qt::Key_Escape && ui->stackedWidget->currentIndex()==2)
        ui->stackedWidget->setCurrentIndex(0);
    else {
        if(ev->key()==Qt::Key_Escape && ui->stackedWidget->currentIndex()==0)
            qApp->quit();
        else
        {
            if(ev->key()==Qt::Key_Escape && ui->stackedWidget->currentIndex()==1)
            {
                QMessageBox::StandardButton reply;
                reply=QMessageBox::question(this,"Exit test","Do you want to exit test",QMessageBox::Yes|QMessageBox::No);
                if(reply==QMessageBox::No)
                {
                    return;
                }
                this->anull_questions();
                ui->stackedWidget->setCurrentIndex(0);
            }
        }
    }
}

void MainWindow::get_current_id()
{
    int id=db->get_current_id();
    id=id+1;
    this->current_id=id;
}

void MainWindow::on_LoadQuestionImagepushButton_clicked()
{
    ba1.clear();
    QString image_ref;
    image_ref=QFileDialog::getOpenFileName(this,"Load Image","c:\\","JPG(*.jpg);;BMP(*.bmp);;PNG(*png)");
    //if(image_ref.isEmpty())
      //  return;
    QPixmap pic(image_ref);
    QPixmap pic2=pic.scaled(180,180,Qt::IgnoreAspectRatio,Qt::FastTransformation);
    ui->QuestionImagelabel->setPixmap(pic2);
    ui->QuestionImagelabel->setAlignment(Qt::AlignCenter);

    QByteArray barray;
    QBuffer buffer(&barray);
    buffer.open(QIODevice::WriteOnly);
    pic.save(&buffer,"JPG");
    this->question.question_byte_array=barray;
    ba1=barray;
}

//Save button clicked (Add question with answers)
void MainWindow::on_pushButton_3_clicked()
{
    this->question.id=this->current_id;
    this->question.question_text=ui->QuestiontextEdit->toPlainText();
    if(question.question_byte_array.isEmpty() && (!question.question_text.isEmpty()))
        question.type=1;
    else {
        if((question.question_byte_array.isEmpty()) && (!question.question_text.isEmpty()))
            question.type=2;
        else {
            if((!question.question_byte_array.isEmpty()) && (!question.question_text.isEmpty()))
                question.type=3;
            else {
                if(question.question_text.isEmpty() && question.question_byte_array.isEmpty())
                {
                    QMessageBox::critical(this,"Critical","You have not load anything for question");
                    question.clear();
                    return;
                }
            }
        }
    }
    //QMessageBox::critical(this,"Critical",QString::number((question.type)));
    QString mes;
    question.write_question_in_db(db->get_query(),mes);
    if(mes!="GOOD")
        QMessageBox::information(this,"Warning",mes);
    else {
        this->current_id++;
        ui->IDlabel->setText(QString::number(this->current_id));
    }

    QString mes1="GOOD";
    for(int i=0;i<this->answers.size();i++)
    {
        answers[i].write_answer_in_db(db->get_query(),mes1);
        //QMessageBox::information(this,"Question "+QString::number(i)+" not inserted ",QString::number(answers[i].question_id));
        if(mes1!="GOOD")
            QMessageBox::information(this,"Question "+QString::number(i)+" not inserted ",mes1);
    }

    answers.clear();
    question.clear();
    ba1.clear();
    ba2.clear();
    QPixmap pic;
    ui->QuestionImagelabel->setPixmap(pic);
    ui->QuestionImagelabel->setAlignment(Qt::AlignCenter);
    ui->QuestiontextEdit->clear();
    ui->listView->setModel(&(this->emptymodel));

    modelstrl.clear();
    this->model.setStringList(modelstrl);
}

void MainWindow::on_AddpushButton_clicked()
{
    QString txt=ui->AnswertextEdit->toPlainText();
    if(txt.isEmpty())
        return;
    modelstrl.append(txt);
    model.setStringList(modelstrl);
    ui->listView->setModel(&model);

    QAnswer ans;
    ans.question_id=this->current_id;
    if(!ba2.isEmpty())
      ans.ba=ba2;
    if(!ui->AnswertextEdit->toPlainText().isEmpty())
        ans.answer=txt;
    if((!txt.isEmpty()) && ans.ba.isEmpty())
        ans.type=1;
    if(txt.isEmpty() && (!ans.ba.isEmpty()))
        ans.type=2;
    if((!txt.isEmpty()) && (!ans.ba.isEmpty()))
        ans.type=3;
    if(ui->FalseradioButton->isChecked())
        ans.is_correct=0;
    else
        if(ui->TrueradioButton->isChecked())
           ans.is_correct=1;

    this->answers.push_back(ans);
    QPixmap pic;
    ui->label_8->setPixmap(pic);
    ui->label_8->setAlignment(Qt::AlignCenter);
    ui->AnswertextEdit->clear();
}

void MainWindow::on_LoadAnserImagepushButton_4_clicked()
{
    ba2.clear();
    QString image_ref;
    image_ref=QFileDialog::getOpenFileName(this,"Load Image","c:\\","JPG(*.jpg);;BMP(*.bmp);;PNG(*png)");
    //if(image_ref.isEmpty())
      //  return;
    QPixmap pic(image_ref);
    QPixmap pic2=pic.scaled(180,180,Qt::IgnoreAspectRatio,Qt::FastTransformation);
    ui->label_8->setPixmap(pic2);
    ui->label_8->setAlignment(Qt::AlignCenter);

    QByteArray barray;
    QBuffer buffer(&barray);
    buffer.open(QIODevice::WriteOnly);
    pic.save(&buffer,"JPG");
    //this->question.question_byte_array=barray;
    ba2=barray;
}

void MainWindow::on_listView_doubleClicked(const QModelIndex &index)
{
    QString answertxt=index.data().toString();
    if(answertxt.isEmpty())
        return;
    for(int i=0;i<answers.size();i++)
    {
        if(answers[i].answer==answertxt)
        {
            this->removable_answer=answers[i];
            //answers.remove(i);
            break;
        }
    }
    ui->AnswertextEdit->setText(removable_answer.answer);
    QPixmap tmppic;
    tmppic.loadFromData(this->removable_answer.ba,"jpg");
    QPixmap pic2=tmppic.scaled(180,180,Qt::IgnoreAspectRatio,Qt::FastTransformation);
    ui->label_8->setPixmap(pic2);
    ui->label_8->setAlignment(Qt::AlignCenter);
}

void MainWindow::on_RemovepushButton_clicked()
{
    for(int i=0;i<answers.size();i++)
    {
        if(answers[i]==this->removable_answer)
        {
            this->removable_answer=answers[i];
            answers.remove(i);
            for(int j=0;j<modelstrl.size();j++)
            {
                if(modelstrl.at(j)==this->removable_answer.answer)
                 {
                    ui->listView->setModel(&(this->emptymodel));
                    this->modelstrl.removeAt(j);
                    this->model.setStringList(modelstrl);
                    ui->listView->setModel(&(this->model));
                    QPixmap pic;
                    ui->label_8->setPixmap(pic);
                    ui->label_8->setAlignment(Qt::AlignCenter);
                    ui->AnswertextEdit->clear();
                    break;
                }
            }
            break;
        }
    }
}

//On answer button clicked
void MainWindow::on_pushButton_2_clicked()
{
    if(TMP_current_question_and_answer==current_question_and_answer)
    {
        QMessageBox::information(this,"Info","You already answered to this question"+QString::number(TMP_current_question_and_answer.get_question_id())+"  "+QString::number(current_question_and_answer.get_question_id()));
        return;
    }
    QString s;
    for(int i=0;i<3;i++)
    {
        if(this->rad[i].isChecked())
        {
            s=this->rad[i].text();
            break;
        }
    }
    if(s.isEmpty())
    {
        QMessageBox::information(this,"Information","Select any answer");
        return;
    }
    TMP_current_question_and_answer=current_question_and_answer;
    for(int i=0;i<3;i++)
    {
        if(this->current_question_and_answer[i].answer==s)
        {
            if(current_question_and_answer[i].is_correct==1)
            {
                ui->StatustextEdit->setText("Correct answer!!!");
                this->score+=1;
                this->current_quetion+=1;
                answered_clicked=true;
                return;
            }
            else
            {
                ui->StatustextEdit->setText("Incorrect answer!!!");
                answered_clicked=true;
                for(int j=0;j<3;j++)
                {
                    if(this->current_question_and_answer[j].is_correct==1)
                    {
                        ui->StatustextEdit->setText("Wrong answer!!!\n Correct is "+current_question_and_answer[j].answer);
                        break;
                    }
                }
                this->current_quetion+=1;
                return;
            }
        }
    }
}

//On next button clicked
void MainWindow::on_pushButton_4_clicked()
{
    //if(answered_clicked==false)
      //  return;
    answered_clicked=false;
    this->answer1lbl->hide();
    this->answer2lbl->hide();
    this->answer3lbl->hide();
    this->question_image_lbl->hide();
    ui->StatustextEdit->clear();

    this->set_first_checked();

    if(this->current_quetion==this->random_id.size())
    {
        ui->StatustextEdit->setText("Your score from " +QString::number(this->random_id.size())+" questions is:\n Correct:"
                                  +QString::number(this->score)+"\nWrong:"+QString::number(random_id.size()-score));
        this->current_quetion=0;
        this->score=0;
        random_id.clear();
        return;
    }
    qlbl->setText(m[random_id[current_quetion]].get_question_text());
    qlbl->setStyleSheet("font: 75 18pt MS Shell Dlg 2;color: rgb(85, 123, 255);border-radius:3px;border-color:blue;");
    ui->questionhorizontalLayout_8->addWidget(qlbl);

    this->current_question_and_answer=m[random_id[this->current_quetion]];
    if(current_question_and_answer.get_question_type()==2 || current_question_and_answer.get_question_type()==3)
    {
        //QMessageBox::information(this,"INFO","WITH IMAGE");
        QByteArray ba=current_question_and_answer.get_question_image();

        picquestion.loadFromData(ba,"jpg");
        picquestion=picquestion.scaled(180,180,Qt::IgnoreAspectRatio,Qt::FastTransformation);
        this->question_image_lbl->setPixmap(picquestion);
        this->question_image_lbl->setAlignment(Qt::AlignCenter);
        question_image_lbl->show();
    }

    //Adding answers
    for(int i=0;i<3;i++)
    {
        rad[i].setText(m[random_id[this->current_quetion]].get_answer_text(i));
        rad[i].setStyleSheet("color: rgb(4, 207, 51);font: 14pt MS Shell Dlg 2;");
        if(m[random_id[this->current_quetion]].get_answer_type(i)==2 || m[random_id[this->current_quetion]].get_answer_type(i)==3)
        {
            if(i==0)
            {
              ans_1_pic.loadFromData(m[random_id[this->current_quetion]].get_anser_image(i));
              ans_1_pic=ans_1_pic.scaled(80,80,Qt::IgnoreAspectRatio,Qt::FastTransformation);
              this->answer1lbl->setPixmap(ans_1_pic);
              this->answer1lbl->setAlignment(Qt::AlignCenter);
              this->answer1lbl->show();
            }

            if(i==1)
            {
              ans_2_pic.loadFromData(m[random_id[this->current_quetion]].get_anser_image(i));
              ans_2_pic=ans_2_pic.scaled(80,80,Qt::IgnoreAspectRatio,Qt::FastTransformation);
              this->answer2lbl->setPixmap(ans_2_pic);
              this->answer2lbl->setAlignment(Qt::AlignCenter);
              this->answer2lbl->show();
            }

            if(i==2)
            {
              ans_3_pic.loadFromData(m[random_id[this->current_quetion]].get_anser_image(i));
              ans_3_pic=ans_3_pic.scaled(80,80,Qt::IgnoreAspectRatio,Qt::FastTransformation);
              this->answer3lbl->setPixmap(ans_3_pic);
              this->answer3lbl->setAlignment(Qt::AlignCenter);
              this->answer3lbl->show();
            }

        }

    }
}

void MainWindow::on_ExpertButton_clicked()
{
    this->anull_questions();
    ui->stackedWidget->setCurrentIndex(1);
    this->intit_questions(this->current_id-1);
    this->questiont_qty=this->current_id-1;


    if(random_id.isEmpty() || m.isEmpty())
    {
        QMessageBox::warning(this,"Warning","There are not selected questions.");
        return;
    }

    qlbl->setText(m[random_id[current_quetion]].get_question_text());
    qlbl->setStyleSheet("font: 75 18pt MS Shell Dlg 2;color: rgb(85, 123, 255);border-radius:3px;border-color:blue;");
    ui->questionhorizontalLayout_8->addWidget(qlbl);

    this->current_question_and_answer=m[random_id[this->current_quetion]];
    if(current_question_and_answer.get_question_type()==2 || current_question_and_answer.get_question_type()==3)
    {
        QByteArray ba=current_question_and_answer.get_question_image();

        picquestion.loadFromData(ba,"jpg");
        picquestion=picquestion.scaled(180,180,Qt::IgnoreAspectRatio,Qt::FastTransformation);
        this->question_image_lbl->setPixmap(picquestion);
        this->question_image_lbl->setAlignment(Qt::AlignCenter);
        question_image_lbl->show();
    }
    emit ui->pushButton_4->clicked();
}

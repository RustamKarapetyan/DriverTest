#ifndef QDATABASECONNECTOR_H
#define QDATABASECONNECTOR_H

#include<QtSql/QSqlDatabase>
#include<QtSql/QSqlQuery>
#include<QtSql/QSqlError>
#include<QtSql/QSqlQueryModel>
#include<QMessageBox>
#include<QImage>
#include<QBuffer>
#include<QPixmap>
#include<QByteArray>
#include<QString>
#include<QDebug>
#include<QStringList>
#include<QStringListModel>
#include<QSqlQueryModel>

struct QQuestion
{
    QString question_text;
    QByteArray question_byte_array;
    int id;
    int type;
    void clear()
    {
        type=0;
        id=0;
        question_text.clear();
        question_byte_array.clear();
    }
    void write_question_in_db(QSqlQuery &query,QString &mes)
    {
        mes="GOOD";
        QString req="INSERT INTO QUESTIONS_TABLE (TYPE";   //QUESTION_TEXT,QUESTION_IMAGE) VALUES()"
        if(type==1)
        {
            req=req+",QUESTION_TEXT) VALUES(:TYPE,:QUESTION_TEXT)";
        }
        else
        {
            if(type==2)
                req=req+",QUESTION_IMAGE) VALUES(:TYPE,:QUESTION_IMAGE)";
            else
            {
                if(type==3)
                    req=req+",QUESTION_TEXT,QUESTION_IMAGE) VALUES(:TYPE,:QUESTION_TEXT,:QUESTION_IMAGE)";
            }
        }

        if(query.prepare(req))
        {
            query.bindValue(":TYPE",this->type);
            if(type==1)
            {
                query.bindValue(":QUESTION_TEXT",this->question_text);
            }
            else
            {
                if(type==2)
                {
                 query.bindValue(":QUESTION_IMAGE",this->question_byte_array);
                }
                else
                {
                    if(type==3)
                    {
                        query.bindValue(":QUESTION_TEXT",this->question_text);
                        query.bindValue(":QUESTION_IMAGE",this->question_byte_array);
                    }
                }
            }
            query.exec();
        }
        else
        mes=query.lastError().text();
    }
};

struct QAnswer
{
    int question_id;
    int type;
    int is_correct;
    QString answer;
    QByteArray ba;
    void write_answer_in_db(QSqlQuery &query,QString &mes)
    {
        mes="GOOD";
        QString req="INSERT INTO ANSWERS_TABLE(QUESTION_ID,TYPE,";             //ANSWER_TEXT,ANSWER_IMAGE,IS_CORRECT) "
                                                                               // "VALUES(:QUESTION_ID,:TYPE,:ANSWER_TEXT,:ANSWER_IMAGE,:IS_CORRECT)";
        if(type==1)
        {
            req=req+"ANSWER_TEXT,IS_CORRECT) VALUES(:QUESTION_ID,:TYPE,:ANSWER_TEXT,:IS_CORRECT)";
        }
        else {
            if(type==2)
            {
                req=req+"ANSWER_IMAGE,IS_CORRECT) VALUES(:QUESTION_ID,:TYPE,ANSWER_IMAGE,:IS_CORRECT)";
            }
            else {
                if(type==3)
                {
                    req=req+"ANSWER_TEXT,ANSWER_IMAGE,IS_CORRECT) "
                         "VALUES(:QUESTION_ID,:TYPE,:ANSWER_TEXT,:ANSWER_IMAGE,:IS_CORRECT)";
                }
                else
                {
                    mes="Incorrect answers\n";
                    return;
                }
            }
        }

        //QMessageBox::information(0,"xxxx ",QString::number(question_id));


        if(type==1)
        {
            //QMessageBox::information(0,"11111111111",this->answer);
            if(query.prepare(req))
            {
                query.bindValue(":QUESTION_ID",this->question_id);
                query.bindValue(":TYPE",this->type);
                query.bindValue(":ANSWER_TEXT",this->answer);
                query.bindValue(":IS_CORRECT",this->is_correct);
                if(!query.exec())
                {
                    mes=query.lastError().text();
                }
                else {
                    //QMessageBox::information(0,"Information","Inserted!!!");
                }
            }
        }
        else {
            if(type==2)
            {
             //QMessageBox::information(0,"2222222222222222","222222222222222222");
                if(query.prepare(req))
                {
                    query.bindValue(":QUESTION_ID",this->question_id);
                    query.bindValue(":TYPE",this->type);
                    query.bindValue(":IS_CORRECT",this->is_correct);
                    query.bindValue(":ANSWER_IMAGE",this->answer);
                    if(!query.exec())
                    {
                        mes=query.lastError().text();
                    }
                    else {
                        QMessageBox::information(0,"Information","Inserted!!!");
                    }
                }
            }
            else {
                if(type==3)
                {
                    //QMessageBox::information(0,"3333333333333333333","3333333333333");
                    if(query.prepare(req))
                    {
                    query.bindValue(":QUESTION_ID",this->question_id);
                    query.bindValue(":TYPE",this->type);
                    query.bindValue(":IS_CORRECT",this->is_correct);
                    query.bindValue(":ANSWER_TEXT",this->answer);
                    query.bindValue(":ANSWER_IMAGE",this->ba);
                    if(!query.exec())
                    {
                        mes=query.lastError().text();
                    }
                    else {
                        QMessageBox::information(0,"Information","Inserted!!!");
                    }
                    }
                }
            }
        }

    }
    bool operator==(const QAnswer &a1)
    {
        if(a1.answer==answer)
            return true;
        else
            return false;
    }
};

class QDataBase
{
private:
    QString user_name,password,host;
    int port;
    QSqlDatabase db;
    QSqlQuery query;
    QQuestion question;
public:
    QDataBase()
    {
        this->port=3306;
        this->host="127.0.0.1";
        this->user_name="root";
        this->password="17052016";
        db=QSqlDatabase::addDatabase("QMYSQL");
        db.setPort(port);
        db.setHostName(host);
        db.setUserName(this->user_name);
        db.setPassword(this->password);
        if(!this->DB_CREATED())
        {
            QString req="CREATE DATABASE IF NOT EXISTS DMVDB";
            query=QSqlQuery(db);
            if(query.exec(req))
            {
                db.setDatabaseName("DMVDB");
                db.open();
                query=QSqlQuery(db);
            }
            else {
                qDebug()<<"-----------------------------"<<query.lastError().text()<<"-------------------------------\n";
            }
        }
        else
        {
         db.setDatabaseName("DMVDB");
         db.open();
         query=QSqlQuery(db);
        }
        this->CREATE_QUESTIONS_TABLE();
        this->CREATE_ANSWRES_TABLE();
    }

    bool DB_CREATED()
    {
        db.open();
        query=QSqlQuery(db);
        QString req="SHOW DATABASES;";
        QStringList strl;
        if(query.exec(req))
        {
            while(query.next())
            {
                strl.append(query.value(0).toString());
            }
        }
        else {
            qDebug()<<"--------------------------------"<<query.lastError().text()<<"------------------------------\n";
        }
        if(strl.contains("DMVDB")|| strl.contains("dmvdb"))
            return true;
        else
            return false;
   }

    void CREATE_QUESTIONS_TABLE()
    {
        QString req="CREATE TABLE IF NOT EXISTS QUESTIONS_TABLE(ID INTEGER NOT NULL AUTO_INCREMENT,"
                    "TYPE INTEGER NOT NULL,"
                    "QUESTION_TEXT TEXT,"
                    "QUESTION_IMAGE LONGBLOB,"
                    "PRIMARY KEY(ID))";
        if(!query.exec(req))
        {
            qDebug()<<"---------------------------------QUESTIONS TABLE    "<<query.lastError().text()<<"-----------------------------------\n";
        }
    }

    void CREATE_ANSWRES_TABLE()
    {
        QString req="CREATE TABLE IF NOT EXISTS ANSWERS_TABLE(ID INTEGER NOT NULL AUTO_INCREMENT,"
                    "QUESTION_ID INTEGER NOT NULL,"
                    "TYPE INTEGER NOT NULL,"
                    "ANSWER_TEXT TEXT,"
                    "ANSWER_IMAGE LONGBLOB,"
                    "IS_CORRECT INTEGER NOT NULL,"
                    "PRIMARY KEY(ID))";
        if(!query.exec(req))
        {
            qDebug()<<"---------------------------------ANSWERS TABLE    "<<query.lastError().text()<<"-----------------------------------\n";
        }
    }

    int get_current_id()
    {
        QString req="SELECT ID FROM QUESTIONS_TABLE";
        int id=0;
        if(query.exec(req))
        {
            while(query.next())
            {
                id=query.value(0).toInt();
            }
        }
        return id;
    }

    QSqlQuery &get_query()
    {
        return query;
    }

    QVector<QAnswer> get_answer_where(int question_i)
    {
        QVector<QAnswer> v;
        QString req="SELECT QUESTION_ID,TYPE,ANSWER_TEXT,ANSWER_IMAGE,IS_CORRECT FROM ANSWERS_TABLE WHERE QUESTION_ID="+QString::number(question_i);
        QAnswer a;
        if(query.exec(req))
        {
            while(query.next())
            {
                a.question_id=query.value(0).toInt();
                a.type=query.value(1).toInt();
                a.answer=query.value(2).toString();
                a.ba=query.value(3).toByteArray();
                a.is_correct=query.value(4).toInt();
                v.push_back(a);
               // QMessageBox::information(0,"Info",a.answer);
            }
        }
        return v;
    }

    QQuestion get_question_where(int id)
    {
        QQuestion q;
        QString question_req1="SELECT ID,TYPE,QUESTION_TEXT,QUESTION_IMAGE FROM QUESTIONS_TABLE WHERE ID="+QString::number(id);
        if(query.exec(question_req1))
        {
            while(query.next())
            {
                q.id=query.value(0).toInt();
                q.type=query.value(1).toInt();
                q.question_text=query.value(2).toString();
                q.question_byte_array=query.value(3).toByteArray();
            }
        }
        return q;
    }
};

#endif // QDATABASECONNECTOR_H

#ifndef QUESTIUONANSWER_H
#define QUESTIUONANSWER_H

#include<QVector>

#include"qdatabaseconnector.h"

class Question_And_Answer
{
private:
    QQuestion question;
    QVector<QAnswer> answers;
    int question_id;
public:
    Question_And_Answer();
    QAnswer operator[](int i);
    bool operator ==(const Question_And_Answer&);
    void clear();
    void set_question_id(int id);
    void set_question(QQuestion q);
    int get_question_id()const;
    int get_question_type()const;

    QString get_question_text();
    QByteArray get_question_image();

    void add_answer(QAnswer a);
    void set_answer_as_vector(const QVector<QAnswer>);
    QString get_answer_text(int );
    QByteArray get_anser_image(int );
    bool is_answer_correct(int );
    int get_answer_type(int );
};

#endif // QUESTIUONANSWER_H

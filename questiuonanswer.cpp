#include"questiuonanswer.h"

Question_And_Answer::Question_And_Answer()
{
    this->question_id=0;
}

QAnswer Question_And_Answer::operator[](int i)
{
    return answers[i];
}

bool Question_And_Answer::operator ==(const Question_And_Answer& aa)
{
    if(aa.get_question_id()==this->question.id)
        return true;
    else
        return false;
}

void Question_And_Answer::clear()
{
    this->question.id=-1;
    this->question.clear();
    this->answers.clear();
}

void Question_And_Answer::set_question_id(int id)
{
    this->question_id=id;
}

void Question_And_Answer::set_question(QQuestion q)
{
    this->question=q;
}

int Question_And_Answer::get_question_id()const
{
    return this->question.id;
}

int Question_And_Answer::get_question_type()const
{
    return this->question.type;
}

QString Question_And_Answer::get_question_text()
{
    return this->question.question_text;
}

QByteArray Question_And_Answer::get_question_image()
{
    return question.question_byte_array;
}

void Question_And_Answer::add_answer(QAnswer a)
{
    this->answers.push_back(a);
}

void Question_And_Answer::set_answer_as_vector(const QVector<QAnswer>v)
{
    for(int i=0;i<v.size();i++)
        this->answers.push_back(v[i]);
}

QString Question_And_Answer::get_answer_text(int i)
{
    return this->answers[i].answer;
}

QByteArray Question_And_Answer::get_anser_image(int i)
{
    return this->answers[i].ba;
}

bool Question_And_Answer::is_answer_correct(int i)
{
    if(this->answers[i].is_correct==1)
        return true;
    else
          return false;
}

int Question_And_Answer::get_answer_type(int i)
{
   return this->answers[i].type;
}

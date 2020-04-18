#ifndef REPETITIONSFRAME_H
#define REPETITIONSFRAME_H

#include <QObject>
#include <QWidget>
#include <QFrame>

class RepetitionsFrame : public QFrame
{
    Q_OBJECT
public:
    RepetitionsFrame(QWidget *parent = 0);
    ~RepetitionsFrame();
};

#endif // REPETITIONSFRAME_H

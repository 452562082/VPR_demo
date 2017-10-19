#ifndef MASKLABEL_H
#define MASKLABEL_H

#include <QLabel>

class MaskLabel : public QLabel
{
    Q_OBJECT

public:
    MaskLabel(QWidget *parent = 0);
    ~MaskLabel();

protected:
    void paintEvent(QPaintEvent *);
    void mouseReleaseEvent(QMouseEvent*);

signals:
    void clicked();
};

#endif // MASKLABEL_H

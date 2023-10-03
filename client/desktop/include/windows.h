#ifndef WINDOWS_H
#define WINDOWS_H

#include <QDialog>
#include <QGridLayout>
#include "canvas.h"
#include <QTimer>
#include <comservice.h>

class Windows : public QDialog
{
    QGridLayout layout;

    Canvas canvas;
    QTimer timer;
    COMService *communication{nullptr};

public:
    Windows(COMService *com);

private:
    void refresh();
};
#endif

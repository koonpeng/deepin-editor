#include "jumplinebar.h"
#include <QDebug>

JumpLineBar::JumpLineBar(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::X11BypassWindowManagerHint);
    
    layout = new QHBoxLayout(this);
    
    label = new QLabel();
    label->setText("跳到行: ");
    editLine = new LineBar();
    
    lineValidator = new QIntValidator;
    editLine->setValidator(lineValidator);    
    
    layout->addWidget(label);
    layout->addWidget(editLine);
    
    setFixedSize(200, 40);
    
    connect(editLine, &LineBar::pressEsc, this, &JumpLineBar::back, Qt::QueuedConnection);
    connect(editLine, &LineBar::pressEnter, this, &JumpLineBar::jump, Qt::QueuedConnection);
    connect(editLine, &LineBar::textChanged, this, &JumpLineBar::tempJump, Qt::QueuedConnection);
    connect(editLine, &LineBar::focusOut, this, &JumpLineBar::cancel, Qt::QueuedConnection);
}

void JumpLineBar::activeInput(QString file, int line, int lineCount, int scrollOffset)
{
    jumpFile = file;
    lineBeforeJump = line;
    jumpFileScrollOffset = scrollOffset;
    lineValidator->setRange(1, lineCount);
    
    editLine->setText("");
    
    show();
    raise();
    
    editLine->setFocus();
}

void JumpLineBar::cancel()
{
    hide();
    
    cancelJump();
}

void JumpLineBar::back()
{
    hide();
    
    backToLine(jumpFile, lineBeforeJump, jumpFileScrollOffset);
}

void JumpLineBar::tempJump()
{
    QString content = editLine->text();
    if (content != "") {
        tempJumpToLine(jumpFile, content.toInt());
    }
}

void JumpLineBar::jump()
{
    hide();
    
    QString content = editLine->text();
    if (content != "") {
        jumpToLine(jumpFile, content.toInt());
    }
}

void JumpLineBar::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setOpacity(1);
    QPainterPath path;
    path.addRect(rect());
    painter.fillPath(path, QColor("#202020"));
}
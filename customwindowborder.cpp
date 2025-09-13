#include "customwindowborder.h"

#define BORDER_WIDTH 5

enum Direction
{
    t,b,l,r,lt,rt,rb,lb
};

static QPoint last_postion;
static QSize  start_size;
static QPoint start_pos;
static Direction direct;

CustomWindowBorder::CustomWindowBorder(QWidget *parent)
    : QWidget{parent}
{
    this                     ->setAttribute(Qt::WA_StyledBackground, true);
    mainWin_                 = qobject_cast<QMainWindow*>(window());
    tilte_                   = "标题";
    this->                   __setupUI__();
    this->                   __connect__();
    QMetaObject::invokeMethod(this,&CustomWindowBorder::delay_init,Qt::QueuedConnection);
}

void CustomWindowBorder::delay_init()
{
    mainWin_->centralWidget()->layout()->setSpacing(0);
    mainWin_->centralWidget()->layout()->setContentsMargins(0,0,0,0);
    qApp->installEventFilter(this);
}

QString CustomWindowBorder::getTitle()
{
    return this             ->tilte_;
}

void CustomWindowBorder::setTitle(QString tilte)
{
    this->tilte_            = tilte;
}

void CustomWindowBorder::changedTitle()
{
    tilte_Button_           ->setText(tilte_);
}

void CustomWindowBorder::__setupUI__()
{
    tilte_Button_           = new QPushButton(this);
    tilte_Button_           ->setText(tilte_);
    minimize_Screen_Button_ = new QPushButton(this);
    full_Screen_Button_     = new QPushButton(this);
    close_Button_           = new QPushButton(this);
    QHBoxLayout * HBL       = new QHBoxLayout(this);
    HBL                     ->addWidget(tilte_Button_,8);
    tilte_Button_           ->setObjectName("tilte_Button");
    HBL                     ->addWidget(minimize_Screen_Button_);
    minimize_Screen_Button_ ->setObjectName("minimize_Screen_Button");
    HBL                     ->addWidget(full_Screen_Button_);
    full_Screen_Button_     ->setObjectName("full_Screen_Button");
    HBL                     ->addWidget(close_Button_);
    close_Button_           ->setObjectName("close_Button");
    HBL                     ->setContentsMargins(0,0,0,0);
    HBL                     ->setSpacing(0);
    this                    ->setLayout(HBL);
}

void CustomWindowBorder::__connect__()
{
    connect(minimize_Screen_Button_,&QPushButton::clicked, mainWin_,&QMainWindow::showMinimized);
    connect(full_Screen_Button_,    &QPushButton::clicked, this,    &CustomWindowBorder::on_maxmize);
    connect(close_Button_,          &QPushButton::clicked, mainWin_,&QMainWindow::close);
    connect(tilte_Button_,          &QPushButton::pressed, this,    &CustomWindowBorder::on_tilte_pressed);
}

void CustomWindowBorder::mouseMoveEvent(QMouseEvent* event) {
    if (!is_press) return;
    
    if (mainWin_->isMaximized()) {
        mainWin_->showNormal();
        QPoint newPos   = event->globalPosition().toPoint() - QPoint(mainWin_->height() / 2, 10);
        mainWin_        ->move(newPos);
        last_pos        = event->globalPosition().toPoint();
        return;
    }
    QPoint delta        = event->globalPosition().toPoint() - last_pos;
    mainWin_            ->move(mainWin_->pos() + delta);
    last_pos            = event->globalPosition().toPoint();
    this                ->QWidget::mouseMoveEvent(event);
}

void CustomWindowBorder::mouseReleaseEvent(QMouseEvent* event) {
    if (is_press) {
        is_press = false;
        this->releaseMouse();
    }
    this                ->QWidget::mouseReleaseEvent(event);
}

void CustomWindowBorder::on_maxmize()
{
    if(!mainWin_->isMaximized())mainWin_->showMaximized();
    else mainWin_->showNormal();
}

void CustomWindowBorder::on_tilte_pressed()
{
    this->grabMouse();   
    this->is_press = true;
    this->last_pos = QCursor::pos();
}

bool CustomWindowBorder::eventFilter(QObject *watched, QEvent *event)
{

    if(event->type() == QEvent::MouseMove)
    {
        QPoint mouse_p =  static_cast<QMouseEvent *>(event)->globalPosition().toPoint();

        if(!is_resize_pressed)
        {
            return __check_mouse_postion_and_setting__(mouse_p);
        }else
        {
            return __resize_window_and_move__(mouse_p);
        }
    }
    if(event->type() == QEvent::MouseButtonPress)
    {
        return __update_mouse_and_window_args__();
    }
    if(event->type() == QEvent::MouseButtonRelease)
    {
        if(is_resize_pressed) is_resize_pressed = 0;
    }
    return false;
}

bool CustomWindowBorder::__check_mouse_postion_and_setting__(QPoint mouse_p)
{
    QPoint winPos = mainWin_->pos();
    int winWidth = mainWin_->width();
    int winHeight = mainWin_->height();

    bool leftBorder = (winPos.x() < mouse_p.x() && 
                    winPos.x() + BORDER_WIDTH > mouse_p.x());
    bool rightBorder = (winPos.x() + winWidth - BORDER_WIDTH < mouse_p.x() && 
                    winPos.x() + winWidth > mouse_p.x());
    bool topBorder = (winPos.y() < mouse_p.y() && 
                winPos.y() + BORDER_WIDTH > mouse_p.y());
    bool bottomBorder = (winPos.y() + winHeight - BORDER_WIDTH < mouse_p.y() && 
                    winPos.y() + winHeight > mouse_p.y());

    if (leftBorder && topBorder)
    {
    mainWin_->setCursor(Qt::SizeFDiagCursor);
    is_resize_cursor = 1;
    last_postion = mouse_p;
    direct = lt;
    return true;
    }
    else if (rightBorder && topBorder)
    {
    mainWin_->setCursor(Qt::SizeBDiagCursor);
    is_resize_cursor = 1;
    last_postion = mouse_p;
    direct = rt;
    return true;;
    }
    else if (leftBorder && bottomBorder)
    {
    mainWin_->setCursor(Qt::SizeBDiagCursor);
    is_resize_cursor = 1;
    last_postion = mouse_p;
    direct = lb;
    return true;
    }
    else if (rightBorder && bottomBorder)
    {
    mainWin_->setCursor(Qt::SizeFDiagCursor);
    is_resize_cursor = 1;
    last_postion = mouse_p;
    direct = rb;
    return true;
    }
    else if (leftBorder || rightBorder)
    {
    mainWin_->setCursor(Qt::SizeHorCursor);
    is_resize_cursor = 1;
    last_postion = mouse_p;
    if(leftBorder) direct = l;
    else direct = r;
    return true;
    }
    else if (topBorder || bottomBorder)
    {
    mainWin_->setCursor(Qt::SizeVerCursor);
    is_resize_cursor = 1;
    last_postion = mouse_p;
    if(topBorder) direct = t;
    else direct = b;
    return true;
    }
    else 
    {
    mainWin_->setCursor(Qt::ArrowCursor);
    is_resize_cursor = 0;
    return false;
    }
}

bool CustomWindowBorder::__resize_window_and_move__(QPoint mouse_p)
{
    if(direct == r)
    {
        int dx = mouse_p.x() - last_postion.x();
        mainWin_->resize(start_size.width() + dx,start_size.height());
        return true;
    }
    if(direct == b)
    {
        int dy = mouse_p.y() - last_postion.y();
        mainWin_->resize(start_size.width() ,start_size.height()+dy);
        return true;
    }

    if(direct == t)
    {
        int dy = mouse_p.y() - last_postion.y();
        if(dy > start_size.height()- mainWin_->minimumHeight()) dy = start_size.height()- mainWin_->minimumHeight();
        mainWin_->resize(start_size.width() ,start_size.height() - dy);
        mainWin_->move(start_pos.x() ,start_pos.y() + dy);
        return true;
    }

    if(direct == l)
    {
        int dx = mouse_p.x() - last_postion.x();
        if(dx > start_size.width() - mainWin_->minimumWidth())  dx = start_size.width() - mainWin_->minimumWidth();
        mainWin_->resize(start_size.width()- dx ,start_size.height() );
        mainWin_->move(start_pos.x()+ dx ,start_pos.y() );
        return true;
    }

    if(direct == lt)
    {
        int dx = mouse_p.x() - last_postion.x();
        int dy = mouse_p.y() - last_postion.y();
        if(dy > start_size.height()- mainWin_->minimumHeight()) dy = start_size.height()- mainWin_->minimumHeight();
        if(dx > start_size.width() - mainWin_->minimumWidth())  dx = start_size.width() - mainWin_->minimumWidth();
        mainWin_->resize(start_size.width()- dx ,start_size.height() - dy);
        mainWin_->move(start_pos.x()+ dx ,start_pos.y() + dy);
        return true;
    }

    if(direct == rt)
    {
        int dx = mouse_p.x() - last_postion.x();
        int dy = mouse_p.y() - last_postion.y();
        if(dy > start_size.height()- mainWin_->minimumHeight()) dy = start_size.height()- mainWin_->minimumHeight();
        mainWin_->resize(start_size.width() + dx ,start_size.height() - dy);
        mainWin_->move(start_pos.x() ,start_pos.y() + dy);
        return true;
    }

    if(direct == rb)
    {
        int dx = mouse_p.x() - last_postion.x();
        int dy = mouse_p.y() - last_postion.y();
        mainWin_->resize(start_size.width() + dx ,start_size.height() + dy);
        return true;
    }

    if(direct == lb)
    {
        int dx = mouse_p.x() - last_postion.x();
        int dy = mouse_p.y() - last_postion.y();
        if(dx > start_size.width() - mainWin_->minimumWidth())  dx = start_size.width() - mainWin_->minimumWidth();
        mainWin_->resize(start_size.width() - dx ,start_size.height() + dy);
        mainWin_->move(start_pos.x() + dx ,start_pos.y());
        return true;
    }
    return false;
}

bool CustomWindowBorder::__update_mouse_and_window_args__()
{
    if(!is_resize_cursor)  return false;
    start_size = mainWin_->size();
    start_pos  = mainWin_->pos();
    is_resize_pressed = 1;
    return true;
}
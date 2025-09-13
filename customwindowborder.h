#ifndef CUSTOMWINDOWBORDER_H
#define CUSTOMWINDOWBORDER_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QMainWindow>
#include <QHBoxLayout>
#include <QMouseEvent>

class CustomWindowBorder : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString tilte READ getTitle  WRITE setTitle NOTIFY changedTitle FINAL);
public:
    explicit CustomWindowBorder(QWidget *parent = nullptr);

    QString getTitle();

    void delay_init();

    void setTitle(QString tilte);

    void changedTitle();

    void mouseMoveEvent(QMouseEvent *event);    

    void mouseReleaseEvent(QMouseEvent* event);

    bool eventFilter(QObject *watched, QEvent *event);

    void on_maxmize();

    void on_tilte_pressed();

    void on_tilte_released();
private:
    QString         tilte_;
    QPushButton *   tilte_Button_;
    QMainWindow *   mainWin_;
    QPushButton *   minimize_Screen_Button_;
    QPushButton *   full_Screen_Button_;
    QPushButton *   close_Button_;
    QPoint          last_pos;
    bool            is_press = 0;
    bool            is_resize_cursor = 0;
    bool            is_resize_pressed = 0;
    void __connect__();
    void __setupUI__();
    bool __check_mouse_postion_and_setting__(QPoint mouse_p);
    bool __resize_window_and_move__(QPoint mouse_p);
    bool __update_mouse_and_window_args__();
};
#endif // CUSTOMWINDOWBORDER_H



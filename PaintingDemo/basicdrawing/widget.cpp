#include "widget.h"
#include "ui_widget.h"
#include "renderarea.h"
#include <QColorDialog>
#include <QDebug>
#include <QMetaEnum>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    init();
    connectSlots();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::init()
{
    // 获取画图形状Shape枚举名称和值，写入下拉框
    QMetaEnum metaEnum = QMetaEnum::fromType<RenderArea::Shape>();
    for(int i=0; i < metaEnum.keyCount(); ++i)
    {
        ui->com_shape->addItem(metaEnum.key(i), metaEnum.value(i));
    }
    // 画笔样式枚举
    metaEnum = QMetaEnum::fromType<Qt::PenStyle>();
    for(int i=0; i < metaEnum.keyCount(); ++i)
    {
        ui->com_penStyle->addItem(metaEnum.key(i), metaEnum.value(i));
    }
    // 画笔笔帽样式枚举
    metaEnum = QMetaEnum::fromType<Qt::PenCapStyle>();
    for(int i=0; i < metaEnum.keyCount(); ++i)
    {
        ui->com_penCapStyle->addItem(metaEnum.key(i), metaEnum.value(i));
    }
    // 线段转角处连接样式枚举
    metaEnum = QMetaEnum::fromType<Qt::PenJoinStyle>();
    for(int i=0; i < metaEnum.keyCount(); ++i)
    {
        ui->com_penJoinStyle->addItem(metaEnum.key(i), metaEnum.value(i));
    }
    // 笔刷样式枚举
    metaEnum = QMetaEnum::fromType<Qt::BrushStyle>();
    for(int i=0; i < metaEnum.keyCount(); ++i)
    {
        ui->com_brushStyle->addItem(metaEnum.key(i), metaEnum.value(i));
    }

    ui->widget->installEventFilter(this);
    ui->com_shape->setCurrentIndex(0);
    ui->com_penStyle->setCurrentIndex(1);
}

void Widget::connectSlots()
{
    connect(ui->com_penStyle, QOverload<int>::of(&QComboBox::activated), this, &Widget::setPen);
    connect(ui->com_penCapStyle, QOverload<int>::of(&QComboBox::activated), this, &Widget::setPen);
    connect(ui->com_penJoinStyle, QOverload<int>::of(&QComboBox::activated), this, &Widget::setPen);
    connect(ui->spinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &Widget::setPen);
}

void Widget::setPen()
{
    QPen pen;
    pen.setStyle(Qt::PenStyle(ui->com_penStyle->currentData().toInt()));   // 设置画笔样式
    Qt::PenJoinStyle joinStyle = Qt::PenJoinStyle(ui->com_penJoinStyle->currentData().toInt());
    pen.setJoinStyle(joinStyle);
    if(Qt::MiterJoin == joinStyle)
    {
        pen.setMiterLimit(20);
    }
    pen.setWidth(ui->spinBox->value());     // 设置画笔线宽
    pen.setColor(m_color);                  // 设置画笔颜色
//    pen.setBrush()
    pen.setCapStyle(Qt::PenCapStyle(ui->com_penCapStyle->currentData().toInt()));  // 设置画笔笔帽样式
    ui->widget->setPen(pen);
}

/**
 * @brief            事件过滤器
 * @param watched
 * @param event
 * @return
 */
bool Widget::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->widget && event->type() == QEvent::Resize)
    {
        ui->horizontalSlider_origin->setMaximum(ui->widget->width());
        ui->verticalSlider_origin->setMaximum(ui->widget->height());
        ui->horizontalSlider_origin->setValue(0);
        ui->verticalSlider_origin->setValue(ui->widget->height());
    }

    return  QWidget::eventFilter(watched, event);
}

/**
 * @brief         水平偏移原点
 * @param value
 */
void Widget::on_horizontalSlider_origin_valueChanged(int value)
{
    ui->widget->setTranslate(value,  ui->widget->height() - ui->verticalSlider_origin->value());
}

/**
 * @brief        垂直偏移原点
 * @param value
 */
void Widget::on_verticalSlider_origin_valueChanged(int value)
{
    ui->widget->setTranslate(ui->horizontalSlider_origin->value(), ui->widget->height() - value);
}

/**
 * @brief          旋转
 * @param value
 */
void Widget::on_horizontalSlider_rotate_valueChanged(int value)
{
    ui->widget->setRotate(value);
}

/**
 * @brief           设置抗锯齿
 * @param checked
 */
void Widget::on_checkBox_clicked(bool checked)
{
    ui->widget->setAntialiased(checked);
}

void Widget::on_but_color_clicked()
{
    m_color = QColorDialog::getColor(m_color, this);
    setPen();
}

void Widget::on_com_brushStyle_activated(int index)
{
    Q_UNUSED(index)
    Qt::BrushStyle style = Qt::BrushStyle(ui->com_brushStyle->currentData().toInt());
    QBrush brush;
    brush.setStyle(style);
    brush.setColor(Qt::red);
    ui->widget->setBrush(brush);
}

/**
 * @brief        设置绘制图案类型
 * @param index
 */
void Widget::on_com_shape_activated(int index)
{
    Q_UNUSED(index)
    ui->widget->setShape(RenderArea::Shape(ui->com_shape->currentData().toInt()));
}

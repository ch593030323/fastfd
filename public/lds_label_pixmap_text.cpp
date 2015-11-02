#include "lds_label_pixmap_text.h"
#include <QHBoxLayout>

lds_label_pixmap_text::lds_label_pixmap_text(QWidget *parent) :
    QLabel(parent)
{
    QHBoxLayout *hlayout=new QHBoxLayout;
    _label_text=new QLabel;
    hlayout->addWidget(_label_text);
    hlayout->setMargin(0);
    this->setLayout(hlayout);

    _label_text->setAlignment(Qt::AlignCenter);
    _label_text->setStyleSheet("font-size:20pt;  color:steelblue;");
}

QString lds_label_pixmap_text::label_text()
{
    return _label_text->text();
}

void lds_label_pixmap_text::setLabel_text(const QString &text)
{
    _label_text->setText(text);
}

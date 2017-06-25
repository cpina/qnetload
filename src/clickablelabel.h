#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

// Copied and changed from https://wiki.qt.io/Clickable_QLabel

#include <QLabel>
#include <QWidget>

class ClickableLabel : public QLabel
{
	Q_OBJECT

	public:
		explicit ClickableLabel(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
		~ClickableLabel();

	Q_SIGNALS:
		void clicked();

	protected:
		void mousePressEvent(QMouseEvent* event);
};

#endif // CLICKABLELABEL_H

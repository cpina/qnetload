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
        void leftClicked();

	protected:
		void mousePressEvent(QMouseEvent* event);

        void leaveEvent(QEvent* event);
        void enterEvent(QEvent* event);

        virtual void paintEvent(QPaintEvent* event);

    private:
        bool m_inside;
};

#endif // CLICKABLELABEL_H

#ifndef HILOHEADERVIEW_H
#define HILOHEADERVIEW_H

#include <QHeaderView>
#include <QDebug>

class HiloHeaderView : public QHeaderView
{
public:
  HiloHeaderView(Qt::Orientation orientation, QWidget * parent = 0) : QHeaderView(orientation, parent)
  {
      setStretchLastSection(true);
      setSectionsClickable(true);
      setFixedHeight(47);
      setDefaultAlignment(Qt::AlignTop | Qt::AlignHCenter);
      setSortIndicatorShown(true);
  }

  QWidget* widgetList[20];
protected:
  void paintEvent(QPaintEvent *event)
  {
      QHeaderView::paintEvent(event);

      for(int i=0; i<6; i++)
      {
          widgetList[i]->setParent(this);
          widgetList[i]->move(sectionViewportPosition(i+1), 20);
          widgetList[i]->setFixedWidth(sectionSize(i+1));
      }
  }

};

#endif // HILOHEADERVIEW_H

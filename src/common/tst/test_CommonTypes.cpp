#include <QtTest/QTest>

#include "Region.h"
#include "DataMarks.h"
#include "JsonDataMark.h"
#include "GpsInfo.h"

#define POINT_IN_REGION QSharedPointer<DataMark>(new JsonDataMark(0.,500.,500.,"","","",QDateTime::currentDateTime()))
#define POINT_OUT_REGION QSharedPointer<DataMark>(new JsonDataMark(0.,1500.,500.,"","","",QDateTime::currentDateTime()))

namespace Test
{

  class CommonTypes : public QObject
  {
    Q_OBJECT

      private slots:

      void testRegionPoints()
      {
        // Object for testing
        common::Region *  m_tstObject = new common::Region();

        QSharedPointer<DataMark> p1(new JsonDataMark(0.,0.,0.,"","","",QDateTime::currentDateTime()));
        QSharedPointer<DataMark> p2(new JsonDataMark(0.,1000.,0.,"","","",QDateTime::currentDateTime()));
        QSharedPointer<DataMark> p3(new JsonDataMark(0.,1000.,1000.,"","","",QDateTime::currentDateTime()));
        QSharedPointer<DataMark> p4(new JsonDataMark(0.,0.,1000.,"","","",QDateTime::currentDateTime()));

        m_tstObject->addPoint(p1);
        m_tstObject->addPoint(p2);
        m_tstObject->addPoint(p3);
        m_tstObject->addPoint(p4);

        QVERIFY(m_tstObject->atRegion(POINT_IN_REGION));
        QVERIFY(!m_tstObject->atRegion(POINT_OUT_REGION));
      }

      void testGpsInstanceDefaults()
      {
        QCOMPARE(common::GpsInfo::getInstance().isReady(),false);
        QCOMPARE(common::GpsInfo::getInstance().getLatitude(),0.);
        QCOMPARE(common::GpsInfo::getInstance().getLongitude(),0.);
      }
  };

}


QTEST_APPLESS_MAIN(Test::CommonTypes)
#include "test_CommonTypes.moc"

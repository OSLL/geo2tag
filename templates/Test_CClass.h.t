/* \$Id$ */
/*!
 * \file ${FTEST}
 * \brief Test suite for ${CLASS} class
 *
 * PROJ: OSLL/
 * ----------------------------------------------------------- */


class ${CLASS}_TestSuite : public CxxTest::TestSuite 
{
public:
  void testSample( void )
  {
    TS_ASSERT( 1 + 1 > 1 );
    TS_ASSERT_EQUALS( 1 + 1, 2 );
  }
}; // class ${CLASS}_TestSuite

/* ===[ End of file \$HeadURL$ ]=== */

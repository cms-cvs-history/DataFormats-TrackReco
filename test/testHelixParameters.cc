#include <cppunit/extensions/HelperMacros.h>
#include "DataFormats/TrackReco/interface/HelixParameters.h"

class testHelixParameters : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(testHelixParameters);
  CPPUNIT_TEST(checkTrivial);
  CPPUNIT_TEST(checkParms);
  CPPUNIT_TEST(checkCov00);
  CPPUNIT_TEST(checkCov01);
  CPPUNIT_TEST(checkCov02);
  CPPUNIT_TEST(checkCov03);
  CPPUNIT_TEST(checkCov04);
  CPPUNIT_TEST(checkCov11);
  CPPUNIT_TEST(checkCov12);
  CPPUNIT_TEST(checkCov13);
  CPPUNIT_TEST(checkCov14);
  CPPUNIT_TEST(checkCov22);
  CPPUNIT_TEST(checkCov23);
  CPPUNIT_TEST(checkCov24);
  CPPUNIT_TEST(checkCov33);
  CPPUNIT_TEST(checkCov34);
  CPPUNIT_TEST(checkCov44);
  CPPUNIT_TEST_SUITE_END();
public:
  void setUp() {
    double vv[] = { 0.01, 0.02, -1.5, 0.03, 0.5 }; 
    v = reco::HelixParameters::Parameters( vv );
    //             d0    phi0  omega dz    tandip
    double ee[] = { 1.00, 0.10, 0.15, 0.20, 0.25,   // d0
		          1.10, 0.30, 0.35, 0.40,   // phi0
                                1.20, 0.45, 0.50,   // omega
                                      1.30, 0.55,   // dz
                                            1.40 }; // tandip
    e = reco::HelixParameters::Error( ee );  
    h = reco::HelixParameters( v, e );
    q = vv[ 2 ] > 0 ? 1 : -1;

    reco::HelixParameters::Point poca = h.poca();
    reco::HelixParameters::Vector p = h.momentum();
    reco::Error<6> cov = h.posMomError();
    h1 = reco::HelixParameters( q, poca, p, cov );
    err = h1.covariance();
  }
  void tearDown() {}
  void checkTrivial(); 
  void checkParms(); 

  void checkCov00(); 
  void checkCov01(); 
  void checkCov02(); 
  void checkCov03(); 
  void checkCov04(); 
  void checkCov11(); 
  void checkCov12(); 
  void checkCov13(); 
  void checkCov14(); 
  void checkCov22(); 
  void checkCov23(); 
  void checkCov24(); 
  void checkCov33(); 
  void checkCov34(); 
  void checkCov44(); 
private:
  int q;
  reco::HelixParameters::Parameters v;
  reco::HelixParameters::Error e;
  reco::HelixParameters h, h1;
  reco::HelixParameters::Error err;
};

CPPUNIT_TEST_SUITE_REGISTRATION(testHelixParameters);

void testHelixParameters::checkTrivial() {
  CPPUNIT_ASSERT( h.parameters() == v );
  CPPUNIT_ASSERT( h.covariance() == e );
  CPPUNIT_ASSERT( h1.charge() == q );
}

void testHelixParameters::checkParms() {
  CPPUNIT_ASSERT( fabs( h.d0() - h1.d0() ) < 1.e-6 );
  CPPUNIT_ASSERT( fabs( h.dz() - h1.dz() ) < 1.e-6 );
  CPPUNIT_ASSERT( fabs( h.tanDip() - h1.tanDip() ) < 1.e-6 );
  CPPUNIT_ASSERT( fabs( h.phi0() - h1.phi0() ) < 1.e-6 );
  CPPUNIT_ASSERT( fabs( h.omega() - h1.omega() ) < 1.e-6 );
}

void testHelixParameters::checkCov00() {
  CPPUNIT_ASSERT( fabs( err.get<0,0>() - e.get<0,0>() ) < 1.e-4 );
}

void testHelixParameters::checkCov01() {
  CPPUNIT_ASSERT( fabs( err.get<0,1>() - e.get<0,1>() ) < 1.e-4 );
}

void testHelixParameters::checkCov02() {
  CPPUNIT_ASSERT( fabs( err.get<0,2>() - e.get<0,2>() ) < 1.e-4 );
}

void testHelixParameters::checkCov03() {
  CPPUNIT_ASSERT( fabs( err.get<0,3>() - e.get<0,3>() ) < 1.e-4 );
}

void testHelixParameters::checkCov04() {
  CPPUNIT_ASSERT( fabs( err.get<0,4>() - e.get<0,4>() ) < 1.e-4 );
}

void testHelixParameters::checkCov11() {
  CPPUNIT_ASSERT( fabs( err.get<1,1>() - e.get<1,1>() ) < 1.e-4 );
}

void testHelixParameters::checkCov12() {
  CPPUNIT_ASSERT( fabs( err.get<1,2>() - e.get<1,2>() ) < 1.e-4 );
}

void testHelixParameters::checkCov13() {
  CPPUNIT_ASSERT( fabs( err.get<1,3>() - e.get<1,3>() ) < 1.e-4 );
}

void testHelixParameters::checkCov14() {
  CPPUNIT_ASSERT( fabs( err.get<1,4>() - e.get<1,4>() ) < 1.e-4 );
}

void testHelixParameters::checkCov22() {
  CPPUNIT_ASSERT( fabs( err.get<2,2>() - e.get<2,2>() ) < 1.e-4 );
}

void testHelixParameters::checkCov23() {
  CPPUNIT_ASSERT( fabs( err.get<2,3>() - e.get<2,3>() ) < 1.e-4 );
}

void testHelixParameters::checkCov24() {
  CPPUNIT_ASSERT( fabs( err.get<2,4>() - e.get<2,4>() ) < 1.e-4 );
}

void testHelixParameters::checkCov33() {
  CPPUNIT_ASSERT( fabs( err.get<3,3>() - e.get<3,3>() ) < 1.e-4 );
}

void testHelixParameters::checkCov34() {
  CPPUNIT_ASSERT( fabs( err.get<3,4>() - e.get<3,4>() ) < 1.e-4 );
}

void testHelixParameters::checkCov44() {
  CPPUNIT_ASSERT( fabs( err.get<4,4>() - e.get<4,4>() ) < 1.e-4 );
}

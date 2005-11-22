// $Id: HelixParameters.cc,v 1.24 2005/11/17 08:56:11 llista Exp $
// Author : Luca Lista, INFN
#include "DataFormats/TrackReco/interface/HelixParameters.h"
#include <cmath>
using namespace reco;

HelixParameters::HelixParameters( int q, const HelixParameters::Point & v, 
				  const HelixParameters::Vector & p,
				  const HelixParameters::PosMomError & cov ) {
  const double & dx2 = cov( 0, 0 );
  const double & dxy = cov( 0, 1 );
  const double & dy2 = cov( 1, 1 );
  const double & dxz = cov( 0, 2 );
  const double & dyz = cov( 1, 2 );
  const double & dz2 = cov( 2, 2 );

  const double & dpx2  = cov( 3, 3 );
  const double & dpxpy = cov( 3, 4 );
  const double & dpy2  = cov( 4, 4 );
  const double & dpxpz = cov( 3, 5 );
  const double & dpypz = cov( 4, 5 );
  const double & dpz2  = cov( 5, 5 );

  const double & dxpx = cov( 0, 3 );
  const double & dxpy = cov( 0, 4 );
  const double & dxpz = cov( 0, 5 );
  const double & dypx = cov( 1, 3 );
  const double & dypy = cov( 1, 4 );
  const double & dypz = cov( 1, 5 );
  const double & dzpx = cov( 2, 3 );
  const double & dzpy = cov( 2, 4 );
  const double & dzpz = cov( 2, 5 );

  double & dd02 = error_( 0, 0 );
  double & dphi02 = error_( 1, 1 );
  double & domega2 = error_( 2, 2 );
  double & ddz2 = error_( 3, 3 );
  double & dtanDip2 = error_( 4, 4 );

  double & dd0phi0 = error_( 0, 1 );
  double & dd0omega = error_( 0, 2 );
  double & dd0dz = error_( 0, 3 );
  double & dd0tanDip = error_( 0, 4 );
  double & dphi0omega = error_( 1, 2 );
  double & dphi0dz = error_( 1, 3 );
  double & dphi0tanDip = error_( 1, 4 );
  double & domegadz = error_( 2, 3 );
  double & domegatanDip = error_( 2, 4 );
  double & ddztanDip = error_( 3, 4 );

  double px = p.x(), py = p.y(), pz = p.z();
  par_.get< i_dz >() = v.z();
  double d02 = v.perp2(), d0 = sqrt( d02 );
  par_.get< i_d0 >() = d0;
  double pt2 = p.perp2(), px2 = px*px, py2 = py*py, pxpy = px*py;
  double pt = sqrt( pt2 );
  par_.get< i_omega >() = ( q > 0 ? 1. : -1. ) / pt ;
  par_.get< i_tanDip >() = pz / pt;
  par_.get< i_phi0 >() = - atan2( px, py );

  // check v is the p.o.c.a. to ( 0, 0, 0 )
  assert( fabs( p.x() * v.x() + p.y() * v.y() ) < 1.e-2 );

 // first, remove degeneracy:
  // pt = sqrt( px^2 + py^2 )
  // d pt / d px = px / pt
  // d pt / d py = py / pt

  double dpt2 = ( px2 * dpx2 + 2 * pxpy * dpxpy  + py2 * dpy2 ) / pt2;
  // the following are not used:
  //  double dpxpt = ( px * dpx2 + py * dpxpy ) / pt;
  //  double dpypt = ( px * dpxpy + py * dpy2 ) / pt;
  double dpzpt = ( px * dpxpz + py * dpypz ) / pt; 
  double dxpt = ( px * dxpx + py * dxpy ) / pt;
  double dypt = ( px * dypx + py * dypy ) / pt;
  double dzpt = ( px * dzpx + py * dzpy ) / pt;

  // use non-degenerate rep: ( x, y, z, pt, pz )
  // d0 = sqrt( x^2 + y^2 )
  // phi0 = atan2( y, x );
  // omega = q / pt;
  // dz = z
  // tanDip = pz / pt

  // protect against d0 ~ 0:
  // x / d0 = c, y / d0 = s
  // px / pt = -s, py / pt = c
  // pz/pt = tanDip
  
  // d d0 / d x = x / d0 = c
  // d d0 / d y = y / d0 = s
  // d phi0 / d x = - y / d02 = - s / d0;
  // d phi0 / d y = x / d02 = c / d0;
  // d omega / d pt = - q / pt^2 = - omega / pt
  // d dz / d z = 1
  // d tanDip / d pt = - pz / pt^2 = - tanDip / pt
  // d tanDip / d pz = 1 / pt;

  // d ( d0, phi0, dom, ddz, dtd ) / d ( x, y, z, pt, pz ) =
  //
  //           dx    dy    dz    dpt    dpz
  //  d0     :  c  |  s  |  0  |  0   |  0  :
  //  phi0   :-s/d0| c/d0|  0  |  0   |  0  : 
  //  om     :  0  |  0  |  0  |-om/pt|  0  :
  //  dz     :  0  |  0  |  1  |  0   |  0  :
  //  td     :  0  |  0  |  0  |-td/pt| 1/pt:

  // warning: fix if d0 ~ 0
  double phi0 = par_.get< i_phi0 >();
  double omega = par_.get< i_omega >(), omega2 = omega*omega;
  double tanDip = par_.get< i_tanDip >(), tanDip2 = tanDip*tanDip;
  double s = sin( phi0 ), c = cos( phi0 ), c2 = c*c, s2 = s*s, sc = s*c;
  double scdxy = sc * dxy, scdxy2 = 2 * scdxy;
  dd02 = c2 * dx2 + scdxy2 + s2 * dy2;
  dphi02 = ( s2 * dx2 - scdxy2 + c2 * dy2 ) / d02;
  dd0phi0 = ( sc * ( dy2 - dx2 ) + ( c2 - s2 ) * dxy ) / d0;
  domega2 = omega2 * dpt2 / pt2;
  ddz2 = dz2;
  dtanDip2 = ( tanDip2 * dpt2 - 2 * tanDip * dpzpt + dpz2 ) / pt2;
  dd0omega = - omega * ( c * dxpt + s * dypt ) / pt;
  dphi0omega = omega * ( s * dxpt - c * dypt ) / pt / d0;
  dd0dz = c * dxz + s * dyz;
  dphi0dz = ( - s * dxz + c * dyz ) / d0;
  dd0tanDip = ( - tanDip * ( c * dxpt + s * dypt ) + (   c * dxpz + s * dypz ) ) / pt;
  dphi0tanDip = ( tanDip * ( s * dxpt - c * dypt ) + ( - s * dxpz + c * dypz ) ) / pt / d0;
  domegadz = - omega * dzpt / pt;
  domegatanDip = omega * ( tanDip * dpt2 - dpzpt ) / pt2;
  ddztanDip =( - tanDip * dzpt + dzpz ) / pt;
}  

HelixParameters::PosMomError HelixParameters::posMomError() const {
  PosMomError cov;
  double & dx2 = cov( 0, 0 );
  double & dxy = cov( 0, 1 );
  double & dy2 = cov( 1, 1 );
  double & dxz = cov( 0, 2 );
  double & dyz = cov( 1, 2 );
  double & dz2 = cov( 2, 2 );

  double & dpx2  = cov( 3, 3 );
  double & dpxpy = cov( 3, 4 );
  double & dpy2  = cov( 4, 4 );
  double & dpxpz = cov( 3, 5 );
  double & dpypz = cov( 4, 5 );
  double & dpz2  = cov( 5, 5 );

  double & dxpx = cov( 0, 3 );
  double & dxpy = cov( 0, 4 );
  double & dxpz = cov( 0, 5 );
  double & dypx = cov( 1, 3 );
  double & dypy = cov( 1, 4 );
  double & dypz = cov( 1, 5 );
  double & dzpx = cov( 2, 3 );
  double & dzpy = cov( 2, 4 );
  double & dzpz = cov( 2, 5 );
  
  const double & dd02 = error_( 0, 0 );
  const double & dphi02 = error_( 1, 1 );
  const double & domega2 = error_( 2, 2 );
  const double & ddz2 = error_( 3, 3 );
  const double & dtanDip2 = error_( 4, 4 );

  const double & dd0phi0 = error_( 0, 1 );
  const double & dd0omega = error_( 0, 2 );
  const double & dd0dz = error_( 0, 3 );
  const double & dd0tanDip = error_( 0, 4 );
  const double & dphi0omega = error_( 1, 2 );
  const double & dphi0dz = error_( 1, 3 );
  const double & dphi0tanDip = error_( 1, 4 );
  const double & domegadz = error_( 2, 3 );
  const double & domegatanDip = error_( 2, 4 );
  const double & ddztanDip = error_( 3, 4 );
  // d v / d d0 = ( c, s, 0 )
  // d v / d dz = ( 0, 0, 1 )
  // d v / d phi0 = d0 * ( -s, c, 0 )
  // d v / d omega = 0
  // d v / d tanDip = 0

  // d p / d omega = - p / omega = pt * ( s, -c, -tanDip ) / omega = - q pt p
  // d p / d tanDip = ( 0, 0, pt )
  // d p / d phi0 = -pt * ( c, s, 0 ) = (-py,px,0) 
  // d p / d d0 = 0
  // d p / d dz = 0

  // d ( x, y, z, px, py, pz ) / d ( d0, phi0, dom, ddz, dtd )
  //
  //         dd0   dphi0  dom     ddz   dtd
  //  dx   :  c  |-d0*s |   0    |  0  |  0  |
  //  dy   :  s  | d0*c |   0    |  0  |  0  |
  //  dz   :  0  |  0   |   0    |  1  |  0  |
  //  dpx  :  0  |-pt*c | pt*s/o |  0  |  0  |
  //  dpy  :  0  |-pt*s |-pt*c/o |  0  |  0  |
  //  dpz  :  0  |  0   |-pt*td/o|  0  |  pt |

  double c = cos( phi0() ), s = sin( phi0() );
  double c2 = c*c, s2 = s*s, sc = s*c, c2s2 = c2 - s2;
  double pt = HelixParameters::pt(), pt2 = pt * pt;
  double d0 = HelixParameters::d0();
  double tanDip = HelixParameters::tanDip(), tanDip2 = tanDip * tanDip;
  double omega = HelixParameters::omega();
  double scdd0phi02 = 2 * sc * dd0phi0;
  double d0dphi02 = d0 * dphi02;
  dx2 = c2 * dd02 + d0 * (   - scdd0phi02 + s2 * d0dphi02 );
  dy2 = s2 * dd02 + d0 * (     scdd0phi02 + c2 * d0dphi02 );
  dxy = sc * dd02 + d0 * ( c2s2 * dd0phi0 - sc * d0dphi02 );
  dz2 = ddz2;
  double d0dphi0dz = d0 * dphi0dz;
  dxz = c * dd0dz - s * d0dphi0dz;
  dyz = s * dd0dz + c * d0dphi0dz;
  double dphi0omegao = dphi0omega / omega, sc2dphi0omegao = 2 * sc * dphi0omegao;
  double pt2domega2 = pt2 * domega2;
  double c2dphi02 = c2 * dphi02, scdphi02 = sc * dphi02, s2dphi02 = s2 * dphi02;
  dpx2 = pt2 * ( c2dphi02 - sc2dphi0omegao + s2 * pt2domega2 );
  dpy2 = pt2 * ( s2dphi02 + sc2dphi0omegao + c2 * pt2domega2 );
  double domegatanDipo = domegatanDip / omega;
  dpz2 = pt2 * ( tanDip2 * domega2 * pt2 - 2 * tanDip * domegatanDipo + dtanDip2 );
  dpxpy = pt2 * ( sc * ( dphi02 - domega2 * pt2 ) + c2s2 * dphi0omegao );
  dpxpz = pt2 * ( ( c * dphi0omegao - s * pt2domega2 ) * tanDip - c * dphi0tanDip + s * domegatanDipo );
  dpypz = pt2 * ( ( s * dphi0omegao + c * pt2domega2 ) * tanDip - s * dphi0tanDip - c * domegatanDipo );
  double dd0omegao = dd0omega / omega;
  double c2dd0phi0 = c2 * dd0phi0, scdd0phi0 = sc * dd0phi0, s2dd0phi0 = s2 * dd0phi0;
  double c2dd0omegao = c2 * dd0omegao, scdd0omegao = sc * dd0omegao, s2dd0omegao = s2 * dd0omegao;
  double c2dphi0omegao = c2 * dphi0omegao, scdphi0omegao = sc * dphi0omegao, s2dphi0omegao = s2 * dphi0omegao;
  dxpx = pt * ( - c2dd0phi0 + scdd0omegao + d0 * (   scdphi02 - s2dphi0omegao ) );
  dxpy = pt * ( - scdd0phi0 - c2dd0omegao + d0 * (   s2dphi02 + scdphi0omegao ) );
  dypx = pt * ( - scdd0phi0 + s2dd0omegao + d0 * ( - c2dphi02 + scdphi0omegao ) );
  dypy = pt * ( - s2dd0phi0 - scdd0omegao + d0 * ( - scdphi02 - c2dphi0omegao ) );
  double tddd0omegao = tanDip * dd0omegao;
  double d0ddd = d0 * ( tanDip * dphi0omegao - dphi0tanDip );
  dxpz = pt * ( - c * tddd0omegao + c * dd0tanDip + s * d0ddd );
  dypz = pt * ( - s * tddd0omegao + s * dd0tanDip - c * d0ddd );
  double domegadzo = domegadz / omega;
  dzpx = pt * ( - c * dphi0dz  + s * domegadzo );
  dzpy = pt * ( - s * dphi0dz  - c * domegadzo );
  dzpz = pt * ( - tanDip * domegadzo + ddztanDip );

  return cov;
}

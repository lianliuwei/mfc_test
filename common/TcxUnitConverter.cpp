#include "stdafx.h"
// TODO rm stdafx.h dependent.
#include "common/TcxUnitConverter.h"

#include <cmath>

#include "base/string_util.h"
#include "base/stringprintf.h"

#pragma warning( disable:4244 )


/*/////////////////////////////////////////////////////////////////////////////
This is a metamacro that defines all the supported unit.
To insert a new unit, create the proper entry in the table.

Basic unit entries:
   _m##_B( <Abbr>, <Tag> ) for basic units.

Derived unit entries:
   _m##_D( <Abbr>, <Tag>, <K>, <Expr> ) for derived units.

Where:
   - <Abbr> (string) is the abbreviation for the unit name.
   - <Tag> is a valid C/C++ identifier that identifies the unit
     (it's generally the unit name in a C/C++ format)
   - <K> (const double) is the constant multiplier used to
     convert a derived unit.
   - <Expr> (string) is a expression unit used in the conversion.

Important:
   - The table is sorted in ASCII ascending oder. Don't ever break that order.
     If you create a new unit, put it in the right place.
   - If you, for example, create a new unit of length and declare it as a
     basic unit , the conversor won't be able to convert it to other units of
     length. Therfore, you should declare it as derived of other units of
     length.
   - When declaring new derived units, you can base them in any other unit,
     even other derived ones. However, the conversor must reduce any unit
     to basic units. Therfore, it will work faster if you declare your
     derived units based on basic units.
   - Take care with cyclic definitions. The conversor will not detect
     them and it will enter in a infinite loop.

/*/////////////////////////////////////////////////////////////////////////////

#define _UNITS_METADEF( _m )  \
   _m##_B( "A", Ampere )\
   _m##_D( "Bq", becquerel, 1, "l/s" )\
   _m##_D( "Btu", InternationalTableBtu, 1055.05585262, "Kg*m^2/s^2" )\
   _m##_D( "C", Coulomb, 1, "A*s" )\
   _m##_D( "Ci", Curie, 3.7*1010, "l/s" )\
   _m##_D( "F", Farad, 1, "A^2*s^4/Kg*m^2" )\
   _m##_D( "Fdy", Faraday, 96487, "A*s" )\
   _m##_D( "Gy", Gray, 1, "m^2/s^2" )\
   _m##_D( "H", Henry, 1, "Kg*m^2/A^2*s^2" )\
   _m##_D( "Hz", Hertz, 1, "s^-1" )\
   _m##_D( "J", Joule, 1, "Kg*m^2/s^2" )\
   _m##_B( "K", Kelvin )\
   _m##_D( "Knot", NauticalMilesPerHour, 463.0/900.0, "m/s" )\
   _m##_D( "N", Newton, 1, "Kg*m/s^2" )\
   _m##_D( "Ohm", Ohm, 1, "Kg*m^2/A^2*s^3" )\
   _m##_D( "OzUK", UKFluidOunce, 2.8413075e-5, "m^3" )\
   _m##_D( "P", Poise, 0.1, "Kg/m*s" )\
   _m##_D( "Pa", Pascal, 1, "Kg/m*s" )\
   _m##_D( "Pdl", Poundal, 0.13825495376, "Kg*m/s^2" )\
   _m##_D( "Pica", Pica, 1.0/72.0, "in" )\
   _m##_D( "R", Roentgen, 0.000258, "A*s/Kg" )\
   _m##_D( "S", Siemens, 1, "A^2*s^3/Kg*m^2" )\
   _m##_D( "St", Stokes, 0.0001, "m^2/s" )\
   _m##_D( "Sv", Sievert, 1, "m^2/s^2" )\
   _m##_D( "T", Tesla, 1, "Kg/A*s^2" )\
   _m##_D( "U", UnifiedAtomicMass, 1.6605402e-27, "Kg" )\
   _m##_D( "V", Volt, 1, "Kg*m^2/A*s^2" )\
   _m##_D( "W", Watt, 1, "Kg*m^2/s^3" )\
   _m##_D( "Wb", Weber, 1, "Kg*m^2/A*s^2" )\
   _m##_D( "acre", Acre, 4046.87260987, "m^2" )\
   _m##_D( "arcmin", ArcMinute, 2.9088820866e-4, "r" )\
   _m##_D( "arcs", ArcSecond, 4.848136111e-6, "r" )\
   _m##_D( "atm", Attmosphere, 101325, "Kg/m*s^2" )\
   _m##_D( "au", AstronomicalUnit, 1.495979*1011, "m" )\
   _m##_D( "b", Barn, 1e-28, "m^2" )\
   _m##_D( "bar", Bar, 100000, "Kg/m*s^2" )\
   _m##_D( "bbl", Barrel, 0.158987294928, "m^3" )\
   _m##_D( "bu", Bushel, 0.03523907, "m^3" )\
   _m##_D( "c", LightSpeed, 299792458, "m/s" )\
   _m##_D( "cal", Calorie, 4.1868, "Kg*m^2/s^2" )\
   _m##_B( "cd", Candela )\
   _m##_D( "chain", Chain, 20.1168402337, "m" )\
   _m##_D( "ct", Carat, 0.0002, "Kg" )\
   _m##_D( "cu", USCup, 2.365882365e-4, "m^3" )\
   _m##_D( "d", day, 86400, "s" )\
   _m##_D( "dyn", Dyne, 0.00001, "Kg*m/s^2" )\
   _m##_D( "eV", ElectronVolt, 1.60217733e-19, "Kg*m^2/s^2" )\
   _m##_D( "erg", Erg, 0.0000001, "Kg*m^2/s^2" )\
   _m##_D( "fath", Fathom, 1.82880365761, "m" )\
   _m##_D( "fbm", BoardFoot, 0.002359737216, "m^3" )\
   _m##_D( "fc", Footcandle, 10.7639104167, "cd*sr/m^2" )\
   _m##_D( "fermi", Fermi, 1e-15, "m" )\
   _m##_D( "flam", Footlambert, 3.42625909964, "cd/m^2" )\
   _m##_D( "ft", InternationalFoot, 0.3048, "m" )\
   _m##_D( "ftUS", SurveyFoot, 0.304800609601, "m" )\
   _m##_B( "g", Gram )\
   _m##_D( "ga", StandardFreefall, 9.80665, "m/s^2" )\
   _m##_D( "gal", USGallon, 0.003785411784, "m^3" )\
   _m##_D( "galC", CanadianGallon, 0.00454609, "m^3" )\
   _m##_D( "galUK", UKGallon, 0.004546092, "m^3" )\
   _m##_D( "gf", GramForce, 0.00980665, "Kg*m/s^2" )\
   _m##_D( "grad", Grade, 1.57079632679e-2, "r" )\
   _m##_D( "grain", Grain, 0.00006479891, "Kg" )\
   _m##_D( "h", Hour, 3600, "s" )\
   _m##_D( "ha", Hectare, 10000, "m^2" )\
   _m##_D( "hp", horsepower, 745.699871582, "Kg*m^2/s^2" )\
   _m##_D( "in", Inch, 0.0254, "m" )\
   _m##_D( "inH2O", InchesOfWater, 248.84, "Kg/m*s^2" )\
   _m##_D( "inHg", InchesOfMercury, 3386.38815789, "Kg/m*s^2" )\
   _m##_D( "kip", KilopoundForce, 4448.22161526, "Kg*m/s^2" )\
   _m##_D( "kph", KilometersPerHour, 5.0/18.0, "m/s" )\
   _m##_D( "l", Liter, 0.001, "m^3" )\
   _m##_D( "lam", Lambert, 3183.09886184, "cd/m^2" )\
   _m##_D( "lb", AvoirdupoisPound, 0.45359267, "Kg" )\
   _m##_D( "lbf", PoundForce, 4.44822161526, "Kg*m/s^2" )\
   _m##_D( "lbt", TroyPound, 0.3732417216, "Kg" )\
   _m##_D( "lm", Lumen, 1, "cd*sr" )\
   _m##_D( "lx", Lux, 1, "cd*sr/m^2" )\
   _m##_D( "lyr", LightYear, 9.46052840488*1015, "m" )\
   _m##_B( "m", Meter )\
   _m##_D( "mho", Mho, 1, "A^2*s^2/Kg*m^2" )\
   _m##_D( "mi", InternationalMile, 1609.344, "m" )\
   _m##_D( "miUS", USStatuteMile, 1609.34721869, "m" )\
   _m##_D( "mil", Mil, 0.0000254, "m" )\
   _m##_D( "min", Minute, 60, "s" )\
   _m##_D( "mmHg", MilimeterOfMercury, 133.322368421, "Kg/m*s^2" )\
   _m##_B( "mol", Mole )\
   _m##_D( "mph", MilesPerHour, 0.44704, "m/s" )\
   _m##_D( "nmi", NauticalMile, 1852, "m" )\
   _m##_D( "oz", Ounce, 0.028349523125, "Kg" )\
   _m##_D( "ozfl", USFluidOunce, 2.95735295625e-5, "m^3" )\
   _m##_D( "ozt", TroyOunce, 0.0311034768, "Kg" )\
   _m##_D( "pc", Parsec, 3.08567818585106, "m" )\
   _m##_D( "ph", Phot, 10000, "cd*sr/m^2" )\
   _m##_D( "pk", Peck, 0.0088097675, "m^3" )\
   _m##_D( "psi", PoundsPerSquareInch, 6894.75729317, "Kg/m*s^2" )\
   _m##_D( "pt", Pint, 0.000473176473, "m^3" )\
   _m##_D( "qt", Quart, 0.000946352946, "m^3" )\
   _m##_B( "r", Radian )\
   _m##_D( "rad", Rad, 0.01, "m^2/s^2" )\
   _m##_D( "rd", Rod, 5.02921005842, "m" )\
   _m##_D( "rem", Rem, 0.01, "m^2/s^2" )\
   _m##_B( "s", Second )\
   _m##_D( "sb", Stilb, 10000, "cd/m^2" )\
   _m##_D( "slug", Slug, 14.5939029372, "Kg" )\
   _m##_B( "sr", Steradian )\
   _m##_D( "st", Stere, 1, "m^3" )\
   _m##_D( "t", MetricTon, 1000, "Kg" )\
   _m##_D( "tbsp", Tablespoon, 1.47867647813e-5, "m^3" )\
   _m##_D( "therm", EECTherm, 105506000, "Kg*m^2/s^2" )\
   _m##_D( "ton", ShortTon, 907.18474, "Kg" )\
   _m##_D( "tonUK", UKLongTon, 1016.0469088, "Kg" )\
   _m##_D( "torr", Torr, 133.322368421, "Kg/m^2" )\
   _m##_D( "tsp", Teaspoon, 4.92892159375e-6, "m^3" )\
   _m##_D( "yd", InternationalYard, 0.9144, "m" )\
   _m##_D( "yr", Year, 31556925.9747, "s" )\
   // INFO those unit hard to be show in the ANSI, relate to the code page.
   // TODO need to use unicode.
   //_m##_D( "°", Degree, 1.74532925199e-2, "r" )\
   //_m##_D( "°C", DegreeCelsius, 1.0, "K" )\
   //_m##_D( "°F", DegreesFahrenheit, 1.0/1.8, "K" )\
   //_m##_D( "°R", DegreesRankine, 1.0/1.8, "K" )\
   //_m##_D( "µ", Micron, 1e-6, "m" )\
   //_m##_D( "Å", Angstrom, 1e-10, "m" )\


//
// 0 K = -273.15 degC
// 1000 K = 726.85 degC
// 
// 0.00 K = -459.67 degF 
// 1000.00 kelvin = 1340.33 degF 
// 

/*/////////////////////////////////////////////////////////////////////////////
Metamacro to define the multiplier prefixes
/*/////////////////////////////////////////////////////////////////////////////

#define _MULTIPLIERS_METADEF( _m ) \
   _m( 'Y',	Yotta,   1e+24   )\
   _m( 'Z',	Zetta,   1e+21   )\
   _m( 'E',	Exa,     1e+18   )\
   _m( 'P',	Peta,    1e+15   )\
   _m( 'T',	Tera,    1e+12   )\
   _m( 'G',	Giga,    1e+9    )\
   _m( 'M',	Mega,    1e+6    )\
   _m( 'k', kilo,    1e+3    )\
   _m( 'K', Kilo,    1e+3    )\
   _m( 'h', hecto,   1e+2    )\
   _m( 'H', Hecto,   1e+2    )\
   _m( 'D',	Deka,    1e+1    )\
   _m( 'd',	deci,    1e-1    )\
   _m( 'c',	cent,    1e-2    )\
   _m( 'm',	mili,    1e-3    )\
   _m( 'u',	micro,   1e-6    )\
   _m( 'n',	nano,    1e-9    )\
   _m( 'p',	pico,    1e-12   )\
   _m( 'f',	femto,   1e-15   )\
   _m( 'a',	atto,    1e-18   )\
   _m( 'z',	zepto,   1e-21   )\
   _m( 'y',	yocto,   1e-24   )\

///////////////////////////////////////////////////////////////////////////////

#define _ENUM_BASIC_B( Abbr, Tag )                    eTcxBasicUnit_##Tag,
#define _ENUM_BASIC_D( Abbr, Tag, K, Expr )

#define _ENUM_UNIT_B( Abbr, Tag )                     eTcxUnit_##Tag,
#define _ENUM_UNIT_D( Abbr, Tag, K, Expr )            eTcxUnit_##Tag,

// Enumeration of basic units
enum
{
   _UNITS_METADEF( _ENUM_BASIC )
   NumOfBasicUnits
};

// Enumeration of all units
enum
{
   _UNITS_METADEF( _ENUM_UNIT )
   NumOfUnits
};

// Used in unit redution
struct TCXUNIT
{
   double k;
   char aExp[NumOfBasicUnits];
};

// Used in the unit info table
struct TCXUNIT_INFO
{
   LPCTSTR pszAbbr;
   double k;
   LPCTSTR pszExpr;
};

#define _UNIT_INFO_B( Abbr, Tag )               { _T(Abbr), 1.0, (LPCTSTR)eTcxBasicUnit_##Tag },
#define _UNIT_INFO_D( Abbr, Tag, K, Expr )      { _T(Abbr), (double)K, _T(Expr) },

// Table of all units information
static const TCXUNIT_INFO _aUnitInfos[NumOfUnits] =
{
   _UNITS_METADEF( _UNIT_INFO )
};

struct TCXMULTIPLIER_INFO
{
  TCHAR abbr;
  double k;
};

#define _ENUM_MULIIPLIER( Abbr, Tag, K )  eTcxMultiplier_##Tag,
enum
{
  _MULTIPLIERS_METADEF( _ENUM_MULIIPLIER )
  NumOfMultiplier
};

#define _MULTIPLIER_INFO( Abbr, Tag, K) { _T(Abbr), (double)K },

static const TCXMULTIPLIER_INFO _aMultiplierInfos[NumOfMultiplier] = 
{
  _MULTIPLIERS_METADEF( _MULTIPLIER_INFO )
};

/////////////////////////////////////////////////////////////////////////////
// Helping Functions: String 

static inline BOOL _istAbbrChar( TCHAR ch )
   { return _istalpha( ch ) || ch == _T('_') || _istdigit( ch ); }

static int _tcsCmpUnitAbbr( LPCTSTR pszIn, LPCTSTR pszAbbr )
{
   while( *pszIn != _T('\0') && *pszAbbr != _T('\0') )
   {
      if( *pszIn != *pszAbbr )
         return _tccmp( pszIn, pszAbbr );

      pszIn = _tcsinc(pszIn);
      pszAbbr = _tcsinc(pszAbbr);
   }

   if( *pszAbbr != _T('\0') )
      return -1;

   if( _istAbbrChar( *pszIn ) )
      return 1;

   return 0;
}

static LPCTSTR _tcsSkipSpaces( LPCTSTR pszIn )
{
   while( _istspace(*pszIn) )
      pszIn = _tcsinc( pszIn );

   return pszIn;
}

/////////////////////////////////////////////////////////////////////////////
// Helping Functions: Math

double _quickPow( double k, int nPow )
{
   bool bNegative = ( nPow < 0 );
   if( bNegative )
      nPow = -nPow;

   double r = 1.0;

   while( nPow != 0 )
   {
      if( nPow & 1 )
         r *= k;

      k *= k;
      nPow >>= 1;
   }

   if( bNegative )
      r = 1.0 / r;

   return r;
}

/////////////////////////////////////////////////////////////////////////////
// Helping Functions: Units

int _TcxLookupUnit( LPCTSTR pszIn )
{
   int iFirst = 0;
   int iLast = NumOfUnits;
   int i;

   while( iLast >= iFirst )
   {
      i = ( iLast + iFirst ) / 2;

      const TCXUNIT_INFO& info = _aUnitInfos[i];

      int nCmp = _tcsCmpUnitAbbr( pszIn, info.pszAbbr );

      if( nCmp == 0 )
         return i;  // match!!!

      if( nCmp < 0 )
         iLast = i - 1;
      else
         iFirst = i + 1;
   }

   return -1;  // no match
}

/////////////////////////////////////////////////////////////////////////////
// Forward template declarations

template< class U > static LPCTSTR _TcxParseDivUnitExpr( LPCTSTR pszIn, U& u );
template< class U > static LPCTSTR _TcxParseMulUnitExpr( LPCTSTR pszIn, U& u );
template< class U > static LPCTSTR _TcxParsePowUnitExpr( LPCTSTR pszIn, U& u );
template< class U > static LPCTSTR _TcxParseUnitPhase1( LPCTSTR pszIn, U& u );
template< class U > static LPCTSTR _TcxParseUnitPhase2( LPCTSTR pszIn, U& u );

/////////////////////////////////////////////////////////////////////////////
// Unit operations templates

template< class U >
static void _TcxDivUnit( U& /*u1*/, const U& /*u2*/ ) {};

template< class U >
static void _TcxMulUnit( U& /*u1*/, const U& /*u2*/ ) {};

template< class U >
static void _TcxMulUnitK( U& /*u*/, double /*k*/ ) {};

template< class U >
static void _TcxPowUnit( U& /*u*/, char /*nPow*/ ) {};

template< class U >
static void _TcxSetBasicUnit( U& /*u*/, double /*k*/, int /*iBasicUnit*/ ) {};

template< class U >
static void _TcxExpandUnitExpr( U& /*u*/, double /*k*/, LPCTSTR /*pszExpr*/ ) {};

/////////////////////////////////////////////////////////////////////////////
// Unit operations: TCXUNIT specializations

static inline void _TcxDivUnit( TCXUNIT& u1, const TCXUNIT& u2 )
{
   u1.k /= u2.k;

   for( int i = 0; i < NumOfBasicUnits; ++i )
      u1.aExp[i] -= u2.aExp[i];
};

static inline void _TcxMulUnit( TCXUNIT& u1, const TCXUNIT& u2 )
{
   u1.k *= u2.k;

   for( int i = 0; i < NumOfBasicUnits; ++i )
      u1.aExp[i] += u2.aExp[i];
};

static inline void _TcxMulUnitK( TCXUNIT& u, double k )
{
   u.k *= k;
};

static inline void _TcxPowUnit( TCXUNIT& u, char nPow )
{
   for( int i = 0; i < NumOfBasicUnits; ++i )
      u.aExp[i] *= nPow;

   u.k = _quickPow( u.k, nPow );
};

static inline void _TcxSetBasicUnit( TCXUNIT& u, double k, int iBasicUnit )
{
   u.k = k;
   ::memset( u.aExp, 0, sizeof(u.aExp) );

   if( 0 <= iBasicUnit )
      u.aExp[iBasicUnit] = 1;
}

static void _TcxExpandUnitExpr( TCXUNIT& u, double k, LPCTSTR pszExpr )
{
   pszExpr = _TcxParseDivUnitExpr( pszExpr, u );
   ASSERT( pszExpr != NULL && *pszExpr == _T('\0') );
   
   u.k *= k;
}

/////////////////////////////////////////////////////////////////////////////
// Unit interpretation: template declarations

template< class U >
static LPCTSTR _TcxParseDivUnitExpr( LPCTSTR pszIn, U& u )
{
   pszIn = _TcxParseMulUnitExpr( pszIn, u );
   if( pszIn == NULL )
      return NULL;

   LPCTSTR pszMark = pszIn;
   pszIn = _tcsSkipSpaces( pszIn );
   if( *pszIn != _T('/') )
      return pszMark;

   pszIn = _tcsinc( pszIn );
   pszIn = _tcsSkipSpaces( pszIn );

   U uAux;
   pszIn = _TcxParseMulUnitExpr( pszIn, uAux );
   if( pszIn == NULL )
      return pszMark;

   _TcxDivUnit( u, uAux );

   pszMark = pszIn;
   return pszMark;   
}

template< class U >
static LPCTSTR _TcxParseMulUnitExpr( LPCTSTR pszIn, U& u )
{
   pszIn = _TcxParsePowUnitExpr( pszIn, u );
   if( pszIn == NULL )
      return NULL;

   LPCTSTR pszMark = pszIn;

   while( pszIn = _tcsSkipSpaces( pszIn ), *pszIn == _T('*') )
   {
      pszIn = _tcsinc( pszIn );

      U uAux;
      pszIn = _TcxParsePowUnitExpr( pszIn, uAux );
      if( pszIn == NULL )
         return pszMark;

      _TcxMulUnit( u, uAux );

      pszMark = pszIn;
   }

   return pszIn;
}

template< class U >
static LPCTSTR _TcxParsePowUnitExpr( LPCTSTR pszIn, U& u )
{
   pszIn = _TcxParseUnitPhase1( pszIn, u );
   if( pszIn == NULL )
      return NULL;

   LPCTSTR pszMark = pszIn;

   pszIn = _tcsSkipSpaces( pszIn );
   if( *pszIn != _T('^') )
      return pszMark;

   pszIn = _tcsinc( pszIn );
   pszIn = _tcsSkipSpaces( pszIn );

   bool bNegative = ( *pszIn == _T('-') );
   
   if( *pszIn == _T('+') || *pszIn == _T('-') )
      pszIn = _tcsinc( pszIn );

   if( !_istdigit(*pszIn) )
      return pszMark;

   char nPow = 0;

   while( _istdigit(*pszIn) )
   {
      char nPow2 = nPow * 10 + ( *pszIn - _T('0') );
      if( nPow2 < nPow )
         break;

      nPow = nPow2;
      pszIn = _tcsinc( pszIn );
   }

   if( bNegative )
      nPow = -nPow;

   _TcxPowUnit( u, nPow );

   return pszIn;
}

#define _CASE_MULTIPLIER( Prfx, Tag, K )   case _T(Prfx): k = (double)K; break;

template< class U >
static LPCTSTR _TcxSkipMultiplier( LPCTSTR pszIn, U& u)
{
  _TcxSetBasicUnit( u, 1.0, -1 );

  pszIn = _tcsSkipSpaces( pszIn );
  if( *pszIn == _T('1') )
    return pszIn;
  
  LPCTSTR pszMark = pszIn;

  U dump;
  pszIn = _TcxParseUnitPhase2( pszIn, dump);
  if( pszIn != NULL )
    return pszMark;

  pszIn = pszMark;

  double k;

  switch( *pszIn )
  {
    _MULTIPLIERS_METADEF( _CASE_MULTIPLIER )

  default:
    return NULL;
  }

  pszIn = _tcsinc( pszIn );
  _TcxMulUnitK( u, k );

  return pszIn;
}

template< class U >
static LPCTSTR _TcxParseUnitPhase1( LPCTSTR pszIn, U& u )
{
   pszIn = _tcsSkipSpaces( pszIn );

   if( *pszIn == _T('1') )
   {
      _TcxSetBasicUnit( u, 1.0, -1 );
      pszIn = _tcsinc( pszIn );
      pszIn = _tcsSkipSpaces( pszIn );
      return pszIn;
   }
   
   LPCTSTR pszMark = pszIn;

   pszIn = _TcxParseUnitPhase2( pszIn, u );
   if( pszIn != NULL )
      return pszIn;

   pszIn = pszMark;

   double k;

   switch( *pszIn )
   {
      _MULTIPLIERS_METADEF( _CASE_MULTIPLIER )

   default:
      return NULL;
   }

   pszIn = _tcsinc( pszIn );
   pszIn = _TcxParseUnitPhase2( pszIn, u );

   if( pszIn == NULL )
      return NULL;

   _TcxMulUnitK( u, k );

   return pszIn;
}

template< class U >
static LPCTSTR _TcxParseUnitPhase2( LPCTSTR pszIn, U& u )
{
   int iUnit = _TcxLookupUnit( pszIn );

   if( iUnit < 0 )
      return NULL;

   const TCXUNIT_INFO& info = _aUnitInfos[iUnit];

   if( info.pszExpr < (LPCTSTR)NumOfBasicUnits )
      _TcxSetBasicUnit( u, 1.0, (int)info.pszExpr );
   else
      _TcxExpandUnitExpr( u, info.k, info.pszExpr );

   pszIn += _tcslen( info.pszAbbr );

   return pszIn;
}

static BOOL _TcxCheckUnitsTable()
{
   BOOL bSuccess = TRUE;

   for( int i = 0; i < NumOfUnits-1; ++i )
   {
      const TCXUNIT_INFO& info1 = _aUnitInfos[i];
      const TCXUNIT_INFO& info2 = _aUnitInfos[i+1];

      if( _tcscmp( info1.pszAbbr, info2.pszAbbr ) >= 0 )
      {
         TRACE( "TcxUnits [%s] and [%s] : out of order\n", info1.pszAbbr, info2.pszAbbr );
         bSuccess = FALSE;
      }
   }

   return bSuccess;
}

/////////////////////////////////////////////////////////////////////////////
// Main Functions

// Converts the <dValue> from the <pszSrcUnit> to the <pszDstUnit>
// RETURN:
//    - Nonzero if the conversion was successfully done
//    - Zero if the conversion can't be performed
//      (an illegal unit or incompatible units)
BOOL TcxConvertUnit(
   double& dValue,      // [in] value to convert, [out] converted value
   LPCTSTR pszSrcUnit,  // [in] source unit
   LPCTSTR pszDstUnit   // [in] destination unit
   )
{
#ifdef _DEBUG

   static BOOL _bCheckUnitsTable = TRUE;
   if( _bCheckUnitsTable )
   {
      _bCheckUnitsTable = FALSE;
      ASSERT( _TcxCheckUnitsTable() );
   }

#endif

   ASSERT( AfxIsValidString( pszSrcUnit ) );
   ASSERT( AfxIsValidString( pszDstUnit ) );

   pszSrcUnit = _tcsSkipSpaces( pszSrcUnit );
   pszDstUnit = _tcsSkipSpaces( pszDstUnit );

   if( _tcscmp( pszSrcUnit, pszDstUnit ) == 0 )
      return TRUE;

   TCXUNIT uSrcUnit;
   pszSrcUnit = _TcxParseDivUnitExpr( pszSrcUnit, uSrcUnit );
   if( pszSrcUnit == NULL )
      return FALSE;

   TCXUNIT uDstUnit;
   pszDstUnit = _TcxParseDivUnitExpr( pszDstUnit, uDstUnit );
   if( pszDstUnit == NULL )
      return FALSE;

   for( int i = 0; i < NumOfBasicUnits; ++i )
      if( uSrcUnit.aExp[i] != uDstUnit.aExp[i] )
         return FALSE;

   dValue *= uSrcUnit.k;
   dValue /= uDstUnit.k;
    
   return TRUE;
}


// Counts how many of the first characters of the given string can be
// interpreted as a valid unit or unit expression.
// RETURN:
//    - The number of characters that can be interpreted as a valid
//      unit expression (the index of the first character that breaks
//      the unit). If the entire string can be interpreted as a single
//      unit, it return the length of the string.
int TcxSpanUnitStr(
   LPCTSTR pszIn        // [in] text to check
   )
{
   ASSERT( AfxIsValidString( pszIn ) );

   char nc;
   LPCTSTR pszEnd = _TcxParseDivUnitExpr( pszIn, nc );
   
   if( pszEnd == NULL )
      return 0;

   int n = ( pszEnd - pszIn )/sizeof(TCHAR);
   ASSERT( n != 0 );

   return n;
}

// Returns the number of units in the internal table.
int TcxGetNumOfUnits()
{
   return NumOfUnits;
}

// Given a unit table index (i.e. [0..TcxGetNumOfUnits()-1]),
// returns a pointer to a static string with the abbreviation
// of that unit.
LPCTSTR TcxGetUnitAbbreviation(
   int iUnit            // [in] unit index
   )
{
   if( iUnit < 0 || NumOfUnits <= iUnit )
      return NULL;

   return _aUnitInfos[iUnit].pszAbbr;
}

// Given a unit table index (i.e. [0..TcxGetNumOfUnits()-1]),
// returns a pointer to a static string with the unit tag.
LPCTSTR TcxGetUnitTag(
   int iUnit            // [in] unit index
   )
{
#define _UNITTAG_B( Abbr, Tag )          _T(#Tag),
#define _UNITTAG_D( Abbr, Tag, k, Def )  _T(#Tag),

   if( iUnit < 0 || NumOfUnits <= iUnit )
      return NULL;

   static const LPCTSTR _aUnitTags[] =
   {
      _UNITS_METADEF( _UNITTAG )
   };

   ASSERT( iUnit < sizeof(_aUnitTags)/sizeof(_aUnitTags[0]) );

   return _aUnitTags[iUnit];
}

// Given a unit table index (i.e. [0..TcxGetNumOfUnits()-1]),
// returns TRUE if that unit is a basic unit.
BOOL TcxUnitIsBasic(
   int iUnit            // [in] unit index
   )
{
   if( iUnit < 0 || NumOfUnits <= iUnit )
      return FALSE;

   return _aUnitInfos[iUnit].pszExpr < (LPCTSTR)NumOfBasicUnits;
}

bool TcxConvertUnitForEasyRead(double* value, string16* unit) {
  CHECK(value != NULL);
  CHECK(unit != NULL);

  TCXUNIT unit_s;
  LPCTSTR str_unit = _TcxSkipMultiplier(const_cast<LPCTSTR>(unit->c_str()), unit_s);
  if (str_unit == NULL)
    return false;
  int index;
  for (int i = NumOfMultiplier - 1; i >=0; --i) {
    double comp =  _aMultiplierInfos[i].k / unit_s.k ;
    if (std::fabs(*value) < comp) {// first less then 1
      if (i == NumOfMultiplier -1 )
        return std::fabs(*value) == 0.0; // too small or is 0
      index = i + 1; // the before one is the suitable one
      break;
    }
    if (i == 0) // No find too large.
      return false;
  }
  if(index == eTcxMultiplier_hecto || 
     index == eTcxMultiplier_Hecto || 
     index == eTcxMultiplier_Deka || 
     index == eTcxMultiplier_deci || 
     index == eTcxMultiplier_cent)
     return true; // no usual used.
  
  // convert
  *value = *value * unit_s.k / _aMultiplierInfos[index].k;
  *unit = base::StringPrintf(_T("%lc%ls"), 
    _aMultiplierInfos[index].abbr, str_unit);

  return true;
}
